#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.14 2001/08/01 01:28:10 oliver Exp $
#
all: libBALLc.so

SIP_SOURCES_COMMON=\
	sipBALLIndex.C\
	sipBALLConstant.C\
	sipBALLPosition.C\
	sipBALLVersionInfo.C\
	sipBALLGlobalExceptionHandler.C\
	sipBALLGeneralException.C\
	sipBALLIndexOverflow.C\
	sipBALLIndexUnderflow.C\
	sipBALLNullPointer.C\
	sipBALLOutOfMemory.C\
	sipBALLOutOfGrid.C\
	sipBALLFileNotFound.C\
	sipBALLBufferOverflow.C\
	sipBALLDivisionByZero.C\
	sipBALLIllegalSelfOperation.C\
	sipBALLIncompatibleIterators.C\
	sipBALLInvalidFormat.C\
	sipBALLInvalidIterator.C\
	sipBALLInvalidRange.C\
	sipBALLNotImplemented.C\
	sipBALLSizeUnderflow.C

SIP_SOURCES_CONCEPT=\
	sipBALLAtomProcessor.C\
	sipBALLFragmentProcessor.C\
	sipBALLResidueProcessor.C\
	sipBALLCompositeProcessor.C\
	sipBALLComposite.C\
	sipBALLEmbeddable.C\
	sipBALLNamedProperty.C\
	sipBALLObject.C\
	sipBALLPersistentObject.C\
	sipBALLPreciseTime.C\
	sipBALLProcessor.C\
	sipBALLPropertyManager.C\
	sipBALLSelectable.C\
	sipBALLTimeStamp.C\
	sipBALLUnaryAtomPredicate.C\
	sipBALLUnaryCompositePredicate.C

SIP_SOURCES_DATATYPE=\
	sipBALLRegularData1D.C\
	sipBALLRegularData2D.C\
	sipBALLRegularData3D.C\
	sipBALLRegularData2DGridIndex.C\
	sipBALLRegularData3DGridIndex.C\
	sipBALLOptions.C\
	sipBALLRegularExpression.C\
	sipBALLString.C\
	sipBALLSubstring.C\
	sipBALLBit.C\
	sipBALLBitVector.C

SIP_SOURCES_ENERGY=

SIP_SOURCES_FORMAT=\
	sipBALLHINFile.C\
	sipBALLINIFile.C\
	sipBALLMOL2File.C\
	sipBALLPDBFile.C\
	sipBALLDCDFile.C\
	sipBALLDCDHeader.C\
	sipBALLTrajectoryFile.C\
	sipBALLResourceEntry.C\
	sipBALLResourceFile.C\
	sipBALLXYZFile.C

SIP_SOURCES_KERNEL=\
	sipBALLAtom.C\
	sipBALLAtomType.C\
	sipBALLAtomContainer.C\
	sipBALLBond.C\
	sipBALLChain.C\
	sipBALLElement.C\
	sipBALLExpression.C\
	sipBALLExpressionPredicate.C\
	sipBALLFragment.C\
	sipBALLMolecule.C\
	sipBALLNucleotide.C\
	sipBALLNucleicAcid.C\
	sipBALLPDBAtom.C\
	sipBALLPTE_.C\
	sipBALLProtein.C\
	sipBALLResidue.C\
	sipBALLSecondaryStructure.C\
	sipBALLSelector.C\
	sipBALLSystem.C

SIP_SOURCES_MATHS=\
	sipBALLAngle.C\
	sipBALLBox3.C\
	sipBALLCircle3.C\
	sipBALLLine3.C\
	sipBALLMatrix4x4.C\
	sipBALLPlane3.C\
	sipBALLQuaternion.C\
	sipBALLSphere3.C\
	sipBALLSurface.C\
	sipBALLVector2.C\
	sipBALLVector3.C\
	sipBALLVector4.C

SIP_SOURCES_MOLMEC_AMBER=\
	sipBALLAmberFF.C\
	sipBALLAmberBend.C\
	sipBALLAmberNonBonded.C\
	sipBALLAmberStretch.C\
	sipBALLAmberTorsion.C

SIP_SOURCES_MOLMEC_CHARMM=\
	sipBALLCharmmFF.C\
	sipBALLCharmmBend.C\
	sipBALLCharmmImproperTorsion.C\
	sipBALLCharmmNonBonded.C\
	sipBALLCharmmStretch.C\
	sipBALLCharmmTorsion.C

SIP_SOURCES_MOLMEC_MDSIMULATION=\
	sipBALLCanonicalMD.C\
	sipBALLMicroCanonicalMD.C\
	sipBALLMolecularDynamics.C

SIP_SOURCES_MOLMEC_MINIMIZATION=\
	sipBALLConjugateGradientMinimizer.C\
	sipBALLEnergyMinimizer.C

SIP_SOURCES_MOLMEC_COMMON=\
	sipBALLAtomVector.C\
	sipBALLChargeRuleProcessor.C\
	sipBALLForceField.C\
	sipBALLForceFieldComponent.C\
	sipBALLGradient.C\
	sipBALLPeriodicBoundary.C\
	sipBALLRadiusRuleProcessor.C\
	sipBALLRuleEvaluator.C\
	sipBALLRuleProcessor.C\
	sipBALLSnapShot.C\
	sipBALLSnapShotManager.C

