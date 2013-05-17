# Locate Intel Threading Building Blocks include paths and libraries
# TBB can be found at http://www.threadingbuildingblocks.org/ 
# Written by Hannes Hofmann, hannes.hofmann _at_ informatik.uni-erlangen.de
# Adapted by Gino van den Bergen gino _at_ dtecta.com

# GvdB: This module uses the environment variable TBB_ARCH_PLATFORM which defines architecture and compiler.
#   e.g. "ia32/vc8" or "em64t/cc4.1.0_libc2.4_kernel2.6.16.21"
#   TBB_ARCH_PLATFORM is set by the build script tbbvars[.bat|.sh|.csh], which can be found
#   in the TBB installation directory (TBB_INSTALL_DIR).
#
# For backwards compatibility, you may explicitely set the CMake variables TBB_ARCHITECTURE and TBB_COMPILER.
# TBB_ARCHITECTURE     [ ia32 | em64t | itanium ]
#   which architecture to use
# TBB_COMPILER         e.g. vc9 or cc3.2.3_libc2.3.2_kernel2.4.21 or cc4.0.1_os10.4.9
#   which compiler to use (detected automatically on Windows)

# This module respects
# TBB_INSTALL_DIR or $ENV{TBB21_INSTALL_DIR} or $ENV{TBB_INSTALL_DIR}

# This module defines
# TBB_INCLUDE_DIRS, where to find task_scheduler_init.h, etc.
# TBB_LIBRARY_DIRS, where to find libtbb, libtbbmalloc
# TBB_INSTALL_DIR, the base TBB install directory
# TBB_LIBRARIES, the libraries to link against to use TBB.
# TBB_DEBUG_LIBRARIES, the libraries to link against to use TBB with debug symbols.
# TBB_FOUND, If false, don't try to use TBB.

if (WIN32)
    # has em64t/vc8   em64t/vc9
    # has ia32/vc7.1  ia32/vc8   ia32/vc9
    set(_TBB_DEFAULT_INSTALL_DIR "C:/Program Files/Intel/TBB" "C:/Program Files (x86)/Intel/TBB")
    set(_TBB_LIB_NAME "tbb")
    set(_TBB_LIB_MALLOC_NAME "${_TBB_LIB_NAME}malloc")
    set(_TBB_LIB_DEBUG_NAME "${_TBB_LIB_NAME}_debug")
    set(_TBB_LIB_MALLOC_DEBUG_NAME "${_TBB_LIB_MALLOC_NAME}_debug")
    if (MSVC71)
        set (_TBB_COMPILER "vc7.1")
    endif(MSVC71)
    if (MSVC80)
        set(_TBB_COMPILER "vc8")
    endif(MSVC80)
    if (MSVC90)
        set(_TBB_COMPILER "vc9")
    endif(MSVC90)
    if (MSVC10)
        set(_TBB_COMPILER "vc10")
    endif(MSVC10)
    if (MSVC11)
        set(_TBB_COMPILER "vc11")
    endif(MSVC11)
    if (MSVC12)
        set(_TBB_COMPILER "vc12")
    endif(MSVC12)
    if (MSVC14)
        set(_TBB_COMPILER "vc14")
    endif(MSVC14)
    if (NOT _TBB_COMPILER)
        message("ERROR: TBB supports only VC 7.1, 8, 9, 10, 11, 12, and 14 compilers on Windows platforms.")
    endif (NOT _TBB_COMPILER)
    set(_TBB_ARCHITECTURE ${TBB_ARCHITECTURE})
endif (WIN32)

