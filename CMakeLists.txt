########################################################
#
# CMake configuration for BALL
# http://www.ball-project.org
#
########################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.1.0 FATAL_ERROR)

CMAKE_POLICY(SET CMP0042 OLD)
CMAKE_POLICY(SET CMP0043 NEW)
CMAKE_POLICY(SET CMP0048 NEW)

IF(POLICY CMP0071)
	CMAKE_POLICY(SET CMP0071 NEW)
ENDIF()


PROJECT("BALL" VERSION 1.5.0)


########################################################
# General build system settings
########################################################

# Use folders to group TARGTES using the folder target property in IDEs
SET(USE_FOLDERS TRUE)

# Automatically search lib64 folders
SET(FIND_LIBRARY_USE_LIB64_PATHS TRUE)

# Set default build type to Release
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Release)
ENDIF()

# Add cmake directory from BALL source tree to CMAKE_MODULE_PATH
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

# Add include directoy from BALL source tree
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include")

# Add include directoy from build tree for configured files
INCLUDE_DIRECTORIES("${PROJECT_BINARY_DIR}/include")

# Handling user specified path to ball_contrib installation (-DBALL_CONTRIB_PATH)
# TODO: Get rid of BALL_CONTRIB_PATH when globally refactoring build system
SET(BALL_CONTRIB_PATH "" CACHE PATH "Path to the ball_contrib installation directory (optional)")
IF(BALL_CONTRIB_PATH)
	# Add include directoy from user specified BALL_CONTRIB_PATH
	INCLUDE_DIRECTORIES("${BALL_CONTRIB_PATH}/include")

	# Add ball contrib install directories to CMake search paths for find_XXX() functions
	SET(CMAKE_PREFIX_PATH  "${BALL_CONTRIB_PATH}" "${CMAKE_PREFIX_PATH}")

	# Required for CMake include and compile tests (seems to be required only on Windows)
	SET(CMAKE_REQUIRED_INCLUDES ${BALL_CONTRIB_PATH}/include ${CMAKE_REQUIRED_INCLUDES})
ENDIF()

# Include BALL specific CMake macros
INCLUDE(cmake/BALLMacros.cmake)

# BALL specific compiler and linker settings
SET(BALL_PROJECT_COMPILE_FLAGS "")
SET(BALL_PROJECT_COMPILE_DEFNS "")
INCLUDE(BALLCompilerSpecific)

# Determine BALL configuration
INCLUDE(BALLConfiguration)

# We do not have a Component-based installer in MacOS
IF(NOT APPLE)
	INCLUDE(BALLComponents)
ENDIF()

# License choice: LGPL or GPL build
SET(BALL_LICENSE "LGPL" CACHE STRING "BALL licensing model: LGPL (default) or GPL")

IF(${BALL_LICENSE} STREQUAL "LGPL")
	SET(BALL_LICENSE_LGPL TRUE)
	SET(BALL_LICENSE_GPL  FALSE)
ELSEIF(${BALL_LICENSE} STREQUAL "GPL")
	SET(BALL_LICENSE_LGPL FALSE)
	SET(BALL_LICENSE_GPL  TRUE)
ELSE()
	MESSAGE(FATAL_ERROR "BALL_LICENSE can either be 'LGPL' (default) or 'GPL'")
ENDIF()



########################################################
# Search and configure third party dependencies
########################################################

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
	MESSAGE(FATAL_ERROR "Could not find bison parser generator!")
ENDIF()

IF (WIN32)
	SET(BALL_BISON_BAT ${BISON_EXECUTABLE})
	SET(BISON_EXECUTABLE "call")
ENDIF()

FIND_PACKAGE(FLEX)
IF (NOT FLEX_FOUND)
	MESSAGE(FATAL_ERROR "Could not find flex scanner generator!")
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


## XDR related stuff
INCLUDE(BALLConfigXDR)

## Boost and Asio
INCLUDE(BALLConfigBoost)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LIST(APPEND BALL_DEP_LIBRARIES ${Boost_LIBRARIES})



# OpenBabel availability: default false
SET(BALL_HAS_OPENBABEL FALSE)

