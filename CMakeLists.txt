# CMake configuration for BALL (http://www.ball-project.org)
#
# This file is - on purpose - heavily based on the CMake configuration
# of OpenMS (http://www.openms.de) to optimize synergy effects

PROJECT("BALL")

# TODO LIST:
#   BALL specific:
#
#   inherited from OpenMS:
#			- test for contrib headers (CONTRIB_HEADER_DIR ccmake variable - test one header for each required library)
#			- parallel build of test executables
#			- target to execute all EXAMPLES

########################################################
### entries meant to be configured using CMake cache ###
### - do NOT hardcode them here!										 ###
### - edit them within CMakeCache.txt using ccmake	 ###
########################################################
# BALL_BUILD_TYPE
# CONTRIB_CUSTOM_DIR
# STL_DEBUG
# QT_DB_PLUGIN
# MT_CUDA_BUILD_TYPE

########################################################
###    manual entries (edit this for new release)    ###
########################################################

SET(BALL_PACKAGE_VERSION_MAJOR "1")
SET(BALL_PACKAGE_VERSION_MINOR "3")
SET(BALL_PACKAGE_VERSION_PATCH "2")

########################################################
###    end manual	entries													   ###
########################################################

# Heart of the BUILD system : only edit when you know what you�re doing (we don�t)
# quick manual for most commands: http://www.cmake.org/cmake/help/cmake2.6docs.html
# useful predefined variables: http://www.paraview.org/Wiki/CMake_Useful_Variables

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE TRUE)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

SET(CF_BALL_PACKAGE_VERSION "${BALL_PACKAGE_VERSION_MAJOR}.${BALL_PACKAGE_VERSION_MINOR}.${BALL_PACKAGE_VERSION_PATCH}" CACHE INTERNAL "BALL VERSION" FORCE)

# Initialize the various PREFIX variables. This has to happen before any files
# using them are included.
#SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Installation prefix for BALL")

## default build type (for Makefile generators): Release
## TODO: 64 Bit builds etc for Windows
SET(BALL_BUILD_TYPE "Release" CACHE STRING "Build type: 'Debug' or 'Release'")

IF ("${BALL_BUILD_TYPE}" STREQUAL "Debug")
	MESSAGE(STATUS "BALL build type: ${BALL_BUILD_TYPE} (compiler debug info and BALL debug mode enabled)")
	SET(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "This must be equal to BALL_BUILD_TYPE!" FORCE)
ELSEIF ("${BALL_BUILD_TYPE}" STREQUAL "Release")
	MESSAGE(STATUS "BALL build type: ${BALL_BUILD_TYPE} (optimization enabled)")
	SET(CMAKE_BUILD_TYPE "Release" CACHE INTERNAL "This must be equal to BALL_BUILD_TYPE!" FORCE)
ELSE()
	MESSAGE(FATAL_ERROR "BALL_BUILD_TYPE must be 'Debug' or 'Release'!")
ENDIF()

SET(BALL_CONTRIB_PATH "" CACHE PATH "Path to the contrib directory, if installed (optional)")
SET(CMAKE_PREFIX_PATH ${BALL_CONTRIB_PATH} ${CMAKE_PREFIX_PATH})

SET(CMAKE_REQUIRED_INCLUDES ${BALL_CONTRIB_PATH}/include ${CMAKE_REQUIRED_INCLUDES})

SET(BALL_TEST_DATA_PATH ${PROJECT_SOURCE_DIR}/source/TEST/data)
SET(BALL_BENCHMARK_DATA_PATH ${PROJECT_SOURCE_DIR}/source/BENCHMARKS/data)

# Should we build packages as well?
OPTION(BALL_ENABLE_PACKAGING "Enable the building of binary packages" OFF)

# Used for linking BALL dependent applications and libraries (BALLView, Python bindings, ...)
SET(BALL_LIBRARIES optimized BALL debug BALLd)
SET(VIEW_LIBRARIES optimized VIEW debug VIEWd)

# some of our own macros (BALL_CHECKLIB, QT4_WRAP_UI_OWN, ...)
INCLUDE(cmake/BALLMacros.cmake)
INCLUDE(cmake/BALLCompilerSpecific.cmake)
INCLUDE(cmake/BALLConfiguration.cmake)
INCLUDE(cmake/BALLComponents.cmake)

## additional include directories
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include/) ## for configured files, i.e. config.h
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${BALL_CONTRIB_PATH}/include/)

## Extra libraries we want to link into BALL
SET(BALL_DEP_LIBRARIES)
SET(VIEW_DEP_LIBRARIES)

