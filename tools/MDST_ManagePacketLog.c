/*
 * MDST_ManagePacketLog.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */

#include "MDST.h"

int main(int argc, char *argv[])
{
	int PacketLogFd = 0;

	if (!strcmp(argv[2],"LOAD"))
	{
		MDST_AttachIPCS(atoi(argv[1]));

		if ((PacketLogFd=open(argv[3],O_RDONLY)) < 0) MDST_ErrorExit("open");

		memset(MDStream,0,MDSTHeader->StreamSize);
		memset(MPList,0,MDSTHeader->MPListSize*sizeof(struct MDSTMissingPage));
		MDSTHeader->MPListMPIdx = MDSTHeader->MPListRPIdx = 0;

		MDST_PrintTime("");
		printf("Loading packet data file %s\n",argv[2]);

		if ((MPList[MDSTHeader->MPListMPIdx].StartOffset = read(PacketLogFd, MDStream, MDSTHeader->StreamSize)) < 0 ) perror("read");

		MDST_PrintTime("");
		printf("%d bytes are loaded\n",MPList[MDSTHeader->MPListMPIdx].StartOffset);

		close(PacketLogFd);
	}
	else if (!strcmp(argv[2],"SAVE"))
	{
		MDST_AttachIPCS(atoi(argv[1]));

		if ((PacketLogFd=creat(argv[3],S_IWUSR|S_IRUSR|S_IWGRP|S_IRGRP)) < 0) MDST_ErrorExit("creat");

		MDST_PrintTime("");
		printf("Saving packet data into file %s\n",argv[2]);

		if (write(PacketLogFd, MDStream, MPList[MDSTHeader->MPListMPIdx].StartOffset) < 0 ) perror("write");

		MDST_PrintTime("");
		printf("%d bytes are saved\n",MPList[MDSTHeader->MPListMPIdx].StartOffset);

		close(PacketLogFd);
	}

	printf("[OK]\n");
	fflush(stdout);
	exit (0);
}




