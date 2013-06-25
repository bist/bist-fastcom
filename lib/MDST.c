/*
 * MDST.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */
#include <MDST.h>

void MDST_PrintTime(char *tLabel)
{
	int hrs, mins, secs;
	struct timeval tTime;

	gettimeofday(&tTime, NULL);

	secs = tTime.tv_sec % 60;
	mins = tTime.tv_sec / 60;
	hrs = mins / 60;
	mins = mins % 60;
	hrs = hrs % 24;

	printf("[%02d:%02d:%02d:%06ld] %s", hrs, mins, secs, tTime.tv_usec, tLabel);
}

void MDST_ErrorExit(char *pMsg)
{
	int tErrno = errno;

	MDST_PrintTime("Exiting Error: ");
	perror(pMsg);

	exit(tErrno);
}

int MDST_AttachIPCS(unsigned char prmStreamId)
{
	int IPCKey;
	int IPCId;

	char *tPtr;

	IPCKey = KEYBASE + getuid() * 256 + prmStreamId;

	if ((IPCId = shmget(IPCKey, 0, 0)) < 0)
		MDST_ErrorExit("Get SHM");

	tPtr = (char *) shmat(IPCId, 0, 0);

	if (tPtr == ((char *) -1))
		MDST_ErrorExit("Attach SHM");

	MDSTHeader = (struct MDSTStreamHeader *) tPtr;

	MPList
			= (struct MDSTMissingPage *) (tPtr
					+ sizeof(struct MDSTStreamHeader));

	MDStream = (unsigned char *) (tPtr + sizeof(struct MDSTStreamHeader)
			+ sizeof(struct MDSTMissingPage) * MDSTHeader->MPListSize);

	MPListSem = MDSTHeader->MPListSem;

	MDST_PrintTime("Attached IPCS\n");
	fflush(stdout);
	return (0);
}

int MDST_ReleaseSEMWait()
{
	struct sembuf sSemOps = { 0, 0, 0 };

	if (MPListSem) {
		if ((sSemOps.sem_op = semctl(MPListSem, 0, GETNCNT, 0)) > 0) {
			DBG("fnMSReleaseSEMWait sem cnt: %d\n", sSemOps.sem_op);
			if (semop(MPListSem, &sSemOps, 1) < 0) {
				MDST_ErrorExit("semop");
			}
		}
	}

	return sSemOps.sem_op;
}

int MDST_HangSEMWait()
{
	struct sembuf sSemOps = { 0, -1, 0 };

	if (MPListSem) {
		DBG("fnMSHangSEMWait start waiting sem\n");
		if (semop(MPListSem, &sSemOps, 1) < 0) {
			MDST_ErrorExit("semop");
		}
		DBG("fnMSHangSEMWait sem released\n");
	}

	return sSemOps.sem_op;
}

int MDST_read(unsigned int soffset, unsigned char *data, unsigned int len)
{
	struct MDSTMissingPage *tCurr = NULL;

	tCurr = &MPList[MDSTHeader->MPListRPIdx];

	if (soffset >= tCurr->StartOffset)
		return 0;

	if ((soffset + len) >= tCurr->StartOffset) {
		memcpy(data, &MDStream[soffset], tCurr->StartOffset - soffset);
		return tCurr->StartOffset - soffset;
	}

	memcpy(data, &MDStream[soffset], len);
	return len;
}

void MDST_write(unsigned char *data, unsigned int len)
{
	memcpy(&MDStream[MPList[MDSTHeader->MPListMPIdx].StartOffset], data, len);
	MPList[MDSTHeader->MPListMPIdx].StartOffset += len;
}

