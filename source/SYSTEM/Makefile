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
ADD_CXXFLAGS=-I/opt/misc/openmpi-1.0/include/openmpi/ompi -I/opt/misc/openmpi-1.0/include/