if (UNIX)
    if (APPLE)
        # MAC
        set(_TBB_DEFAULT_INSTALL_DIR "/Library/Frameworks/Intel_TBB.framework/Versions")
        # libs: libtbb.dylib, libtbbmalloc.dylib, *_debug
        set(_TBB_LIB_NAME "tbb")
        set(_TBB_LIB_MALLOC_NAME "${_TBB_LIB_NAME}malloc")
        set(_TBB_LIB_DEBUG_NAME "${_TBB_LIB_NAME}_debug")
        set(_TBB_LIB_MALLOC_DEBUG_NAME "${_TBB_LIB_MALLOC_NAME}_debug")
        # has only one flavor: ia32/cc4.0.1_os10.4.9
        set(_TBB_COMPILER "cc4.0.1_os10.4.9")
        set(_TBB_ARCHITECTURE "ia32")
    else (APPLE)
        # LINUX
        set(_TBB_DEFAULT_INSTALL_DIR "/opt/intel/tbb" "/usr/local/include" "/usr/include" "/usr")
        set(_TBB_LIB_NAME "tbb")
        set(_TBB_LIB_MALLOC_NAME "${_TBB_LIB_NAME}malloc")
        set(_TBB_LIB_DEBUG_NAME "${_TBB_LIB_NAME}_debug")
        set(_TBB_LIB_MALLOC_DEBUG_NAME "${_TBB_LIB_MALLOC_NAME}_debug")
        # has em64t/cc3.2.3_libc2.3.2_kernel2.4.21  em64t/cc3.3.3_libc2.3.3_kernel2.6.5  em64t/cc3.4.3_libc2.3.4_kernel2.6.9  em64t/cc4.1.0_libc2.4_kernel2.6.16.21
        # has ia32/*
        # has itanium/*
        set(_TBB_COMPILER ${TBB_COMPILER})
        set(_TBB_ARCHITECTURE ${TBB_ARCHITECTURE})
    endif (APPLE)
endif (UNIX)

if (CMAKE_SYSTEM MATCHES "SunOS.*")
# SUN
# not yet supported
# has em64t/cc3.4.3_kernel5.10
# has ia32/*
endif (CMAKE_SYSTEM MATCHES "SunOS.*")


#-- Clear the public variables
set (TBB_FOUND "NO")


#-- Find TBB install dir and set ${_TBB_INSTALL_DIR} and cached ${TBB_INSTALL_DIR}
# first: use CMake variable TBB_INSTALL_DIR
if (TBB_INSTALL_DIR)    
    set (_TBB_INSTALL_DIR ${TBB_INSTALL_DIR})
endif (TBB_INSTALL_DIR)
# second: use environment variable
if (NOT _TBB_INSTALL_DIR)
    if (NOT "$ENV{TBB_INSTALL_DIR}" STREQUAL "")
        set (_TBB_INSTALL_DIR $ENV{TBB_INSTALL_DIR})
    endif (NOT "$ENV{TBB_INSTALL_DIR}" STREQUAL "")
    # Intel recommends setting TBB21_INSTALL_DIR
    if (NOT "$ENV{TBB21_INSTALL_DIR}" STREQUAL "")
        set (_TBB_INSTALL_DIR $ENV{TBB21_INSTALL_DIR})
    endif (NOT "$ENV{TBB21_INSTALL_DIR}" STREQUAL "")
endif (NOT _TBB_INSTALL_DIR)
# third: try to find path automatically
if (NOT _TBB_INSTALL_DIR)
    if (_TBB_DEFAULT_INSTALL_DIR)
        set (_TBB_INSTALL_DIR ${_TBB_DEFAULT_INSTALL_DIR})
    endif (_TBB_DEFAULT_INSTALL_DIR)
endif (NOT _TBB_INSTALL_DIR)
# sanity check
if (NOT _TBB_INSTALL_DIR)
    message ("ERROR: TBB_INSTALL_DIR not found. ${_TBB_INSTALL_DIR}")
else (NOT _TBB_INSTALL_DIR)
# finally: set the cached CMake variable TBB_INSTALL_DIR
if (NOT TBB_INSTALL_DIR)
    set (TBB_INSTALL_DIR ${_TBB_INSTALL_DIR} CACHE PATH "Intel TBB install directory")
    mark_as_advanced(TBB_INSTALL_DIR)
endif (NOT TBB_INSTALL_DIR)


#-- A macro to rewrite the paths of the library. This is necessary, because 
#   find_library() always found the em64t/vc9 version of the TBB libs
macro(TBB_CORRECT_LIB_DIR var_name)
#    if (NOT "${_TBB_ARCHITECTURE}" STREQUAL "em64t")
        string(REPLACE em64t "${_TBB_ARCHITECTURE}" ${var_name} ${${var_name}})