## Flex and Bison
## This ugly hack is required as long as we cannot tell cmake to set an environment variable for bison...
IF (WIN32)
	FIND_PROGRAM(BISON_EXECUTABLE BALL-bison.bat DOC "path to the bison executable")
ENDIF()
FIND_PACKAGE(BISON)
IF (NOT BISON_FOUND)
	MESSAGE(SEND_ERROR "Could not find bison parser generator!")
ENDIF()

IF (WIN32)
	SET(BALL_BISON_BAT ${BISON_EXECUTABLE})
	SET(BISON_EXECUTABLE "call")
ENDIF()

FIND_PACKAGE(FLEX)
IF (NOT FLEX_FOUND)
	MESSAGE(SEND_ERROR "Could not find flex scanner generator!")
ENDIF()

## Third-party libraries ##

## XDR related stuff
FIND_PACKAGE(XDR)
IF (XDR_FOUND)
	INCLUDE(cmake/BALLConfigXDR.cmake)
ENDIF()

## Boost and Asio
INCLUDE(cmake/BALLConfigBoost.cmake)
INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LIST(APPEND BALL_DEP_LIBRARIES ${BOOST_LIBRARIES})

## GSL
FIND_PACKAGE(GSL)
IF (NOT GSL_FOUND)
	MESSAGE(SEND_ERROR "gsl is required for BALL but could not be found!")
ELSE()
	ADD_DEFINITIONS(${CMAKE_GSL_CXX_FLAGS})
	INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIR})
	LIST(APPEND BALL_DEP_LIBRARIES ${GSL_LIBRARIES})
ENDIF()

## BLAS
FIND_PACKAGE(BLAS)
IF (BLAS_FOUND)
	SET(BALL_HAS_BLAS TRUE)
	INCLUDE_DIRECTORIES(${BLAS_INCLUDE_DIR})
	LIST(APPEND BALL_DEP_LIBRARIES ${BLAS_LIBRARIES})
ENDIF()

## LAPACK
FIND_PACKAGE(LAPACK)
IF (LAPACK_FOUND)
	SET(BALL_HAS_LAPACK TRUE)
	INCLUDE_DIRECTORIES(${LAPACK_INCLUDE_DIR})
	LIST(APPEND BALL_DEP_LIBRARIES ${LAPACK_LIBRARIES})
ENDIF()

## FFTW
OPTION(USE_FFTWD "Use double precision FFTW if found" ON)
OPTION(USE_FFTWF "Use single precision FFTW if found" ON)
OPTION(USE_FFTWF "Use long double precision FFTW if found" ON)
OPTION(USE_FFTW_THREADS "Try to find FFTW with thread support" OFF)

FIND_PACKAGE(FFTW)
IF(FFTW_FOUND)
	INCLUDE(cmake/BALLConfigFFTW.cmake)
ENDIF()

#Set the default complex precision to float if the setting for this value
#could not be determined earlier
IF(NOT BALL_COMPLEX_PRECISION)
	SET(BALL_COMPLEX_PRECISION "float")
ENDIF()

## lpsolve
OPTION(USE_LPSOLVE "Compile with lpsolve support, if lpsolve can be found" ON)

FIND_PACKAGE(LPSolve)
IF (LPSOLVE_FOUND)
	SET(BALL_HAS_LPSOLVE TRUE)
	INCLUDE_DIRECTORIES(${LPSOLVE_INCLUDE_DIR})
ENDIF()

## VRPN
FIND_PACKAGE(VRPN)
IF (VRPN_FOUND)
	SET(BALL_HAS_VRPN TRUE)
	INCLUDE_DIRECTORIES(${VRPN_INCLUDE_DIR})
ENDIF()

## libsvm
OPTION(USE_LIBSVM "Compile with libsvm support if libsvm can be found" ON)

FIND_PACKAGE(libSVM)
IF (LIBSVM_FOUND)
	SET(BALL_HAS_LIBSVM TRUE)
	INCLUDE_DIRECTORIES(${LIBSVM_INCLUDE_DIRS})
ENDIF()

## CUDA
SET(MT_ENABLE_CUDA OFF CACHE BOOL "Should CUDA support be enabled (version <= 2.1 currently supported).")

IF(MT_ENABLE_CUDA)
	FIND_PACKAGE(CUDA REQUIRED)
ENDIF()

## TBB
SET(MT_ENABLE_TBB OFF CACHE BOOL "Should Intel Threading Building Blocks support be enabled.")

