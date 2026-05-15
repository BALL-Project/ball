########################################################
#
# CMake configuration for BALL
# http://www.ball-project.org
#
########################################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.21...3.31 FATAL_ERROR)

CMAKE_POLICY(SET CMP0042 NEW)
CMAKE_POLICY(SET CMP0043 NEW)
CMAKE_POLICY(SET CMP0048 NEW)

IF(POLICY CMP0071)
	CMAKE_POLICY(SET CMP0071 NEW)
ENDIF()

IF(POLICY CMP0167)
	CMAKE_POLICY(SET CMP0167 NEW)
ENDIF()


PROJECT("BALL" VERSION 1.6.0) # 1.6.0-dev (pre-release)


########################################################
# General build system settings
########################################################

# Project-wide C++ standard. Set before any target so it applies project-wide.
# Set EARLY (before BALLConfiguration's C++ feature-detection tests and before
# any target is defined) so the standard is deterministic on every compiler.
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

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


# Include BALL specific CMake macros
INCLUDE(cmake/BALLMacros.cmake)

# BALL specific compiler and linker settings
SET(BALL_PROJECT_COMPILE_FLAGS "")
SET(BALL_PROJECT_COMPILE_DEFNS "")
INCLUDE(BALLCompilerSpecific)

# Determine BALL configuration
INCLUDE(BALLConfiguration)

INCLUDE(GNUInstallDirs)

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
# Minimum dependency version floors (DEPS-04 / D-08)
# Pinned inline at each find_package call site (or via the SET below for Qt).
#
#   CMake   3.21  — CMakePresets.json schema v3 + toolchainFile support
#   Qt6     6.5   — Qt 6.5 LTS floor (D-01); Homebrew ships unversioned qt 6.x, vcpkg ships qtbase
#   Boost   1.70  — config-mode BoostConfig.cmake is reliable from 1.70+
#   Eigen3  3.4   — first Eigen with stable Eigen3Config.cmake (covers the 5.x line; see note at FIND_PACKAGE)
#   TBB    2021.0 — first oneTBB release shipping TBBConfig.cmake / TBB::tbb target
#   OpenBabel 3.0 — first OpenBabel shipping OpenBabel3Config.cmake
########################################################

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



# OpenBabel availability: default false (GPL-gated — set TRUE automatically when
# BALL_LICENSE=GPL and OpenBabel3 is found; LGPL builds never search for it).
# Maintainer decision recorded in Plan 04-01 SUMMARY (2026-05-14).
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


	# OpenBabel (GPL-gated — dynamically linked optional dep; only searched in GPL builds).
	# Decision: OpenBabel stays GPL-gated to avoid any licensing ambiguity in the default
	# LGPL build (maintainer decision recorded in Plan 04-01 SUMMARY, 2026-05-14).
	# The C++ API was ported from 2.x to 3.x in Plan 04-03.
	# Verification: cmake --preset macos-homebrew -DBALL_LICENSE=GPL
	# (or use the ci-macos/ci-linux presets which set BALL_HAS_OPENBABEL=ON via
	# cacheVariables; those presets must also pass -DBALL_LICENSE=GPL in the CI step).
	FIND_PACKAGE(OpenBabel3 3.0 CONFIG)
	IF(OpenBabel3_FOUND)
		SET(BALL_HAS_OPENBABEL TRUE)
		INCLUDE_DIRECTORIES(${OpenBabel3_INCLUDE_DIRS})
		MESSAGE(STATUS "Found OpenBabel ${OpenBabel3_VERSION} (config mode) — molecular file-format conversion enabled")
	ELSE()
		MESSAGE(STATUS "OpenBabel not found — molecular file-format conversion disabled (optional dependency)")
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
	IF(REQUIRE_TBB)
		FIND_PACKAGE(TBB 2021.0 CONFIG REQUIRED)
	ELSE()
		FIND_PACKAGE(TBB 2021.0 CONFIG)
	ENDIF()
	IF(TBB_FOUND)
		SET(BALL_HAS_TBB TRUE)
		MESSAGE(STATUS "Found TBB ${TBB_VERSION} (config mode) — parallel speedups enabled")
	ELSE()
		MESSAGE(STATUS "TBB not found — building without parallel speedups (optional dependency)")
	ENDIF()
