cmake_minimum_required(VERSION 2.6)

include(AddFileDependencies)

# use g++ -g
#SET(CMAKE_BUILD_TYPE Debug)

# use g++ -O3 -DNDEBUG
SET(CMAKE_BUILD_TYPE Release)

include(cmake/contribDetection.cmake)

include(cmake/createConfigH.cmake)

set(BALL_SOURCES "")
set(VIEW_SOURCES "")
set(QUEASY_VERSION "1.0-alpha5")

# rules for uic, moc, yacc, flex
include(cmake/rules.cmake)


#include(BENCHMARKS/CMakeLists.txt)
include(COMMON/CMakeLists.txt)
include(CONCEPT/CMakeLists.txt)
include(DATATYPE/CMakeLists.txt)
include(ENERGY/CMakeLists.txt)
include(FORMAT/CMakeLists.txt)
include(KERNEL/CMakeLists.txt)
include(MATHS/CMakeLists.txt)
include(MOLMEC/CMakeLists.txt)
include(NMR/CMakeLists.txt)
#include(PYTHON/CMakeLists.txt)
include(QSAR/CMakeLists.txt)
include(SOLVATION/CMakeLists.txt)
include(STRUCTURE/CMakeLists.txt)
include(STRUCTURE/DOCKING/CMakeLists.txt)
include(SYSTEM/CMakeLists.txt)
include(PLUGIN/CMakeLists.txt)
include(VIEW/CMakeLists.txt)


ADD_LIBRARY(BALL STATIC ${BALL_SOURCES})
#ADD_LIBRARY(BALL_shared SHARED ${BALL_SOURCES})

ADD_LIBRARY(VIEW STATIC ${VIEW_SOURCES})
#ADD_LIBRARY(VIEW SHARED ${VIEW_SOURCES})

ADD_LIBRARY(QuEasy STATIC ${QUEASY_SOURCES})
#ADD_LIBRARY(QuEasy SHARED ${QUEASY_SOURCES})


if(NOT DISABLE_VIEW)
	if(NOT DISABLE_QUEASY)
		include(APPLICATIONS/QuEasyViz/CMakeLists.txt)
	endif(NOT DISABLE_QUEASY)
	include(APPLICATIONS/BALLVIEW/CMakeLists.txt)
endif(NOT DISABLE_VIEW)

if(NOT DISABLE_QUEASY)
	include(APPLICATIONS/QuEasyRun/CMakeLists.txt)
endif(NOT DISABLE_QUEASY)

if(NOT ${DY_LIB_DIRS} STREQUAL "")
	set(ld_name LD_LIBRARY_PATH)
	if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
		set(ld_name DYLD_LIBRARY_PATH)
	endif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	message(STATUS "\n\nPlease don't forget to add \n  ${DY_LIB_DIRS}\nto your LD_LIBRARY_PATH !\n\n")
endif(NOT ${DY_LIB_DIRS} STREQUAL "")

# include script for building deb-/dmg-package
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	#include(cmake/package_deb.cmake)
	include(cmake/package_stgz.cmake)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
	include(cmake/package_dmg.cmake)
endif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
