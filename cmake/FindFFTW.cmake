# Find FFTW3
#
# Currently, search for FFTW thread libraries is commented out
# because I don't know how the naming scheme for those libs
# currently looks like.
#
# The following CMake variables are set by this module:
#
# FFTW_FOUND            true, if a valid FFTW installation has been identified
# FTWD_FOUND            true, if double precision library found
# FFTWF_FOUND           true, if single precision library found
# FFTWL_FOUND           true, if long double precision library found
# FFTW_INCLUDE_DIR      absolute directory that contains fftw3.h header file
#
# FFTWD_LIB             double precision library
# FFTWF_LIB             single precision library
# FFTWL_LIB             long double precision library

SET(FFTW_FOUND FALSE)
SET(FFTWD_FOUND FALSE)
SET(FFTWF_FOUND FALSE)
SET(FFTWL_FOUND FALSE)

# Define allowed fftw verions
SET(FFTW_ALLOWED_VERSIONS 3 3.0 3.0.1 3.1 3.1.1 3.2 3.2.1 3.3 3.3.1 3.3.2 3.3.3 3.3.4 3.3.5 3.3.6)

# Add additional search paths
SET(CUSTOM_SEARCH_PATHS /usr/local /opt/local)

# Try to find fftw3.h
FIND_PATH(FFTW_INCLUDE_DIR fftw3.h
	  PATHS ${CUSTOM_SEARCH_PATHS}
	  PATH_SUFFIXES include)

# If not found, return.
IF(NOT FFTW_INCLUDE_DIR)
	RETURN()
ENDIF()

# Get a hint where to search for the installed fftw libraries
GET_FILENAME_COMPONENT(FFTW_INSTALL_BASE ${FFTW_INCLUDE_DIR} PATH)
LIST(APPEND CUSTOM_SEARCH_PATHS ${FFTW_INSTALL_BASE})

# The fftw types searched for are
# D: double precision library
# F: single precision library
# L: long double precision library
SET(FFTW_TYPES "D;F;L")

# Now, iterate over these types and search for the corresponding fftw libraries
FOREACH(fftw_type IN LISTS FFTW_TYPES)

	IF(${fftw_type} STREQUAL "D")
		SET(FFTW_TYPE_SUFFIX "")
	ELSE()
		STRING(TOLOWER ${fftw_type} FFTW_TYPE_SUFFIX)
	ENDIF()

	# Generate all possible library names
	SET(FFTW_LIB_NAMES "")
	SET(FFTW_THREAD_LIB_NAMES "")
	FOREACH(fftw_allowed_version ${FFTW_ALLOWED_VERSIONS})
		LIST(APPEND FFTW_LIB_NAMES fftw${FFTW_TYPE_SUFFIX}${fftw_allowed_version} libfftw${FFTW_TYPE_SUFFIX}-${fftw_allowed_version})
		#LIST(APPEND FFTW_THREAD_LIB_NAMES TODO: Find out fftw thread lib names)
	ENDFOREACH()

	FIND_LIBRARY(FFTW${fftw_type}_LIB NAMES ${FFTW_LIB_NAMES}
		     PATHS ${CUSTOM_SEARCH_PATHS}
		     PATH_SUFFIXES lib lib64)

	IF(FFTW${fftw_type}_LIB)
		SET(FFTW${fftw_type}_FOUND TRUE)
	ENDIF()

	#FIND_LIBRARY(FFTW${fftw_type}_THREADS_LIB NAMES ${FFTW_THREAD_LIB_NAMES}
	#	      PATHS ${CUSTOM_SEARCH_PATHS}
	#	      PATH_SUFFIXES lib lib64)

	#IF(FFTW${fftw_type}_THREADS_LIB)
	#	SET(FFTW${fftw_type}_LIB ${FFTW${fftw_type}_LIB} ${FFTW${fftw_type}_THREADS_LIB})
	#ENDIF()

ENDFOREACH()

IF(FFTWD_FOUND OR FFTWF_FOUND OR FFTWL_FOUND)
	SET(FFTW_FOUND TRUE)
ENDIF()

