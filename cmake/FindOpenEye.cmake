# A simple find macro for detecting an OpenEye installation
#
# This macro sets the following variables:
# - OPENEYE_FOUND        if the library has been successfully detected
#                        this variable will be set to TRUE. Else it will have
#                        the value FALSE.
# - OPENEYE_LIBRARIES    a listcontaining the OpenEye libraries
# - OPENEYE_INCLUDE_DIRS a listcontaining the OpenEye include directories

INCLUDE(LibFindMacros)

FIND_PATH(openeye_include oesystem.h)
FIND_LIBRARY(oesystem_lib oesystem)
FIND_LIBRARY(oechem_lib oechem)
FIND_LIBRARY(oeplatform_lib oeplatform)

# Prepare the input for LIBFIND_PROCESS
SET(OPENEYE_PROCESS_INCLUDES openeye_include)
SET(OPENEYE_PROCESS_LIBS oechem_lib oesystem_lib oeplatform_lib)

LIBFIND_PROCESS(OPENEYE)
