# include Makefile.template
CXXFLAGS+=$(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) 
SUBDIRS=

if USE_BALLVIEW
ADD_SRC=MOLVIEWClient
else
ADD_SRC=
endif

# remove the library specific flags
LIB_CXXFLAGS = $(NONLIB_CXXFLAGS)

bin_PROGRAMS=\
	proteinMapper\
	PDB2discover\
	amberEnergy\
	bindingEnergy\
	ramachandran\
	PDBChecker\
	addHydrogens\
	SASAreas\
	$(ADD_SRC)
	
proteinMapper_SOURCES=proteinMapper.C
PDB2discover_SOURCES=PDB2discover.C
amberEnergy_SOURCES=amberEnergy.C
bindingEnergy_SOURCES=bindingEnergy.C
ramachandran_SOURCES=ramachandran.C
PDBChecker_SOURCES=PDBChecker.C
addHydrogens_SOURCES=addHydrogens.C
SASAreas_SOURCES=SASAreas.C

if USE_BALLVIEW
MOLVIEWClient_SOURCES=MOLVIEWClient.C
MOLVIEWClient_LDADD=$(BALLVIEW_LIBS)
MOLVIEWClient_CXXFLAGS=$(CXXFLAGS) $(BALLVIEW_INCLUDES)
endif
