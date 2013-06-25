#!/usr/bin/env bash

MD_PATH=`pwd`/..
MD_BIN=$MD_PATH/bin

$MD_BIN/MDST_ManageIPCS    2  CREATE            65535       268435455
$MD_BIN/MDST_SetPrm        2  INF_ADDR          0.0.0.0     2001
$MD_BIN/MDST_SetPrm        2  RCVR_SRVR_ADDR    0.0.0.0     2002
$MD_BIN/MDST_SetPrm        2  RCVR_CLNT_ADDR    127.0.0.1   2002

$MD_BIN/MDST_Receiver         2  &
$MD_BIN/MDST_RecoveryClient   2  &
