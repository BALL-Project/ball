#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.10 2000/06/27 22:09:41 oliver Exp $
#

all: libBALLc.so


SIP_SOURCES=\
	BALLcmodule.C\
	sipBALLAmberFF.C\
	sipBALLAngle.C\
	sipBALLAtom.C\
	sipBALLAtomVector.C\
	sipBALLBaseFragment.C\
	sipBALLBond.C\
	sipBALLBox3.C\
	sipBALLChain.C\
	sipBALLComposite.C\
	sipBALLConjugateGradientMinimizer.C\
	sipBALLConstant.C\
	sipBALLElement.C\
	sipBALLEnergyMinimizer.C\
	sipBALLFile.C\
	sipBALLFragment.C\
	sipBALLForceField.C\
	sipBALLForceFieldComponent.C\
	sipBALLGradient.C\
	sipBALLHINFile.C\
	sipBALLMolecule.C\
	sipBALLNamedProperty.C\
	sipBALLObject.C\
	sipBALLOptions.C\
	sipBALLMOL2File.C\
	sipBALLPDBAtom.C\
	sipBALLPDBFile.C\
	sipBALLProtein.C\
	sipBALLNucleotide.C\
	sipBALLNucleicAcid.C\
	sipBALLPTE_.C\
	sipBALLPersistentObject.C\
	sipBALLPeriodicBoundary.C\
	sipBALLPropertyManager.C\
	sipBALLResidue.C\
	sipBALLSelectable.C\
	sipBALLSecondaryStructure.C\
	sipBALLSnapShotManager.C\
	sipBALLString.C\
	sipBALLSystem.C\
	sipBALLVector3.C\
	sipBALLXYZFile.C\
	sipBALLPyAtomList.C\
	sipBALLPyFragmentList.C\
	sipBALLPyMoleculeList.C\
	sipBALLPyPDBAtomList.C\
	sipBALLPyResidueList.C\
	sipBALLPySecondaryStructureList.C\
	sipBALLPyChainList.C\
	sipBALLPyProteinList.C\
	sipBALLPyNucleotideList.C\
	sipBALLPyNucleicAcidList.C

PYBALL_SOURCES=\
	extractors.C\
	pyConstant.C\
	pyAtomList.C\
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


sip:
	$(SIP) -c . BALL.sip
	for i in `ls ./sip*cpp`; do mv $$i ./`basename $$i .cpp`.C ; done
	mv ./BALLcmodule.cpp ./BALLcmodule.C
			 	
SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

.SUFFIXES: .sip

clean:	
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES) $(SIBOBJECTS) libBALLc.so

libBALLc.so:	$(SIP_OBJECTS) $(PYBALL_OBJECTS)
	$(DYNAR) $(DYNAROPTS) libBALLc.so $(SIP_OBJECTS) $(PYBALL_OBJECTS) $(SIP_LIB) -L$(BALL_PATH)/lib/$(BINFMT) -lBALL

