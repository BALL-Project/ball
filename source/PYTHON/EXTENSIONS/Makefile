#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.1 2001/09/03 16:53:02 oliver Exp $
#

.SUFFIXES: .sip
include ../../config.mak

SIP_OPTS=-c . -o -e -s ".C" -m "Makefile.inc"

sip:
	@echo "running SIP..."
	@$(SIP) $(SIP_OPTS) BALL.sip

			 	
SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

-include Makefile.inc

 
CLEAN_FILES = \
	BALL.pyc \
	BALL.py \
	sipBALLVersion.h \
	Makefile.inc \
	$(PYBALL_SOURCES) \
	$(PYBALL_HEADERS) \
	$(PYBALL_OBJECTS)
 
clean:
	@echo "removing object files, libraries, and source/header files produced by SIP..."
	@-$(RM) $(CLEAN_FILES) 2>/dev/null
