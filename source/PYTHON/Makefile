
all: libBALLc.so
	
SIPSOURCES=\
	BALLcmodule.C\
	sipBALLAngle.C\
	sipBALLAtom.C\
	sipBALLBaseFragment.C\
	sipBALLBond.C\
	sipBALLChain.C\
	sipBALLComposite.C\
	sipBALLElement.C\
	sipBALLFragment.C\
	sipBALLMolecule.C\
	sipBALLNamedProperty.C\
	sipBALLObject.C\
	sipBALLPDBAtom.C\
	sipBALLPDBFile.C\
	sipBALLProtein.C\
	sipBALLPTE_.C\
	sipBALLPersistentObject.C\
	sipBALLPropertyManager.C\
	sipBALLResidue.C\
	sipBALLSelectable.C\
	sipBALLSecondaryStructure.C\
	sipBALLString.C\
	sipBALLSystem.C\
	sipBALLVector3.C

SIPOBJECTS=$(SIPSOURCES:.C=.o)



include ../config.mak

SIPFLAGS= -I/usr/local/include/python1.5 -I/usr/local/include/sip

clean:	default_clean
	@$(RM) -rf *.o *~ *.log ii_files Templates.DB	core a.out $(EXECUTABLES)

libBALLc.so:	$(SIPOBJECTS)
	$(CXX) -shared -o libBALLc.so $(SIPOBJECTS) -lsip -L/BALL/lib/Linux-i386-g++_2.95.2 -lBALL

.C.o:
	$(CXX) $(SIPFLAGS) $(CPPFLAGS) $(ADD_CPPFLAGS) $(CPP_MODE_FLAGS_NO_OPTIMIZATION) $(BALL_CPPFLAGS) $(BALL_INCLUDES) -c $*.C -o $@
 