IF(MT_ENABLE_TBB)
	FIND_PACKAGE(TBB REQUIRED)
	INCLUDE_DIRECTORIES(${TBB_INCLUDE_DIRS})
	BALL_COMBINE_LIBS(TBB_LIBRARIES "${TBB_LIBRARIES}" "${TBB_DEBUG_LIBRARIES}")
ENDIF()

## MPI
OPTION(MT_ENABLE_MPI "Should MPI support be enabled.")

IF (MT_ENABLE_MPI)
	FIND_PACKAGE(MPI REQUIRED)

	ADD_DEFINITIONS(${MPI_COMPILE_FLAGS})
	INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})

	SET(BALL_HAS_MPI TRUE)
ENDIF()

## VIEW related options/settings/libraries
OPTION(BALL_HAS_VIEW "Compile BALL's VIEW library required for visualization and graphical user interfaces" ON)

## QT
OPTION(BALL_HAS_QTSQL "Compile BALL's database adapaters (requires QtSql)" ON)

FIND_PACKAGE(Qt4 REQUIRED)

SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSQL ${BALL_HAS_QTSQL})
SET(QT_USE_QTOPENGL TRUE)
SET(QT_USE_QTTEST TRUE)
SET(QT_MIN_VERSION "4.4.0")

IF (NOT BALL_HAS_VIEW)
	SET(QT_DONT_USE_QTGUI TRUE)
ENDIF()

INCLUDE(${QT_USE_FILE})

MESSAGE(STATUS "QT qmake at ${QT_QMAKE_EXECUTABLE}")
MESSAGE(STATUS "QT moc at ${QT_MOC_EXECUTABLE}")
MESSAGE(STATUS "QT uic at ${QT_UIC_EXECUTABLE}")

## VIEW dependent packages
IF (BALL_HAS_VIEW)

	## OpenGL
	FIND_PACKAGE(OpenGL)
	IF (NOT OPENGL_FOUND OR NOT OPENGL_GLU_FOUND)
		MESSAGE(SEND_ERROR "Could not find gl/glu!")
	ENDIF()

	## GLEW
	FIND_PACKAGE(GLEW)
	IF (GLEW_FOUND)
		SET(BALL_HAS_GLEW TRUE)
	ENDIF()

ENDIF(BALL_HAS_VIEW)

## Python
OPTION(BALL_PYTHON_SUPPORT "Compile BALL's python interface (requires python development packages)" ON)

IF (BALL_PYTHON_SUPPORT)
	FIND_PACKAGE(PythonInterp)
	IF (PYTHONINTERP_FOUND)
		FIND_PACKAGE(PythonLibs)
		IF (PYTHONLIBS_FOUND)
			BALL_COMBINE_LIBS(PYTHON_LIBRARIES "${PYTHON_LIBRARIES}" "${PYTHON_DEBUG_LIBRARIES}")
			FIND_PACKAGE(SIP)
			SET(BALL_SIP_VERSION ${SIP_VERSION})
			SET(BALL_SIP_VERSION_STR ${SIP_VERSION_STR})
		ENDIF()
	ENDIF()

	IF (NOT PYTHONINTERP_FOUND OR NOT PYTHONLIBS_FOUND OR NOT SIP_FOUND)
		SET(BALL_PYTHON_SUPPORT OFF)
		MESSAGE(WARNING "Disabling python support.")
	ELSE()
		INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
		INCLUDE_DIRECTORIES(${SIP_INCLUDE_DIR})
	ENDIF()
ENDIF()

## On Windows, we might need a seperate regex2-lib
IF (WIN32)
	IF (NOT BALL_HAS_BOOST_REGEX)
		FIND_LIBRARY(REGEX_LIBRARIES
			NAMES regex2 regex
			PATH_SUFFIXES lib dlls
			DOC "Regular expressions library")

		IF (NOT REGEX_LIBRARIES)
			MESSAGE(SEND_ERROR "Could not find required regex2.lib!")
		ENDIF()
	ENDIF()
ENDIF()


OPTION(BALL_BUILD_QUEASY "Build BALL with QSAR frontends" OFF)

IF (BALL_BUILD_QUEASY)
	FIND_PACKAGE(Qwt5 REQUIRED)
	
	SET(QWT_LIBRARIES ${Qwt5_Qt4_LIBRARY})
ENDIF ()

## replace any variables in config.h.in with current values
SET(CONFIGURED_CONFIG_H ${PROJECT_BINARY_DIR}/include/BALL/CONFIG/config.h)
CONFIGURE_FILE(cmake/config.h.in ${CONFIGURED_CONFIG_H})

