# CMake configuration for BALL (http://www.ball-project.org)
#
# This file is - on purpose - heavily based on the CMake configuration
# of OpenMS (http://www.openms.de) to optimize synergy effects

PROJECT("BALL")

########################################################
###    manual entries (edit this for new release)    ###
########################################################

SET(BALL_PACKAGE_VERSION_MAJOR "1")
SET(BALL_PACKAGE_VERSION_MINOR "4")
SET(BALL_PACKAGE_VERSION_PATCH "79")

########################################################
###    end manual entries                            ###
########################################################

# Heart of the BUILD system:
# Only edit when you know what you are doing!
#
# Quick manual for most commands:     https://cmake.org/cmake/help/v2.8.12/cmake.html
# Useful predefined CMake variables:  https://cmake.org/Wiki/CMake_Useful_Variables

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12 FATAL_ERROR)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

SET(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE TRUE)

IF(POLICY CMP0020)
        CMAKE_POLICY(SET CMP0020 OLD)
ENDIF()

IF(POLICY CMP0042)
        CMAKE_POLICY(SET CMP0042 OLD)
ENDIF()

IF(POLICY CMP0043)
	CMAKE_POLICY(SET CMP0043 NEW)
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

OPTION(BALL_USE_PROJECT_FOLDERS ON)
IF (BALL_USE_PROJECT_FOLDERS)
	SET(USE_FOLDERS TRUE)
ENDIF()

SET(CF_BALL_PACKAGE_VERSION "${BALL_PACKAGE_VERSION_MAJOR}.${BALL_PACKAGE_VERSION_MINOR}.${BALL_PACKAGE_VERSION_PATCH}" CACHE INTERNAL "BALL VERSION" FORCE)

# Initialize the various PREFIX variables. This has to happen before any files
# using them are included.
#SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH "Installation prefix for BALL")

## default build type (for Makefile generators): Release
## TODO: 64 Bit builds etc for Windows
IF(BALL_BUILD_TYPE)
	MESSAGE(WARNING "The BALL_BUILD_TYPE should no longer be used. Use CMAKE_BUILD_TYPE instead")
	SET(CMAKE_BUILD_TYPE ${BALL_BUILD_TYPE})
ENDIF()

## if no build type is given, default to Release
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF()

SET(BALL_CONTRIB_PATH "" CACHE PATH "Path to the contrib directory, if installed (optional)")
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${BALL_CONTRIB_PATH})

IF(UNIX AND BALL_CONTRIB_PATH)
	SET(CMAKE_LIBRARY_PATH ${BALL_CONTRIB_PATH}/lib ${BALL_CONTRIB_PATH}/lib64 ${CMAKE_LIBRARY_PATH})
	SET(CMAKE_INCLUDE_PATH ${BALL_CONTRIB_PATH}/include ${CMAKE_INCLUDE_PATH})
ENDIF()

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
IF (BALL_CONTRIB_PATH)
	INCLUDE_DIRECTORIES(${BALL_CONTRIB_PATH}/include/)
ENDIF()

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
## Figure out if the used flex version supports ...lex_destroy()
MESSAGE(STATUS "Checking for yylex_destroy support in flex")
STRING(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.([0-9]+).*$" "\\1;\\2;\\3"
	FLEX_VERSION_NUMERIC ${FLEX_VERSION})
LIST(GET FLEX_VERSION_NUMERIC 2 FLEX_PATCH_LEVEL)
IF (FLEX_PATCH_LEVEL GREATER 8)
	MESSAGE(STATUS "Checking for yylex_destroy support in flex - found")
	SET(BALL_HAS_YYLEX_DESTROY TRUE)
ELSE()
	MESSAGE(STATUS "Checking for yylex_destroy support in flex - Not found")
	SET(BALL_HAS_YYLEX_DESTROY FALSE)
ENDIF()

## Third-party libraries ##

## XDR related stuff
FIND_PACKAGE(XDR)
IF (XDR_FOUND)
	INCLUDE(cmake/BALLConfigXDR.cmake)
