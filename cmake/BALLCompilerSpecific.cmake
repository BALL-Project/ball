########################################################
###    compiler flags                                ###
########################################################

SET(USE_PEDANTIC ON CACHE BOOL "Use pedantic compilation options if offered by the compiler")
SET(USE_ASAN OFF CACHE BOOL "Compile everything with AddressSanitizer enabled (GCC/Clang only)")

SET(CXX_COMPILER_ID "(unknown)" CACHE INTERNAL "The C++ compiler id")

SET(CXX_COMPILER_VERSION "0" CACHE INTERNAL "The C++ compiler version")
SET(CXX_COMPILER_VERSION_MAJOR "0" CACHE INTERNAL "The C++ compiler major version")
SET(CXX_COMPILER_VERSION_MINOR "0" CACHE INTERNAL "The C++ compiler minor version")
SET(CXX_COMPILER_VERSION_MINOR_MINOR "0" CACHE INTERNAL "The C++ compiler minor minor version")

IF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

	SET(CXX_COMPILER_ID "GXX")

	## determine version number of the compiler
	EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
		OUTPUT_VARIABLE CXX_COMPILER_VERSION
	)
	STRING(STRIP ${CXX_COMPILER_VERSION} CXX_COMPILER_VERSION)

	## just to make sure that the compiler version has 3 numbers and 2 dots
	## (sometimes, g++ gives, e.g., 4.4 instead of 4.4.0)
	STRING(REGEX REPLACE "^([0-9])\\.([0-9])$" "\\1.\\2.0"
		CXX_COMPILER_VERSION ${CXX_COMPILER_VERSION})

	STRING(REGEX REPLACE "([0-9])\\.([0-9])\\.([0-9])$" "\\1"
		CXX_COMPILER_VERSION_MAJOR ${CXX_COMPILER_VERSION})
	STRING(REGEX REPLACE "([0-9])\\.([0-9])\\.([0-9])$" "\\2"
		CXX_COMPILER_VERSION_MINOR ${CXX_COMPILER_VERSION})
	STRING(REGEX REPLACE "([0-9])\\.([0-9])\\.([0-9])$" "\\3"
		CXX_COMPILER_VERSION_MINOR_MINOR ${CXX_COMPILER_VERSION})

	## -Wconversion flag for GCC
	SET(CXX_WARN_CONVERSION OFF CACHE BOOL "Enables warnings for type conversion problems (GCC only)")
	IF(CXX_WARN_CONVERSION)
		IF(CMAKE_COMPILER_IS_GNUCXX)
			SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
		ENDIF()
	ENDIF()
	MESSAGE(STATUS "Compiler checks for conversion: ${CXX_WARN_CONVERSION}")

	IF (MT_ENABLE_CUDA)
		SET(USE_PEDANTIC OFF)
	ENDIF()

	SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -std=c++0x")

	# Added -Wno-deprecated-declarations as Eigen3 currently uses binder2nd which spams the compiler output.
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

	IF(USE_PEDANTIC)
		SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -pedantic")
	ENDIF()

	IF(USE_ASAN)
		SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	ENDIF()

	## Recommended setting for eclipse, see http://www.cmake.org/Wiki/CMake:Eclipse
	IF(CMAKE_GENERATOR STREQUAL "Eclipse CDT4 - Unix Makefiles")
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
	ENDIF()

