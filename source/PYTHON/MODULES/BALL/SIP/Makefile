#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.2 2000/07/18 23:27:07 oliver Exp $
#

include ../../../../config.mak

sip:
	@echo "running SIP..."
	@$(SIP) -c ../CPP BALL.sip
	@echo "removing SIP header..."
	@for i in `ls ../CPP/sip*cpp` ../CPP/BALLcmodule.cpp; do cat $$i | sed 1,4d > $$i.tmp ; rm $$i ; done
	@echo "replacing new files..."
	@for i in `ls ../CPP/*cpp.tmp`; do if cmp $$i ../CPP/`basename $$i .cpp.tmp`.C 2>/dev/null >/dev/null ; then rm $$i ;  else mv $$i ../CPP/`basename $$i .cpp.tmp`.C ; echo "replacing file: `basename $$i .cpp.tmp`" ; fi ; done
			 	
SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

.SUFFIXES: .sip
