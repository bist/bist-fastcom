
CC=gcc

INCLUDE_DIR=../inc
BIN_DIR=../bin
LIBRARY_DIR=../lib

AM_CFLAGS=-I$(INCLUDE_DIR) -L$(LIBRARY_DIR)

LIBS+=-lMDST

noinst_HEADERS=$(INCLUDE_DIR)/MDST.h

bin_PROGRAMS =  \
	$(BIN_DIR)/MDST_Sender \
	$(BIN_DIR)/MDST_RecoveryServer

__BIN_DIR__MDST_Sender_SOURCES=MDST_Sender.c
__BIN_DIR__MDST_RecoveryServer_SOURCES=MDST_RecoveryServer.c

install-data-local:
	mkdir -p ${prefix}/log
