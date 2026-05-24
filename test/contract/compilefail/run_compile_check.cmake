# -*- Mode: CMake -*-
#
# Phase 999.60 (v1.7.x-25) — Owner-narrowing compile-check driver.
#
# ARCHITECTURE-CONTRACT.md §11c. Run as a ctest test (label `contract`).
# Attempts to compile a single source TU against the BALL/VIEW headers and
# asserts the compile OUTCOME matches the EXPECTED outcome for the current
# phase.
#
# Invoked with -D variables:
#   SOURCE          absolute path to the .C to compile
#   EXPECT          "pass" | "fail"  (expected compile outcome this phase)
#   CXX             the C++ compiler
#   INCLUDE_DIRS    ;-list of -I include dirs
#   CXX_STANDARD    e.g. 17
#   WORKDIR         a writable scratch dir for the object output
#   DIAG_MATCH      (optional) substring the diagnostic must contain when EXPECT=fail
#
# HARNESS-FIRST: the negative tests EXPECT=pass today (owner setters still
# public). After 999.59-04 privatises them, the test/CMakeLists.txt flips
# their EXPECT to "fail" and provides DIAG_MATCH (`is private` /
# `no matching`). The positive test is EXPECT=pass in every phase.

if(NOT DEFINED SOURCE OR NOT DEFINED EXPECT OR NOT DEFINED CXX)
	message(FATAL_ERROR "run_compile_check.cmake: SOURCE, EXPECT and CXX are required")
endif()

if(NOT DEFINED WORKDIR)
	set(WORKDIR "${CMAKE_CURRENT_BINARY_DIR}")
endif()
file(MAKE_DIRECTORY "${WORKDIR}")

# Build the -I flags.
set(INCLUDE_FLAGS "")
if(DEFINED INCLUDE_DIRS)
	foreach(dir ${INCLUDE_DIRS})
		list(APPEND INCLUDE_FLAGS "-I${dir}")
	endforeach()
endif()

# Build the -F framework-search flags (macOS Qt is shipped as frameworks;
# QtCore/QEvent etc. only resolve with -F on Apple). No-op on Linux/Windows.
if(DEFINED FRAMEWORK_DIRS)
	foreach(dir ${FRAMEWORK_DIRS})
		if(NOT dir STREQUAL "")
			list(APPEND INCLUDE_FLAGS "-F${dir}")
		endif()
	endforeach()
endif()

if(NOT DEFINED CXX_STANDARD)
	set(CXX_STANDARD 17)
endif()

get_filename_component(_src_name "${SOURCE}" NAME_WE)
set(_obj "${WORKDIR}/${_src_name}.compilecheck.o")

# Syntax-only compile (-fsyntax-only is GCC/Clang; MSVC uses /Zs). We pick
# the flag from the compiler id passed in, defaulting to -fsyntax-only.
if(DEFINED CXX_IS_MSVC AND CXX_IS_MSVC)
	set(_compile_cmd "${CXX}" "/std:c++${CXX_STANDARD}" ${INCLUDE_FLAGS} "/Zs" "${SOURCE}")
else()
	set(_compile_cmd "${CXX}" "-std=c++${CXX_STANDARD}" ${INCLUDE_FLAGS} "-fsyntax-only" "${SOURCE}")
endif()

message(STATUS "compile-check: ${_compile_cmd}")

execute_process(
	COMMAND ${_compile_cmd}
	RESULT_VARIABLE _result
	OUTPUT_VARIABLE _stdout
	ERROR_VARIABLE  _stderr
)

set(_combined "${_stdout}\n${_stderr}")

if(EXPECT STREQUAL "pass")
	if(NOT _result EQUAL 0)
		message(FATAL_ERROR
			"compile-check FAILED: expected ${SOURCE} to COMPILE (pass) but it did not.\n"
			"--- compiler output ---\n${_combined}")
	endif()
	message(STATUS "compile-check OK: ${SOURCE} compiled as expected (pass).")
elseif(EXPECT STREQUAL "fail")
	if(_result EQUAL 0)
		message(FATAL_ERROR
			"compile-check FAILED: expected ${SOURCE} to FAIL to compile (owner "
			"setters should be private per §3) but it compiled.\n"
			"--- compiler output ---\n${_combined}")
	endif()
	if(DEFINED DIAG_MATCH AND NOT DIAG_MATCH STREQUAL "")
		string(FIND "${_combined}" "${DIAG_MATCH}" _pos)
		if(_pos EQUAL -1)
			message(FATAL_ERROR
				"compile-check FAILED: ${SOURCE} failed to compile (good) but the "
				"diagnostic did not contain the expected substring '${DIAG_MATCH}'.\n"
				"--- compiler output ---\n${_combined}")
		endif()
	endif()
	message(STATUS "compile-check OK: ${SOURCE} failed to compile as expected (fail).")
else()
	message(FATAL_ERROR "compile-check: EXPECT must be 'pass' or 'fail', got '${EXPECT}'")
endif()
