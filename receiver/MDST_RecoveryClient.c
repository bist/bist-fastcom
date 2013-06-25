/*
 * FixMS_RecoveryClient.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */
#include "MDST.h"

/*	START: write shared memory to back_up_file	*/
#include <signal.h>

int back_up_file;
int yazilan;
struct sigaction act;

void sig_close_file(int signum)
{
	printf("Received signal %d:%s\n", signum, strsignal(signum));
	printf("Backup file last written index %d\n", yazilan);

	close(back_up_file);

	exit(EXIT_SUCCESS);
}

void open_back_up_file(char *file)
{
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_close_file;
	sigaction(SIGTERM, &act, NULL);
	sigaction(SIGINT, &act, NULL);

	back_up_file = creat(file, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP);
	if (back_up_file < 0) {
		perror("creat back_up_file");
		return;
	}
	yazilan = 0;
}

void write_back_up_file()
{
	struct MDSTMissingPage *tCurr = NULL;
	int ret = 0, wsize = 0;

	tCurr = &MPList[MDSTHeader->MPListRPIdx];

	if (yazilan == tCurr->StartOffset)
		return;

	wsize = tCurr->StartOffset - yazilan;
	ret = write(back_up_file, &MDStream[yazilan], wsize);
	if (ret < 0) {
		perror("write backupfile");
	} else {
		DBG("ret: %d from %d, %d Bytes written to file (start: %d)\n", ret,
				yazilan, wsize, tCurr->StartOffset);
		yazilan += wsize;
	}
}
/*	END: write shared memory to back_up_file	*/

int main(int argc, char *argv[])
{
	int iInfSock;
	int okunan;
	int s_opt = 1;
	struct MDSTMissingPage tMP;
	struct MDSTMissingPage *tCurrMP;
	struct timeval tTimeout = { 1, 0 };
	char tmpBuf[16];

	MDST_AttachIPCS(atoi(argv[1]));

	if ((iInfSock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		MDST_ErrorExit("sock");

	if (setsockopt(iInfSock, SOL_SOCKET, SO_BROADCAST, &s_opt, sizeof(s_opt)))
		MDST_ErrorExit("setsockopt");

	setsockopt(iInfSock, SOL_SOCKET, SO_RCVTIMEO, &tTimeout, sizeof(tTimeout));

	if (argc == 3)
		open_back_up_file(argv[2]);

	for (;;) {
		for (; MDSTHeader->MPListRPIdx < MDSTHeader->MPListMPIdx; MDSTHeader->MPListRPIdx++) {
			tCurrMP = &MPList[MDSTHeader->MPListRPIdx];

			while (tCurrMP->EndOffset > tCurrMP->StartOffset) {
				tMP.StartOffset = htonl(tCurrMP->StartOffset);
				tMP.EndOffset = htonl(tCurrMP->EndOffset);

				/*
				 fnPrintTime("");
				 printf(
				 "Recovery request for the page between %d and %d will be sent\n",
				 tCurrMP->StartOffset, tCurrMP->EndOffset);
				 */

				if (sendto(iInfSock, &tMP, sizeof(tMP), 0,
						(struct sockaddr *) &(MDSTHeader->RecoveryClientAddr),
						sizeof(MDSTHeader->RecoveryClientAddr)) < 0) {
					perror("send recovery request");
					sleep(1);
				} else {
					if ((okunan = recv(iInfSock, MDStream
							+ tCurrMP->StartOffset, tCurrMP->EndOffset
							- tCurrMP->StartOffset, 0)) < 0) {
						if (errno != EAGAIN)
							perror("recv");
					} else {
						MDST_PrintTime("");
						printf("The page between %d and %d is recovered\n",
								tCurrMP->StartOffset, tCurrMP->StartOffset
										+ okunan);
						tCurrMP->StartOffset += okunan;
						MDST_ReleaseSEMWait();
					}
				}
				fflush(stdout);
			}
		}

		for (;;) {
			okunan = recv(iInfSock, tmpBuf, sizeof(tmpBuf), MSG_DONTWAIT);
			if (okunan < 0) {
				if (errno == EWOULDBLOCK)
					break;
				else
					perror("purge");
			}
		}

		if (MDSTHeader->EOSRcvrPage.EndOffset)
			sleep(1);
		else {
			MDSTHeader->EOSRcvrPage.StartOffset
					= MPList[MDSTHeader->MPListRPIdx].StartOffset;

			tMP.StartOffset = htonl(MDSTHeader->EOSRcvrPage.StartOffset);
			tMP.EndOffset = htonl(MDSTHeader->EOSRcvrPage.StartOffset
					+ sizeof(MDSTHeader->EOSRcvrBufr));

			//			printf("Recovery request for the end of stream after %d will be sent\n",MDSTHeader->EOSRcvrPage.StartOffset);

			if (sendto(iInfSock, &tMP, sizeof(tMP), 0,
					(struct sockaddr *) &(MDSTHeader->RecoveryClientAddr),
					sizeof(MDSTHeader->RecoveryClientAddr)) < 0)
				perror("send recovery request");
			else {
				if ((okunan = recv(iInfSock, MDSTHeader->EOSRcvrBufr,
						sizeof(MDSTHeader->EOSRcvrBufr), 0)) < 0) {
					if (errno != EAGAIN) {
						perror("recv");
						sleep(1);
					}
				} else {
					MDSTHeader->EOSRcvrPage.EndOffset
							= MDSTHeader->EOSRcvrPage.StartOffset + okunan;
					MDST_PrintTime("");
					printf("The page between %d and %d is recovered\n",
							MDSTHeader->EOSRcvrPage.StartOffset,
							MDSTHeader->EOSRcvrPage.EndOffset);
					MDST_ReleaseSEMWait();
					fflush(stdout);
				}
			}
		}

		if (3 == argc)
			write_back_up_file();
	}

	exit(0);
}

