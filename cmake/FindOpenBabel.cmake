# A simple find macro for detecting an OpenBabel installation
#
# This macro sets the following variables:
# - OPENBABEL_FOUND        if the library has been successfully detected
#                          this variable will be set to TRUE. Else it will have
#                          the value FALSE.
# - OPENBABEL_LIBRARIES    a listcontaining the OpenBabel libraries
# - OPENBABEL_INCLUDE_DIRS a listcontaining the OpenBabel include directories

FIND_PATH(openbabel_include
	NAMES openbabel/mol.h
	PATH_SUFFIXES openbabel-2.0
)

FIND_LIBRARY(openbabel_lib openbabel)

# Prepare the input for LIBFIND_PROCESS
SET(OPENBABEL_PROCESS_INCLUDES openbabel_include)
SET(OPENBABEL_PROCESS_LIBS openbabel_lib)

LIBFIND_PROCESS(OPENBABEL)
