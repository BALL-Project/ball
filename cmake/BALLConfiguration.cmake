########################################################
###    configure config.h														 ###
########################################################

INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckCXXSourceCompiles)
INCLUDE(TestBigEndian)

## some debug options
IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	SET(BALL_DEBUG TRUE CACHE INTERNAL "BALL configured in debug mode" FORCE)
	SET(BALL_VIEW_DEBUG TRUE CACHE INTERNAL "VIEW configured in debug mode" FORCE)
	SET(BALL_NO_INLINE_FUNCTIONS TRUE CACHE STRING "Reduce usage of inline functions (useful for debugging purposes)")
ELSEIF(CMAKE_BUILD_TYPE STREQUAL "Release")
	SET(BALL_DEBUG FALSE CACHE INTERNAL "BALL configured in debug mode" FORCE)
	SET(BALL_VIEW_DEBUG FALSE CACHE INTERNAL "VIEW configured in debug mode" FORCE)
	SET(BALL_NO_INLINE_FUNCTIONS FALSE CACHE STRING "Reduce usage of inline functions (useful for debugging purposes)")
ENDIF()


# Define some directories
# TODO: I'm not sure what will be the most sensible approach here for BALL...
IF ("${CMAKE_INSTALL_PREFIX}" STREQUAL ".")
	SET(BALL_PATH ${PROJECT_SOURCE_DIR} CACHE STRING "Path to the top level install location.")
	SET(BALL_DATA_PATH ${BALL_PATH}/data CACHE STRING "Path to the BALL data directory.")
ELSE()
	SET(BALL_PATH ${CMAKE_INSTALL_PREFIX} CACHE STRING "Path to the top level install location.")
	SET(BALL_DATA_PATH ${BALL_PATH}/share/BALL/data CACHE STRING "Path to the BALL data directory.")
ENDIF()

SET(BALL_TEST_DATA_PATH      "${PROJECT_SOURCE_DIR}/test/data")
SET(BALL_BENCHMARK_DATA_PATH "${PROJECT_SOURCE_DIR}/source/BENCHMARKS/data")



## some information about the processor and system type
SET(BALL_BINFMT "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}" CACHE INTERNAL "BALL binary format")
SET(BALL_ARCH ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "The processor architecture")

## the operating system
SET(BALL_OS "${CMAKE_SYSTEM_NAME}" CACHE INTERNAL "Operating system")

## some convenient defines
IF (${BALL_OS} MATCHES "Linux")
	SET(BALL_OS_LINUX TRUE CACHE INTERNAL "This system is a Linux")
ELSEIF (${BALL_OS} MATCHES "SunOS")
	## Try to find out if it's a Solaris or an ancient one...
	IF (${CMAKE_SYSTEM_VERSION} MATCHES "5.*")
		SET(BALL_OS_SOLARIS TRUE CACHE INTERNAL "This system is a Solaris")
	ELSE()
		SET(BALL_OS_SUNOS TRUE CACHE INTERNAL "This system is a SunOS")
	ENDIF()
ELSEIF (${BALL_OS} MATCHES "IRIX")
	SET(BALL_OS_IRIX TRUE CACHE INTERNAL "This system is an IRIX")
ELSEIF (${BALL_OS} MATCHES "FreeBSD")
	SET(BALL_OS_FREEBSD TRUE CACHE INTERNAL "This system is a FreeBSD")
ELSEIF (${BALL_OS} MATCHES "NetBSD")
	SET(BALL_OS_NETBSD TRUE CACHE INTERNAL "This system is a NetBSD")
ELSEIF (${BALL_OS} MATCHES "OpenBSD")
	SET(BALL_OS_OPENBSD TRUE CACHE INTERNAL "This system is an OpenBSD")
ELSEIF (${BALL_OS} MATCHES "Darwin")
	SET(BALL_OS_DARWIN TRUE CACHE INTERNAL "This system is a MacOS X")
ELSEIF (${BALL_OS} MATCHES "Windows")
	SET(BALL_OS_WINDOWS TRUE CACHE INTERNAL "This system is a Windows")
	SET(BALL_PLATFORM_WINDOWS TRUE CACHE INTERNAL "This system is a Windows")
ENDIF()

## BALL_COMPILER_* defines (as prepared by BALLCompilerSpecific.cmake)
IF (CXX_COMPILER_ID STREQUAL "MSVC")
	SET(BALL_COMPILER_MSVC TRUE CACHE INTERNAL "Do we use the Microsoft Visual Studio Compiler?")
ELSEIF (CXX_COMPILER_ID STREQUAL "GXX")
	SET(BALL_COMPILER_GXX TRUE CACHE INTERNAL "Do we use the Gnu Compiler Collection?")
ELSEIF (CXX_COMPILER_ID STREQUAL "Intel")
	SET(BALL_COMPILER_INTEL TRUE CACHE INTERNAL "Do we use the Intel Compiler?")
ELSEIF ((CXX_COMPILER_ID STREQUAL "LLVM"))
	SET(BALL_COMPILER_LLVM TRUE CACHE INTERNAL "Do we use the Clang Compiler?")
ENDIF()

SET(BALL_COMPILER "${CXX_COMPILER_ID}" CACHE INTERNAL "The C++ compiler used for BALL" FORCE)
SET(BALL_COMPILER_VERSION "${CXX_COMPILER_VERSION}" CACHE INTERNAL "The C++ compiler version" FORCE)
SET(BALL_COMPILER_VERSION_MAJOR "${CXX_COMPILER_VERSION_MAJOR}" CACHE INTERNAL "The C++ compiler major version" FORCE)
SET(BALL_COMPILER_VERSION_MINOR "${CXX_COMPILER_VERSION_MINOR}" CACHE INTERNAL "The C++ compiler minor version" FORCE)
SET(BALL_COMPILER_VERSION_MINOR_MINOR "${CXX_COMPILER_VERSION_MINOR_MINOR}" CACHE INTERNAL "The C++ compiler minor minor version" FORCE)