## build a config.h file for the python support to not disturb the
## rest of BALL
CONFIGURE_FILE(cmake/BALLPythonConfig.h.in ${PROJECT_BINARY_DIR}/include/BALL/PYTHON/BALLPythonConfig.h)

## build a config.h file for our tests to not disturb the rest of BALL
CONFIGURE_FILE(cmake/BALLTestConfig.h.in ${PROJECT_BINARY_DIR}/source/TEST/BALLTestConfig.h)

## build a config.h file for our benchmarks to not disturb the rest of BALL
CONFIGURE_FILE(cmake/BALLBenchmarkConfig.h.in ${PROJECT_BINARY_DIR}/source/BENCHMARKS/BALLBenchmarkConfig.h)

ADD_DEFINITIONS(${BALL_DEFINITIONS})

SET(BALL_EXECUTABLES "")
########################################################
###    BUILD the lib																 ###
########################################################

## are we building a shared or static lib?! (BOTH within 
## the same BUILD-tree is NOT possible currently!!)
SET(BUILD_SHARED_LIBS TRUE)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

## These variables are used in all install-targets
IF (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(BALL_RUNTIME_INSTALL_DIRECTORY "bin")
	SET(BALL_LIBRARY_INSTALL_DIRECTORY "lib")
	SET(BALL_ARCHIVE_INSTALL_DIRECTORY "lib")
	SET(BALL_HEADER_INSTALL_DIRECTORY   ".")
	SET(BALL_DATA_INSTALL_DIRECTORY    "share/BALL")
	SET(BALL_DOCUMENTATION_INSTALL_DIRECTORY    "share/BALL/doc")
	IF(WIN32)
		SET(BALL_PYTHON_INSTALL_DIRECTORY  "bin")
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  "bin")
	ELSE()
		SET(BALL_PYTHON_INSTALL_DIRECTORY  "lib")
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  "lib")
	ENDIF()
ELSE()
	SET(BALL_BUNDLE_INSTALL_DIRECTORY  					"Applications")
	SET(BALL_RUNTIME_INSTALL_DIRECTORY 					"Applications/BALLView.app/Contents/Frameworks")
	SET(BALL_LIBRARY_INSTALL_DIRECTORY 					"Applications/BALLView.app/Contents/Frameworks")
	SET(BALL_ARCHIVE_INSTALL_DIRECTORY 					"Applications/BALLView.app/Contents/Frameworks")
	SET(BALL_DATA_INSTALL_DIRECTORY    					"Applications/BALLView.app/Contents/Resources")
	SET(BALL_DOCUMENTATION_INSTALL_DIRECTORY    "Applications/BALLView.app/Contents/Resources/doc")
	SET(BALL_HEADER_INSTALL_DIRECTORY  					"Applications/BALLView.app/Contents/Frameworks")
	SET(BALL_PYTHON_INSTALL_DIRECTORY  					"Applications/BALLView.app/Contents/Frameworks")
	SET(BALL_PLUGIN_INSTALL_DIRECTORY           "Applications/BALLView.app/Contents/plugins")
ENDIF()

## big include file for headers and C files, which fills the BALL_sources variable
INCLUDE(cmake/BALLIncludes.cmake)
IF (BALL_HAS_VIEW)
	INCLUDE(cmake/VIEWIncludes.cmake)
ENDIF()

IF (MSVC)
	## use BALLd.dll, VIEWd.dll in debug mode
	SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()

## mark header files as headers...
SET_PROPERTY(SOURCE ${BALL_headers} APPEND PROPERTY HEADER_FILE_ONLY TRUE)
IF (BALL_HAS_VIEW)
	SET_PROPERTY(SOURCE ${VIEW_headers} APPEND PROPERTY HEADER_FILE_ONLY TRUE)
ENDIF()
## add library target
## warning: set BUILD_SHARED_LIBS to decide if library is shared or static (see above)! 
## We need the BUILD_SHARED_LIBS flag to set declspec flags for MSVC!
IF (FOUND_CUDART AND MT_ENABLE_CUDA) # we need both conditions due to possible present cached entries
	CUDA_ADD_LIBRARY(BALL ${BALL_sources} ${BALL_headers} ${Cuda_sources})
	IF (BALL_HAS_VIEW)
		CUDA_ADD_LIBRARY(VIEW ${VIEW_sources} ${VIEW_headers} ${Cuda_sources})
	ENDIF()
ELSE()
	ADD_LIBRARY(BALL SHARED ${BALL_sources} ${BALL_headers})
	IF (BALL_HAS_VIEW)
		ADD_LIBRARY(VIEW SHARED ${VIEW_sources} ${VIEW_headers})
	ENDIF()
