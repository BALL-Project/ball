%Makefile Makefile.inc .o

SIP_SOURCES=\
	$S

SIP_OBJECTS	=	\
	$O

SIP_HEADERS = \
	$$(PYBALL_SOURCES:.C=.h) \
	

SIP_HELPER_SOURCES = sip_helper$c
SIP_HELPER_EXE = sip_helper

SIP_HELPER_LDADD = $$(SIP_LDLIBDIRS) $$(SIP_LDXFLAGS)

sipBALLDeclBALL.h:	sipBALLVersion.h

sipBALLVersion.h:	sip_helper
	./sip_helper > sipBALLVersion.h
%End
