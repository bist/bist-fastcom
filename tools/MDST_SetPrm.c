/*
 * MDST_SetPrm.c
 *
 * Author:	Kadir Turkoglu	<kadir.turkoglu@borsaistanbul.com>
 * Author:	Murat Akzeybek	<murat.akzeybek@borsaistanbul.com>
 */
#include "MDST.h"

#define CHCK_PRM(n) (strncmp(argv[2],n,strlen(n)) == 0)

int fnGetServPort(char *prmServName)
{
	struct servent *tServ;

	tServ = getservbyname(prmServName, "udp");

	if (!tServ) {
		if (!errno)
			if (atoi(prmServName))
				return htons(atoi(prmServName));
		MDST_ErrorExit("fnGetServPort");
	}

	return tServ->s_port;
}

void fnGetHostAddr(char *prmHostName, struct in_addr *prmAddr)
{
	struct hostent *tHost;

	tHost = gethostbyname(prmHostName);

	if (!tHost) {
		switch (h_errno) {
		case HOST_NOT_FOUND:
			MDST_ErrorExit("The specified host is unknown.\n");
			break;

		case NO_ADDRESS:
			MDST_ErrorExit(
					"The requested name is valid but does not have an IP address.\n");
			break;

		case NO_RECOVERY:
			MDST_ErrorExit("A non-recoverable name server error occurred.\n");
			break;

		case TRY_AGAIN:
			MDST_ErrorExit(
					"A temporary error occurred on an authoritative name server.  Try again later.\n");
			break;

		default:
			MDST_ErrorExit("Unknown error\n");
			break;
		}
	}

	memcpy(prmAddr, tHost->h_addr,tHost->h_length);
}

int main(int argc, char *argv[])
{
	MDST_AttachIPCS(atoi(argv[1]));

	if (CHCK_PRM("INF_ADDR"))
	{
		fnGetHostAddr(argv[3],&(MDSTHeader->InfAddr.sin_addr));
		MDSTHeader->InfAddr.sin_family				= PF_INET;
		MDSTHeader->InfAddr.sin_port				= fnGetServPort(argv[4]);

		printf("Interface IP address:port is set to %s:%s\n",argv[3],argv[4]);
	}
	else if (CHCK_PRM("RCVR_SRVR_ADDR"))
	{
		fnGetHostAddr(argv[3],&(MDSTHeader->RecoveryServerAddr.sin_addr));
		MDSTHeader->RecoveryServerAddr.sin_family	= PF_INET;
		MDSTHeader->RecoveryServerAddr.sin_port		= fnGetServPort(argv[4]);
		printf("Recovery server IP address:port is set to %s:%s\n",argv[3],argv[4]);
	}
	else if (CHCK_PRM("RCVR_CLNT_ADDR"))
	{
		fnGetHostAddr(argv[3],&(MDSTHeader->RecoveryClientAddr.sin_addr));
		MDSTHeader->RecoveryClientAddr.sin_family	= PF_INET;
		MDSTHeader->RecoveryClientAddr.sin_port		= fnGetServPort(argv[4]);
		printf("Recovery Client IP address:port is set to %s:%s\n",argv[3],argv[4]);
	}
	else printf("Unknown paremeter <%s> !!\n",argv[1]);

	fflush(stdout);
	exit (0);
}



