DIRECTORY=STRUCTURE/DOCKING

THISLIB=$(LIBNAME)

FFT_DEPENDENT_SOURCES=$(BALL_HAS_FFTW:true=geometricFit.C)

CPP_SOURCES= \
	dockingAlgorithm.C\
	conformationSet.C\
	$(FFT_DEPENDENT_SOURCES)

include ../../common.mak
include .Dependencies
