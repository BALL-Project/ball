%Makefile Makefile.inc .o

SIP_SOURCES=\
	$S

SIP_OBJECTS	=	\
	$O

SIP_HEADERS = \
	$$(PYBALL_SOURCES:.C=.h) \
	
%End
