# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.38.8.5 2007-06-12 20:17:53 oliver Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=PYTHON/EXTENSIONS
SUBDIRS=BALL

ifneq (BALL_HAS_VIEW, "")
  SUBDIRS+=VIEW
endif

.PHONY: all subdirs depend subdirsdepend clean

all:
	@$(MAKE) subdirs TARGET=$@

include ../../common.mak

subdirs:
	@for i in ${SUBDIRS}; do $(MAKE) -C $$i $(TARGET); done

clean:
	@$(MAKE) subdirs TARGET=$@

subdirsdepend:
	@$(MAKE) subdirs TARGET=depend

subdirssip:
	@$(MAKE) subdirs TARGET=sip

install:
	@$(MAKE) subdirs TARGET=$@
	$(CP) BALL.py $(PREFIX)/lib

depend: subdirsdepend
sip: subdirssip

