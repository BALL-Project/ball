DIRECTORY=TEST

XDR_DEPENDENT_EXECUTABLES=\
	XDRPersistenceManager_test

EXECUTABLES_BASIC = \
	ClassTest_test\
	RegularExpression_test\
	GlobalTypes_test\
	Limits_test\
	VersionInfo_test\
	RTTI_test

EXECUTABLES_MATHS = \
	MathsCommon_test\
	AnalyticalGeometry_test\
	Angle_test\
	Vector2_test\
	Vector3_test\
	Vector4_test\
	Quaternion_test\
	Matrix44_test\
	NumericalIntegrator_test\
	Line3_test\
	Circle3_test\
	Box3_test\
	Plane3_test\
	Sphere3_test\
	Surface_test\
	Function_test\
	PiecewiseFunction_test\
	PiecewisePolynomial_test

EXECUTABLES_SYSTEM = \
	Directory_test\
	FileSystem_test\
	File_test\
	Path_test\
	PreciseTime_test\
	Socket_test\
	Timer_test\
	TimeStamp_test\
	TCPTransfer_test

EXECUTABLES_DATATYPES = \
	String_test\
	BitVector_test\
	List_test\
	Hash_test\
	HashMap_test\
	StringHashMap_test\
	HashSet_test\
	HashGrid3_test\
	Options_test\
	Triple_test\
	Quadruple_test\
	RegularData1D_test\
	RegularData2D_test\
	RegularData3D_test

EXECUTABLES_CONCEPTS =$(BALL_HAS_XDR:true=$(XDR_DEPENDENT_EXECUTABLES))\
	LogStream_test\
	AutoDeletable_test\
	Object_test\
	PersistentObject_test\
	PersistenceManager_test\
	TextPersistenceManager_test\
	Selectable_test\
	PropertyManager_test\
	Composite_test\
	Embeddable_test\
	RandomAccessIterator_test

EXECUTABLES_KERNEL = \
	PTE_test\
	Atom_test\
	AtomIterator_test\
	Bond_test\
	AtomContainer_test\
	Fragment_test\
	Chain_test\
	Molecule_test\
	SecondaryStructure_test\
	System_test\
	Protein_test\
	PDBAtom_test\
	NucleicAcid_test\
	Nucleotide_test\
	Residue_test\
	Expression_test\
	KernelPredicate_test\
	Selector_test\
	RuleEvaluator_test\
	StandardPredicates_test\
	DefaultProcessors_test\
	FragmentDB_test\
	ForEach_test\
	GlobalKernel_test

EXECUTABLES_MOLMEC = \
	Gradient_test\
	AtomVector_test\
	AmberFF_test\
	CharmmFF_test\
	EnergyMinimizer_test\
	SnapShot_test\
	SnapShotManager_test

EXECUTABLES_NMR = \
	AssignShiftProcessor_test\
	RandomCoilShiftProcessor_test\
	AnisotropyShiftProcessor_test\
	ClearShiftProcessor_test\
	CreateSpectrumProcessor_test\
	EFShiftProcessor_test\
	HaighMallionShiftProcessor_test\
	JohnsonBoveyShiftProcessor_test\
	ShiftModel_test\
	ShiftModule_test\
	Spectrum_test\
	Peak_test\
	PeakList_test

EXECUTABLES_FORMATS =\
	LineBasedFile_test\
	BinaryFileAdaptor_test\
	INIFile_test\
	Parameters_test\
	ParameterSection_test\
	ResourceFile_test\
	GenericMolFile_test\
	HINFile_test\
	MOLFile_test\
	SDFile_test\
	MOL2File_test\
	NMRStarFile_test\
	DCDFile_test\
	PDBFile_test\
	TrajectoryFile_test\
	XYZFile_test

EXECUTABLES_ENERGY = \
	AtomicContactEnergy_test\
	ComposedEnergyProcessor_test\
	EnergyProcessor_test\
	OoiEnergy_test

EXECUTABLES_STRUCTURE = \
	Enumerator_test\
	AnalyticalSES_test\
	NumericalSAS_test\
	RadialDistributionFunction_test\
	RDFIntegrator_test\
	RDFParameter_test\
	RDFSection_test\
	StructureMapper_test\
	TransformationApplicator_test\
	TranslationApplicator_test

EXECUTABLES_SOLVATION = \
	PoissonBoltzmann_test\
	SolventDescriptor_test\
	SolventParameter_test\
	ClaverieParameter_test\
	ReissCavFreeEnergyProcessor_test\
  UhligCavFreeEnergyProcessor_test\
  PCMCavFreeEnergyProcessor_test\
  PierottiCavFreeEnergyProcessor_test\
	Pair6_12RDFIntegrator_test\
	Pair6_12InteractionEnergyProcessor_test\
	PairExpInteractionEnergyProcessor_test\
	PairExpRDFIntegrator_test