ENDIF()

## MPI
OPTION(USE_MPI "Should MPI support be enabled?" OFF)
OPTION(REQUIRE_MPI "Should MPI support be required?" OFF)

IF (USE_MPI)
	IF (NOT BALL_HAS_XDR)
		MESSAGE(STATUS "XDR has not been found, which is required for MPI support. Disabling MPI support.")
	ELSEIF (REQUIRE_MPI)
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

## Phase 5 renderer-backend spike (THROWAWAY prototype — Plan 05-05 / 05-06).
## Selects between OFF (default, production fixed-function path), "GLCore"
## (Plan 05-05 GL 3.2+ core-profile prototype), and "QRhi" (Plan 05-06 prototype).
## A NULL release build is unchanged — no compile-defn is appended unless the
## value is explicitly set to "GLCore" or "QRhi".
SET(BALL_SPIKE_BACKEND "OFF" CACHE STRING
    "Renderer backend spike (OFF|GLCore|QRhi) -- THROWAWAY Phase 5 prototype, never production")
SET_PROPERTY(CACHE BALL_SPIKE_BACKEND PROPERTY STRINGS "OFF" "GLCore" "QRhi")
IF (BALL_SPIKE_BACKEND STREQUAL "GLCore")
    LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_GLCORE")
    MESSAGE(STATUS "Phase 5 spike: GL-core renderer ENABLED (throwaway; not for release builds)")
ENDIF()
IF (BALL_SPIKE_BACKEND STREQUAL "QRhi")
    # QRhiWidget (the embedding mechanism for QRhi inside a QWidget hierarchy)
    # only ships in Qt 6.7+. The project's D-01 floor is Qt 6.5. If the resolved
    # Qt version is too old, emit a clear actionable WARNING and do NOT set the
    # compile defn -- the spike sources then stay out of the build.
    # NOTE: Qt6_VERSION is set by FIND_PACKAGE(Qt6 ...) further below, but the
    # version variables `Qt6Core_VERSION` / `Qt6Widgets_VERSION` populated by
    # FIND_PACKAGE are not yet in scope at this point in CMakeLists.txt -- the
    # Qt6 find_package call lives ~25 lines below this block. We therefore
    # defer the version check until after FIND_PACKAGE(Qt6 ... Core ...) runs;
    # the check is implemented by setting a flag and re-evaluating later.
    SET(BALL_SPIKE_BACKEND_QRHI_REQUESTED ON)
ENDIF()

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

SET(QT_MIN_VERSION 6.5)

FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED Core Network Xml)

LIST(APPEND BALL_DEP_LIBRARIES Qt6::Core
                               Qt6::Network
                               Qt6::Xml)
LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DQT_NO_KEYWORDS")

# Phase 5 spike: QRhi arm of BALL_SPIKE_BACKEND option (Plan 05-06).
# QRhiWidget requires Qt 6.7+. Defer the version-floor check until after
# FIND_PACKAGE(Qt6 ... Core ...) has populated Qt6Core_VERSION; if the floor
# is met, append the BALL_SPIKE_BACKEND_QRHI compile defn so the spike code
# in rendererFactory.{h,C} + rhiRenderer.{h,C} + qtRhiSurface.{h,C} compiles.
# Otherwise emit a clear actionable WARNING that points to the GL-core arm.
# THROWAWAY: this block exists only for SPIKE-01; it is removed before PIPE-01.
IF (BALL_SPIKE_BACKEND_QRHI_REQUESTED)
    IF (Qt6Core_VERSION VERSION_LESS 6.7)
        MESSAGE(WARNING "Phase 5 spike: QRhi backend requested but Qt ${Qt6Core_VERSION} < 6.7 (QRhiWidget unavailable). Build proceeds without spike; rerun with -DBALL_SPIKE_BACKEND=GLCore or upgrade Qt.")
    ELSE()
        LIST(APPEND BALL_PROJECT_COMPILE_DEFNS "-DBALL_SPIKE_BACKEND_QRHI")
        MESSAGE(STATUS "Phase 5 spike: QRhi renderer ENABLED (throwaway; Qt ${Qt6Core_VERSION})")
    ENDIF()