IF(${BALL_LICENSE_GPL})

	# FFTW
	OPTION(USE_FFTWD "Use double precision FFTW if found" ON)
	OPTION(USE_FFTWF "Use single precision FFTW if found" ON)
	OPTION(USE_FFTWF "Use long double precision FFTW if found" ON)
	OPTION(USE_FFTW_THREADS "Try to find FFTW with thread support" OFF)

	FIND_PACKAGE(FFTW)
	IF(FFTW_FOUND)
		INCLUDE_DIRECTORIES(${FFTW_INCLUDE_DIR})
		INCLUDE(BALLConfigFFTW)
	ENDIF()

	#Set the default complex precision to float if the setting for this value
	#could not be determined earlier
	IF(NOT BALL_COMPLEX_PRECISION)
		SET(BALL_COMPLEX_PRECISION "float")
	ENDIF()


	# OpenBabel
	FIND_PACKAGE(OpenBabel2)
	IF(OPENBABEL2_FOUND)
		SET(BALL_HAS_OPENBABEL TRUE)
		INCLUDE_DIRECTORIES(${OPENBABEL2_INCLUDE_DIRS})
		LIST(APPEND BALL_DEP_LIBRARIES ${OPENBABEL2_LIBRARIES})
	ELSE()
		MESSAGE("Could not find a OpenBabel2 installation. Skipping OpenBabel dependencies.")
	ENDIF()

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

	IF(${BALL_LICENSE_GPL} OR NOT ${TBB_VERSION_MAJOR} VERSION_LESS 2017)

		IF (TBB_FOUND)
			INCLUDE_DIRECTORIES(${TBB_INCLUDE_DIRS})
			BALL_COMBINE_LIBS(TBB_LIBRARIES "${TBB_LIBRARIES}" "${TBB_DEBUG_LIBRARIES}")
			LIST(REMOVE_DUPLICATES TBB_LIBRARIES)

			SET(BALL_HAS_TBB TRUE)
		ENDIF()

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
		SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS}")
		INCLUDE_DIRECTORIES(${MPI_CXX_INCLUDE_PATH})

		SET(BALL_HAS_MPI TRUE)
	ENDIF()

ENDIF()

## VIEW related options/settings/libraries
SET(UPDATE_TRANSLATIONS OFF CACHE BOOL "If enabled, translations are update")
OPTION(BALL_HAS_VIEW "Compile BALL's VIEW library required for visualization and graphical user interfaces" ON)

## QT
OPTION(USE_QTWEBENGINE "Add support for QtWebEngine" ON)

# Enable automatic invocation of moc if required
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOMOC_MOC_OPTIONS "-DBALL_DEPRECATED=") # Fix issues on some machines where moc would ignore classes with
                                                    # multiple attribute specifications via global macros

SET(QTDIR "" CACHE PATH "Path to the qt installation (optional)")
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QTDIR})

IF(QTDIR)
	INCLUDE_DIRECTORIES(BEFORE "${QTDIR}/include")
ENDIF()

SET(QT_MIN_VERSION 5.5)

FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED Core Network Xml)

LIST(APPEND BALL_DEP_LIBRARIES Qt5::Core
                               Qt5::Network
                               Qt5::Xml)
LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DQT_NO_KEYWORDS")

