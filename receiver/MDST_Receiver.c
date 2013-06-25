/*
 * MDST_Receiver.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */
#include <MDST.h>

int main(int argc, char *argv[])
{
	int	iInfSock;
	int okunan;
	unsigned char *tStreamPtr;
	unsigned int tmpOffset;
	struct MDSTMissingPage *tMP;
	int	s_opt = 1;
	struct timeval tTimeout = {1,0};
	struct  sockaddr_in	sDaddr;
	socklen_t	iDlen	= sizeof(struct sockaddr_in);

	MDST_AttachIPCS(atoi(argv[1]));

	if ((iInfSock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) MDST_ErrorExit("socket");
	if (setsockopt(iInfSock, SOL_SOCKET, SO_BROADCAST, &s_opt,    sizeof(s_opt)   )) MDST_ErrorExit("setsockopt:SO_BROADCAST");
	if (setsockopt(iInfSock, SOL_SOCKET, SO_RCVTIMEO,  &tTimeout, sizeof(tTimeout))) MDST_ErrorExit("setsockopt:SO_RCVTIMEO" );
	if (bind(iInfSock ,(struct sockaddr *)&(MDSTHeader->InfAddr), sizeof(MDSTHeader->InfAddr)) < 0) MDST_ErrorExit("bind");

	tMP = MPList+MDSTHeader->MPListMPIdx;
	tMP->EndOffset = MDSTHeader->StreamSize;
	tStreamPtr = MDStream + tMP->StartOffset;
	MDSTHeader->EOSRcvrPage.EndOffset = 0;

	for (;;)
	{
//		fnPrintTime("Waiting for a new packet\n");
		fflush(stdout);

		for (;;)
		{
			okunan = recvfrom(iInfSock, tStreamPtr, MAX_STREAM_PAGE_SIZE, 0, (struct sockaddr *)&sDaddr, &iDlen);

			if (okunan >= (int)sizeof(unsigned int)) break;
			else if (okunan < 0)
			{
				if (errno == EWOULDBLOCK)
				{
					if (MDSTHeader->EOSRcvrPage.EndOffset)
					{
						if (tMP->StartOffset == MDSTHeader->EOSRcvrPage.StartOffset)
						{
								okunan = MDSTHeader->EOSRcvrPage.EndOffset-MDSTHeader->EOSRcvrPage.StartOffset;
								memcpy(tStreamPtr,MDSTHeader->EOSRcvrBufr,okunan);
								tStreamPtr += okunan;
								tMP->StartOffset += okunan;
						}
						MDSTHeader->EOSRcvrPage.EndOffset = 0;
					}
					continue;
				}
				else perror("recvfrom");
			}
			sleep(1);
		}

		MDST_PrintTime("Packet received. ");

		tStreamPtr += (okunan -= sizeof(unsigned int));

		tmpOffset = ntohl(*((unsigned int *)tStreamPtr));

		if (tmpOffset == (tMP->StartOffset + okunan))
		{
			tMP->StartOffset = tmpOffset;
			MDST_ReleaseSEMWait();
			printf("Received page size : %d, stream offset moved to : %d.\n",okunan,tMP->StartOffset);
		}
		else if (tmpOffset > (tMP->StartOffset + okunan)) // kayip paket var, handle edilecek
		{
			printf("!!! A page of %d bytes between %d and %d is missing !!!. Stream offset moved to : %d.\n",tmpOffset - tMP->StartOffset ,tMP->StartOffset,tmpOffset ,tmpOffset);

			tMP->EndOffset = tmpOffset;
			tMP = MPList+MDSTHeader->MPListMPIdx+1;
			tMP->StartOffset = tmpOffset;
			tMP->EndOffset = MDSTHeader->StreamSize;
			MDSTHeader->MPListMPIdx++;
			tStreamPtr = MDStream + tMP->StartOffset;
		}
		else
		{
			tStreamPtr -= okunan;
			printf("!!! Current::%d and new offset::%d values are inconsistent, page will be discarded !!!\n",tMP->StartOffset,tmpOffset);
		}
	}
}
