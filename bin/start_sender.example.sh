#!/usr/bin/env bash

MD_PATH=`pwd`/..
MD_BIN=$MD_PATH/bin

$MD_BIN/MDST_ManageIPCS    1  CREATE            65535       268435455
$MD_BIN/MDST_SetPrm        1  INF_ADDR          127.0.0.1   2001
$MD_BIN/MDST_SetPrm        1  RCVR_SRVR_ADDR    0.0.0.0     2002
$MD_BIN/MDST_SetPrm        1  RCVR_CLNT_ADDR    0.0.0.0     2002

$MD_BIN/MDST_Sender           1  2000  &
$MD_BIN/MDST_RecoveryServer   1  &