ENDIF()

## all the dependency libraries are linked into libBALL.so, except Qt and CUDA which are still dynamic
LIST(APPEND BALL_DEP_LIBRARIES
	${BOOST_LIBRARIES}
	${REGEX_LIBRARIES}
	${XDR_LIBRARIES}
	${GSL_LIBRARIES}
	${FFTW_LIBRARIES}
	${LPSOLVE_LIBRARIES}
	${LIBSVM_LIBRARIES}
	${MPI_LIBRARIES}
	${PYTHON_LIBRARIES}
)

IF(TBB_FOUND)
	LIST(APPEND BALL_DEP_LIBRARIES ${TBB_LIBRARIES})
ENDIF()

SET_TARGET_PROPERTIES(BALL PROPERTIES SOVERSION "${BALL_PACKAGE_VERSION_MAJOR}.${BALL_PACKAGE_VERSION_MINOR}")
IF (MSVC)
	SET(BALL_COMPILE_DEFINTIONS ${COMPILE_DEFINITIONS} "BALL_BUILD_DLL" CACHE INTERNAL "")
	SET_TARGET_PROPERTIES(BALL PROPERTIES COMPILE_DEFINITIONS "${BALL_COMPILE_DEFINTIONS}")
	# With MALL, BALL breaks incremental linking (seems to be too large...)
	SET(BALL_LINK_FLAGS "${BALL_LINK_FLAGS} /NODEFAULTLIB:libcmt /INCREMENTAL:no")
ELSEIF (APPLE)
	SET_TARGET_PROPERTIES(BALL PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
ENDIF()

IF (BALL_HAS_VIEW)
## TODO: Qt handling!
	LIST(APPEND VIEW_DEP_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARY})

	IF (BALL_BUILD_QUEASY)
		LIST(APPEND VIEW_DEP_LIBRARIES ${QWT_LIBRARIES})
	ENDIF()

	SET_TARGET_PROPERTIES(VIEW PROPERTIES SOVERSION "${BALL_PACKAGE_VERSION_MAJOR}.${BALL_PACKAGE_VERSION_MINOR}")
	IF(MSVC)
		## Do we really need this?
		SET(VIEW_COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} "BALL_VIEW_BUILD_DLL" CACHE INTERNAL "")
		SET_TARGET_PROPERTIES(VIEW PROPERTIES COMPILE_DEFINITIONS "${VIEW_COMPILE_DEFINITIONS}")
		SET(VIEW_LINK_FLAGS "${VIEW_LINK_FLAGS} /NODEFAULTLIB:libcmt")
	ELSEIF(APPLE)
		SET_TARGET_PROPERTIES(VIEW PROPERTIES INSTALL_NAME_DIR "@executable_path/../Frameworks")
	ENDIF()

ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
	IF(ENABLE_COVERAGE_TESTING)
		SET(BALL_COMPILE_FLAGS "${BALL_COMPILE_FLAGS} -fprofile-arcs -ftest-coverage")
		SET(BALL_LINK_FLAGS "${BALL_LINK_FLAGS} -fprofile-arcs")
		SET(VIEW_COMPILE_FLAGS "${VIEW_COMPILE_FLAGS} -fprofile-arcs -ftest-coverage")
		SET(VIEW_LINK_FLAGS "${VIEW_LINK_FLAGS} -fprofile-arcs")
	ENDIF()

	IF(USE_HIDDEN_VISIBILITY)
		SET(BALL_COMPILE_FLAGS "${BALL_COMPILE_FLAGS} -fvisibility=hidden")
		SET(VIEW_COMPILE_FLAGS "${VIEW_COMPILE_FLAGS} -fvisibility=hidden")
	ENDIF()

	IF(USE_AS_NEEDED)
		SET(BALL_LINK_FLAGS "${BALL_LINK_FLAGS} -Wl,--as-needed")
		SET(VIEW_LINK_FLAGS "${VIEW_LINK_FLAGS} -Wl,--as-needed")
	ENDIF()
ENDIF()

TARGET_LINK_LIBRARIES(BALL ${BALL_DEP_LIBRARIES}
                           ${QT_QTCORE_LIBRARY}
                           ${QT_QTSQL_LIBRARY}
                           ${QT_QTNETWORK_LIBRARY}
                           ${QT_QTXML_LIBRARY})


TARGET_LINK_LIBRARIES(VIEW BALL
                           ${VIEW_DEP_LIBRARIES}
                           ${QT_QTCORE_LIBRARY}
                           ${QT_QTNETWORK_LIBRARY}
                           ${QT_QTGUI_LIBRARY}
                           ${QT_QTTEST_LIBRARY}
                           ${QT_QTOPENGL_LIBRARY})

