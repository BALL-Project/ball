DIRECTORY=BENCHMARKS

EXECUTABLES = \
	AmberFF_bench

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)

include ../common.mak
include .Dependencies

# remove the library specific flags
BALL_CXXFLAGS=

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)
# 
%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

bench:	$(EXECUTABLES)
	@./runbenchmarks $(EXECUTABLES)

clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	SunWS_cache _*.TMP core a.out $(EXECUTABLES)
 
