
CC=gcc

INCLUDE_DIR=../inc
BIN_DIR=../bin
LIBRARY_DIR=../lib

AM_CFLAGS=-I$(INCLUDE_DIR) -L$(LIBRARY_DIR)

LIBS+=-lMDST

noinst_HEADERS=$(INCLUDE_DIR)/MDST.h

bin_PROGRAMS =  \
	$(BIN_DIR)/MDST_ManageIPCS \
	$(BIN_DIR)/MDST_ManagePacketLog \
	$(BIN_DIR)/MDST_SetPrm

__BIN_DIR__MDST_ManageIPCS_SOURCES=MDST_ManageIPCS.c
__BIN_DIR__MDST_ManagePacketLog_SOURCES=MDST_ManagePacketLog.c
__BIN_DIR__MDST_SetPrm_SOURCES=MDST_SetPrm.c

install-data-local:
	mkdir -p ${prefix}/log
