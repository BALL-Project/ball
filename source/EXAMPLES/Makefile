# $Id: Makefile,v 1.16 2001/06/24 16:37:57 oliver Exp $
#========================
# BALL/EXAMPLES Makefile
#========================

DIRECTORY=EXAMPLES
EXECUTABLES=proteinMapper PDB2discover amberEnergy bindingEnergy ramachandran PDBChecker addHydrogens MOLVIEWClient

all:	$(EXECUTABLES)

include ../common.mak

# remove the library specific flags
LIB_CXXFLAGS = $(NONLIB_CXXFLAGS)

clean:	default_clean
	@-$(RM) $(EXECUTABLES) *.o 2>/dev/null

proteinMapper:	proteinMapper.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDB2discover:	PDB2discover.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

amberEnergy:	amberEnergy.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

bindingEnergy:	bindingEnergy.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

MOLVIEWClient:	MOLVIEWClient.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(BALLVIEW_LIBS)

ramachandran:	ramachandran.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDBChecker:	PDBChecker.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

addHydrogens:	addHydrogens.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

SASAreas:	SASAreas.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)
