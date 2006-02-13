DIRECTORY=SYSTEM

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	directory.C\
	file.C\
	fileSystem.C\
	MPISupport.C\
	path.C\
	socket.C\
	sysinfo.C\
	TCPTransfer.C\
	timer.C

include ../common.mak
-include .Dependencies

#workaround!
ADD_CXXFLAGS=-I/usr/include/lam
