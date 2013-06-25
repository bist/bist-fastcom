#!/usr/bin/env bash

USER=`whoami`

KILL_BKG_PROC ()
{
   echo Killing $1
   TPID=` ps -C$1 -opid,user | grep $USER | awk '{print $1}'`

   if [ "$TPID" != "" ]
   then
      echo $TPID
      kill $TPID
      echo $1 killed
   else
      echo $1 is not running !!!!
   fi
}

KILL_BKG_PROC  MDST_Receiver
KILL_BKG_PROC  MDST_RecoveryClient

MD_PATH=`pwd`/..
MD_BIN=$MD_PATH/bin

$MD_BIN/MDST_ManageIPCS    1  REMOVE
