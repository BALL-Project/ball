#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.6 2000/06/21 08:50:04 oliver Exp $
#

all: libBALLc.so


sip:
	$(SIP) -c . BALL.sip
	for i in `ls ./sip*cpp`; do mv $$i ./`basename $$i .cpp`.C ; done
	mv ./BALLcmodule.cpp ./BALLcmodule.C
			 
	
SIPSOURCES=\
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
	sipBALLXYZFile.C

SIPFLAGS += $(PYTHON_INCLUDES) $(SIP_INCLUDES)
CXXFLAGS += $(SIPFLAGS)

include ../${BALL_BINFMT}/config.mak

	
sipBALLAtom.C:	atom.sip

SIPOBJECTS=$(SIPSOURCES:.C=.o)

.SUFFIXES: .sip

.sip.C:
	sip

clean:	
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES) $(SIBOBJECTS) libBALLc.so

libBALLc.so:	$(SIPOBJECTS)
	$(DYNAR) $(DYNAROPTS) libBALLc.so $(SIPOBJECTS) $(SIP_LIB) -L$(BALL_PATH)/lib/$(BINFMT) -lBALL

test:
	@echo "CXXFLAGS = "$(CXXFLAGS)
	@echo "SIPFLAGS = "$(SIPFLAGS)
	@echo "PY_INCLUDES = "$(PYTHON_INCLUDES)