ENDIF()

## Boost and Asio
INCLUDE(cmake/BALLConfigBoost.cmake)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LIST(APPEND BALL_DEP_LIBRARIES ${Boost_LIBRARIES})
# make the rest of CMake aware of boost's existence
#LIST(APPEND CMAKE_REQUIRED_INCLUDES ${Boost_INCLUDE_DIR})

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


## libsvm
OPTION(USE_LIBSVM "Compile with libsvm support if libsvm can be found" ON)

#this setting crashes cmake when libSVM is installed
#FIND_PACKAGE(libSVM)
#IF (LIBSVM_FOUND)
#	SET(BALL_HAS_LIBSVM TRUE)
#	INCLUDE_DIRECTORIES(${LIBSVM_INCLUDE_DIRS})
## ENDIF()

IF(USE_LIBSVM)
  FIND_PACKAGE(libSVM)
  IF (LIBSVM_FOUND)
  SET(BALL_HAS_LIBSVM TRUE)
  INCLUDE_DIRECTORIES(${LIBSVM_INCLUDE_DIRS}) 
  ENDIF()
ENDIF()

## CUDA
SET(USE_CUDA OFF CACHE BOOL "Should CUDA support be enabled (version <= 2.1 currently supported)?")

IF(USE_CUDA)
	FIND_PACKAGE(CUDA REQUIRED)
ENDIF()

## TBB
SET(USE_TBB ON CACHE BOOL "Should Intel Threading Building Blocks support be enabled?")
SET(REQUIRE_TBB OFF CACHE BOOL "Should Intel Threading Building Blocks support be required?")

## RTFact would require tbb... so check if it is required
IF(REQUIRE_RTFACT)
	SET(REQUIRE_TBB ON)
ENDIF()

IF(USE_TBB)
	IF (REQUIRE_TBB)
		FIND_PACKAGE(TBB REQUIRED)
	ELSE()
		FIND_PACKAGE(TBB)
		IF (NOT TBB_FOUND)
			MESSAGE(STATUS "Intel Threading Building Blocks not found!")
		ENDIF()
	ENDIF()

	IF (TBB_FOUND)
		INCLUDE_DIRECTORIES(${TBB_INCLUDE_DIRS})
		BALL_COMBINE_LIBS(TBB_LIBRARIES "${TBB_LIBRARIES}" "${TBB_DEBUG_LIBRARIES}")
		LIST(REMOVE_DUPLICATES TBB_LIBRARIES)

		SET(BALL_HAS_TBB TRUE)
	ENDIF()
ENDIF()

## MPI
OPTION(USE_MPI "Should MPI support be enabled?" ON)
OPTION(REQUIRE_MPI "Should MPI support be required?" OFF)

IF (USE_MPI)
	IF (REQUIRE_MPI)
		FIND_PACKAGE(MPI REQUIRED)
	ELSE()
		FIND_PACKAGE(MPI)
	ENDIF()

	IF (MPI_FOUND)
		ADD_DEFINITIONS(${MPI_CXX_COMPILE_FLAGS})
		INCLUDE_DIRECTORIES(${MPI_CXX_INCLUDE_PATH})

		SET(BALL_HAS_MPI TRUE)
	ENDIF()

ENDIF()

## VIEW related options/settings/libraries
OPTION(BALL_HAS_VIEW "Compile BALL's VIEW library required for visualization and graphical user interfaces" ON)

## QT
OPTION(USE_QTWEBENGINE "Add support for QtWebEngine" ON)

# Enable automatic invocation of moc if required
SET(CMAKE_AUTOMOC ON)

SET(QTDIR "" CACHE PATH "Path to the qt installation (optional)")
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QTDIR})

IF(QTDIR)
	INCLUDE_DIRECTORIES(BEFORE "${QTDIR}/include")
ENDIF()

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Xml)
FIND_PACKAGE(Qt5Network)

