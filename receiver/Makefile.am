
CC=gcc

INCLUDE_DIR=../inc
BIN_DIR=../bin
LIBRARY_DIR=../lib

AM_CFLAGS=-I$(INCLUDE_DIR) -L$(LIBRARY_DIR)

LIBS+=-lMDST

noinst_HEADERS=$(INCLUDE_DIR)/MDST.h

bin_PROGRAMS =  \
	$(BIN_DIR)/MDST_Receiver \
	$(BIN_DIR)/MDST_RecoveryClient

__BIN_DIR__MDST_Receiver_SOURCES=MDST_Receiver.c
__BIN_DIR__MDST_RecoveryClient_SOURCES=MDST_RecoveryClient.c

install-data-local:
	mkdir -p ${prefix}/log