IF (BALL_HAS_VIEW)
	FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} REQUIRED OpenGL PrintSupport Test Widgets)
	FIND_PACKAGE(Qt5LinguistTools ${QT_MIN_VERSION})

	IF(NOT Qt5LinguistTools_FOUND)
		MESSAGE(WARNING "Qt5LinguistTools not found: BALLView translations cannot be created and compiled.")
	ENDIF()

	LIST(APPEND VIEW_DEP_LIBRARIES Qt5::OpenGL
	                               Qt5::PrintSupport
	                               Qt5::Test
	                               Qt5::Widgets)

	# WebEngine support
	IF(USE_QTWEBENGINE)
		FIND_PACKAGE(Qt5 ${QT_MIN_VERSION} QUIET COMPONENTS WebEngine WebEngineWidgets WebChannel)

		SET(BALL_HAS_QTWEBENGINE TRUE)
		IF(NOT Qt5WebEngine_FOUND)
			MESSAGE(STATUS "QtWebEngine has not been found. Disabling WebEngine support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()

		IF(NOT Qt5WebEngineWidgets_FOUND)
			MESSAGE(STATUS "QtWebEngineWidgets has not been found. Disabling WebEngine support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()

		IF(NOT Qt5WebChannel_FOUND)
			MESSAGE(STATUS "QtWebChannel has not been found. Disabling WebEngine support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()

		IF(BALL_HAS_QTWEBENGINE)
			LIST(APPEND VIEW_DEP_LIBRARIES Qt5::WebEngineWidgets Qt5::WebChannel)
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
	OPTION(USE_RTFACT "Enable RTfact support" ON)
	OPTION(REQUIRE_RTFACT "Require RTfact support" OFF)

	## XDR
	IF(NOT BALL_HAS_XDR)
		MESSAGE(FATAL_ERROR "VIEW requires XDR support!")
	ENDIF()

	## OpenGL
	FIND_PACKAGE(OpenGL)
	IF (NOT OPENGL_FOUND OR NOT OPENGL_GLU_FOUND)
		MESSAGE(FATAL_ERROR "Could not find gl/glu!")
	ENDIF()
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

	## GLEW
	FIND_PACKAGE(GLEW)
	IF (GLEW_FOUND)
		SET(BALL_HAS_GLEW TRUE)
	ENDIF()

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
				IF (NOT BALL_HAS_TBB)
					MESSAGE(FATAL_ERROR "RTfact support requires TBB!")
				ENDIF()
			ENDIF()

			SET(BALL_HAS_RTFACT TRUE)
			INCLUDE_DIRECTORIES(${RTFACT_INCLUDE_DIR})
		ENDIF()
	ENDIF()

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
			FIND_PACKAGE(SIP 4.8)
			SET(BALL_SIP_VERSION ${SIP_VERSION})
			SET(BALL_SIP_VERSION_STR ${SIP_VERSION_STR})
		ENDIF()
	ENDIF()

	IF (NOT PYTHONINTERP_FOUND OR NOT PYTHONLIBS_FOUND OR NOT SIP_FOUND)
		SET(BALL_PYTHON_SUPPORT OFF)
		MESSAGE(STATUS "Disabling python support.")
	ELSE()
		INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
		INCLUDE_DIRECTORIES(${SIP_INCLUDE_DIR})
	ENDIF()
ENDIF()


IF(APPLE)
	FIND_PROGRAM(MACDEPLOYQT_EXECUTABLE macdeployqt PATHS ${BALL_CONTRIB_PATH}/bin)

	IF(${MACDEPLOYQT_EXECUTABLE} STREQUAL "MACDEPLOYQT_EXECUTABLE-NOTFOUND")
		MESSAGE(WARNING "MacOS Bundle generation not possible: macdeployqt not found")
	ELSE()
		SET(QT_DEPLOY_EXECUTABLE ${MACDEPLOYQT_EXECUTABLE})
	ENDIF()
ELSEIF(WIN32)
	FIND_PROGRAM(WINDEPLOYQT_EXECUTABLE windeployqt.exe PATHS ${BALL_CONTRIB_PATH}/bin)

	IF(${WINDEPLOYQT_EXECUTABLE} STREQUAL "WINDEPLOYQT_EXECUTABLE-NOTFOUND")
		MESSAGE(WARNING "Package generation not possible: windeployqt.exe not found")
	ELSE()
		SET(QT_DEPLOY_EXECUTABLE ${WINDEPLOYQT_EXECUTABLE})
	ENDIF()
ENDIF()


## replace any variables in config.h.in with current values
SET(CONFIGURED_CONFIG_H ${PROJECT_BINARY_DIR}/include/BALL/CONFIG/config.h)
CONFIGURE_FILE(cmake/config.h.in ${CONFIGURED_CONFIG_H})
	
## automatically create the version numbers
CONFIGURE_FILE(cmake/templates/BALL.doc.in  ${PROJECT_SOURCE_DIR}/include/BALL/BALL.doc)

## build a config.h file for the python support to not disturb the
## rest of BALL
CONFIGURE_FILE(cmake/BALLPythonConfig.h.in ${PROJECT_BINARY_DIR}/include/BALL/PYTHON/BALLPythonConfig.h)

## build a config.h file for our tests to not disturb the rest of BALL
CONFIGURE_FILE(cmake/BALLTestConfig.h.in ${PROJECT_BINARY_DIR}/test/BALLTestConfig.h)

## build a config.h file for our benchmarks to not disturb the rest of BALL
CONFIGURE_FILE(cmake/BALLBenchmarkConfig.h.in ${PROJECT_BINARY_DIR}/source/BENCHMARKS/BALLBenchmarkConfig.h)


SET(BALL_EXECUTABLES "")
########################################################
###    BUILD the lib																 ###
########################################################


# Add BALL project specific compiler flags and definitions
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BALL_PROJECT_COMPILE_FLAGS}")
ADD_DEFINITIONS("${BALL_PROJECT_COMPILE_DEFNS}")


## are we building a shared or static lib?! (BOTH within 
## the same BUILD-tree is NOT possible currently!!)
SET(BUILD_SHARED_LIBS TRUE)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

## These variables are used in all install-targets
IF (NOT APPLE)
	SET(BALL_RUNTIME_INSTALL_DIRECTORY "bin")
	SET(BALL_LIBRARY_INSTALL_DIRECTORY "lib")
	SET(BALL_ARCHIVE_INSTALL_DIRECTORY "lib")
	SET(BALL_HEADER_INSTALL_DIRECTORY   ".")
	SET(BALL_DATA_INSTALL_DIRECTORY    "share/BALL")
	SET(BALL_DOCUMENTATION_INSTALL_DIRECTORY    "share/BALL/doc")
	SET(BALL_TOOLS_INSTALL_DIRECTORY   "bin/BALLTools")

	IF(WIN32)
		SET(BALL_PYTHON_INSTALL_DIRECTORY  "bin")
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  "bin")
	ELSE()
		SET(BALL_PYTHON_INSTALL_DIRECTORY  "lib")
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  "lib")
	ENDIF()