LIST(APPEND BALL_DEP_LIBRARIES Qt5::Core
                               Qt5::Xml
                               Qt5::Network)

IF (BALL_HAS_VIEW)
	FIND_PACKAGE(Qt5Widgets REQUIRED)
	FIND_PACKAGE(Qt5Test)
	FIND_PACKAGE(Qt5OpenGL)
	FIND_PACKAGE(Qt5PrintSupport)

	ADD_DEFINITIONS(${Qt5Widgets_DEFINITIONS})

	LIST(APPEND VIEW_DEP_LIBRARIES Qt5::Widgets
	                               Qt5::OpenGL
	                               Qt5::PrintSupport
	                               Qt5::Network
	                               Qt5::Test
	                               Qt5::Xml)

	IF(USE_QTWEBENGINE)
		FIND_PACKAGE(Qt5WebEngine 5.5 REQUIRED)
		FIND_PACKAGE(Qt5WebEngineWidgets REQUIRED)
		FIND_PACKAGE(Qt5WebChannel REQUIRED)
		IF(Qt5WebEngine_FOUND)
			MESSAGE(STATUS "Found QtWebEngine")
			IF(Qt5WebChannel_FOUND)
				MESSAGE(STATUS "Found QtWebChannel")
				SET(BALL_HAS_QTWEBENGINE TRUE)
				LIST(APPEND VIEW_DEP_LIBRARIES Qt5::WebEngineWidgets Qt5::WebChannel)
			ELSE()
				MESSAGE(STATUS "QtWebChannel has not been found. Disabling browser support.")
				SET(BALL_HAS_QTWEBENGINE FALSE)
			ENDIF()
		ELSE()
			MESSAGE(STATUS "QtWebEngine has not been found. Disabling browser support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()
	ENDIF()
ENDIF()

## On Windows, we might need to explicitly deploy openssl libraries for Qt
IF(WIN32)
	FIND_PATH(QT_SSLEAY_LIB NAME ssleay32.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH)
	IF(QT_SSLEAY_LIB)
		LIST(APPEND QT_LIBRARIES ${QT_SSLEAY_LIB}/ssleay32.dll)
	ENDIF()

	FIND_PATH(QT_LIBEAY_LIB NAME libeay32.dll PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH)
	IF(QT_SSLEAY_LIB)
		LIST(APPEND QT_LIBRARIES ${QT_LIBEAY_LIB}/libeay32.dll)
	ENDIF()
ENDIF()

SET(EIGEN3_VERSION_OK "3.0.0")
FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})

## VIEW dependent packages
IF (BALL_HAS_VIEW)

	## VIEW dependent options
	OPTION(BALL_ENABLE_CG "Build against NVIDIA Cg library" OFF)
	OPTION(BALL_REQUIRE_CG "Fail if NVIDIA Cg could not be detected" OFF)

	OPTION(USE_RTFACT "Enable RTfact support" ON)
	OPTION(REQUIRE_RTFACT "Require RTfact support" OFF)

	## OpenGL
	FIND_PACKAGE(OpenGL)
	IF (NOT OPENGL_FOUND OR NOT OPENGL_GLU_FOUND)
		MESSAGE(SEND_ERROR "Could not find gl/glu!")
	ENDIF()
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

	## RTfact
	IF (USE_RTFACT)
		IF (REQUIRE_RTFACT)
			FIND_PACKAGE(RTfact REQUIRED)
		ELSE()
			FIND_PACKAGE(RTfact)
		ENDIF()

		IF (RTFACT_FOUND)
			## RTfact requires TBB
			IF (NOT CMAKE_COMPILER_IS_INTELCXX)
				IF (NOT TBB_FOUND)
					MESSAGE(SEND_ERROR "RTfact support requires TBB!")
				ENDIF()
			ENDIF()

			SET(BALL_HAS_RTFACT TRUE)
			INCLUDE_DIRECTORIES(${RTFACT_INCLUDE_DIR})
		ENDIF()
	ENDIF()

	## Cg
	IF (BALL_ENABLE_CG)
		IF (BALL_REQUIRE_CG)
			SET(CG_MESSAGE "Looking for Cg library (required)...")
		ELSE()
			SET(CG_MESSAGE "Looking for Cg library...")
		ENDIF()

		MESSAGE(STATUS ${CG_MESSAGE})
		FIND_PACKAGE(Cg)

		IF (CG_FOUND)
			MESSAGE(STATUS "${CG_MESSAGE} - found  ${CG_LIBRARY}  ${CG_GL_LIBRARY}")
			SET(BALL_HAS_CG TRUE)
			SET(CG_LIBRARIES ${CG_LIBRARY} ${CG_GL_LIBRARY})
			INCLUDE_DIRECTORIES(${RTFACT_INCLUDE_PATH})
		ELSE()
			IF (BALL_REQUIRE_CG)
				MESSAGE(SEND_ERROR "Could not find Cg library, but Cg was marked as required!")
			ELSE()
				MESSAGE(STATUS "Disabling Cg support")
			ENDIF()
		ENDIF()
	ENDIF(BALL_ENABLE_CG)

