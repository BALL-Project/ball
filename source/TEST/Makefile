default:	all

DIRECTORY=TEST

include ../common.mak
include .Dependencies

# remove the library specific flags
BALL_CXXFLAGS=

# do not optimize tests (reduces compile time drastically)
CPP_MODE_FLAGS=$(CPP_MODE_FLAGS_NO_OPTIMIZATION)

EXECUTABLES = \
	AmberFF_test\
	AnalyticalGeometry_test\
	AnalyticalSES_test\
	Angle_test\
	Atom_test\
	AtomicContactEnergy_test\
	AtomVector_test\
	BitVector_test\
	Bond_test\
	BaseFragment_test\
	Box3_test\
	Chain_test\
	CharmmFF_test\
	ClassTest_test\
	Composite_test\
	DefaultProcessors_test\
	EnergyMinimizer_test\
	Expression_test\
	Fragment_test\
	FragmentDB_test\
	Gradient_test\
	HashMap_test\
	HashSet_test\
	KernelPredicate_test\
	Limits_test\
	Line3_test\
	List_test\
	MathsCommon_test\
	Matrix44_test\
	MOL2File_test\
	Molecule_test\
	NumericalSAS_test\
	Nucleotide_test\
	Options_test\
	PDBFile_test\
	Plane3_test\
	PointGrid_test\
	PoissonBoltzmann_test\
	PropertyManager_test\
	Protein_test\
	PTE_test\
	Quaternion_test\
	Residue_test\
	ResourceFile_test\
	RTTI_test\
	SecondaryStructure_test\
	Socket_test\
	Sphere3_test\
	StandardPredicates_test\
	String_test\
	StructureMapper_test\
	TransformationApplicator_test\
	TranslationApplicator_test\
	Vector3_test\
	Vector4_test\
	XYZFile_test

CPP_SOURCES=$(EXECUTABLES:=.C)

all:	$(EXECUTABLES)

test:	$(EXECUTABLES)
	@./runtests $(EXECUTABLES)

AmberFF_test:	AmberFF_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

AnalyticalGeometry_test: AnalyticalGeometry_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

AnalyticalSES_test:	AnalyticalSES_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Angle_test:	Angle_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Atom_test:	Atom_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

AtomVector_test:	AtomVector_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

AtomicContactEnergy_test:	AtomicContactEnergy_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

BaseFragment_test:	BaseFragment_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Bond_test:	Bond_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

BitVector_test:	BitVector_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Box3_test:	Box3_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

CharmmFF_test:	CharmmFF_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Chain_test:	Chain_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Circle3_test:	Circle3_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

ClassTest_test:	ClassTest_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Composite_test:	Composite_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

DefaultProcessors_test:	DefaultProcessors_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

EnergyMinimizer_test:	EnergyMinimizer_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Expression_test:	Expression_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Fragment_test:	Fragment_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

FragmentDB_test:	FragmentDB_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Gradient_test:	Gradient_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

HashMap_test:	HashMap_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

HashSet_test:	HashSet_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

KernelPredicate_test:	KernelPredicate_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Limits_test:	Limits_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Line3_test:	Line3_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

List_test:	List_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

MathsCommon_test:	MathsCommon_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Matrix44_test:	Matrix44_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

MOL2File_test:	MOL2File_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Molecule_test:	Molecule_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

NucleicAcid_test:	NucleicAcid_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Nucleotide_test:	Nucleotide_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Options_test:	Options_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

NumericalSAS_test:	NumericalSAS_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PDBFile_test:	PDBFile_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Plane3_test:	Plane3_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PointGrid_test:	PointGrid_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PoissonBoltzmann_test:	PoissonBoltzmann_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PropertyManager_test:	PropertyManager_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Protein_test:	Protein_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

PTE_test:	PTE_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Quaternion_test: Quaternion_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Residue_test:	Residue_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

ResourceFile_test:	ResourceFile_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

RTTI_test:	RTTI_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

SecondaryStructure_test:	SecondaryStructure_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Socket_test:	Socket_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Skeleton_test:	Skeleton_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Sphere3_test:	Sphere3_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

StandardPredicates_test:	StandardPredicates_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

StructureMapper_test:	StructureMapper_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

String_test:	String_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

System_test:	System_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

TranslationApplicator_test:	TranslationApplicator_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

TransformationApplicator_test:	TransformationApplicator_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Vector3_test:	Vector3_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

Vector4_test:	Vector4_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)

XYZFile_test:	XYZFile_test.o
	$(CXX)	$(LDFLAGS) -o $@ $@.o $(LIBS)



clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	SunWS_cache _*.TMP core a.out $(EXECUTABLES)
 
