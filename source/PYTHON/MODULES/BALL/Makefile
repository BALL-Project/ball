#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.1 2000/07/18 21:30:12 oliver Exp $
#



all: libBALLc.so

SIP_SOURCES_COMMON=\
	CPP/sipBALLSize.C\
	CPP/sipBALLIndex.C\
	CPP/sipBALLPosition.C\
	CPP/sipBALLVersionInfo.C

SIP_SOURCES_CONCEPT=\
	CPP/sipBALLAtomProcessor.C\
	CPP/sipBALLCompositeProcessor.C\
	CPP/sipBALLComposite.C\
	CPP/sipBALLEmbeddable.C\
	CPP/sipBALLNamedProperty.C\
	CPP/sipBALLObject.C\
	CPP/sipBALLPersistentObject.C\
	CPP/sipBALLProcessor.C\
	CPP/sipBALLPropertyManager.C\
	CPP/sipBALLSelectable.C\
	CPP/sipBALLUnaryAtomPredicate.C\
	CPP/sipBALLUnaryCompositePredicate.C

SIP_SOURCES_DATATYPE=\
	CPP/sipBALLGridIndex.C\
	CPP/sipBALLFloatPointGrid.C\
	CPP/sipBALLOptions.C\
	CPP/sipBALLRegularExpression.C\
	CPP/sipBALLString.C\
	CPP/sipBALLSubstring.C\
	CPP/sipBALLBit.C\
	CPP/sipBALLBitVector.C

SIP_SOURCES_ENERGY=

SIP_SOURCES_FORMAT=\
	CPP/sipBALLHINFile.C\
	CPP/sipBALLINIFile.C\
	CPP/sipBALLMOL2File.C\
	CPP/sipBALLPDBFile.C\
	CPP/sipBALLResourceEntry.C\
	CPP/sipBALLResourceFile.C\
	CPP/sipBALLXYZFile.C

SIP_SOURCES_KERNEL=\
	CPP/sipBALLAtom.C\
	CPP/sipBALLAtomType.C\
	CPP/sipBALLBaseFragment.C\
	CPP/sipBALLBond.C\
	CPP/sipBALLChain.C\
	CPP/sipBALLElement.C\
	CPP/sipBALLExpression.C\
	CPP/sipBALLExpressionPredicate.C\
	CPP/sipBALLFragment.C\
	CPP/sipBALLMolecule.C\
	CPP/sipBALLNucleotide.C\
	CPP/sipBALLNucleicAcid.C\
	CPP/sipBALLPDBAtom.C\
	CPP/sipBALLPTE_.C\
	CPP/sipBALLProtein.C\
	CPP/sipBALLResidue.C\
	CPP/sipBALLSecondaryStructure.C\
	CPP/sipBALLSelector.C\
	CPP/sipBALLSystem.C

SIP_SOURCES_MATHS=\
	CPP/sipBALLAngle.C\
	CPP/sipBALLBox3.C\
	CPP/sipBALLCircle3.C\
	CPP/sipBALLLine3.C\
	CPP/sipBALLMatrix4x4.C\
	CPP/sipBALLPlane3.C\
	CPP/sipBALLQuaternion.C\
	CPP/sipBALLSphere3.C\
	CPP/sipBALLSurface.C\
	CPP/sipBALLVector3.C\
	CPP/sipBALLVector4.C

SIP_SOURCES_MOLMEC_AMBER=\
	CPP/sipBALLAmberFF.C\
	CPP/sipBALLAmberBend.C\
	CPP/sipBALLAmberNonBonded.C\
	CPP/sipBALLAmberStretch.C\
	CPP/sipBALLAmberTorsion.C

SIP_SOURCES_MOLMEC_CHARMM=\
	CPP/sipBALLCharmmFF.C\
	CPP/sipBALLCharmmBend.C\
	CPP/sipBALLCharmmImproperTorsion.C\
	CPP/sipBALLCharmmNonBonded.C\
	CPP/sipBALLCharmmStretch.C\
	CPP/sipBALLCharmmTorsion.C

SIP_SOURCES_MOLMEC_MDSIMULATION=\
	CPP/sipBALLCanonicalMD.C\
	CPP/sipBALLMicroCanonicalMD.C\
	CPP/sipBALLMolecularDynamics.C

SIP_SOURCES_MOLMEC_MINIMIZATION=\
	CPP/sipBALLConjugateGradientMinimizer.C\
	CPP/sipBALLEnergyMinimizer.C