ENDIF()

## big include file for headers and C files, which fills the BALL_sources variable
INCLUDE(BALLIncludes)
IF (BALL_HAS_VIEW)
	INCLUDE(VIEWIncludes)
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
	SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)

IF (MSVC)
	SET(BALL_COMPILE_DEFINTIONS ${COMPILE_DEFINITIONS} "BALL_BUILD_DLL" CACHE INTERNAL "")
	SET_TARGET_PROPERTIES(BALL PROPERTIES COMPILE_DEFINITIONS "${BALL_COMPILE_DEFINTIONS}")
	# With MALL, BALL breaks incremental linking (seems to be too large...)
	SET(BALL_LINK_FLAGS "${BALL_LINK_FLAGS} /NODEFAULTLIB:libcmt /INCREMENTAL:no")
ENDIF()

IF (BALL_HAS_VIEW)
## TODO: Qt handling!
	IF(GLEW_FOUND)
		LIST(APPEND VIEW_DEP_LIBRARIES ${GLEW_LIBRARY})
	ENDIF()

	LIST(APPEND VIEW_DEP_LIBRARIES ${RTFACT_LIBRARIES})

	SET_TARGET_PROPERTIES(VIEW PROPERTIES
		SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
)

	IF(MSVC)
		## Do we really need this?
		SET(VIEW_COMPILE_DEFINITIONS ${COMPILE_DEFINITIONS} "BALL_VIEW_BUILD_DLL" CACHE INTERNAL "")
		SET_TARGET_PROPERTIES(VIEW PROPERTIES COMPILE_DEFINITIONS "${VIEW_COMPILE_DEFINITIONS}")
		SET(VIEW_LINK_FLAGS "${VIEW_LINK_FLAGS} /NODEFAULTLIB:libcmt")
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

# TODO read version from the command line (CTDConverter/CTDSchema versions), default to "master"


# TOOLS and Ballaxy targets
OPTION(BALL_BUILD_TOOLS "Build BALL command line tools (BALLTools)" ON)
OPTION(BALL_BUILD_BALLAXY "Build Ballaxy stubs. To create full Ballaxy instance including Galaxy also set -DBALLAXY_INSTANCE=ON" ON)
OPTION(BALL_BUILD_CWL "Build CWL stubs." ON)
OPTION(BALLAXY_INSTANCE "Create working Ballaxy instance." OFF)