## add targets for Python modules
ADD_SUBDIRECTORY(source/PYTHON/EXTENSIONS)

## add the plugins
ADD_SUBDIRECTORY(source/EXTENSIONS)

## add BALLView target
ADD_SUBDIRECTORY(source/APPLICATIONS/BALLVIEW)
LIST(APPEND BALL_EXECUTABLES "BALLView" "BALLView")

## add BENCMARK target
ADD_SUBDIRECTORY(source/BENCHMARKS/)

SET_TARGET_PROPERTIES(BALL PROPERTIES
	COMPILE_FLAGS "${BALL_COMPILE_FLAGS}"
	LINK_FLAGS "${BALL_LINK_FLAGS}"
)

SET_TARGET_PROPERTIES(VIEW PROPERTIES
	COMPILE_FLAGS "${VIEW_COMPILE_FLAGS}"
	LINK_FLAGS "${VIEW_LINK_FLAGS}"
)

###### add Utility targets ########
SET(UTILITIES_EXECUTABLES)
INCLUDE(source/APPLICATIONS/UTILITIES/BALLUtilities.cmake)

FOREACH(i ${UTILITIES_EXECUTABLES})
	ADD_EXECUTABLE(${i} source/APPLICATIONS/UTILITIES/${i}.C)
	TARGET_LINK_LIBRARIES(${i} BALL VIEW)
ENDFOREACH(i)
ADD_CUSTOM_TARGET(UTILITIES EXCLUDE_FROM_ALL)
ADD_DEPENDENCIES(UTILITIES ${UTILITIES_EXECUTABLES})

###### add MMFF94 test targets ##########
SET(MMFF94_EXECUTABLES)
INCLUDE(source/APPLICATIONS/MMFF94/BALLMMFF94.cmake)

FOREACH(i ${MMFF94_EXECUTABLES})
	ADD_EXECUTABLE(${i} source/APPLICATIONS/MMFF94/${i}.C)
	TARGET_LINK_LIBRARIES(${i} BALL VIEW)
ENDFOREACH(i)
ADD_CUSTOM_TARGET(MMFF94 EXCLUDE_FROM_ALL)
ADD_DEPENDENCIES(MMFF94 ${MMFF94_EXECUTABLES})

OPTION(BALL_BUILD_QUEASY "Build BALL with QSAR frontends" OFF)

IF (BALL_BUILD_QUEASY)
	## add QuEasyRun target
	ADD_SUBDIRECTORY(source/APPLICATIONS/QuEasyRun)

	## add QuEasyViz target
	ADD_SUBDIRECTORY(source/APPLICATIONS/QuEasyViz)
	LIST(APPEND BALL_EXECUTABLES "QuEasyViz" "QuEasyViz")
ENDIF()

########################################################
###    Documentation    														 ###
########################################################

INCLUDE(cmake/BALLDoc.cmake)

######################################################################
####        Installation Routines                                  ###
######################################################################

IF (WIN32)
	INCLUDE(InstallRequiredSystemLibraries)
ENDIF()

INSTALL(TARGETS BALL
	EXPORT BALLExportGroup
	COMPONENT "${COMPONENT_LIBBALL}"
	RUNTIME DESTINATION "${BALL_RUNTIME_INSTALL_DIRECTORY}"
	LIBRARY DESTINATION "${BALL_LIBRARY_INSTALL_DIRECTORY}"
	ARCHIVE DESTINATION "${BALL_ARCHIVE_INSTALL_DIRECTORY}"
)

INSTALL(TARGETS VIEW
	EXPORT BALLExportGroup
	COMPONENT "${COMPONENT_LIBVIEW}"
	RUNTIME DESTINATION "${BALL_RUNTIME_INSTALL_DIRECTORY}"
	LIBRARY DESTINATION "${BALL_LIBRARY_INSTALL_DIRECTORY}"
	ARCHIVE DESTINATION "${BALL_ARCHIVE_INSTALL_DIRECTORY}"
)

## If we are not installing into the source directory directly, take
## care of data files and headers
IF (NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL ".")
	## Install the data directories
	INSTALL(DIRECTORY data DESTINATION "${BALL_DATA_INSTALL_DIRECTORY}" COMPONENT ${COMPONENT_LIBBALL})
	
	## Install the includes
	INSTALL(DIRECTORY include DESTINATION "${BALL_HEADER_INSTALL_DIRECTORY}" COMPONENT ${COMPONENT_LIBBALL_DEV})
