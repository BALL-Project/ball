default:	all

include ../common.mak

DEXECUTABLES	= AmberFF_test
EXECUTABLES = \
	AmberFF_test\
	AnalyticalSES_test\
	Atom_test\
	Bond_test\
	BaseFragment_test\
	Composite_test\
	Fragment_test\
	FragmentDB_test\
	Limits_test\
	NumericalSAS_test\
	Options_test\
	PointGrid_test\
	PoissonBoltzmann_test\
	PropertyManager_test\
	String_test\
	StructureMapper_test\
	TransformationApplicator_test\
	TranslationApplicator_test

t:	t.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o

all:	$(EXECUTABLES)

test:	$(EXECUTABLES)
	@./runtests $(EXECUTABLES)

AmberFF_test:	AmberFF_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

AnalyticalSES_test:	AnalyticalSES_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Atom_test:	Atom_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

BaseFragment_test:	BaseFragment_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Bond_test:	Bond_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Composite_test:	Composite_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Fragment_test:	Fragment_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

FragmentDB_test:	FragmentDB_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Limits_test:	Limits_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Options_test:	Options_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

NumericalSAS_test:	NumericalSAS_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PointGrid_test:	PointGrid_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PoissonBoltzmann_test:	PoissonBoltzmann_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PropertyManager_test:	PropertyManager_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Skeleton_test:	Skeleton_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

tmp:	tmp.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

StructureMapper_test:	StructureMapper_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

String_test:	String_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

TranslationApplicator_test:	TranslationApplicator_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

TransformationApplicator_test:	TransformationApplicator_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)



clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES)
