#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.11 2000/06/28 20:30:51 oliver Exp $
#

all: libBALLc.so

SIP_SOURCES_COMMON=\
	sipBALLSize.C\
	sipBALLIndex.C\
	sipBALLPosition.C\
	sipBALLVersionInfo.C

SIP_SOURCES_CONCEPT=\
	sipBALLComposite.C\
	sipBALLNamedProperty.C\
	sipBALLObject.C\
	sipBALLPersistentObject.C\
	sipBALLPropertyManager.C\
	sipBALLSelectable.C

SIP_SOURCES_DATATYPE=\
	sipBALLOptions.C\
	sipBALLString.C\
	sipBALLBit.C\
	sipBALLBitVector.C

SIP_SOURCES_ENERGY=

SIP_SOURCES_FORMAT=\
	sipBALLHINFile.C\
	sipBALLMOL2File.C\
	sipBALLPDBFile.C\
	sipBALLXYZFile.C

SIP_SOURCES_KERNEL=\
	sipBALLAtom.C\
	sipBALLBaseFragment.C\
	sipBALLBond.C\
	sipBALLChain.C\
	sipBALLElement.C\
	sipBALLFragment.C\
	sipBALLMolecule.C\
	sipBALLNucleotide.C\
	sipBALLNucleicAcid.C\
	sipBALLPDBAtom.C\
	sipBALLPTE_.C\
	sipBALLProtein.C\
	sipBALLResidue.C\
	sipBALLSecondaryStructure.C\
	sipBALLSystem.C

SIP_SOURCES_MATHS=\
	sipBALLAngle.C\
	sipBALLBox3.C\
	sipBALLVector3.C

SIP_SOURCES_MOLMEC_AMBER=\
	sipBALLAmberFF.C

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
	sipBALLFile.C

SIP_SOURCES_PYTHON=\
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

SIP_SOURCES_SOLVATION=

SIP_SOURCES_STRUCTURE=

SIP_SOURCES=\
	$(SIP_SOURCES_COMMON)\
	$(SIP_SOURCES_CONCEPT)\
	$(SIP_SOURCES_DATATYPE)\
	$(SIP_SOURCES_FORMAT)\
	$(SIP_SOURCES_KERNEL)\
	$(SIP_SOURCES_MATHS)\
	$(SIP_SOURCES_MOLMEC_AMBER)\
	$(SIP_SOURCES_MOLMEC_COMMON)\
	$(SIP_SOURCES_MOLMEC_MINIMIZATION)\
	$(SIP_SOURCES_NMR)\
	$(SIP_SOURCES_PYTHON)\
	$(SIP_SOURCES_SOLVATION)\
	$(SIP_SOURCES_STRUCTURE)\
	$(SIP_SOURCES_SYSTEM)\
	BALLcmodule.C

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

