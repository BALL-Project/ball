#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.17 2000/07/18 08:30:00 oliver Exp $
#



all: libBALLc.so

SIP_SOURCES_COMMON=\
	sipBALLSize.C\
	sipBALLIndex.C\
	sipBALLPosition.C\
	sipBALLVersionInfo.C

SIP_SOURCES_CONCEPT=\
	sipBALLAtomProcessor.C\
	sipBALLCompositeProcessor.C\
	sipBALLComposite.C\
	sipBALLEmbeddable.C\
	sipBALLNamedProperty.C\
	sipBALLObject.C\
	sipBALLPersistentObject.C\
	sipBALLProcessor.C\
	sipBALLPropertyManager.C\
	sipBALLSelectable.C\
	sipBALLUnaryAtomPredicate.C\
	sipBALLUnaryCompositePredicate.C

SIP_SOURCES_DATATYPE=\
	sipBALLGridIndex.C\
	sipBALLFloatPointGrid.C\
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
	sipBALLResourceEntry.C\
	sipBALLResourceFile.C\
	sipBALLXYZFile.C

SIP_SOURCES_KERNEL=\
	sipBALLAtom.C\
	sipBALLAtomType.C\
	sipBALLBaseFragment.C\
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
	sipBALLConstant.C\
	sipBALLForceField.C\
	sipBALLForceFieldComponent.C\
	sipBALLPeriodicBoundary.C\
	sipBALLSnapShotManager.C\
	sipBALLGradient.C

SIP_SOURCES_NMR=

SIP_SOURCES_SYSTEM=\
	sipBALLFile.C\
	sipBALLOpenMode.C

SIP_SOURCES_PYTHON=\
	sipBALLPyAtomDict.C\
	sipBALLPyAtomList.C\
	sipBALLPyBondList.C\
	sipBALLPyBaseFragmentList.C\
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
	sipBALLClearChargeProcessor.C\
	sipBALLClearRadiusProcessor.C\
	sipBALLAssignRadiusProcessor.C\
	sipBALLAssignChargeProcessor.C\
	sipBALLTranslationProcessor.C\
	sipBALLTransformationProcessor.C\
	sipBALLBoundingBoxProcessor.C\
	sipBALLGeometricCenterProcessor.C\
	sipBALLFragmentDistanceCollector.C

SIP_SOURCES_VIEW_OPENGL_KERNEL=\
	sipBALLCompositeDescriptor.C\
	sipBALLMainControl.C

SIP_SOURCES_VIEW_OPENGL_WIDGETS=\
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
	BALLcmodule.C

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

include ../${BALL_BINFMT}/config.mak

SIPFLAGS += $(PYTHON_INCLUDES) $(SIP_INCLUDES)
CXXFLAGS += $(SIPFLAGS)

BALL_INCLUDES=$(BALLVIEW_INCLUDES)
BALL_LIBS=$(BALLVIEW_LIBS)

sip:
	@echo "running SIP..."
	@$(SIP) -c . BALL.sip
	@echo "removing SIP header..."
	@for i in `ls ./sip*cpp` BALLcmodule.cpp; do cat $$i | sed 1,4d > $$i.tmp ; rm $$i ; done
	@echo "replacing new files..."
	@for i in `ls ./*cpp.tmp`; do if cmp $$i `basename $$i .cpp.tmp`.C 2>/dev/null >/dev/null ; then rm $$i ;  else mv $$i `basename $$i .cpp.tmp`.C ; echo "replacing file: `basename $$i .cpp.tmp`" ; fi ; done
			 	
SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

.SUFFIXES: .sip

clean:	
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES) $(SIBOBJECTS) libBALLc.so

libBALLc.so:	$(SIP_OBJECTS) $(PYBALL_OBJECTS)
	$(DYNAR) $(DYNAROPTS) libBALLc.so $(SIP_OBJECTS) $(PYBALL_OBJECTS) $(SIP_LIB) -L$(BALL_PATH)/lib/$(BINFMT) $(BALL_LIBS)

install: libBALLc.so
	cp libBALLc.so BALL.py BALL.pyc $(BALL_PATH)/lib/$(BINFMT)