#    endif (NOT "${_TBB_ARCHITECTURE}" STREQUAL "em64t")
    string(REPLACE ia32 "${_TBB_ARCHITECTURE}" ${var_name} ${${var_name}})
    string(REPLACE vc7.1 "${_TBB_COMPILER}" ${var_name} ${${var_name}})
    string(REPLACE vc8 "${_TBB_COMPILER}" ${var_name} ${${var_name}})
    string(REPLACE vc9 "${_TBB_COMPILER}" ${var_name} ${${var_name}})
endmacro(TBB_CORRECT_LIB_DIR var_content)


#-- Look for include directory and set ${TBB_INCLUDE_DIR}
set (TBB_INC_SEARCH_DIR ${_TBB_INSTALL_DIR}/include)
find_path(TBB_INCLUDE_DIR
    tbb/task_scheduler_init.h
    PATHS ${TBB_INC_SEARCH_DIR}
)
mark_as_advanced(TBB_INCLUDE_DIR)


#-- Look for libraries
# GvdB: $ENV{TBB_ARCH_PLATFORM} is set by the build script tbbvars[.bat|.sh|.csh]
if (NOT $ENV{TBB_ARCH_PLATFORM} STREQUAL "")
    set (_TBB_ARCH_PLATFORM $ENV{TBB_ARCH_PLATFORM})    
else (NOT $ENV{TBB_ARCH_PLATFORM} STREQUAL "")
    # HH: deprecated
    message(STATUS "[Warning] FindTBB.cmake: The use of TBB_ARCHITECTURE and TBB_COMPILER is deprecated and may not be supported in future versions. Please set $ENV{TBB_ARCH_PLATFORM} (using tbbvars.[bat|csh|sh]).")
    set (_TBB_ARCH_PLATFORM "${_TBB_ARCHITECTURE}/${_TBB_COMPILER}")
endif (NOT $ENV{TBB_ARCH_PLATFORM} STREQUAL "")

# In RTfact contrib, the path to libraries is lib/x64/vc10 but in the original TBB distribution it is lib/intel64/vc10
# Changing it in either is not possible, so we have to replace it here and put both into search paths
set (TBB_LIBRARY_DIR "${_TBB_INSTALL_DIR}/lib/${_TBB_ARCH_PLATFORM}")

find_library(TBB_LIBRARY
            NAMES ${_TBB_LIB_NAME}
            PATHS ${TBB_LIBRARY_DIR}
            NO_DEFAULT_PATH)
            
IF (NOT TBB_LIBRARY)
	find_library(TBB_LIBRARY
                NAMES ${_TBB_LIB_NAME}
                PATHS ${TBB_LIBRARY_DIR})
ENDIF(NOT TBB_LIBRARY)

find_library(TBB_MALLOC_LIBRARY
            NAMES ${_TBB_LIB_MALLOC_NAME}
            PATHS ${TBB_LIBRARY_DIR}
            NO_DEFAULT_PATH)
            
IF (NOT TBB_MALLOC_LIBRARY)
	find_library(TBB_MALLOC_LIBRARY
                NAMES ${_TBB_LIB_MALLOC_NAME}
                PATHS ${TBB_LIBRARY_DIR})
ENDIF(NOT TBB_MALLOC_LIBRARY)

GET_FILENAME_COMPONENT( _TBB_LIBRARY_NAME ${TBB_LIBRARY} NAME )
GET_FILENAME_COMPONENT( _TBB_LIBRARY_PATH ${TBB_LIBRARY} PATH )
SET(TBB_LIBRARY ${_TBB_LIBRARY_NAME})
SET(TBB_LIBRARY_PATH ${_TBB_LIBRARY_PATH})
GET_FILENAME_COMPONENT( _TBB_MALLOC_LIBRARY_NAME ${TBB_MALLOC_LIBRARY} NAME )
SET(TBB_MALLOC_LIBRARY ${_TBB_MALLOC_LIBRARY_NAME})

mark_as_advanced(TBB_LIBRARY TBB_MALLOC_LIBRARY)

#-- Look for debug libraries
find_library(TBB_LIBRARY_DEBUG
             NAMES ${_TBB_LIB_DEBUG_NAME}
             PATHS ${TBB_LIBRARY_DIR}
             NO_DEFAULT_PATH)

