DIRECTORY=TEST
SUBDIRS=

CXXFLAGS+=$(ADD_CXXFLAGS) $(CPP_MODE_FLAGS) $(LIB_CXXFLAGS) $(BALL_INCLUDES) $(ADD_INCLUDES) 

if BALL_HAS_XDR
XDR_DEPENDENT_EXECUTABLES=XDRPersistenceManager_test
else
XDR_DEPENDENT_EXECUTABLES=
endif

EXECUTABLES_BASIC = \
	ClassTest_test\
	RegularExpression_test\
	GlobalTypes_test\
	Limits_test\
	VersionInfo_test\
	RTTI_test

ClassTest_test_SOURCES=ClassTest_test.C
RegularExpression_test_SOURCES=RegularExpression_test.C
GlobalTypes_test_SOURCES=GlobalTypes_test.C
Limits_test_SOURCES=Limits_test.C
VersionInfo_test_SOURCES=VersionInfo_test.C
RTTI_test_SOURCES=RTTI_test.C

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

MathsCommon_test_SOURCES=MathsCommon_test.C
AnalyticalGeometry_test_SOURCES=AnalyticalGeometry_test.C
Angle_test_SOURCES=Angle_test.C
Vector2_test_SOURCES=Vector2_test.C
Vector3_test_SOURCES=Vector3_test.C
Vector4_test_SOURCES=Vector4_test.C
Quaternion_test_SOURCES=Quaternion_test.C
Matrix44_test_SOURCES=Matrix44_test.C
NumericalIntegrator_test_SOURCES=NumericalIntegrator_test.C
Line3_test_SOURCES=Line3_test.C
Circle3_test_SOURCES=Circle3_test.C
Box3_test_SOURCES=Box3_test.C
Plane3_test_SOURCES=Plane3_test.C
Sphere3_test_SOURCES=Sphere3_test.C
Surface_test_SOURCES=Surface_test.C
Function_test_SOURCES=Function_test.C
PiecewiseFunction_test_SOURCES=PiecewiseFunction_test.C
PiecewisePolynomial_test_SOURCES=PiecewisePolynomial_test.C

EXECUTABLES_SYSTEM = \
	FileSystem_test\
	Directory_test\
	Socket_test\
	TCPTransfer_test\
	File_test\
	Path_test\
	PreciseTime_test\
	Timer_test\
	TimeStamp_test\
	TransformationManager_test

FileSystem_test_SOURCES=FileSystem_test.C
Directory_test_SOURCES=Directory_test.C
Socket_test_SOURCES=Socket_test.C
TCPTransfer_test_SOURCES=TCPTransfer_test.C
File_test_SOURCES=File_test.C
Path_test_SOURCES=Path_test.C
PreciseTime_test_SOURCES=PreciseTime_test.C
Timer_test_SOURCES=Timer_test.C
TimeStamp_test_SOURCES=TimeStamp_test.C
TransformationManager_test_SOURCES=TransformationManager_test.C

EXECUTABLES_DATATYPES = \
	BitVector_test\
	List_test\
	Hash_test\
	HashMap_test\
	StringHashMap_test\
	HashSet_test\
	HashGrid3_test\
	Options_test\
	String_test\
	Substring_test\
	Triple_test\
	Quadruple_test\
	RegularData1D_test\
	RegularData2D_test\
	RegularData3D_test

BitVector_test_SOURCES=BitVector_test.C
List_test_SOURCES=List_test.C
Hash_test_SOURCES=Hash_test.C
HashMap_test_SOURCES=HashMap_test.C
StringHashMap_test_SOURCES=StringHashMap_test.C
HashSet_test_SOURCES=HashSet_test.C
HashGrid3_test_SOURCES=HashGrid3_test.C
Options_test_SOURCES=Options_test.C
String_test_SOURCES=String_test.C
Substring_test_SOURCES=Substring_test.C
Triple_test_SOURCES=Triple_test.C
Quadruple_test_SOURCES=Quadruple_test.C
RegularData1D_test_SOURCES=RegularData1D_test.C
RegularData2D_test_SOURCES=RegularData2D_test.C
RegularData3D_test_SOURCES=RegularData3D_test.C

EXECUTABLES_CONCEPTS = \
	Enumerator_test\
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
	RandomAccessIterator_test\
	$(XDR_DEPENDENT_EXECUTABLES)