ENDIF(BALL_HAS_VIEW)

## Python
OPTION(BALL_PYTHON_SUPPORT "Compile BALL's python interface (requires python development packages)" ON)

IF (BALL_PYTHON_SUPPORT)
	# make sure that we set the modules that come with contrib
	IF(BALL_CONTRIB_PATH)
		IF(WIN32)
			SET(ENV{PYTHONPATH} "$ENV{PYTHONPATH};${BALL_CONTRIB_PATH}/install/lib/python")
		ELSE()
			SET(ENV{PYTHONPATH} "$ENV{PYTHONPATH}:${BALL_CONTRIB_PATH}/install/lib/python")
		ENDIF()
		SET(BALL_PYTHONPATH $ENV{PYTHONPATH})
	ENDIF()
	FIND_PACKAGE(PythonInterp 2.7)
	IF (PYTHONINTERP_FOUND)
		FIND_PACKAGE(PythonLibs 2.7)
		IF (PYTHONLIBS_FOUND)
			BALL_COMBINE_LIBS(PYTHON_LIBRARIES "${PYTHON_LIBRARIES}" "${PYTHON_DEBUG_LIBRARIES}")
			SET(SIP_EXTRA_OPTIONS "-e")
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

# Check for the presence of OpenBabel version 2
SET(BALL_HAS_OPENBABEL FALSE)

FIND_PACKAGE(OpenBabel2)
IF(OPENBABEL2_FOUND)
	SET(BALL_HAS_OPENBABEL TRUE)
	INCLUDE_DIRECTORIES(${OPENBABEL2_INCLUDE_DIRS})
	LIST(APPEND BALL_DEP_LIBRARIES ${OPENBABEL2_LIBRARIES})
ELSE()
	MESSAGE("Could not find a OpenBabel2 installation. Skipping OpenBabel dependencies.")
ENDIF()

## replace any variables in config.h.in with current values
SET(CONFIGURED_CONFIG_H ${PROJECT_BINARY_DIR}/include/BALL/CONFIG/config.h)
CONFIGURE_FILE(cmake/config.h.in ${CONFIGURED_CONFIG_H})
	
## automatically create the version numbers
CONFIGURE_FILE(cmake/templates/version.h.in ${PROJECT_BINARY_DIR}/include/BALL/COMMON/version.h)
CONFIGURE_FILE(cmake/templates/BALL.doc.in  ${PROJECT_SOURCE_DIR}/include/BALL/BALL.doc)

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
		SET(BALL_TOOLS_INSTALL_DIRECTORY   "bin/BALLTools")
	ELSE()
		SET(BALL_PYTHON_INSTALL_DIRECTORY  "lib")
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  "lib")
		SET(BALL_TOOLS_INSTALL_DIRECTORY   "lib/BALL")
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
	SET(BALL_TOOLS_INSTALL_DIRECTORY            "Applications/BALLView.app/Contents/BALLTools")
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
IF (FOUND_CUDART AND USE_CUDA) # we need both conditions due to possible present cached entries
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
	${XDR_LIBRARIES}
	${FFTW_LIBRARIES}
	${LPSOLVE_LIBRARIES}
	${MPI_LIBRARIES}
	${PYTHON_LIBRARIES}
)