ELSEIF(MSVC)

	SET(CXX_COMPILER_ID "MSVC")

	## determine version number of the compiler
	SET(CXX_COMPILER_VERSION ${MSVC_VERSION})

	STRING(REGEX REPLACE "([0-9])([0-9])([0-9])([0-9])" "\\1\\2"
		CXX_COMPILER_VERSION_MAJOR ${CXX_COMPILER_VERSION})
	STRING(REGEX REPLACE "([0-9])([0-9])([0-9])([0-9])" "\\3\\4"
		CXX_COMPILER_VERSION_MINOR ${CXX_COMPILER_VERSION})
	
	#add_definitions(/Wall) ## disable for now.. its just too much!

	## disable dll-interface warning
	#add_definitions(/wd4251 /wd4275)
	
	## disable deprecated functions warning (e.g. for POSIX functions)
	#add_definitions(/wd4996)

	## disable explicit template instantiation request for partially defined classes
	#add_definitions(/wd4661)
	
	## don't warn about unchecked std::copy()
	#add_definitions(/D_SCL_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_WARNINGS /D_CRT_SECURE_NO_DEPRECATE)

	## disable min and max macros by default
	## see CGAL and BOOST configs for more elaborate explanations
	SET(BALL_PROJECT_COMPILE_DEFNS "${BALL_PROJECT_COMPILE_DEFNS} /DNOMINMAX")

	## compile such that GSL is in DLL mode
	#add_definitions(/DGSL_DLL)

	## minimal code rebuild
	#add_definitions(/Gm /Zi)

	## if requested, produce a parallel solution
	OPTION(BALL_BUILD_SOLUTION_PARALLEL OFF)
	IF (BALL_BUILD_SOLUTION_PARALLEL)
		ADD_DEFINITIONS(/MP)
	ENDIF()
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
	
	SET(CXX_COMPILER_ID "Intel")

	SET(CMAKE_COMPILER_IS_INTELCXX TRUE CACHE INTERNAL "Is Intel C++ compiler (icpc)")

	## determine version number of the compiler
	EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
		OUTPUT_VARIABLE CXX_COMPILER_VERSION
	)
	STRING(STRIP ${CXX_COMPILER_VERSION} CXX_COMPILER_VERSION)
	
	## just to make sure that the compiler version has 3 numbers and 2 dots
	## (sometimes, g++ gives, e.g., 4.4 instead of 4.4.0)
	STRING(REGEX REPLACE "^([0-9][0-9])\\.([0-9])$" "\\1.\\2.0"
		CXX_COMPILER_VERSION ${CXX_COMPILER_VERSION})

	STRING(REGEX REPLACE "([0-9][0-9])\\.([0-9])\\.([0-9])$" "\\1"
		CXX_COMPILER_VERSION_MAJOR ${CXX_COMPILER_VERSION})
	STRING(REGEX REPLACE "([0-9][0-9])\\.([0-9])\\.([0-9])$" "\\2"
		CXX_COMPILER_VERSION_MINOR ${CXX_COMPILER_VERSION})
	STRING(REGEX REPLACE "([0-9][0-9])\\.([0-9])\\.([0-9])$" "\\3"
		CXX_COMPILER_VERSION_MINOR_MINOR ${CXX_COMPILER_VERSION})
	
ELSEIF(${CMAKE_CXX_COMPILER_ID} MATCHES ".*Clang")
   # using Clang
	SET(CXX_COMPILER_ID "LLVM")
	SET(CMAKE_COMPILER_IS_LLVM TRUE CACHE INTERNAL "Is LLVM C++ compiler (llvm)")

	## determine version number of the compiler
	EXECUTE_PROCESS(COMMAND ${CMAKE_CXX_COMPILER} --version
		OUTPUT_VARIABLE CXX_COMPILER_VERSION
	)
	STRING(STRIP ${CXX_COMPILER_VERSION} CXX_COMPILER_VERSION)

	STRING(REGEX REPLACE ".*([0-9])\\.([0-9]).*" "\\1"
		CXX_COMPILER_VERSION_MAJOR ${CXX_COMPILER_VERSION})
	STRING(REGEX REPLACE ".*([0-9])\\.([0-9]).*" "\\2"
		CXX_COMPILER_VERSION_MINOR ${CXX_COMPILER_VERSION})

	SET(CXX_COMPILER_VERSION "${CXX_COMPILER_VERSION_MAJOR}.${CXX_COMPILER_VERSION_MINOR}")

	SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -std=c++11")

	IF(USE_ASAN)
		SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
	ENDIF()

	# Disable "inconsistent missing override" warnings for now, as this is primarily macro-induced
	# (e.g., via BALL_EMBEDDABLE) and, unfortunately, won't be resolved until many more parts of our
	# code consistently use override specifiers.
	SET(BALL_PROJECT_COMPILE_FLAGS "${BALL_PROJECT_COMPILE_FLAGS} -Wno-inconsistent-missing-override")
ENDIF()
