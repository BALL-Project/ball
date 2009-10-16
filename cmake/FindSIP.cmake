# Try to find the python interface generator SIP (http://www.riverbankcomputing.co.uk/sip/index.php)
#
# Unfortunately, we cannot use the standard way of calling sipconfig.py, since this seems to hard-code
# the paths during sip build time and these do not have to coincide with wherever BALL_contrib has been
# placed by the user...
#
# This file defines the following variables:
#
# SIP_FOUND       = system has sip support, and sip modules could be loaded successfully
#
# SIP_LIBRARIES   = full path to the sip libraries
#
# SIP_VERSION     = The version of SIP found expressed as a 6 digit hex number
#                   suitable for comparison as a string.
#
# SIP_VERSION_STR = The version of SIP found as a human readable string.
#
# SIP_EXECUTABLE  = Full path of the SIP command line executable.
#
# SIP_INCLUDE_DIR = Directory holding the SIP C++ header file.
#

IF(SIP_VERSION)
  # Already in cache, be silent
  SET(SIP_FOUND TRUE)
ELSE(SIP_VERSION)

  # (a) Try to find the sip headers:

  # Use the path to the python installation as a hint for finding sip
  SET(SIP_POSSIBLE_INC_DIRS "${PYTHON_INCLUDE_PATH}")

  FIND_PATH(SIP_INCLUDE_DIR
    NAMES sip.h sip/sip.h Python/sip.h
    PATHS ${SIP_POSSIBLE_INC_DIRS}
    PATH_SUFFIXES include
    DOC "SIP header include dir")

  # (b) Try to find the sip library:

  # Use the path to the python installation as a hint for finding sip
  GET_FILENAME_COMPONENT(SIP_POSSIBLE_LIB_DIRS "${PYTHON_LIBRARIES}"  ABSOLUTE)

	SET(OLD_CMAKE_FIND_LIBRARY_PREFIXES "${CMAKE_FIND_LIBRARY_PREFIXES}" CACHE INTERNAL "")
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
  FIND_LIBRARY(SIP_LIBRARIES
    NAMES sip libsip
    PATHS ${SIP_POSSIBLE_LIB_DIRS}
    PATH_SUFFIXES lib dlls
    DOC "SIP module")
	SET(CMAKE_FIND_LIBRARY_PREFIXES "${OLD_CMAKE_FIND_LIBRARY_PREFIXES}")

  # (c) Try to find the sip executable:

  # Use the path to the python installation as a hint for finding sip
  GET_FILENAME_COMPONENT(SIP_POSSIBLE_BIN_DIRS "${PYTHON_EXECUTABLE}" ABSOLUTE)
  FIND_PROGRAM(SIP_EXECUTABLE sip
    ${SIP_POSSIBLE_BIN_DIRS}
    /usr/bin/
  )

  # (d) Try to extract version information from sip.h:
  
  FILE(READ ${SIP_INCLUDE_DIR}/sip.h SIP_H_TEXT)
  STRING(REGEX MATCH   "define SIP_VERSION[ \t]+([0-9x]+)"       SIP_VERSION "${SIP_H_TEXT}" )
  STRING(REGEX REPLACE "define SIP_VERSION[ \t]+([0-9x]+)" "\\1" SIP_VERSION "${SIP_VERSION}")

  STRING(REGEX MATCH   "define SIP_VERSION_STR[ \t]+\"([^\"]*)\""        SIP_VERSION_STR "${SIP_H_TEXT}"     )
  STRING(REGEX REPLACE "define SIP_VERSION_STR[ \t]+\"([^\"]*)\""  "\\1" SIP_VERSION_STR "${SIP_VERSION_STR}")

  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(SIP DEFAULT_MSG 
    SIP_EXECUTABLE 
    SIP_LIBRARIES 
    SIP_INCLUDE_DIR
    SIP_VERSION
    SIP_VERSION_STR)

  MARK_AS_ADVANCED(SIP_EXECUTABLE)
  MARK_AS_ADVANCED(SIP_LIBRARIES)
  MARK_AS_ADVANCED(SIP_INCLUDE_DIR)

ENDIF(SIP_VERSION)