find_library(TBB_MALLOC_LIBRARY_DEBUG
             NAMES ${_TBB_LIB_MALLOC_DEBUG_NAME}
             PATHS ${TBB_LIBRARY_DIR}
             NO_DEFAULT_PATH)
#TBB_CORRECT_LIB_DIR(TBB_LIBRARY_DEBUG)
#TBB_CORRECT_LIB_DIR(TBB_MALLOC_LIBRARY_DEBUG)
mark_as_advanced(TBB_LIBRARY_DEBUG TBB_MALLOC_LIBRARY_DEBUG)

GET_FILENAME_COMPONENT( _TBB_LIBRARY_DEBUG_NAME ${TBB_LIBRARY_DEBUG} NAME )
GET_FILENAME_COMPONENT( _TBB_LIBRARY_DEBUG_PATH ${TBB_LIBRARY} PATH )
SET(TBB_LIBRARY_DEBUG ${_TBB_LIBRARY_DEBUG_NAME})
SET(TBB_LIBRARY_DEBUG_PATH ${_TBB_LIBRARY_DEBUG_PATH})
GET_FILENAME_COMPONENT( _TBB_MALLOC_LIBRARY_DEBUG_NAME ${TBB_MALLOC_LIBRARY_DEBUG} NAME )
SET(TBB_MALLOC_LIBRARY_DEBUG ${_TBB_MALLOC_LIBRARY_DEBUG_NAME})


if (TBB_INCLUDE_DIR)
    FIND_FILE(TBB_STDDEF_H "tbb_stddef.h" PATHS ${TBB_INCLUDE_DIR} PATH_SUFFIXES tbb)
    FILE(STRINGS ${TBB_STDDEF_H} TBB_VERSION_MAJOR_TMP REGEX "#define TBB_VERSION_MAJOR [0-9]+")
    SEPARATE_ARGUMENTS(TBB_VERSION_MAJOR_TMP)
    LIST(GET TBB_VERSION_MAJOR_TMP -1 TBB_VERSION_MAJOR)

    if (TBB_LIBRARY)
        set (TBB_FOUND "YES")
   	#On Windows, we always should have both debug and release libraries. On other platforms, the debug libraries are often not present.
	if(WIN32 OR (TBB_LIBRARY_DEBUG AND TBB_MALLOC_LIBRARY_DEBUG))
	   set (_TBB_LIBRARIES optimized ${TBB_LIBRARY} optimized ${TBB_MALLOC_LIBRARY} debug ${TBB_LIBRARY_DEBUG} debug ${TBB_MALLOC_LIBRARY_DEBUG} ${TBB_LIBRARIES})
	#and if not, use just the release ones
	else()
	  set (_TBB_LIBRARIES general ${TBB_LIBRARY} ${TBB_MALLOC_LIBRARY})
	endif()
        set (TBB_LIBRARIES ${_TBB_LIBRARIES} CACHE STRING "Intel TBB Libraries")
        set (TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIR} CACHE PATH "TBB include directory" FORCE)
        set (_TBB_LIBRARY_DIRS ${TBB_LIBRARY_PATH} ${TBB_LIBRARY_DEBUG_PATH})
        list(REMOVE_DUPLICATES _TBB_LIBRARY_DIRS)
        set (TBB_LIBRARY_DIRS ${_TBB_LIBRARY_DIRS} CACHE PATH "TBB library directories" FORCE)
        mark_as_advanced(TBB_INCLUDE_DIRS TBB_LIBRARY_DIRS TBB_LIBRARIES TBB_DEBUG_LIBRARIES)
	message(STATUS "Found Intel TBB: major version ${TBB_VERSION_MAJOR}")
    endif (TBB_LIBRARY)

endif (TBB_INCLUDE_DIR)

if (NOT TBB_FOUND)
    message(STATUS "Looked for Threading Building Blocks in ${_TBB_INSTALL_DIR}")
    # do only throw fatal, if this pkg is REQUIRED
    if (TBB_FIND_REQUIRED)
        message(FATAL_ERROR "Could NOT find TBB library.")
    endif (TBB_FIND_REQUIRED)
endif (NOT TBB_FOUND)

endif (NOT _TBB_INSTALL_DIR)
