# $Id: Makefile,v 1.11 2000/03/25 22:45:06 oliver Exp $
#========================
# BALL/EXAMPLES Makefile
#========================

DIRECTORY=EXAMPLES
EXECUTABLES=proteinMapper PDB2discover amberEnergy bindingEnergy ramachandran PDBChecker addHydrogens MOLVIEWClient

all:	$(EXECUTABLES)

include ../common.mak

clean:	default_clean
	@$(RM) $(EXECUTABLES)

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
