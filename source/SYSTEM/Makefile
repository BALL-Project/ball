include ../config.mak

DIRECTORY=SYSTEM

THISLIB=$(LIBNAME)

ifeq ($(BALL_HAS_VIEW),true)
	ASIO_SOURCES=$(BALL_HAS_ASIO:true=networking.C)
endif

CPP_SOURCES=$(ASIO_SOURCES)\
	directory.C\
	file.C\
	fileSystem.C\
	path.C\
	sysinfo.C\
	systemCalls.C\
	TCPTransfer.C\
	timer.C

ifeq ($(BALL_HAS_MPI),true)
	CPP_SOURCES+=\
	MPISupport.C
endif
		 
	
ifeq ($(BALL_HAS_VIEW),true)
	CPP_SOURCES+=\
	mutex.C
endif

include ../common.mak
-include .Dependencies