ENDIF()

IF (BALL_HAS_VIEW)
	FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} REQUIRED OpenGL PrintSupport Test Widgets OpenGLWidgets)
	FIND_PACKAGE(Qt6LinguistTools ${QT_MIN_VERSION})

	IF(NOT Qt6LinguistTools_FOUND)
		MESSAGE(WARNING "Qt6LinguistTools not found: BALLView translations cannot be created and compiled.")
	ENDIF()

	LIST(APPEND VIEW_DEP_LIBRARIES Qt6::OpenGL
	                               Qt6::PrintSupport
	                               Qt6::Test
	                               Qt6::Widgets
	                               Qt6::OpenGLWidgets)

	# THROWAWAY SPIKE (Plan 05-06): the QRhi spike sources (rhiRenderer.C +
	# qtRhiSurface.C) include <rhi/qrhi.h>, which lives in Qt's private API
	# include tree. Qt6::GuiPrivate is the canonical CMake target that exposes
	# the private headers (still public-API-stable from Qt 6.6+; QRhi is a
	# documented public class even though its header lives under the private
	# include path -- see Qt 6.7 release notes). The Private target requires
	# an explicit FIND_PACKAGE(Qt6 ... Gui ...) call to be in scope.
	# Production builds never link this target -- removed alongside the spike
	# before PIPE-01.
	IF (BALL_SPIKE_BACKEND STREQUAL "QRhi" AND Qt6Core_VERSION VERSION_GREATER_EQUAL 6.7)
		# Private-headers target is shipped as a SEPARATE package
		# (Qt6GuiPrivate), not a component of Qt6 Gui. Find it explicitly.
		FIND_PACKAGE(Qt6GuiPrivate ${QT_MIN_VERSION} REQUIRED)
		LIST(APPEND VIEW_DEP_LIBRARIES Qt6::GuiPrivate)
	ENDIF()

	# WebEngine support
	IF(USE_QTWEBENGINE)
		FIND_PACKAGE(Qt6 ${QT_MIN_VERSION} QUIET COMPONENTS WebEngineCore WebEngineWidgets WebChannel)

		SET(BALL_HAS_QTWEBENGINE TRUE)
		IF(NOT Qt6WebEngineCore_FOUND)
			MESSAGE(STATUS "QtWebEngine has not been found. Disabling WebEngine support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()

		IF(NOT Qt6WebEngineWidgets_FOUND)
			MESSAGE(STATUS "QtWebEngineWidgets has not been found. Disabling WebEngine support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()

		IF(NOT Qt6WebChannel_FOUND)
			MESSAGE(STATUS "QtWebChannel has not been found. Disabling WebEngine support.")
			SET(BALL_HAS_QTWEBENGINE FALSE)
		ENDIF()

		IF(BALL_HAS_QTWEBENGINE)
			# Qt 6: QWebEnginePage / QWebEngineProfile / QWebEngineCertificateError
			# moved into the QtWebEngineCore module. Link both Core and Widgets so
			# moc-generated translation units can resolve every QWebEngine* header
			# regardless of which module owns it.
			LIST(APPEND VIEW_DEP_LIBRARIES Qt6::WebEngineCore Qt6::WebEngineWidgets Qt6::WebChannel)
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

# Note: do NOT use a version range (3.4...6) here. The version-range syntax requires
# the Eigen3ConfigVersion.cmake to support range queries, but Ubuntu 22.04's system
# libeigen3-dev (3.4.0) ships an older script that does not handle ranges and rejects
# the upper bound. Instead, use NO version constraint in find_package and enforce the
# floor via a cmake IF check. Homebrew's Eigen 5.x ships a modern ConfigVersion script
# that handles both plain and range requests correctly, so this works on all platforms.
FIND_PACKAGE(Eigen3 REQUIRED NO_MODULE)
IF(Eigen3_VERSION VERSION_LESS "3.4")
	MESSAGE(FATAL_ERROR "Eigen3 >= 3.4 required (found ${Eigen3_VERSION}). "
		"Install a newer Eigen3 package.")
ENDIF()
MESSAGE(STATUS "Found Eigen3 ${Eigen3_VERSION} (config mode) — Eigen3::Eigen target")

## VIEW dependent packages
IF (BALL_HAS_VIEW)

	## VIEW dependent options
	OPTION(USE_GLEW "Use the OpenGL Extension Wrangler library" ON)
	OPTION(USE_RTFACT "Enable RTfact support" ON)
	OPTION(REQUIRE_RTFACT "Require RTfact support" OFF)

	## OpenGL
	FIND_PACKAGE(OpenGL)
	IF (NOT OPENGL_FOUND OR NOT OPENGL_GLU_FOUND)
		MESSAGE(FATAL_ERROR "Could not find gl/glu!")
	ENDIF()
	INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

	## GLEW
	IF (USE_GLEW)
		FIND_PACKAGE(GLEW)
		IF (GLEW_FOUND)
			SET(BALL_HAS_GLEW TRUE)
		ENDIF()
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
			IF (NOT CXX_COMPILER_ID STREQUAL "Intel")
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
	FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)

	IF (Python3_FOUND)
		SET(SIP_EXTRA_OPTIONS "-e")
		FIND_PACKAGE(SIP 4.9 REQUIRED)
		SET(BALL_SIP_VERSION ${SIP_VERSION})
		SET(BALL_SIP_VERSION_STR ${SIP_VERSION_STR})
		INCLUDE_DIRECTORIES(${Python3_INCLUDE_DIR})
		INCLUDE_DIRECTORIES(${SIP_INCLUDE_DIR})
	ELSE()
		SET(BALL_PYTHON_SUPPORT OFF)
		MESSAGE(STATUS "Disabling python support.")
	ENDIF()

ENDIF()


IF(APPLE)
	FIND_PROGRAM(MACDEPLOYQT_EXECUTABLE macdeployqt)

	IF(${MACDEPLOYQT_EXECUTABLE} STREQUAL "MACDEPLOYQT_EXECUTABLE-NOTFOUND")
		MESSAGE(WARNING "MacOS Bundle generation not possible: macdeployqt not found")
	ELSE()
		SET(QT_DEPLOY_EXECUTABLE ${MACDEPLOYQT_EXECUTABLE})
	ENDIF()
ELSEIF(WIN32)
	FIND_PROGRAM(WINDEPLOYQT_EXECUTABLE windeployqt.exe)

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
# Unquoted: BALL_PROJECT_COMPILE_DEFNS is a CMake list — each element must
# reach ADD_DEFINITIONS as its own argument (e.g. -DQT_NO_KEYWORDS /DNOMINMAX).
# Quoting collapses the list into one mangled "-Dx;/Dy" argument.
ADD_DEFINITIONS(${BALL_PROJECT_COMPILE_DEFNS})


## are we building a shared or static lib?! (BOTH within 
## the same BUILD-tree is NOT possible currently!!)
SET(BUILD_SHARED_LIBS TRUE)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

## These variables are used in all install-targets
IF (NOT APPLE)
	IF(WIN32)
		SET(BALL_PYTHON_INSTALL_DIRECTORY  "bin")
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  "bin")
	ELSE()
		SET(BALL_PYTHON_INSTALL_DIRECTORY  ${CMAKE_INSTALL_LIBDIR})
		SET(BALL_PLUGIN_INSTALL_DIRECTORY  ${CMAKE_INSTALL_LIBDIR})
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
	${Python3_LIBRARIES}
	Eigen3::Eigen
)


