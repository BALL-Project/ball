// $Id: Makefile,v 1.8 2000/01/17 10:32:49 oliver Exp $
//========================
// BALL/EXAMPLES Makefile
//========================

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

