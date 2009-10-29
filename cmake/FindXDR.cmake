# Try to find the XDR library and set some required variables
#
# Once run this will define:
#
# XDR_FOUND					= system has XDR lib
#
# XDR_LIBRARIES			= full path to the libraries, if required
#

INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckFunctionExists)
INCLUDE(CheckTypeSize)
INCLUDE(CheckLibraryExists)

## First try to find the required header files (rpc/types.h, rpc/xdr.h)

#check for the XDR functions: their interface and the libraries they're hidden in.
CHECK_INCLUDE_FILE_CXX(rpc/types.h XDR_HAS_RPC_TYPES_H)
IF(NOT XDR_HAS_RPC_TYPES_H)
    MESSAGE(STATUS "Cannot find RPC headers (rpc/types.h). Persistence will be disabled!")
ELSE()
	CHECK_CXX_SOURCE_COMPILES("#include <rpc/types.h>
		#include <rpc/xdr.h>

		int main(int /*argc*/, char** /*argv*/)
		{
			return 0;
		}" XDR_HAS_RPC_XDR_H)
	IF(NOT XDR_HAS_RPC_XDR_H)
    MESSAGE(SEND_ERROR "Cannot find RPC headers (rpc/xdr.h). Persistence will be disabled!")
	ENDIF()
ENDIF()

IF (XDR_HAS_RPC_TYPES_H AND XDR_HAS_RPC_XDR_H)
	## Now let's see if we need an extra lib to compile it
	SET(XDR_INT_FOUND)
	CHECK_FUNCTION_EXISTS(xdr_int XDR_INT_FOUND)
	IF (NOT XDR_INT_FOUND)
		FOREACH(lib nsl oncrpc)
			## Try to find the corresponding lib
			SET(XDR_INT_LIBRARY)
			FIND_LIBRARY(XDR_INT_LIBRARY ${lib})

			IF (XDR_INT_LIBRARY)
				CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY} xdr_int "" XDR_INT_SYMBOL_FOUND)
			ENDIF()
			IF (XDR_INT_SYMBOL_FOUND)
				SET(XDR_LIBRARIES ${XDR_INT_LIBRARY})
				## try to find the debug version as well
				FIND_LIBRARY(XDR_INT_LIBRARY_D ${lib}d)

				IF (XDR_INT_LIBRARY_D)
					CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY_D} xdr_int "" XDR_INT_SYMBOL_D_FOUND)
					IF (XDR_INT_SYMBOL_D_FOUND)
						BALL_COMBINE_LIBS(XDR_LIBRARIES ${XDR_LIBRARIES} ${XDR_INT_LIBRARY_D})	
					ENDIF()
				ENDIF()

				SET(XDR_INT_FOUND TRUE)
				BREAK()
			ENDIF()
		ENDFOREACH()
	ENDIF()

	IF(NOT XDR_INT_FOUND)
		MESSAGE(SEND_ERROR "Could not locate xdr symbols in libc or libnsl. Persistence will be disabled!")
	ELSE()

		SET(XDR_FOUND TRUE)

		## Let's see if we have an implementation for xdr_u_hyper
		IF (XDR_LIBRARIES)
			CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY} xdr_u_hyper "" BALL_HAS_XDR_U_HYPER)
		ELSE()
			CHECK_FUNCTION_EXISTS(xdr_u_hyper BALL_HAS_XDR_U_HYPER)
		ENDIF()

		IF(BALL_HAS_XDR_U_HYPER)
				SET(XDR_TEST_HEADER "#include <rpc/types.h>
										#include <rpc/xdr.h>
										int main(){")
				SET(XDR_TEST_FOOTER "XDR xdrs;
										xdr_u_hyper(&xdrs, &q); }")

				MESSAGE(STATUS "Looking for 64-bit XDR type (for xdr_u_hyper)")

				#List containing possible types for BALL_XDR_UINT64_TYPE
				SET(POSSIBLE_64BIT_TYPES u_quad_t u_longlong_t "unsigned long long" __uint64_t)

				#iterate over the list and try out the types
				FOREACH(XDR_UINT64_TYPE ${POSSIBLE_64BIT_TYPES})
						CHECK_CXX_SOURCE_COMPILES( "${XDR_TEST_HEADER}${XDR_UINT64_TYPE} q;${XDR_TEST_FOOTER}" XDR_64Bit_Type )
						IF (XDR_64Bit_Type)
							SET(BALL_XDR_UINT64_TYPE ${XDR_UINT64_TYPE} CACHE STRING "Unsigned 64bit type for xdr_u_hyper" FORCE)
							BREAK()
						ENDIF()
				ENDFOREACH()

				IF (NOT XDR_64Bit_Type)
						SET(BALL_XDR_UINT64_TYPE OFF)
						MESSAGE(SEND_ERROR "Could not identify an appropriate type for XDR_64Bit_Type.")
				ENDIF()

			ELSE(BALL_HAS_XDR_U_HYPER)
					CHECK_TYPE_SIZE("unsigned long long int" SUPPORTS_64_BIT)
					IF(SUPPORTS_64_BIT)
							SET(BALL_U_QUAD_TYPE "unsigned long long int")
					ELSE(SUPPORTS_64_BIT)
							MESSAGE(SEND_ERROR "Could not identify an 64 bit unsigned type (long long).")
					ENDIF(SUPPORTS_64_BIT)
			ENDIF(BALL_HAS_XDR_U_HYPER)
	ENDIF()
ENDIF()
