# - Try to find LIBSVM
# Once done, this will define
#
#  LIBSVM_FOUND - system has LIBSVM
#  LIBSVM_INCLUDE_DIRS - the LIBSVM include directories
#  LIBSVM_LIBRARIES - link these to use LIBSVM

INCLUDE(LibFindMacros)

# Use pkg-config to get hints about paths
LIBFIND_PKG_CHECK_MODULES(LIBSVM_PKGCONF libsvm)

# Include dir
FIND_PATH(LIBSVM_INCLUDE_DIR
  NAMES svm.h
	  PATHS ${LIBSVM_PKGCONF_INCLUDE_DIRS}
		PATH_SUFFIXES libsvm libsvm-2.0/libsvm
)

# Finally the library itself
FIND_LIBRARY(LIBSVM_LIBRARY
  NAMES svm libsvm
	  PATHS ${LIBSVM_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
SET(LIBSVM_PROCESS_INCLUDES LIBSVM_INCLUDE_DIR)
SET(LIBSVM_PROCESS_LIBS LIBSVM_LIBRARY)
LIBFIND_PROCESS(LIBSVM)
