# include Makefile.template
CXXFLAGS+=$(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) 
SUBDIRS=

# remove the library specific flags
LIB_CXXFLAGS = $(NONLIB_CXXFLAGS)

bin_PROGRAMS = \
	KernelIteration_bench\
	AmberFF_bench\
	CharmmFF_bench\
	PDB_bench\
	PoissonBoltzmann_bench\
	MolmecSupport_bench

KernelIteration_bench_SOURCES=KernelIteration_bench.C
AmberFF_bench_SOURCES=AmberFF_bench.C
CharmmFF_bench_SOURCES=CharmmFF_bench.C
PDB_bench_SOURCES=PDB_bench.C
PoissonBoltzmann_bench_SOURCES=PoissonBoltzmann_bench.C
MolmecSupport_bench_SOURCES=MolmecSupport_bench.C

benchmark:
	@./runbenchmarks $(PROGRAMS)
