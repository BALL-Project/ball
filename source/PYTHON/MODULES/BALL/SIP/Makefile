#
# BALL Python support Makefile
#
# $Id: Makefile,v 1.3 2000/07/20 19:35:05 oliver Exp $
#

include ../../../../config.mak

sip:
	@echo "running SIP..."
	@$(SIP) -c ../CPP BALL.sip
	@echo "removing SIP header..."
	@for i in `ls ../CPP/sip*cpp` ../CPP/BALLcmodule.cpp; do cat $$i | sed 1,4d > $$i.tmp ; rm $$i ; done
	@echo "replacing new files..."
	@for i in `ls ../CPP/*cpp.tmp`; do if cmp $$i ../CPP/`basename $$i .cpp.tmp`.C 2>/dev/null >/dev/null ; then rm $$i ;  else cat $$i > ../CPP/`basename $$i .cpp.tmp`.C ; rm $$i; echo "replacing file: `basename $$i .cpp.tmp`" ; fi ; done

force_sip:
	@echo "running SIP..."
	@$(SIP) -c ../CPP BALL.sip
	@echo "removing SIP header..."
	@for i in `ls ../CPP/sip*cpp` ../CPP/BALLcmodule.cpp; do mv $$i ../CPP/`basename $$i .cpp.tmp`.C ; done
			 	
SIP_OBJECTS			=	$(SIP_SOURCES:.C=.o)
PYBALL_OBJECTS 	= $(PYBALL_SOURCES:.C=.o)

.SUFFIXES: .sip