ENDIF()

## The configured files have to be copied in any case
INSTALL(FILES 
		${PROJECT_BINARY_DIR}/include/BALL/CONFIG/config.h 
		DESTINATION "${BALL_HEADER_INSTALL_DIRECTORY}/include/BALL/CONFIG"
		COMPONENT ${COMPONENT_LIBBALL_DEV}
)

INSTALL(FILES 
		${PROJECT_BINARY_DIR}/include/BALL/PYTHON/BALLPythonConfig.h
		DESTINATION "${BALL_HEADER_INSTALL_DIRECTORY}/include/BALL/PYTHON"
		COMPONENT ${COMPONENT_LIBBALL_DEV}
)

IF (WIN32)
	SET(FIND_DLL_INPUT ${BALL_DEP_LIBRARIES} ${SIP_LIBRARIES} ${VIEW_DEP_LIBRARIES} ${QT_LIBRARIES})
	BALL_LIB_TO_DLL(BALL_DEP_DEBUG_DLLS BALL_DEP_OPT_DLLS "${FIND_DLL_INPUT}")
	FOREACH(LIB ${BALL_DEP_OPT_DLLS})
		STRING(TOLOWER "${LIB}" LOWER_LIB)
		IF (NOT LOWER_LIB MATCHES "system32|syswow64")
			INSTALL(FILES "${LIB}" 
				DESTINATION "${BALL_RUNTIME_INSTALL_DIRECTORY}"
				COMPONENT ${COMPONENT_LIBBALL})
		ENDIF()
	ENDFOREACH()
ENDIF()

#IF (NOT APPLE AND NOT MSVC) ##=Linux
#	target_link_libraries(OpenMS ${OPENMS_DEP_LIBRARIES}) ## do not link OpenMS against QT on Linux
#else()
#	target_link_libraries(OpenMS ${QT_LIBRARIES} ${OPENMS_DEP_LIBRARIES})
#endif()

## OPENMS_LIBRARIES defines the libraries used by OpenMS; this should be used to link against executables
#set(OPENMS_LIBRARIES OpenMS)
#if (MSVC) ## Windows
#	## somehow the link dependencies of cmake are broken such that when using POSTFIX names for libs the dependencies are not forwarded...
#	## we fix this by adding the dependencies of OpenMS directly to the executables as well.
#	set(OPENMS_LIBRARIES optimized OpenMS${CMAKE_RELEASE_POSTFIX} debug OpenMS${CMAKE_DEBUG_POSTFIX} ${QT_LIBRARIES} ${OPENMS_DEP_LIBRARIES})
#elseif (NOT APPLE) ## Linux
#	set(OPENMS_LIBRARIES optimized OpenMS${CMAKE_RELEASE_POSTFIX} debug OpenMS${CMAKE_DEBUG_POSTFIX} ${QT_LIBRARIES})
#endif()
#
### directory for OpenMS(d).lib (required for linking executables)
#if (MSVC)
#	link_directories(${PROJECT_BINARY_DIR})
#endif()

####### BALLView #########
#include(source/APPLICATIONS/BALLView/executables.cmake)
#
#set(BALLVIEW_DIR source/APPLICATIONS/BALLVIEW/)
#foreach(i ${TOPP_executables})
#	set(resource_file ${CMAKE_SOURCE_DIR}/${TOPP_DIR}${i}.rc)
#	set(resource_dir ${CMAKE_SOURCE_DIR}/${TOPP_DIR}/${i}-resources/)
#	## add icons to TOPPView and INIFileEditor
#	if (MSVC AND EXISTS ${resource_file})
#		Message(STATUS "Setting resource file ${resource_file} for ${i}")
#		add_executable(${i} ${TOPP_DIR}${i}.C ${resource_file})
#  elseif(APPLE AND EXISTS ${resource_dir}) 
#    set_source_files_properties(${resource_dir}/${i}.icns 
#      PROPERTIES
#      MACOSX_PACKAGE_LOCATION Resources)
#
#    add_executable(${i} MACOSX_BUNDLE ${TOPP_DIR}${i}.C ${resource_dir}/${i}.icns)         
#    ## transform TOPPView and INIFileEditor to real Mac OS X applications
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_INFO_STRING "${i} - Version 1.5")
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "1.5")
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_VERSION "1.5")
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "1.5")
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_ICON_FILE "${i}.icns")
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "de.openms.${i}")
#    set_target_properties(${i} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${i}") 
#  else()
#		add_executable(${i} ${TOPP_DIR}${i}.C)
#	endif()
#	
#	target_link_libraries(${i} ${OPENMS_LIBRARIES})
#	if (OPENMP_FOUND AND NOT MSVC)
#		set_target_properties(${i} PROPERTIES LINK_FLAGS ${OpenMP_CXX_FLAGS})
#	endif()
#endforeach(i)
#add_custom_target(TOPP)
#add_dependencies(TOPP ${TOPP_executables})
#
#
###### Doc progs ####
#set(OpenMS_doc_executables)
#include(doc/doxygen/parameters/executables.cmake)
#foreach(i ${OpenMS_doc_executables})
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "doc/doxygen/parameters")
#	add_executable(${i} EXCLUDE_FROM_ALL doc/doxygen/parameters/${i}.C)
#	target_link_libraries(${i} ${OPENMS_LIBRARIES})
#	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
#endforeach(i)
#add_custom_target(doc_progs)
#add_dependencies(doc_progs ${OpenMS_doc_executables} TOPP UTILS)

