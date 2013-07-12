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
Assign values for socket addresses described in the struct below.

This utility takes 4 arguments:

1. id of shared memory
2. which socket value to assign (INF_ADDR, RCVR_SRVR_ADDR or RCVR_CLNT_ADDR)
3. IPV4 address for the socket
4. port number for the socket

Example usage:

    $ ./MDST_SetPrm 2 INF_ADDR        0.0.0.0    2001
    $ ./MDST_SetPrm 2 RCVR_SRVR_ADDR  0.0.0.0    2002
    $ ./MDST_SetPrm 2 RCVR_CLNT_ADDR  127.0.0.1  2002

### MDST_ManagePacketLog
You can save the data from shared memory to a file OR you can load data to shared memory at once.

This utility takes 3 arguments:

1. id of shared memory
2. operation (LOAD or SAVE)
3. filename to save or load

Example usage:

    $ ./MDST_ManagePacketLog 2 LOAD data_load_file
    $ ./MDST_ManagePacketLog 2 SAVE data_save_file

### MDST SENDER

#### MDST_Sender
Sends shared memory data via broadcast.

This utility takes 2 arguments:

1. id of shared memory
2. loop number for send the other paket

Example usage:

    $ ./MDST_Sender 2 2000
    $ ./MDST_Sender 2 0

#### MDST_RecoveryServer
Sends shared memory data if broadcast not received by receiver.

This utility takes 1 arguments:

1. id of shared memory

Example usage:

    $ ./MDST_RecoveryServer 2

### MDST RECEIVER

#### MDST_Receiver
Receives data via broadcast and write to shared memory.

This utility takes 1 arguments:

1. id of shared memory

Example usage:

    $ ./MDST_Receiver 1

#### MDST_RecoveryClient
Requests data if broadcast not received and receives data that send from MDST_RecoveryServer.

This utility takes 1 arguments:

1. id of shared memory

Example usage:

    $ ./MDST_RecoveryClient 1
