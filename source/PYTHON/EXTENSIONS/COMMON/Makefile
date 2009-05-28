DIRECTORY=PYTHON/EXTENSIONS/COMMON

all: dummy

include ../../../common.mak

PYBALL_SOURCES = \
	pyCompositeList.C

PYBALL_OBJECTS  = $(PYBALL_SOURCES:.C=.$(OBJECT_EXTENSION))

CPP_SOURCES = $(PYBALL_SOURCES)
CPP_OBJECTS = $(PYBALL_OBJECTS)

CLEAN_FILES = \
	$(CPP_OBJECTS) \
	$(PYBALL_LIBTOOL_OBJECTS) \

dummy: $(CPP_OBJECTS)

clean:
	@echo "removing object files, libraries, and source/header files produced by SIP..."
	@-$(RM) -f $(CLEAN_FILES) 2>/dev/null
	@-$(RM) -f .libs/*

install:

-include .Dependencies
