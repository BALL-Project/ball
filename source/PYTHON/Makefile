SIP						= /KM/comp-bio/BALL-contrib/sip/bin/sip
SIP_INCLUDE		= -I/KM/comp-bio/BALL-contrib/sip/include/sip
PY_INCLUDE    = -I/opt/misc/src/Python-1.5.2/Include -I/opt/misc/src/Python-1.5.2

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
	sipBALLMolecule.C\
	sipBALLNamedProperty.C\
	sipBALLObject.C\
	sipBALLOptions.C\
	sipBALLPDBAtom.C\
	sipBALLPDBFile.C\
	sipBALLHINFile.C\
	sipBALLProtein.C\
	sipBALLPTE_.C\
	sipBALLPersistentObject.C\
	sipBALLPeriodicBoundary.C\
	sipBALLPropertyManager.C\
	sipBALLResidue.C\
	sipBALLSelectable.C\
	sipBALLSecondaryStructure.C\
	sipBALLSnapShot.C\
	sipBALLSnapShotManager.C\
	sipBALLString.C\
	sipBALLSystem.C\
	sipBALLVector3.C

	
sipBALLAtom.C:	atom.sip

SIPOBJECTS=$(SIPSOURCES:.C=.o)

.SUFFIXES: .sip

.sip.C:
	sip

include ../Solaris-5.7-sparc-g++_2.95.2/config.mak

SIPFLAGS= $(PY_INCLUDE) $(SIP_INCLUDE)

clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES)

libBALLc.so:	$(SIPOBJECTS)
	$(CXX) -shared -o libBALLc.so $(SIPOBJECTS) -lsip -L/BALL/lib/Linux-i386-g++_2.95.2 -lBALL

.C.o:
	$(CXX) $(SIPFLAGS) $(CPPFLAGS) $(ADD_CPPFLAGS) $(CPP_MODE_FLAGS_NO_OPTIMIZATION) $(BALL_CPPFLAGS) $(BALL_INCLUDES) -c $*.C -o $@
 