IF(UNIX)
	LIST(APPEND BALL_DEP_LIBRARIES "pthread")
ENDIF()

IF(TBB_FOUND)
	LIST(APPEND BALL_DEP_LIBRARIES ${TBB_LIBRARIES})
ENDIF()

IF(LIBSVM_FOUND)
	LIST(APPEND BALL_DEP_LIBRARIES ${LIBSVM_LIBRARIES})
ENDIF()

SET(BALL_COMPILE_FLAGS)
SET(BALL_LINK_FLAGS)
SET(VIEW_COMPILE_FLAGS)
SET(VIEW_LINK_FLAGS)

SET_TARGET_PROPERTIES(BALL PROPERTIES
	SOVERSION "${BALL_PACKAGE_VERSION_MAJOR}.${BALL_PACKAGE_VERSION_MINOR}"
)

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
	LIST(APPEND VIEW_DEP_LIBRARIES ${RTFACT_LIBRARIES})

	SET_TARGET_PROPERTIES(VIEW PROPERTIES
		SOVERSION "${BALL_PACKAGE_VERSION_MAJOR}.${BALL_PACKAGE_VERSION_MINOR}"
)

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
ENDIF()

TARGET_LINK_LIBRARIES(BALL ${BALL_DEP_LIBRARIES})

IF(BALL_HAS_VIEW)
	TARGET_LINK_LIBRARIES(VIEW BALL
                                   ${VIEW_DEP_LIBRARIES}
                                   ${OPENGL_LIBRARIES})
ENDIF()

IF(BALL_HAS_VIEW)
	## add BALLView target
	ADD_SUBDIRECTORY(source/APPLICATIONS/BALLVIEW)
	LIST(APPEND BALL_EXECUTABLES "BALLView" "BALLView")
ENDIF()

SET_TARGET_PROPERTIES(BALL PROPERTIES
	COMPILE_FLAGS "${BALL_COMPILE_FLAGS}"
	LINK_FLAGS "${BALL_LINK_FLAGS}"
)

IF(BALL_HAS_VIEW)
	SET_TARGET_PROPERTIES(VIEW PROPERTIES
		COMPILE_FLAGS "${VIEW_COMPILE_FLAGS}"
		LINK_FLAGS "${VIEW_LINK_FLAGS}"
	)
ENDIF()

###### Add BALLTools #########
OPTION(BALL_BUILD_TOOLS "Build BALL command line tools (BALLTools)" ON)
IF(BALL_BUILD_TOOLS)
        ADD_SUBDIRECTORY(source/APPLICATIONS/TOOLS)
        ADD_SUBDIRECTORY(source/APPLICATIONS/UTILITIES)
ENDIF()

## add targets for Python modules
ADD_SUBDIRECTORY(source/PYTHON/EXTENSIONS)

## add the plugins
ADD_SUBDIRECTORY(source/EXTENSIONS)

## add BENCHMARK target
ADD_SUBDIRECTORY(source/BENCHMARKS/)

###### add MMFF94 test targets ##########
OPTION(BALL_BUILD_MMFF94 "Build MMFF94 command line tools" ON)
IF(BALL_BUILD_MMFF94)
        ADD_SUBDIRECTORY(source/APPLICATIONS/MMFF94)
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

IF(BALL_HAS_VIEW)
	INSTALL(TARGETS VIEW
		EXPORT BALLExportGroup
		COMPONENT "${COMPONENT_LIBVIEW}"
		RUNTIME DESTINATION "${BALL_RUNTIME_INSTALL_DIRECTORY}"
		LIBRARY DESTINATION "${BALL_LIBRARY_INSTALL_DIRECTORY}"
		ARCHIVE DESTINATION "${BALL_ARCHIVE_INSTALL_DIRECTORY}"
	)
