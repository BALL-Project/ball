
EXECUTABLES=proteinMapper PDB2discover amberEnergy bindingEnergy

all:	$(EXECUTABLES)

include ../common.mak




clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES)

proteinMapper:	proteinMapper.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDB2discover:	PDB2discover.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

amberEnergy:	amberEnergy.o 
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

