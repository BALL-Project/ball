DIRECTORY=SYSTEM

THISLIB=$(LIBNAME)

ASIO_SOURCES=$(BALL_HAS_ASIO:true=networking.C)

CPP_SOURCES=$(ASIO_SOURCES)\
	directory.C\
	file.C\
	fileSystem.C\
	mutex.C\
	path.C\
	sysinfo.C\
	systemCalls.C\
	TCPTransfer.C\
	timer.C

include ../common.mak
-include .Dependencies