EXECUTABLES_QSAR = \
	Descriptor_test\
	SimpleDescriptors_test

EXECUTABLES = \
	$(EXECUTABLES_BASIC)\
	$(EXECUTABLES_SYSTEM)\
	$(EXECUTABLES_MATHS)\
	$(EXECUTABLES_DATATYPES)\
	$(EXECUTABLES_CONCEPTS)\
	$(EXECUTABLES_KERNEL)\
	$(EXECUTABLES_FORMATS)\
	$(EXECUTABLES_MOLMEC)\
	$(EXECUTABLES_NMR)\
	$(EXECUTABLES_ENERGY)\
	$(EXECUTABLES_STRUCTURE)\
	$(EXECUTABLES_QSAR)\
	$(EXECUTABLES_SOLVATION)

# default target
all:	$(EXECUTABLES)

# required for clean and depend
CPP_SOURCES=$(EXECUTABLES:=.C)
OBJECTS=$(EXECUTABLES:=.o)

include ../common.mak
include .Dependencies

# remove the library specific flags
# and replace them by the aplication specific
# flags
LIB_CXXFLAGS=$(NONLIB_CXXFLAGS)

# do not optimize tests (reduces compile time drastically)
CPP_MODE_FLAGS=$(CPP_MODE_FLAGS_NO_OPTIMIZATION)

.o:
	$(CXX) $(CXXFLAGS) $*.o $(LDFLAGS) -o $@ $(LIBS)
# 
%%.C:
	$(CXX) $(CXXFLAGS) $(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(BALL_CXXFLAGS) $(BALL_INCLUDES) $*.C $(LDFLAGS) -o $@ $(LIBS)

test: info basic_test system_test maths_test datatypes_test concepts_test kernel_test formats_test energy_test molmec_test nmr_test structure_test qsar_test solvation_test
	@./runtests -success

info:
	@./runtests -info

basic_test:	$(EXECUTABLES_BASIC)
	@./runtests -section "BASIC" $(TEST_OPTIONS) $(EXECUTABLES_BASIC)

system_test:	$(EXECUTABLES_SYSTEM)
	@./runtests -section "SYSTEM" $(TEST_OPTIONS) $(EXECUTABLES_SYSTEM)

maths_test:	$(EXECUTABLES_MATHS)
	@./runtests -section "MATHS" $(TEST_OPTIONS) $(EXECUTABLES_MATHS)

datatypes_test:	$(EXECUTABLES_DATATYPES)
	@./runtests -section "DATATYPES" $(TEST_OPTIONS) $(EXECUTABLES_DATATYPES)
	
concepts_test:	$(EXECUTABLES_CONCEPTS)
	@./runtests -section "CONCEPTS" $(TEST_OPTIONS) $(EXECUTABLES_CONCEPTS)
	
kernel_test:	$(EXECUTABLES_KERNEL)
	@./runtests -section "KERNEL" $(TEST_OPTIONS) $(EXECUTABLES_KERNEL)
	
formats_test:	$(EXECUTABLES_FORMATS)
	@./runtests -section "FORMATS" $(TEST_OPTIONS) $(EXECUTABLES_FORMATS)
	
energy_test:	$(EXECUTABLES_ENERGY)
	@./runtests -section "ENERGY" $(TEST_OPTIONS) $(EXECUTABLES_ENERGY)
	
molmec_test:	$(EXECUTABLES_MOLMEC)
	@./runtests -section "MOLMEC" $(TEST_OPTIONS) $(EXECUTABLES_MOLMEC)
	
nmr_test:	$(EXECUTABLES_NMR)
	@./runtests -section "NMR" $(TEST_OPTIONS) $(EXECUTABLES_NMR)

structure_test:	$(EXECUTABLES_STRUCTURE)
	@./runtests -section "STRUCTURE" $(TEST_OPTIONS) $(EXECUTABLES_STRUCTURE)

qsar_test:	$(EXECUTABLES_QSAR)
	@./runtests -section "QSAR" $(TEST_OPTIONS) $(EXECUTABLES_QSAR)

solvation_test:	$(EXECUTABLES_SOLVATION)
	@./runtests -section "SOLVATION" $(TEST_OPTIONS) $(EXECUTABLES_SOLVATION)

clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	SunWS_cache _*.TMP core a.out $(EXECUTABLES)
 
#MOLFile_test: MOLFile_test.o ../FORMAT/MOLFile.o ../FORMAT/lineBasedFile.o
#	$(CXX) $(CXXFLAGS) MOLFile_test.o ../FORMAT/MOLFile.o ../FORMAT/lineBasedFile.o $(LDFLAGS) -o $@ $(LIBS)