ENDIF()

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
		${PROJECT_BINARY_DIR}/include/BALL/COMMON/version.h
		DESTINATION "${BALL_HEADER_INSTALL_DIRECTORY}/include/BALL/COMMON"
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
	# NOTE: the special treatment for cg is necessary on 64 bit machines
	LIST(APPEND BALL_DEP_OPT_DLLS ${CG_DLLS})
	LIST(REMOVE_DUPLICATES BALL_DEP_OPT_DLLS)

	# If we know where to find a python installer, we can package it
	IF (PYTHON_INSTALLER_PATH)
		FILE(TO_CMAKE_PATH "${PYTHON_INSTALLER_PATH}" PYTHON_INSTALLER_PATH_TMP)
		GET_FILENAME_COMPONENT(PYTHON_INSTALLER ${PYTHON_INSTALLER_PATH_TMP} NAME)
	ENDIF()

	FOREACH(LIB ${BALL_DEP_OPT_DLLS})
		STRING(TOLOWER "${LIB}" LOWER_LIB)
		INSTALL(FILES "${LIB}" 
			DESTINATION "${BALL_RUNTIME_INSTALL_DIRECTORY}"
			COMPONENT ${COMPONENT_LIBBALL})
	ENDFOREACH()
ENDIF()

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
SET(BALL_CMAKE_EXPORT_PATH "lib/cmake/BALL" CACHE PATH "Path to the cmake config and export files")
SET(CF_BALLExport ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)

## create code that allows to call find_package()
## (must come after the above code that sets the variables!)
GET_DIRECTORY_PROPERTY(BALL_INCLUDE_DIRS  INCLUDE_DIRECTORIES)

## Configure the BALLConfig for compilation against the version in PROJECT_BINARY_DIR
CONFIGURE_FILE(
	"${PROJECT_SOURCE_DIR}/cmake/BALLConfig.cmake.in"
	"${PROJECT_BINARY_DIR}/cmake/BALLConfig.cmake"
	@ONLY
)
SET(CF_BALLExport ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)

## Configure the BALLConfig for compilation against the installed version
## If we want to install the package, we need to rewrite the include directories
LIST(REMOVE_ITEM BALL_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/include" "${PROJECT_SOURCE_DIR}/include")
SET(BALL_INCLUDE_DIRS ${BALL_PATH}/include/ ${BALL_INCLUDE_DIRS})
SET(CF_BALLExport "${BALL_PATH}/${BALL_CMAKE_EXPORT_PATH}/BALLExport.cmake")
CONFIGURE_FILE(
	"${PROJECT_SOURCE_DIR}/cmake/BALLConfig.cmake.in"
	"${PROJECT_BINARY_DIR}/exports/BALLConfig.cmake"
	@ONLY
)
INSTALL(FILES 
	"${PROJECT_BINARY_DIR}/exports/BALLConfig.cmake"
	DESTINATION "${BALL_CMAKE_EXPORT_PATH}/"
	COMPONENT   ${COMPONENT_LIBBALL_DEV}
)

## Create BALLExport.cmake
IF(${CMAKE_VERSION} VERSION_LESS "2.8.0")
	MESSAGE(STATUS "Cannot register BALL with CMake! For external code, set the path to BALL during find_package() manually.")
ELSE()
	## register BALL with CMake so that it can be found easily
	EXPORT(PACKAGE BALL)
ENDIF()
## export our libs

IF(BALL_HAS_VIEW)
	EXPORT(TARGETS BALL VIEW FILE ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)
ELSE()
	EXPORT(TARGETS BALL FILE ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)
ENDIF()

INSTALL(EXPORT BALLExportGroup DESTINATION ${BALL_CMAKE_EXPORT_PATH} FILE BALLExport.cmake COMPONENT "{COMPONENT_LIBBALL_DEV}")

INCLUDE(cmake/BALLCTags.cmake)