SIP_SOURCES_MOLMEC_COMMON=\
	CPP/sipBALLAtomVector.C\
	CPP/sipBALLConstant.C\
	CPP/sipBALLForceField.C\
	CPP/sipBALLForceFieldComponent.C\
	CPP/sipBALLPeriodicBoundary.C\
	CPP/sipBALLSnapShotManager.C\
	CPP/sipBALLGradient.C

SIP_SOURCES_NMR=

SIP_SOURCES_SYSTEM=\
	CPP/sipBALLFile.C\
	CPP/sipBALLOpenMode.C

SIP_SOURCES_PYTHON=\
	CPP/sipBALLPyAtomDict.C\
	CPP/sipBALLPyAtomList.C\
	CPP/sipBALLPyBondList.C\
	CPP/sipBALLPyBaseFragmentList.C\
	CPP/sipBALLPyCompositeDescriptorList.C\
	CPP/sipBALLPyFragmentList.C\
	CPP/sipBALLPyMoleculeList.C\
	CPP/sipBALLPyPDBAtomList.C\
	CPP/sipBALLPyResidueList.C\
	CPP/sipBALLPySecondaryStructureList.C\
	CPP/sipBALLPyChainList.C\
	CPP/sipBALLPyProteinList.C\
	CPP/sipBALLPyNucleotideList.C\
	CPP/sipBALLPyNucleicAcidList.C

SIP_SOURCES_SOLVATION=

SIP_SOURCES_STRUCTURE=\
	CPP/sipBALLClearChargeProcessor.C\
	CPP/sipBALLClearRadiusProcessor.C\
	CPP/sipBALLAssignRadiusProcessor.C\
	CPP/sipBALLAssignChargeProcessor.C\
	CPP/sipBALLTranslationProcessor.C\
	CPP/sipBALLTransformationProcessor.C\
	CPP/sipBALLBoundingBoxProcessor.C\
	CPP/sipBALLGeometricCenterProcessor.C\
	CPP/sipBALLFragmentDistanceCollector.C

SIP_SOURCES_VIEW_OPENGL_KERNEL=\
	CPP/sipBALLCompositeDescriptor.C\
	CPP/sipBALLMainControl.C

SIP_SOURCES_VIEW_OPENGL_WIDGETS=\
	CPP/sipBALLModularWidget.C\
	CPP/sipBALLPyWidget.C

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
	CPP/BALLcmodule.C

PYBALL_SOURCES=\
	extractors.C\
	pyBALLSipHelper.C\
	pyAtomList.C\
	pyBaseFragmentList.C\
	pyBondList.C\
	pyConstant.C\
	pyFragmentList.C\
	pyMoleculeList.C\
	pyPDBAtomList.C\
	pyResidueList.C\
	pyProteinList.C\
	pyChainList.C\
	pySecondaryStructureList.C\
	pyNucleotideList.C\
	pyNucleicAcidList.C

include /KM/usr/oliver/BALL/source/${BALL_BINFMT}/config.mak

SIPFLAGS += $(PYTHON_INCLUDES) $(SIP_INCLUDES)
CXXFLAGS += $(SIPFLAGS)

BALL_INCLUDES=$(BALLVIEW_INCLUDES)
BALL_LIBS=$(BALLVIEW_LIBS)

sip:
	@echo "running SIP..."
	@$(SIP) -c CPP -I SIP SIP/BALL.sip
	@echo "removing SIP header..."
	@for i in `ls CPP/sip*cpp` CPP/BALLcmodule.cpp; do cat $$i | sed 1,4d > $$i.tmp ; rm $$i ; done
	@echo "replacing new files..."
	@for i in `ls CPP/*cpp.tmp`; do if cmp $$i `basename $$i .cpp.tmp`.C 2>/dev/null >/dev/null ; then rm $$i ;  else mv $$i CPP/`basename $$i .cpp.tmp`.C ; echo "replacing file: `basename $$i .cpp.tmp`" ; fi ; done
			 	
SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

.SUFFIXES: .sip

clean:	
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES) $(SIBOBJECTS) libBALLc.so

libBALLc.so:	$(SIP_OBJECTS) $(PYBALL_OBJECTS)
	$(DYNAR) $(DYNAROPTS) libBALLc.so $(SIP_OBJECTS) $(PYBALL_OBJECTS) $(SIP_LIB) -L$(BALL_PATH)/lib/$(BINFMT) $(BALL_LIBS)

install: libBALLc.so
	cp libBALLc.so BALL.py BALL.pyc $(BALL_PATH)/lib/$(BINFMT)
