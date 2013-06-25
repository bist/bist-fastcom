/*
 * MDST.h
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */

#ifndef MDST_H_
#define MDST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <netinet/in.h>
#include <errno.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>

#ifdef DEBUG
#define DBG(...)	printf("[DBG] " __VA_ARGS__);	\
		fflush(stdout);
#else
#define DBG(...)
#endif

#define	KEYBASE						0x00570000
#define MAX_STREAM_PAGE_SIZE		1460
#define MAX_RCVR_PAGE_LEN 			65280

struct MDSTMissingPage
{
	unsigned int StartOffset;
	unsigned int EndOffset;
};

struct MDSTStreamHeader
{
	struct sockaddr_in InfAddr;
	struct sockaddr_in RecoveryClientAddr;
	struct sockaddr_in RecoveryServerAddr;
	unsigned int MPListMPIdx;
	unsigned int MPListRPIdx;
	unsigned int StreamSize;
	unsigned int MPListSize;
	unsigned int MPListSem;
	struct MDSTMissingPage EOSRcvrPage;
	char EOSRcvrBufr[MAX_RCVR_PAGE_LEN];
};

struct MDSTStreamHeader *MDSTHeader;
struct MDSTMissingPage *MPList;
unsigned char *MDStream;
unsigned int MPListSem;

void MDST_PrintTime(char *tLabel);
void MDST_ErrorExit(char *pMsg);
int MDST_AttachIPCS(unsigned char prmStreamId);
int MDST_ReleaseSEMWait();
int MDST_HangSEMWait();
int MDST_read(unsigned int soffset, unsigned char *data, unsigned int len);
void MDST_write(unsigned char *data, unsigned int len);

#endif /* MDST_H_ */