SIP_SOURCES_NMR=

SIP_SOURCES_SYSTEM=\
	sipBALLFile.C\
	sipBALLOpenMode.C\
	sipBALLstd__openmode.C

SIP_SOURCES_PYTHON=\
	sipBALLPyAtomDict.C\
	sipBALLPyAtomList.C\
	sipBALLPyBondList.C\
	sipBALLPyAtomContainerList.C\
	sipBALLPyCompositeDescriptorList.C\
	sipBALLPyFragmentList.C\
	sipBALLPyMoleculeList.C\
	sipBALLPyPDBAtomList.C\
	sipBALLPyResidueList.C\
	sipBALLPySecondaryStructureList.C\
	sipBALLPyChainList.C\
	sipBALLPyProteinList.C\
	sipBALLPyNucleotideList.C\
	sipBALLPyNucleicAcidList.C

SIP_SOURCES_SOLVATION=

SIP_SOURCES_STRUCTURE=\
	sipBALLAddHydrogensProcessor.C\
	sipBALLAssignRadiusProcessor.C\
	sipBALLAssignChargeProcessor.C\
	sipBALLBoundingBoxProcessor.C\
	sipBALLBuildBondsProcessor.C\
	sipBALLClearChargeProcessor.C\
	sipBALLClearRadiusProcessor.C\
	sipBALLGeometricCenterProcessor.C\
	sipBALLFragmentDB.C\
	sipBALLFragmentDistanceCollector.C\
	sipBALLNormalizeNamesProcessor.C\
	sipBALLResidueChecker.C\
	sipBALLTranslationProcessor.C\
	sipBALLTransformationProcessor.C

SIP_SOURCES_VIEW_OPENGL_KERNEL=\
	sipBALLCompositeDescriptor.C\
	sipBALLMainControl.C

SIP_SOURCES_VIEW_OPENGL_WIDGETS=\
	sipBALLModularWidget.C\
	sipBALLPyWidget.C

SIP_SOURCES=\
	$(SIP_SOURCES_COMMON)\
	$(SIP_SOURCES_CONCEPT)\
	$(SIP_SOURCES_DATATYPE)\
	$(SIP_SOURCES_FORMAT)\
	$(SIP_SOURCES_KERNEL)\
	$(SIP_SOURCES_MATHS)\
	$(SIP_SOURCES_MOLMEC_AMBER)\
	$(SIP_SOURCES_MOLMEC_CHARMM)\
	$(SIP_SOURCES_MOLMEC_COMMON)\
	$(SIP_SOURCES_MOLMEC_MINIMIZATION)\
	$(SIP_SOURCES_MOLMEC_MDSIMULATION)\
	$(SIP_SOURCES_NMR)\
	$(SIP_SOURCES_PYTHON)\
	$(SIP_SOURCES_SOLVATION)\
	$(SIP_SOURCES_STRUCTURE)\
	$(SIP_SOURCES_SYSTEM)\
	$(SIP_SOURCES_VIEW_OPENGL_KERNEL)\
	$(SIP_SOURCES_VIEW_OPENGL_WIDGETS)\
	BALLcmodule.C

PYBALL_SOURCES=\
	extractors.C\
	pyBALLSipHelper.C\
	pyConstant.C\
	pyAtomList.C\
	pyAtomContainerList.C\
	pyBondList.C\
	pyFragmentList.C\
	pyMoleculeList.C\
	pyPDBAtomList.C\
	pyResidueList.C\
	pyProteinList.C\
	pyChainList.C\
	pySecondaryStructureList.C\
	pyNucleotideList.C\
	pyNucleicAcidList.C

include ../../../../config.mak

SIPFLAGS += $(PYTHON_INCLUDES) $(SIP_INCLUDES)
CXXFLAGS += $(SIPFLAGS)

BALL_INCLUDES +=$(BALLVIEW_INCLUDES)
BALL_LIBS=$(BALLVIEW_LIBS)

SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

CPP_SOURCES = $(SIP_SOURCES) $(PYBALL_SOURCES)

clean:	
	@$(RM) -rf *.o *~ *.log ii_files cxx_repository Templates.DB	core a.out $(EXECUTABLES) $(SIBOBJECTS) libBALLc.so

libBALLc.so:	$(SIP_OBJECTS) $(PYBALL_OBJECTS)
	$(DYNAR) $(DYNAROPTS) libBALLc.so $(SIP_OBJECTS) $(PYBALL_OBJECTS) $(SIP_LIB) -L$(BALL_PATH)/lib/$(BINFMT) $(BALL_LIBS)

install: libBALLc.so
	cp libBALLc.so BALL.py $(BALL_PATH)/lib/$(BINFMT)

depend:
	@echo "creating dependencies for $(DIRECTORY)..."
	@echo "" > .Dependencies
	@/usr/local/bin/g++ -M  -fPIC   $(CPP_SOURCES) $(ADD_CPP_SOURCES) $(STD_CPP_INCLUDES) $(ADD_INCLUDES) $(SIP_INCLUDES) $(PYTHON_INCLUDES) $(BALL_INCLUDES)  >.Dependencies

include .Dependencies

