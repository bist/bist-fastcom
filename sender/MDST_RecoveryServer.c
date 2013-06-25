/*
 * MDST_RecoveryServer.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */
#include "MDST.h"

int main(int argc, char *argv[])
{
	int	iInfSock;
	int okunan;
	int	s_opt = 1;
	unsigned int tCurrOffset;
	int tPageLength;
	struct MDSTMissingPage tMP;

	struct  sockaddr_in	sDaddr;
	struct  sockaddr	*pDaddr	= (struct sockaddr *)&sDaddr;
	socklen_t		iDlen	= sizeof(struct sockaddr_in);

	MDST_AttachIPCS(atoi(argv[1]));

	if ((iInfSock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) MDST_ErrorExit("socket");

	if (setsockopt(iInfSock, SOL_SOCKET, SO_BROADCAST, &s_opt, sizeof(s_opt))) MDST_ErrorExit("setsockopt");

	if (bind(iInfSock ,(struct sockaddr *)&(MDSTHeader->RecoveryServerAddr), sizeof(MDSTHeader->RecoveryServerAddr)) < 0) MDST_ErrorExit("bind");

	for (;;)
	{
		if ((okunan=recvfrom(iInfSock,&tMP,sizeof(tMP),0,pDaddr,&iDlen)) < sizeof(tMP)) perror("recvfrom");
		else
		{
			tMP.StartOffset =ntohl(tMP.StartOffset);
			tMP.EndOffset =ntohl(tMP.EndOffset);
			DBG("requested start: %d end: %d\n", tMP.StartOffset, tMP.EndOffset);

			tCurrOffset = MPList[MDSTHeader->MPListRPIdx].StartOffset;
			DBG("Current id:%d start: %d end: %d\n", MDSTHeader->MPListRPIdx,
					MPList[MDSTHeader->MPListRPIdx].StartOffset,
					MPList[MDSTHeader->MPListRPIdx].EndOffset);

			if (tMP.EndOffset > tCurrOffset) tPageLength = tCurrOffset - tMP.StartOffset;
			else tPageLength = tMP.EndOffset - tMP.StartOffset;
			DBG("tPageLength: %d\n", tPageLength);

			if (tPageLength > 0)
			{
				if (tPageLength > MAX_RCVR_PAGE_LEN) tPageLength = MAX_RCVR_PAGE_LEN;

				if (sendto(iInfSock,MDStream+tMP.StartOffset,tPageLength,0,pDaddr,iDlen) < 0 ) perror("send missing packet");
				else {
					MDST_PrintTime("");
					printf("Gateway[%s] requested a missing page. ",inet_ntoa(sDaddr.sin_addr));
					printf("%d bytes of requested page between %d and %d is sent\n",tPageLength, tMP.StartOffset, tMP.EndOffset);
				}
			}
			else {
				DBG("Gateway[%s] requested a missing page. ",inet_ntoa(sDaddr.sin_addr));
				DBG("Requested page between %d and %d is unavailable!!\n", tMP.StartOffset, tMP.EndOffset);
			}

			fflush(stdout);
		}
	}

	exit(0);
}

