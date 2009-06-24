# -*- Mode: C++; tab-width: 2; -*-
# vi: set ts=2:
#
# $Id: Makefile,v 1.7.14.2 2007/03/25 23:30:45 amoll Exp $
#
# Author:
#   Oliver Kohlbacher
#

DIRECTORY=VIEW/RENDERING

THISLIB=$(LIBNAME)

VIEW_RENDERING_SUBDIRS=\
	RENDERERS

CPP_SOURCES=\
	glDisplayList.C\
	glRenderer.C\
	renderer.C\
	renderSetup.C\
	POVRenderer.C\
	STLRenderer.C\
	tilingRenderer.C\
	VRMLRenderer.C\
	vertexBuffer.C\
	glRenderWindow.C\
	glOffscreenTarget.C\
	renderWindow.C\
	pixelFormat.C\
	raytracingRenderer.C

include ../../common.mak
-include .Dependencies

depend:	subdirsdepend 

default_clean:	subdirsclean

collectlib: subdirscollectlib

dirs: dummy
	@$(MAKE) subdirs SUBDIRS="$(VIEW_RENDERING_SUBDIRS)" TARGET=default

subdirs:	dummy
	@set -e; for i in $(SUBDIRS); do cd $$i; if $(MAKE) $(TARGET); then cd .. ; else cd .. ; exit 1 ; fi ; done

subdirsdepend:	dummy
	@echo "" > .Dependencies
	@for i in $(VIEW_RENDERING_SUBDIRS); do ( cd $$i; echo "" >.Dependencies; cd .. ) ; done
	@$(MAKE) subdirs SUBDIRS="$(VIEW_RENDERING_SUBDIRS)" TARGET=depend

subdirsclean:
	@$(MAKE) subdirs SUBDIRS="$(VIEW_RENDERING_SUBDIRS)" TARGET=default_clean

subdirscollectlib:	dummy
	@$(MAKE) subdirs SUBDIRS="$(VIEW_RENDERING_SUBDIRS)" TARGET=collectlib LIBNAME="$(THISLIB)"

dummy:
