%Makefile Makefile.inc .o
PYBALL_SOURCES=\
	sip_helper.C \
	$S

PYBALL_OBJECTS	=	\
	sip_helper \
	$O

PYBALL_HEADERS=$$(PYBALL_SOURCES:.C=.h)

SIP_HELPER_SOURCES = sip_helper$c
SIP_HELPER_LDADD = $$(SIP_LDLIBDIRS) $$(SIP_LDXFLAGS)


PYBALL_OBJECTS 	= $$(PYBALL_SOURCES:.C=.o)

sipBALLDeclBALL.h:	sipBALLVersion.h

sipBALLVersion.h:	sip_helper
	./sip_helper > sipBALLVersion.h

libBALLc.so:	$$(PYBALL_SOURCES) 
%End
