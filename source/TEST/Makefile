DIRECTORY=TEST

EXECUTABLES = \
	AmberFF_test\
	AnalyticalGeometry_test\
	AnalyticalSES_test\
	Angle_test\
	Atom_test\
	AtomicContactEnergy_test\
	AtomVector_test\
	AutoDeleteable_test\
	BinarySearchTree_test\
	BitVector_test\
	Bond_test\
	BaseFragment_test\
	Box3_test\
	Chain_test\
	CharmmFF_test\
	ClassTest_test\
	Composite_test\
	Directory_test\
	DefaultProcessors_test\
	Embeddable_test\
	EnergyMinimizer_test\
	Enumerator_test\
	Expression_test\
	File_test\
	Fragment_test\
	FragmentDB_test\
	Gradient_test\
	Hash_test\
	HashMap_test\
	HashSet_test\
	HINFile_test\
	INIFile_test\
	KernelPredicate_test\
	Limits_test\
	Line3_test\
	List_test\
	LogStream_test\
	MathsCommon_test\
	Matrix44_test\
	MOL2File_test\
	Molecule_test\
	NumericalSAS_test\
	Nucleotide_test\
	OoiEnergy_test\
	Options_test\
	Path_test\
	PDBFile_test\
	Plane3_test\
	PointGrid_test\
	PoissonBoltzmann_test\
	PropertyManager_test\
	Protein_test\
	PTE_test\
	Quaternion_test\
	RegularExpression_test\
	Residue_test\
	ResourceFile_test\
	RTTI_test\
	RuleEvaluator_test\
	SecondaryStructure_test\
	Selectable_test\
	Socket_test\
	Sphere3_test\
	SplayTree_test\
	StandardPredicates_test\
	String_test\
	StructureMapper_test\
	TransformationApplicator_test\
	TranslationApplicator_test\
	Vector3_test\
	Vector4_test\
	VersionInfo_test\
	XYZFile_test

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)

include ../common.mak
include .Dependencies

# remove the library specific flags
BALL_CXXFLAGS=

# do not optimize tests (reduces compile time drastically)
CPP_MODE_FLAGS=$(CPP_MODE_FLAGS_NO_OPTIMIZATION)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)
# 
%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)
 	

test:	$(EXECUTABLES)
	@./runtests $(EXECUTABLES)

clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	SunWS_cache _*.TMP core a.out $(EXECUTABLES)
 