Enumerator_test_SOURCES=Enumerator_test.C
LogStream_test_SOURCES=LogStream_test.C
AutoDeletable_test_SOURCES=AutoDeletable_test.C
Object_test_SOURCES=Object_test.C
PersistentObject_test_SOURCES=PersistentObject_test.C
PersistenceManager_test_SOURCES=PersistenceManager_test.C
TextPersistenceManager_test_SOURCES=TextPersistenceManager_test.C
Selectable_test_SOURCES=Selectable_test.C
PropertyManager_test_SOURCES=PropertyManager_test.C
Composite_test_SOURCES=Composite_test.C
Embeddable_test_SOURCES=Embeddable_test.C
RandomAccessIterator_test_SOURCES=RandomAccessIterator_test.C

if BALL_HAS_XDR
XDRPersistenceManager_test_SOURCES=XDRPersistenceManager_test.C
endif

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
	ExpressionParser_test\
	KernelPredicate_test\
	Selector_test\
	RuleEvaluator_test\
	StandardPredicates_test\
	DefaultProcessors_test\
	FragmentDB_test\
	ForEach_test\
	GlobalKernel_test

PTE_test_SOURCES=PTE_test.C
Atom_test_SOURCES=Atom_test.C
AtomIterator_test_SOURCES=AtomIterator_test.C
Bond_test_SOURCES=Bond_test.C
AtomContainer_test_SOURCES=AtomContainer_test.C
Fragment_test_SOURCES=Fragment_test.C
Chain_test_SOURCES=Chain_test.C
Molecule_test_SOURCES=Molecule_test.C
SecondaryStructure_test_SOURCES=SecondaryStructure_test.C
System_test_SOURCES=System_test.C
Protein_test_SOURCES=Protein_test.C
PDBAtom_test_SOURCES=PDBAtom_test.C
NucleicAcid_test_SOURCES=NucleicAcid_test.C
Nucleotide_test_SOURCES=Nucleotide_test.C
Residue_test_SOURCES=Residue_test.C
Expression_test_SOURCES=Expression_test.C
ExpressionParser_test_SOURCES=ExpressionParser_test.C
KernelPredicate_test_SOURCES=KernelPredicate_test.C
Selector_test_SOURCES=Selector_test.C
RuleEvaluator_test_SOURCES=RuleEvaluator_test.C
StandardPredicates_test_SOURCES=StandardPredicates_test.C
DefaultProcessors_test_SOURCES=DefaultProcessors_test.C
FragmentDB_test_SOURCES=FragmentDB_test.C
ForEach_test_SOURCES=ForEach_test.C
GlobalKernel_test_SOURCES=GlobalKernel_test.C

EXECUTABLES_MOLMEC = \
	Gradient_test\
	AtomVector_test\
	MolmecSupport_test\
	AmberFF_test\
	CharmmFF_test\
	EnergyMinimizer_test\
	SnapShot_test\
	SnapShotManager_test

Gradient_test_SOURCES=Gradient_test.C
AtomVector_test_SOURCES=AtomVector_test.C
MolmecSupport_test_SOURCES=MolmecSupport_test.C
AmberFF_test_SOURCES=AmberFF_test.C
CharmmFF_test_SOURCES=CharmmFF_test.C
EnergyMinimizer_test_SOURCES=EnergyMinimizer_test.C
SnapShot_test_SOURCES=SnapShot_test.C
SnapShotManager_test_SOURCES=SnapShotManager_test.C

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

AssignShiftProcessor_test_SOURCES=AssignShiftProcessor_test.C
RandomCoilShiftProcessor_test_SOURCES=RandomCoilShiftProcessor_test.C
AnisotropyShiftProcessor_test_SOURCES=AnisotropyShiftProcessor_test.C
ClearShiftProcessor_test_SOURCES=ClearShiftProcessor_test.C
CreateSpectrumProcessor_test_SOURCES=CreateSpectrumProcessor_test.C
EFShiftProcessor_test_SOURCES=EFShiftProcessor_test.C
HaighMallionShiftProcessor_test_SOURCES=HaighMallionShiftProcessor_test.C
JohnsonBoveyShiftProcessor_test_SOURCES=JohnsonBoveyShiftProcessor_test.C
ShiftModel_test_SOURCES=ShiftModel_test.C
ShiftModule_test_SOURCES=ShiftModule_test.C
Spectrum_test_SOURCES=Spectrum_test.C
Peak_test_SOURCES=Peak_test.C
PeakList_test_SOURCES=PeakList_test.C

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

