%Build Makefile.inc .o
SIP_SOURCES=\
	$B

SIP_OBJECTS	=	\
	$O

SIP_HEADERS = \
	$$(PYBALL_SOURCES:.C=.h) \

SIP_HEADER_FILES =\
	$H
	
%End
