## Detect the raytracing library rtfact

INCLUDE(CheckLibraryExists)

## Set some possible paths to use when looking for the RTfact installation
SET(RTFACT_POSSIBLE_ROOT_DIRS
	${RTFACT_ROOT_DIR}
	$ENV{RTFACT_ROOT_DIR}
)

## First, try to find the required header files (RTpie/IRayTracer.hpp)
FIND_PATH(RTFACT_INCLUDE_PATH
	NAMES RTpie/IRayTracer.hpp
	PATHS ${RTFACT_POSSIBLE_ROOT_DIRS}
	PATH_SUFFIXES include
	DOC "RTfact header include dir")

IF (NOT RTFACT_INCLUDE_PATH)
	MESSAGE(STATUS "Could not find RTfact header file (RTpie/IRayTracer.hpp)! Real time ray tracing will be disabled!")
ELSE()
	SET(RTFACT_INCLUDE_DIR ${RTFACT_INCLUDE_PATH} CACHE STRING "Full path to the RTfact headers")

	## Now, try to find the rtfact library itself.
	FIND_LIBRARY(RTFACT_OPT_LIBRARY
		NAMES RTfactRTpie
		PATHS ${RTFACT_POSSIBLE_ROOT_DIRS} ${RTFACT_POSSIBLE_ROOT_DIRS}/build ${RTFACT_POSSIBLE_ROOT_DIRS}/build/lib ${RTFACT_POSSIBLE_ROOT_DIRS}/build/Release ${RTFACT_POSSIBLE_ROOT_DIRS}/build/lib/Release ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build/lib ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build/Release ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build/lib/Release

		PATH_SUFFIXES lib
		DOC "RTfact library, optimized")

	## And a possible debug version
	FIND_LIBRARY(RTFACT_DEBUG_LIBRARY
		NAMES RTfactRemoted RTfactRTpie
		PATHS ${RTFACT_POSSIBLE_ROOT_DIRS} ${RTFACT_POSSIBLE_ROOT_DIRS}/build ${RTFACT_POSSIBLE_ROOT_DIRS}/build/lib ${RTFACT_POSSIBLE_ROOT_DIRS}/build/Debug ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build/Debug ${RTFACT_POSSIBLE_ROOT_DIRS}/remote/build/lib
		PATH_SUFFIXES lib
		DOC "RTfact library, debug")

	IF (RTFACT_OPT_LIBRARY OR RTFACT_DEBUG_LIBRARY)
		BALL_COMBINE_LIBS(RTFACT_LIBRARIES ${RTFACT_OPT_LIBRARY} ${RTFACT_DEBUG_LIBRARY})
		SET(RTFACT_FOUND TRUE)
	ELSE()
		MESSAGE(STATUS "Could not find RTfact libraries!")
		SET(RTFACT_FOUND FALSE)
	ENDIF()

	INCLUDE(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(RTFACT DEFAULT_MSG
		RTFACT_INCLUDE_DIR
		RTFACT_LIBRARIES)

	MARK_AS_ADVANCED(RTFACT_INCLUDE_DIR)
	MARK_AS_ADVANCED(RTFACT_LIBRARIES)

ENDIF()
