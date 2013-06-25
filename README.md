# Market Data Stream Transmitter (MDST)

MDST TOOLS
----------
* MDST_ManageIPCS
	creates ipcs structure MDSTStreamHeader
	parameter1 gives the number for id of shared memory
	parameter2 gives operation(CREATE or REMOVE)
	parameter3 gives the list size of MDSTMissingPage
	parameter4 gives the size for Stream shared memory

	Ex: MDST_ManageIPCS 2 CREATE 65535 268435455
		MDST_ManageIPCS 2 REMOVE

* MDST_SetPrm
	set the values for stream sender and receiver parameters in shared memory
	parameter1 gives the number for id of shared memory

	Ex:	MDST_SetPrm 2 INF_ADDR 0.0.0.0 2001
		MDST_SetPrm 2 RCVR_SRVR_ADDR 0.0.0.0 2002
		MDST_SetPrm 2 RCVR_CLNT_ADDR 127.0.0.1 2002

* MDST_ManagePacketLog
	write or read data from stream shared memory
	parameter1 gives the number for id of shared memory
	Ex: MDST_ManagePacketLog 2 LOAD data_file
		MDST_ManagePacketLog 2 SAVE data_save_file

MDST SENDER
-----------
* MDST_Sender
	sends shared memory stream data via broadcast
	parameter1 gives shared memory id
	parameter2 gives dummy loop number
	Ex: MDST_Sender 2 2000

* MDST_RecoveryServer
	sends shared memory stream if broadcast not received
	parameter1 gives shared memory id
	Ex: MDST_RecoveryServer 2

MDST RECEIVER
-------------
* MDST_Receiver
	receives stream data via broadcast and write to shared memory
	parameter1 gives shared memory id
	Ex: MDST_Receiver 2

* MDST_RecoveryClient
	receives stream data if broadcast not received
	parameter1 gives shared memory id
	Ex: MDST_RecoveryClient 2






