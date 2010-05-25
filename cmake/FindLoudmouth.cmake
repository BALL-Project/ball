# - Try to find libloudmouth
# Once done, this will define
#
#  LIBLOUDMOUTH_FOUND - system has LIBLOUDMOUTH
#  LIBLOUDMOUTH_INCLUDE_DIRS - the LIBLOUDMOUTH include directories
#  LIBLOUDMOUTH_LIBRARIES - link these to use LIBLOUDMOUTH

INCLUDE(LibFindMacros)

# Use pkg-config to get hints about paths
LIBFIND_PKG_CHECK_MODULES(LIBLOUDMOUTH_PKGCONF loudmouth-1.0)

# Include dir
FIND_PATH(LIBLOUDMOUTH_INCLUDE_DIR
  NAMES loudmouth/loudmouth.h
	  PATHS ${LIBLOUDMOUTH_PKGCONF_INCLUDE_DIRS}
		PATH_SUFFIXES loudmouth-1.0
)

# Finally the library itself
FIND_LIBRARY(LIBLOUDMOUTH_LIBRARY
  NAMES loudmouth-1
	  PATHS ${LIBLOUDMOUTH_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
SET(LIBLOUDMOUTH_PROCESS_INCLUDES LIBLOUDMOUTH_INCLUDE_DIR)
SET(LIBLOUDMOUTH_PROCESS_LIBS LIBLOUDMOUTH_LIBRARY)
LIBFIND_PROCESS(LIBLOUDMOUTH)
