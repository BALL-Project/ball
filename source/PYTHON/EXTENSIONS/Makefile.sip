# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile.sip,v 1.4 2003/08/26 09:18:11 oliver Exp $
#


%Makefile Makefile.inc .o

SIP_SOURCES=\
	$S

SIP_OBJECTS	=	\
	$O

SIP_HEADERS = \
	$$(PYBALL_SOURCES:.C=.h) \
	
%End