SET(TOOLS_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/TOOLS")
SET(UTILS_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/UTILITIES")

## read the version for CTDSchema/CTDConverter if they are needed
IF(BALL_BUILD_BALAXY OR BALL_BUILD_CWL)
	IF (NOT CTDSCHEMA_VERSION)
		MESSAGE(STATUS "Setting CTDSchema version to its default value (v1.7)")
		SET(CTDSCHEMA_VERSION "v1.7")
	ENDIF()
	IF (NOT CTDCONVERTER_VERSION)
		MESSAGE(STATUS "Setting CTDConverter version to its default value (v2.0)")
		SET(CTDCONVERTER_VERSION "v2.0")
	ENDIF()	
ENDIF()

IF(BALL_BUILD_BALLAXY)
	# Galaxy release version (GitHub release branch) can be changed via the the following CMake variable.
	# Only used when using -DBALLAXY_INSTANCE=ON to create a full Ballaxy instance including Galaxy.
	SET(GALAXY_RELEASE "release_16.10")

	# We need the TOOLS for Ballaxy
	SET(BALL_BUILD_TOOLS ON)

	ADD_SUBDIRECTORY(workflow_support/galaxy)
ENDIF()

IF(BALL_BUILD_CWL)
	# We need the TOOLS to generate the CWL stubs
	SET(BALL_BUILD_TOOLS ON)
	ADD_SUBDIRECTORY(workflow_support/cwl)
ENDIF()

IF(BALL_BUILD_TOOLS)
	ADD_SUBDIRECTORY(source/APPLICATIONS/TOOLS)
	ADD_SUBDIRECTORY(source/APPLICATIONS/UTILITIES)
ENDIF()

## add targets for Python modules
ADD_SUBDIRECTORY(source/PYTHON/EXTENSIONS)

## add the plugins
ADD_SUBDIRECTORY(source/EXTENSIONS)

IF(BALL_HAS_VIEW)
	## add BALLView target
	ADD_SUBDIRECTORY(source/APPLICATIONS/BALLVIEW)
	LIST(APPEND BALL_EXECUTABLES "BALLView" "BALLView")
ENDIF()

## add BENCHMARK target
ADD_SUBDIRECTORY(source/BENCHMARKS/)

###### add MMFF94 test targets ##########
OPTION(BALL_BUILD_MMFF94 "Build MMFF94 command line tools" ON)
IF(BALL_BUILD_MMFF94)
        ADD_SUBDIRECTORY(source/APPLICATIONS/MMFF94)
ENDIF()



######################################################################
# Installation Routines
######################################################################

IF (WIN32)
	INCLUDE(InstallRequiredSystemLibraries)
ENDIF()

IF(NOT APPLE)
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
		### Install the data directories
		INSTALL(DIRECTORY data DESTINATION "${BALL_DATA_INSTALL_DIRECTORY}" COMPONENT ${COMPONENT_LIBBALL})

		### Install the includes
		INSTALL(DIRECTORY include DESTINATION "${BALL_HEADER_INSTALL_DIRECTORY}" COMPONENT ${COMPONENT_LIBBALL_DEV})
	ENDIF()

	## The configured files have to be copied in any case
	INSTALL(FILES ${PROJECT_BINARY_DIR}/include/BALL/CONFIG/config.h
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

		IF(QT_DEPLOY_EXECUTABLE)
			INSTALL(CODE
				"EXECUTE_PROCESS(COMMAND ${QT_DEPLOY_EXECUTABLE} \$\{CMAKE_INSTALL_PREFIX\}/${BALL_RUNTIME_INSTALL_DIRECTORY}/BALL.dll)
				EXECUTE_PROCESS(COMMAND ${QT_DEPLOY_EXECUTABLE} \$\{CMAKE_INSTALL_PREFIX\}/${BALL_RUNTIME_INSTALL_DIRECTORY}/VIEW.dll)"
				COMPONENT "${COMPONENT_LIBBALL}")
		ENDIF()
	ENDIF()
ENDIF()



########################################################
# Packaging, Documentation, Tests
########################################################

# Build BALL packages y/n
OPTION(BALL_ENABLE_PACKAGING "Enable the building of binary packages" OFF)

# Packaging
IF(BALL_ENABLE_PACKAGING)
	INCLUDE(BALLPackageConfig)
ENDIF()

# Documentation
INCLUDE(BALLDoc)

# Tests
INCLUDE(Dart)
ENABLE_TESTING()
ADD_SUBDIRECTORY(test EXCLUDE_FROM_ALL) ## configure the Test project (but do not add it to BALL itself)

# Targets list
INCLUDE(BALLTargets)

# Message after BALL has been built
INCLUDE(BALLFinishedMessage)


# CMake configuration message
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
# Create CMake package configurations to be used with
# find_package()for:
#
#   - local build tree BALL
#   - installed BALL
#
######################################################

INCLUDE(CMakePackageConfigHelpers)


# Collect Qt5 dependencies
GET_TARGET_PROPERTY(QT5_LINK_LIBRARIES BALL LINK_LIBRARIES)
IF(BALL_HAS_VIEW)
	GET_TARGET_PROPERTY(VIEW_LINK_LIBRARIES VIEW LINK_LIBRARIES)
	SET(QT5_LINK_LIBRARIES ${QT5_LINK_LIBRARIES} ${VIEW_LINK_LIBRARIES})
ENDIF()

SET(QT5_IMPORTED_TARGETS "")
FOREACH(TMP_LINKED_LIB ${QT5_LINK_LIBRARIES})
	IF(${TMP_LINKED_LIB} MATCHES ".*Qt5")
		STRING(REPLACE "::" "" TMP_QT5_LINK_LIBRARY ${TMP_LINKED_LIB})
		SET(QT5_IMPORTED_TARGETS ${QT5_IMPORTED_TARGETS} ${TMP_QT5_LINK_LIBRARY})
	ENDIF()
ENDFOREACH()


# Collect BALL include directories
GET_DIRECTORY_PROPERTY(BALL_INCLUDE_DIRS INCLUDE_DIRECTORIES)


# Generate CMake package configuration for BALL build tree
CONFIGURE_FILE(
	"${PROJECT_SOURCE_DIR}/cmake/BALLConfig.cmake.in"
	"${PROJECT_BINARY_DIR}/cmake/BALLConfig.cmake"
	@ONLY
)

# Generate BALL package config version file
write_basic_package_version_file(
	"${PROJECT_BINARY_DIR}/cmake/BALLConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# Generate exports
EXPORT(TARGETS BALL
	FILE ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)

IF(BALL_HAS_VIEW)
	EXPORT(TARGETS VIEW
		APPEND
		FILE ${PROJECT_BINARY_DIR}/cmake/BALLExport.cmake)
ENDIF()


# Store BALL build directory in the CMake user package registry
EXPORT(PACKAGE BALL)


# Generate CMake package configuration for BALL installation
IF(NOT APPLE)
	# Installation path for BALL CMake package configuration files
	SET(BALL_CMAKE_EXPORT_PATH "lib/cmake/BALL" CACHE PATH "Path to the cmake package configuration files")

	LIST(REMOVE_ITEM BALL_INCLUDE_DIRS "${PROJECT_BINARY_DIR}/include")
	LIST(REMOVE_ITEM BALL_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/include")
	SET(BALL_INCLUDE_DIRS ${BALL_PATH}/include/ ${BALL_INCLUDE_DIRS})

	CONFIGURE_FILE(
		"${PROJECT_SOURCE_DIR}/cmake/BALLConfig.cmake.in"
		"${PROJECT_BINARY_DIR}/exports/BALLConfig.cmake"
		@ONLY
	)

	INSTALL(FILES
		"${PROJECT_BINARY_DIR}/exports/BALLConfig.cmake"
		"${PROJECT_BINARY_DIR}/cmake/BALLConfigVersion.cmake"
		DESTINATION "${BALL_CMAKE_EXPORT_PATH}/"
		COMPONENT   ${COMPONENT_LIBBALL_DEV}
	)

	INSTALL(EXPORT BALLExportGroup
		DESTINATION ${BALL_CMAKE_EXPORT_PATH}
		FILE BALLExport.cmake
		COMPONENT "${COMPONENT_LIBBALL_DEV}")
ENDIF()


######################################################
# Generate CTags for BALL project
######################################################
INCLUDE(BALLCTags)
