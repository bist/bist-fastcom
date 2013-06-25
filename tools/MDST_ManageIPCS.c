/*
 * MDST_ManageIPCS.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */
#include "MDST.h"

int main(int argc, char *argv[])
{
	int StreamId;
	int SHMSize;
	int IPCKey;
	int IPCId;
	char *tPtr;
	int tMPListSize;
	int tStreamSize;

	StreamId = atoi(argv[1]);

	MDST_PrintTime("MDST_ManageIPCS:");
	printf("%s\n",argv[2]);

	IPCKey = KEYBASE + getuid()*256 + StreamId;

	if (!strcmp(argv[2],"CREATE"))
	{
		printf("Creating MDST IPCs:");
		fflush(stdout);

		tMPListSize = atol(argv[3]);

		tStreamSize =  atol(argv[4]);

		SHMSize = sizeof(struct MDSTStreamHeader)+tMPListSize*sizeof(struct MDSTMissingPage)+tStreamSize;

		if ( ( IPCId = shmget( IPCKey , SHMSize ,IPC_CREAT|0x1B6 ) ) < 0 ) MDST_ErrorExit("Create SHM");
		else if ( ( tPtr = shmat( IPCId, 0, 0 ) ) == (void *)-1L ) MDST_ErrorExit("Attach SHM");

		memset(tPtr, 0, SHMSize);

		MDSTHeader = (struct MDSTStreamHeader *) tPtr;

		MDSTHeader->StreamSize = tStreamSize;
		MDSTHeader->MPListSize = tMPListSize;

		MPList = (struct MDSTMissingPage *)(tPtr+sizeof(struct MDSTStreamHeader));

		MDStream = (unsigned char *)(tPtr+sizeof(struct MDSTStreamHeader)+sizeof(struct MDSTMissingPage)*MDSTHeader->MPListSize);

		if ( ( IPCId = semget( IPCKey , 1 ,IPC_CREAT|0x1B6 ) ) < 0 ) MDST_ErrorExit("Create SEM");
		MDSTHeader->MPListSem = IPCId;
		MPListSem = MDSTHeader->MPListSem;
	}
	else if (!strcmp(argv[2],"REMOVE"))
	{
		if ( ( IPCId = semget( IPCKey , 1 , 0) ) < 0 ) MDST_ErrorExit("Get SEM ID");
		else if ( semctl(IPCId, 1, IPC_RMID ,0) < 0) MDST_ErrorExit("Cannot Remove SEM");
		else printf("Semaphore %d is removed\n",IPCId);

		if ( ( IPCId = shmget( IPCKey, 0, 0 ) ) < 0 ) MDST_ErrorExit("Get SHM ID");
		else if ( shmctl(IPCId, IPC_RMID ,0) < 0) MDST_ErrorExit("Cannot Remove SHM");
		else printf("Shared Memory %d is removed\n",IPCId);
	}

	printf("[OK]\n");
	fflush(stdout);
	exit (0);
}


