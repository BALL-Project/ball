# include Makefile.template
CXXFLAGS+=$(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) 
SUBDIRS=

# remove the library specific flags
LIB_CXXFLAGS = $(NONLIB_CXXFLAGS)

bin_PROGRAMS=\
	proteinMapper\
	PDB2discover\
	amberEnergy\
	bindingEnergy\
	MOLVIEWClient\
	ramachandran\
	PDBChecker\
	addHydrogens\
	SASAreas
	
proteinMapper_SOURCES=proteinMapper.C
PDB2discover_SOURCES=PDB2discover.C
amberEnergy_SOURCES=amberEnergy.C
bindingEnergy_SOURCES=bindingEnergy.C
MOLVIEWClient_SOURCES=MOLVIEWClient.C
ramachandran_SOURCES=ramachandran.C
PDBChecker_SOURCES=PDBChecker.C
addHydrogens_SOURCES=addHydrogens.C
SASAreas_SOURCES=SASAreas.C
