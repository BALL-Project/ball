DIRECTORY=MOLMEC/CHARMM

all: test

THISLIB=$(LIBNAME)

CPP_SOURCES=\
	charmm.C\
	charmmBend.C\
	charmmImproperTorsion.C\
	charmmNonBonded.C\
	charmmStretch.C\
	charmmTorsion.C


include ../../common.mak
include .Dependencies

test:	$(OBJECTS) charmm_test.o
	echo $(OBJECTS)
	$(CXX) -o test charmm_test.o $(OBJECTS) $(LIBS)
		