##### Packaging #####
IF (BALL_ENABLE_PACKAGING)
	INCLUDE(cmake/BALLPackageConfig.cmake)
ENDIF()

##### Tests #####
#IF ("${PACKAGE_TYPE}" STREQUAL "none")
	INCLUDE(Dart)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY(source/TEST EXCLUDE_FROM_ALL) ## configure the Test project (but do not add it to BALL itself)
#ENDIF()

#### configs for external include files, e.g. to develop code with OpenMS
#configure_file(
#	"${PROJECT_SOURCE_DIR}/cmake/OpenMSConfig.cmake.in"
#	"${PROJECT_BINARY_DIR}/cmake/OpenMSConfig.cmake"
#	@ONLY
#)


##### targets list #####
INCLUDE(cmake/BALLTargets.cmake)
	
##### Message after BALL has been built #####
INCLUDE(cmake/BALLFinishedMessage.cmake)

##### Messages at the end of cmake #####
MESSAGE(STATUS "")
MESSAGE(STATUS "-----------------------------------------------------------------")
MESSAGE(STATUS "")
MESSAGE(STATUS "You have successfully configured BALL.")
MESSAGE(STATUS "")
IF (MSVC)
  MESSAGE(STATUS "Execute the 'targets' project to see prominent targets!")
ELSE()
  MESSAGE(STATUS "For a full list of make targets execute:")
  MESSAGE(STATUS "'make targets'")
ENDIF()
MESSAGE(STATUS "")
MESSAGE(STATUS "-----------------------------------------------------------------")
MESSAGE(STATUS "")

######################################################
#### build settings and configs for external code ####
######################################################

## grep definitions (-D<XXX>) that were set using 'add_definitions()'
GET_PROPERTY(BALL_ADDED_COMPILE_DEFINITIONS DIRECTORY . PROPERTY COMPILE_DEFINITIONS)
## convert to CXX flags
FOREACH(CURRENT_DEFINITION ${BALL_ADDED_COMPILE_DEFINITIONS})
	LIST(APPEND CF_BALL_ADDCXX_FLAGS "-D${CURRENT_DEFINITION}")
ENDFOREACH()

## for add_definition commands (currently empty, but who knows...)
SET(CF_BALL_ADD_DEFINITIONS)

## the filename for library settings (this file will be included in BALLConfig.cmake)
SET(CF_BALLExport ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)

## create code that allows to call find_package()
## (must come after the above code that sets the variables!)
GET_DIRECTORY_PROPERTY(BALL_INCLUDE_DIRS  INCLUDE_DIRECTORIES)
CONFIGURE_FILE(
	"${PROJECT_SOURCE_DIR}/cmake/BALLConfig.cmake.in"
	"${PROJECT_BINARY_DIR}/cmake/BALLConfig.cmake"
	@ONLY
)

## Create BALLExport.cmake
STRING(COMPARE LESS ${CMAKE_MINOR_VERSION} "8" CMAKE_DEPRECATED_VERSION)
IF (${CMAKE_DEPRECATED_VERSION})
	MESSAGE(STATUS "Cannot register BALL with CMake! For external code, set the path to BALL during find_package() manually.")
ELSE()
	## register BALL with CMake so that it can be found easily
	EXPORT(PACKAGE BALL)
ENDIF()
## export our libs
EXPORT(TARGETS BALL VIEW FILE ${CF_BALLExport})

INSTALL(EXPORT BALLExportGroup DESTINATION cmake/ FILE BALLExport.cmake)

INCLUDE(cmake/BALLCTags.cmake)