## The endianness of the system
TEST_BIG_ENDIAN(BALL_BIG_ENDIAN)
IF(BALL_BIG_ENDIAN)
	SET(BALL_LITTLE_ENDIAN FALSE)
ELSE()
	SET(BALL_LITTLE_ENDIAN TRUE)
ENDIF()

## Required type information
INCLUDE(cmake/BALLConfigTypes.cmake)


# Header and Compile checks
IF(NOT APPLE)
	SET(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} ${BALL_PROJECT_COMPILE_FLAGS}")
	SET(CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS} ${BALL_PROJECT_COMPILE_DEFNS}")
ENDIF()

## system headers:
CHECK_INCLUDE_FILE_CXX("unistd.h"  BALL_HAS_UNISTD_H)
CHECK_INCLUDE_FILE_CXX("process.h" BALL_HAS_PROCESS_H)
CHECK_INCLUDE_FILE_CXX("time.h"    BALL_HAS_TIME_H)
CHECK_INCLUDE_FILE_CXX("dirent.h"  BALL_HAS_DIRENT_H)
CHECK_INCLUDE_FILE_CXX("direct.h"  BALL_HAS_DIRECT_H)
CHECK_INCLUDE_FILE_CXX("pwd.h"     BALL_HAS_PWD_H)
CHECK_INCLUDE_FILE_CXX("stdint.h"  BALL_HAS_STDINT_H)

CHECK_INCLUDE_FILE_CXX("sys/time.h"    BALL_HAS_SYS_TIME_H)
CHECK_INCLUDE_FILE_CXX("sys/stat.h"    BALL_HAS_SYS_STAT_H)
CHECK_INCLUDE_FILE_CXX("sys/times.h"   BALL_HAS_SYS_TIMES_H)
CHECK_INCLUDE_FILE_CXX("sys/types.h"   BALL_HAS_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CXX("sys/param.h"   BALL_HAS_SYS_PARAM_H)
CHECK_INCLUDE_FILE_CXX("sys/sysinfo.h" BALL_HAS_SYS_SYSINFO_H)

CHECK_CXX_SOURCE_COMPILES("#include <sys/stat.h>
	int main(int /*argc*/, char** /*argv*/)
	{
		mode_t x;

		return 0;
	}" BALL_HAS_MODE_T)
IF (NOT BALL_HAS_MODE_T)
	SET(mode_t)
	SET(BALL_MODE_T int)
ENDIF()

CHECK_INCLUDE_FILE_CXX("ieeefp.h" BALL_HAS_IEEEFP_H)

CHECK_FUNCTION_EXISTS(kill BALL_HAS_KILL)
CHECK_FUNCTION_EXISTS(sysconf BALL_HAS_SYSCONF)

## Can we overload long int with LongIndex?
IF (BALL_HAS_STDINT_H)
	CHECK_CXX_SOURCE_COMPILES("#include <stdint.h>
	void f(long int l) {l+=2;}
	void f(${BALL_LONG64_TYPE} l) {l+=2;}
	int main(int /*argc*/, char** /*argv*/)
	{
		return 0;
	}" BALL_ALLOW_LONG64_TYPE_OVERLOADS) 
ELSE()
	CHECK_CXX_SOURCE_COMPILES("
	void f(long int l) {l+=2;}
	void f(${BALL_LONG64_TYPE} l) {l+=2;}
	int main(int /*argc*/, char** /*argv*/)
	{
		return 0;
	}" BALL_ALLOW_LONG64_TYPE_OVERLOADS) 
ENDIF()

## Check for thread_local
INCLUDE(cmake/BALLConfigThreadLocal.cmake)

## Check for the presence of C++11 noexcept
INCLUDE(cmake/BALLConfigNoexcept.cmake)

## Check for the presence of C++11 features in string
INCLUDE(cmake/BALLConfigStdStringFeatures.cmake)

## Test whether vsnprintf is available
CHECK_FUNCTION_EXISTS(vsnprintf BALL_HAVE_VSNPRINTF)

## Used by GenericPDBFile
SET(BALL_STRICT_PDB_LINE_IMPORT FALSE CACHE BOOL "Use strict checking for PDB line lengths (discouraged!)")
MARK_AS_ADVANCED(BALL_STRICT_PDB_LINE_IMPORT)

SET(BALL_MAX_LINE_LENGTH 65535 CACHE STRING "The maximum line length for reading from files")
MARK_AS_ADVANCED(BALL_MAX_LINE_LENGTH)

## STL-DEBUG (only for GCC and in debug mode)
SET(STL_DEBUG OFF CACHE BOOL "[GCC only] Enable STL-DEBUG mode (very slow).")
IF (STL_DEBUG)
	IF (CXX_COMPILER_ID STREQUAL "GXX")
		IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
			# add compiler flag
			ADD_DEFINITIONS(/D_GLIBCXX_DEBUG)
			MESSAGE(STATUS "STL debug mode: ${STL_DEBUG}")
		ELSE()
			MESSAGE(WARNING "STL debug mode is supported for BALL debug mode only")
		ENDIF()
	ELSE()
		MESSAGE(WARNING "STL debug mode is supported for compiler GCC only")
	ENDIF()
ELSE()
	MESSAGE(STATUS "[GCC only] STL debug mode: ${STL_DEBUG}")
ENDIF()


## user flag with default "QMYSQL"
set(QT_DB_PLUGIN "QMYSQL" CACHE STRING "User switch to change the Qt database plugin.")

