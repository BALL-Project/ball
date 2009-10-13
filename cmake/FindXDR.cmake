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

## First try to find the required header files (rpc/types.h, rpc/xdr.h)

#check for the XDR functions: their interface and the libraries they're hidden in.
CHECK_INCLUDE_FILE_CXX(rpc/types.h XDR_HAS_RPC_TYPES_H)
IF(NOT XDR_HAS_RPC_TYPES_H)
    MESSAGE(SEND_ERROR "Cannot find RPC headers (rpc/types.h). Persistence will be disabled!")
ELSE()
	CHECK_INCLUDE_FILE_CXX(rpc/xdr.h XDR_HAS_RPC_XDR_H)
	IF(NOT XDR_HAS_RPC_XDR_H)
    MESSAGE(SEND_ERROR "Cannot find RPC headers (rpc/xdr.h). Persistence will be disabled!")
	ENDIF()
ENDIF()

IF (XDR_HAS_RPC_TYPES_H AND XDR_HAS_RPC_XDR_H)
	## Now let's see if we need an extra lib to compile it
	CHECK_FUNCTION_EXISTS(xdr_int XDR_IN_LIBC)

	SET(XDR_FOUND_SYMBOLS TRUE)

	IF (NOT XDR_IN_LIBC)
		## Let's see if we can find the symbols in libnsl (Solaris hides it there sometimes...)
		CHECK_LIBRARY_EXISTS(nsl xdr_int XDR_IN_LIBNSL)

		IF (XDR_IN_LIBNSL)
			SET(XDR_LIBRARIES "-lnsl")		
		ELSE()
			SET(XDR_FOUND_SYMBOLS FALSE)
			MESSAGE(SEND_ERROR "Could not locate xdr symbols in libc or libnsl. Persistence will be disabled!")
		ENDIF()
	ENDIF()

	IF (XDR_FOUND_SYMBOLS)

		SET(XDR_FOUND TRUE)

		## Let's see if we have an implementation for xdr_u_hyper
		CHECK_FUNCTION_EXISTS(xdr_u_hyper BALL_HAS_XDR_U_HYPER)

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
				LIST( LENGTH POSSIBLE_64BIT_TYPES LEN )
				WHILE( LEN GREATER 0 AND NOT XDR_64Bit_Type )
						LIST( GET POSSIBLE_64BIT_TYPES 0 BALL_XDR_UINT64_TYPE )
						LIST( REMOVE_AT POSSIBLE_64BIT_TYPES 0 )
						MATH( EXPR LEN "${LEN} - 1" )
						CHECK_CXX_SOURCE_COMPILES( "${XDR_TEST_HEADER}${BALL_XDR_UINT64_TYPE} q;${XDR_TEST_FOOTER}" XDR_64Bit_Type )
				ENDWHILE( LEN GREATER 0 AND NOT XDR_64Bit_Type )

				IF(XDR_64Bit_Type)
						#This puts the value of BALL_HAS_XDR_U_HYPER into the cache
						#Necessary, because cmake would skip the assignement on a second run because XDR_64Bit_Type is true
						SET(BALL_XDR_UINT64_TYPE ${BALL_XDR_UINT64_TYPE} CACHE STRING "Unsigned 64bit type for xdr_u_hyper")
				ELSE(XDR_64Bit_Type)
						SET(BALL_XDR_UINT64_TYPE OFF)
						MESSAGE(SEND_ERROR "Could not identify an appropriate type for XDR_64Bit_Type.")
				ENDIF(XDR_64Bit_Type)

			ELSE(BALL_HAS_XDR_U_HYPER)
					CHECK_TYPE_SIZE("unsigned long long int" SUPPORTS_64_BIT)
					IF(SUPPORTS_64_BIT)
							SET(BALL_U_QUAD_TYPE "unsigned long long int")
					ELSE(SUPPORTS_64_BIT)
							MESSAGE(SEND_ERROR "Could not identify an 64 bit unsigned type (long long).")
					ENDIF(SUPPORTS_64_BIT)
			ENDIF(BALL_HAS_XDR_U_HYPER)
	ENDIF(XDR_FOUND_SYMBOLS)
ENDIF()
