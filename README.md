# BIST FastCOM

`Bist FastCOM` is highly scalable, very fast and reliable stream transmitter library. It is mainly designed to elavate the capabilities of `UDP` to provide reliable and fast data transmission by using.

Package comes with

## Tools

FastCOM also comes with handy command-line utilities that help u create/remove shared-memory, set shared-memory parameters and load data to the specified shared-memory.

### ManageIPCS

Creates and removes IPC with MDSTStreamHeader struct:

    struct StreamHeader
    {
       struct sockaddr_in InfAddr;
       struct sockaddr_in RecoveryClientAddr;
       struct sockaddr_in RecoveryServerAddr;
       unsigned int MPListMPIdx;
       unsigned int MPListRPIdx;
       unsigned int StreamSize;
       unsigned int MPListSize;
       unsigned int MPListSem;
       struct MissingPage EOSRcvrPage;
       char EOSRcvrBufr[MAX_RCVR_PAGE_LEN];
    };

This utility takes 4 arguments:

1. id of shared memory
2. operation (CREATE or REMOVE)
3. size of MissingPage struct (for recovery channel)
4. size of the stream data to be hold in shared memory

Example usage:

    $ ./ManageIPCS 2 CREATE 65535 268435455
    $ ./ManageIPCS 2 REMOVE

### MDST_SetPrm
   set the values for stream sender and receiver parameters in shared memory
   parameter1 gives the number for id of shared memory

   Ex:   MDST_SetPrm 2 INF_ADDR 0.0.0.0 2001
      MDST_SetPrm 2 RCVR_SRVR_ADDR 0.0.0.0 2002
      MDST_SetPrm 2 RCVR_CLNT_ADDR 127.0.0.1 2002

### MDST_ManagePacketLog
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


