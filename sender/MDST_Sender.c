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
	int	s_opt = 1;
	unsigned char *tStreamPtr;
	unsigned int tOffset = 0;
	unsigned int tCopySize = 0;
	char	tmpBfr[MAX_STREAM_PAGE_SIZE];
	int ii=0;
	int nn=0;

	nn=atoi(argv[2]);

/*	struct  sockaddr_in	tInfAddr;

	tInfAddr.sin_addr.s_addr= INADDR_ANY;
	tInfAddr.sin_family		= PF_INET;
	tInfAddr.sin_port		= MDSTHeader->InfAddr.sin_port;
*/
	MDST_AttachIPCS(atoi(argv[1]));

	if ((iInfSock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) MDST_ErrorExit("socket");

	if (setsockopt(iInfSock, SOL_SOCKET, SO_BROADCAST, &s_opt, sizeof(s_opt))) MDST_ErrorExit("setsockopt");

	//if (bind(iInfSock ,(struct sockaddr *)&(tInfAddr), sizeof(tInfAddr)) < 0) ErrorExit("bind");

	tStreamPtr = MDStream;

	for (;;)
	{
		tCopySize = sizeof(tmpBfr)-sizeof(unsigned int);

		if (tCopySize > MPList[MDSTHeader->MPListMPIdx].StartOffset-tOffset) tCopySize = MPList[MDSTHeader->MPListMPIdx].StartOffset-tOffset;

		if (tCopySize > 0)
		{
			memcpy(tmpBfr,tStreamPtr,tCopySize);

			tOffset += tCopySize;

			*((unsigned int *)(tmpBfr+tCopySize)) = htonl(tOffset);

			tStreamPtr += tCopySize;

			tCopySize += sizeof(unsigned int);

			MDST_PrintTime("");

			if (sendto(iInfSock,tmpBfr,tCopySize,0, (struct sockaddr *)&(MDSTHeader->InfAddr), sizeof(MDSTHeader->InfAddr)) < tCopySize) MDST_ErrorExit("send");
			else printf("%d bytes are sent from %d\n",(int)(tCopySize-sizeof(unsigned int)),(int)(tOffset-(tCopySize-sizeof(unsigned int))));
			fflush(stdout);

			for (ii=0;ii<nn; ii++)
			{
					ii++;
					ii--;
			}
		}
		else sleep(1);
	}
}
