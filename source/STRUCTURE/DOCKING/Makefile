DIRECTORY=STRUCTURE/DOCKING

THISLIB=$(LIBNAME)

FFT_DEPENDENT_SOURCES=$(BALL_HAS_FFTW:true=geometricFit.C)

CPP_SOURCES= \
	dockResult.C\
	dockingAlgorithm.C\
	conformationSet.C\
	energeticEvaluation.C\
	forceFieldEvaluation.C\
	amberEvaluation.C\
	randomEvaluation.C\
	$(FFT_DEPENDENT_SOURCES)

include ../../common.mak
include .Dependencies

ADD_CXXFLAGS=-I/opt/misc/openmpi-1.0/include -I/opt/misc/openmpi-1.0/include/openmpi/ompi