LineBasedFile_test_SOURCES=LineBasedFile_test.C
BinaryFileAdaptor_test_SOURCES=BinaryFileAdaptor_test.C
INIFile_test_SOURCES=INIFile_test.C
Parameters_test_SOURCES=Parameters_test.C
ParameterSection_test_SOURCES=ParameterSection_test.C
ResourceFile_test_SOURCES=ResourceFile_test.C
GenericMolFile_test_SOURCES=GenericMolFile_test.C
HINFile_test_SOURCES=HINFile_test.C
MOLFile_test_SOURCES=MOLFile_test.C
SDFile_test_SOURCES=SDFile_test.C
MOL2File_test_SOURCES=MOL2File_test.C
NMRStarFile_test_SOURCES=NMRStarFile_test.C
DCDFile_test_SOURCES=DCDFile_test.C
PDBFile_test_SOURCES=PDBFile_test.C
TrajectoryFile_test_SOURCES=TrajectoryFile_test.C
XYZFile_test_SOURCES=XYZFile_test.C

EXECUTABLES_ENERGY = \
	AtomicContactEnergy_test\
	ComposedEnergyProcessor_test\
	EnergyProcessor_test\
	OoiEnergy_test

AtomicContactEnergy_test_SOURCES=AtomicContactEnergy_test.C
ComposedEnergyProcessor_test_SOURCES=ComposedEnergyProcessor_test.C
EnergyProcessor_test_SOURCES=EnergyProcessor_test.C
OoiEnergy_test_SOURCES=OoiEnergy_test.C

EXECUTABLES_STRUCTURE = \
	AnalyticalSES_test\
	MolecularGraph_test\
	NumericalSAS_test\
	Peptides_test\
	RadialDistributionFunction_test\
	RDFIntegrator_test\
	RDFParameter_test\
	RDFSection_test\
	ReconstructFragmentProcessor_test\
	SmilesParser_test\
	StructureMapper_test\
	TransformationApplicator_test\
	TranslationApplicator_test
	
AnalyticalSES_test_SOURCES=AnalyticalSES_test.C
MolecularGraph_test_SOURCES=MolecularGraph_test.C
NumericalSAS_test_SOURCES=NumericalSAS_test.C
Peptides_test_SOURCES=Peptides_test.C
RadialDistributionFunction_test_SOURCES=RadialDistributionFunction_test.C
RDFIntegrator_test_SOURCES=RDFIntegrator_test.C
RDFParameter_test_SOURCES=RDFParameter_test.C
RDFSection_test_SOURCES=RDFSection_test.C
ReconstructFragmentProcessor_test_SOURCES=ReconstructFragmentProcessor_test.C
SmilesParser_test_SOURCES=SmilesParser_test.C
StructureMapper_test_SOURCES=StructureMapper_test.C
TransformationApplicator_test_SOURCES=TransformationApplicator_test.C
TranslationApplicator_test_SOURCES=TranslationApplicator_test.C

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

PoissonBoltzmann_test_SOURCES=PoissonBoltzmann_test.C
SolventDescriptor_test_SOURCES=SolventDescriptor_test.C
SolventParameter_test_SOURCES=SolventParameter_test.C
ClaverieParameter_test_SOURCES=ClaverieParameter_test.C
ReissCavFreeEnergyProcessor_test_SOURCES=ReissCavFreeEnergyProcessor_test.C
UhligCavFreeEnergyProcessor_test_SOURCES=UhligCavFreeEnergyProcessor_test.C
PCMCavFreeEnergyProcessor_test_SOURCES=PCMCavFreeEnergyProcessor_test.C
PierottiCavFreeEnergyProcessor_test_SOURCES=PierottiCavFreeEnergyProcessor_test.C
Pair6_12RDFIntegrator_test_SOURCES=Pair6_12RDFIntegrator.C
Pair6_12InteractionEnergyProcessor_test_SOURCES=Pair6_12InteractionEnergyProcessor.C
PairExpInteractionEnergyProcessor_test_SOURCES=PairExpInteractionEnergyProcessor_test.C
PairExpRDFIntegrator_test_SOURCES=PairExpRDFIntegrator_test.C

EXECUTABLES_QSAR = \
	Descriptor_test\
	SimpleDescriptors_test

Descriptor_test_SOURCES=Descriptor_test.C
SimpleDescriptors_test_SOURCES=SimpleDescriptors_test.C


bin_PROGRAMS= \
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

Enumerator_test_LDADD=../CONCEPT/enumerator.o
