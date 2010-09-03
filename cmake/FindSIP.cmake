# Find SIP
# ~~~~~~~~
#
# SIP website: http://www.riverbankcomputing.co.uk/sip/index.php
#
# Find the installed version of SIP. FindSIP should be called after Python
# has been found.
#
# This file defines the following variables:
#
# SIP_VERSION - The version of SIP found expressed as a 6 digit hex number
#     suitable for comparision as a string.
#
# SIP_VERSION_STR - The version of SIP found as a human readable string.
#
# SIP_BINARY_PATH - Path and filename of the SIP command line executable.
#
# SIP_INCLUDE_DIR - Directory holding the SIP C++ header file.
#
# SIP_DEFAULT_SIP_DIR - Default directory where .sip files should be installed
#     into.

# Copyright (c) 2007, Simon Edwards <simon@simonzone.com>
# Redistribution and use is allowed according to the terms of the BSD license.

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

# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

OPTION(USE_FIND_SIP_PY "Use the FindSIP.py script to detect the python configuration" On)

IF(SIP_VERSION)
  # Already in cache, be silent
  SET(SIP_FOUND TRUE)
ELSE(SIP_VERSION)
	# determine the version of python we use
	EXECUTE_PROCESS ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_version; print get_python_version()"
		OUTPUT_VARIABLE PYTHON_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	IF(USE_FIND_SIP_PY)
		FIND_FILE(_find_sip_py FindSIP.py PATHS ${CMAKE_MODULE_PATH})

		EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} ${_find_sip_py} OUTPUT_VARIABLE sip_config ERROR_QUIET)
		IF(sip_config)
			STRING(REGEX REPLACE "^sip_version:([^\n]+).*$" "\\1" SIP_VERSION ${sip_config})
			STRING(REGEX REPLACE ".*\nsip_version_str:([^\n]+).*$" "\\1" SIP_VERSION_STR ${sip_config})
			STRING(REGEX REPLACE ".*\nsip_bin:([^\n]+).*$" "\\1" SIP_EXECUTABLE ${sip_config})
			STRING(REGEX REPLACE ".*\ndefault_sip_dir:([^\n]+).*$" "\\1" SIP_DEFAULT_SIP_DIR ${sip_config})
			STRING(REGEX REPLACE ".*\nsip_inc_dir:([^\n]+).*$" "\\1" SIP_INCLUDE_DIR ${sip_config})
			SET(SIP_FOUND TRUE)
		ENDIF(sip_config)

		IF(SIP_FOUND)
			IF(NOT SIP_FIND_QUIETLY)
				MESSAGE(STATUS "Found SIP version: ${SIP_VERSION_STR}")
			ENDIF(NOT SIP_FIND_QUIETLY)
		ENDIF(SIP_FOUND)
	ENDIF(USE_FIND_SIP_PY)
ENDIF(SIP_VERSION)

IF(NOT SIP_VERSION)
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
	LIST(APPEND SIP_POSSIBLE_LIB_DIRS "/usr/lib/pyshared/python${PYTHON_VERSION}")

	SET(OLD_CMAKE_FIND_LIBRARY_PREFIXES "${CMAKE_FIND_LIBRARY_PREFIXES}" CACHE INTERNAL "")
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
  FIND_LIBRARY(SIP_LIBRARIES
    NAMES sip libsip
    PATHS ${SIP_POSSIBLE_LIB_DIRS} ${PYTHON_SITE_PACKAGES}
    PATH_SUFFIXES lib dlls
    DOC "SIP module")
	SET(CMAKE_FIND_LIBRARY_PREFIXES "${OLD_CMAKE_FIND_LIBRARY_PREFIXES}")

  # (c) Try to find the sip executable:
  
  # let's try the configured binary directory anyway, in case, you know, it's actually correct...
  EXECUTE_PROCESS ( COMMAND ${PYTHON_EXECUTABLE} -c "import sipconfig; print sipconfig.Configuration().sip_bin"
		OUTPUT_VARIABLE SIP_CONFIG_EXECUTABLE
		OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  GET_FILENAME_COMPONENT(SIP_CONFIG_BIN_DIRS "${SIP_CONFIG_EXECUTABLE}" PATH)
  
  # Use the path to the python installation as a hint for finding sip
  GET_FILENAME_COMPONENT(SIP_POSSIBLE_BIN_DIRS "${PYTHON_EXECUTABLE}" PATH)
  FIND_PROGRAM(SIP_EXECUTABLE sip
    ${SIP_POSSIBLE_BIN_DIRS}
    /usr/bin/
    ${SIP_CONFIG_BIN_DIRS}
  )

  # (c) Try to extract version information from sip.h:
  
  FILE(READ ${SIP_INCLUDE_DIR}/sip.h SIP_H_TEXT)
  STRING(REGEX MATCH   "define SIP_VERSION[ \t]+([0-9a-fx]+)"       SIP_VERSION "${SIP_H_TEXT}" )
  STRING(REGEX REPLACE "define SIP_VERSION[ \t]+([0-9a-fx]+)" "\\1" SIP_VERSION "${SIP_VERSION}")

  STRING(REGEX MATCH   "define SIP_VERSION_STR[ \t]+\"([^\"]*)\""        SIP_VERSION_STR "${SIP_H_TEXT}"     )
  STRING(REGEX REPLACE "define SIP_VERSION_STR[ \t]+\"([^\"]*)\""  "\\1" SIP_VERSION_STR "${SIP_VERSION_STR}")
ENDIF(NOT SIP_VERSION)

IF(NOT SIP_LIBRARIES)
  # Try to find the sip library:
	EXECUTE_PROCESS ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
		OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

  # Use the path to the python installation as a hint for finding sip
  GET_FILENAME_COMPONENT(SIP_POSSIBLE_LIB_DIRS "${PYTHON_LIBRARIES}"  PATH)
	LIST(APPEND SIP_POSSIBLE_LIB_DIRS "/usr/lib/pyshared/python${PYTHON_VERSION}")

	SET(OLD_CMAKE_FIND_LIBRARY_PREFIXES "${CMAKE_FIND_LIBRARY_PREFIXES}" CACHE INTERNAL "")
	SET(CMAKE_FIND_LIBRARY_PREFIXES "")
  FIND_LIBRARY(SIP_LIBRARIES
    NAMES sip libsip
    PATHS ${SIP_POSSIBLE_LIB_DIRS} ${PYTHON_SITE_PACKAGES}
    PATH_SUFFIXES lib dlls
    DOC "SIP module")
	SET(CMAKE_FIND_LIBRARY_PREFIXES "${OLD_CMAKE_FIND_LIBRARY_PREFIXES}")
ENDIF()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SIP DEFAULT_MSG
	SIP_EXECUTABLE
	SIP_LIBRARIES
	SIP_INCLUDE_DIR
	SIP_VERSION
	SIP_VERSION_STR
)

MARK_AS_ADVANCED(SIP_EXECUTABLE)
MARK_AS_ADVANCED(SIP_LIBRARIES)
MARK_AS_ADVANCED(SIP_INCLUDE_DIR)