IF(UNIX)
	LIST(APPEND BALL_DEP_LIBRARIES "pthread")
ENDIF()

IF(TBB_FOUND)
	LIST(APPEND BALL_DEP_LIBRARIES TBB::tbb)
ENDIF()

IF(BALL_HAS_OPENBABEL)
	LIST(APPEND BALL_DEP_LIBRARIES openbabel)
	# OpenBabel 3.1.1's plugin.h inherits from std::binary_function, which was removed
	# in C++17. Re-enable it via the libc++ (macOS) and libstdc++ (Linux) compatibility
	# defines so BALL can include OpenBabel headers cleanly under C++17.
	IF(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		ADD_COMPILE_DEFINITIONS(_LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION)
	ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		ADD_COMPILE_DEFINITIONS(_GLIBCXX_USE_DEPRECATED=1)
	ENDIF()
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
		IF(TARGET GLEW::GLEW)
			LIST(APPEND VIEW_DEP_LIBRARIES GLEW::GLEW)
		ELSE()
			LIST(APPEND VIEW_DEP_LIBRARIES ${GLEW_LIBRARIES})
		ENDIF()
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

IF(CXX_COMPILER_ID STREQUAL "GXX")
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
		RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
		LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	)

	IF(BALL_HAS_VIEW)
		INSTALL(TARGETS VIEW
			EXPORT BALLExportGroup
			COMPONENT "${COMPONENT_LIBVIEW}"
			RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
			LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
			ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
		)
	ENDIF()

	## If we are not installing into the source directory directly, take
	## care of data files and headers
	IF (NOT "${CMAKE_INSTALL_PREFIX}" STREQUAL ".")
		### Install the data directories
		INSTALL(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/BALL COMPONENT ${COMPONENT_LIBBALL})

		### Install the includes
		INSTALL(DIRECTORY include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT ${COMPONENT_LIBBALL_DEV})
	ENDIF()

	## The configured files have to be copied in any case
	INSTALL(FILES ${PROJECT_BINARY_DIR}/include/BALL/CONFIG/config.h
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/BALL/CONFIG"
		COMPONENT ${COMPONENT_LIBBALL_DEV}
	)

	INSTALL(FILES
		${PROJECT_BINARY_DIR}/include/BALL/PYTHON/BALLPythonConfig.h
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/BALL/PYTHON"
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
				DESTINATION "${CMAKE_INSTALL_BINDIR}"
				COMPONENT ${COMPONENT_LIBBALL})
		ENDFOREACH()

		IF(QT_DEPLOY_EXECUTABLE)
			INSTALL(CODE
				"EXECUTE_PROCESS(COMMAND ${QT_DEPLOY_EXECUTABLE} \$\{CMAKE_INSTALL_PREFIX\}/${CMAKE_INSTALL_BINDIR}/BALL.dll)
				EXECUTE_PROCESS(COMMAND ${QT_DEPLOY_EXECUTABLE} \$\{CMAKE_INSTALL_PREFIX\}/${CMAKE_INSTALL_BINDIR}/VIEW.dll)"
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
INCLUDE(CTest)
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


# Collect Qt6 dependencies
GET_TARGET_PROPERTY(QT6_LINK_LIBRARIES BALL LINK_LIBRARIES)
IF(BALL_HAS_VIEW)
	GET_TARGET_PROPERTY(VIEW_LINK_LIBRARIES VIEW LINK_LIBRARIES)
	SET(QT6_LINK_LIBRARIES ${QT6_LINK_LIBRARIES} ${VIEW_LINK_LIBRARIES})
ENDIF()

SET(QT6_IMPORTED_TARGETS "")
FOREACH(TMP_LINKED_LIB ${QT6_LINK_LIBRARIES})
	IF(${TMP_LINKED_LIB} MATCHES ".*Qt6")
		STRING(REPLACE "::" "" TMP_QT6_LINK_LIBRARY ${TMP_LINKED_LIB})
		SET(QT6_IMPORTED_TARGETS ${QT6_IMPORTED_TARGETS} ${TMP_QT6_LINK_LIBRARY})
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
	SET(BALL_CMAKE_EXPORT_PATH ${CMAKE_INSTALL_LIBDIR}/cmake/BALL CACHE PATH "Path to the cmake package configuration files")

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
