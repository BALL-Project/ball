# Try to find the XDR library and set some required variables
#
# Once run this will define:
#
# XDR_FOUND	 = system has XDR lib
#
# XDR_LIBRARIES	 = full path to the libraries, if required
#


INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckIncludeFileCXX)
INCLUDE(CheckLibraryExists)


SET(XDR_TEST_HEADER "
	#include<rpc/rpc.h>
	#include<rpc/xdr.h>

	int main(int /*argc*/, char** /*argv*/)
	{
		XDR *x = NULL;")

SET(XDR_TEST_FOOTER "
		return 0;
	}")


# Check RPC header files
CHECK_INCLUDE_FILE_CXX(rpc/types.h RPC_TYPES_H)
IF(NOT RPC_TYPES_H)
	MESSAGE(STATUS "Cannot find RPC header rpc/types.h. Persistence will be disabled!")
	RETURN()
ENDIF()

CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}
			   ${XDR_TEST_FOOTER}" RPC_XDR_H)
IF(NOT RPC_XDR_H)
	MESSAGE(STATUS "Cannot find RPC header rpc/xdr.h. Persistence will be disabled!")
	RETURN()
ENDIF()


# Check for symbol xdr_int
CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}
			   int i;
			   xdr_int(x, &i);
			   ${XDR_TEST_FOOTER}" RPC_XDR_INT)
IF(NOT RPC_XDR_INT)
	FOREACH(lib nsl oncrpc)
		# Try to find the corresponding lib manually
		SET(XDR_INT_LIBRARY)
		FIND_LIBRARY(XDR_INT_LIBRARY ${lib})

		IF (XDR_INT_LIBRARY)
			CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY} xdr_int "" XDR_INT_SYMBOL_FOUND)
		ENDIF()

		IF (XDR_INT_SYMBOL_FOUND)
			SET(XDR_LIBRARIES ${XDR_INT_LIBRARY})

			# Try to find the debug version as well
			FIND_LIBRARY(XDR_INT_LIBRARY_D ${lib}d)

			IF (XDR_INT_LIBRARY_D)
				CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY_D} xdr_int "" XDR_INT_SYMBOL_D_FOUND)
				IF (XDR_INT_SYMBOL_D_FOUND)
					BALL_COMBINE_LIBS(XDR_LIBRARIES ${XDR_LIBRARIES} ${XDR_INT_LIBRARY_D})
				ENDIF()
			ENDIF()

			SET(RPC_XDR_INT TRUE)
			BREAK()
		ENDIF()
	ENDFOREACH()
ENDIF()

IF(NOT RPC_XDR_INT)
	MESSAGE(STATUS "XRD symbol 'xdr_int' not found. Persistence will be disabled!")
	RETURN()
ENDIF()


# We have found XDR
SET(XDR_FOUND TRUE)


# Check for symbol xdr_u_hyper
CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}
			   u_quad_t q;
			   xdr_u_hyper(x, &q);
			   ${XDR_TEST_FOOTER}" BALL_HAS_XDR_U_HYPER)

IF(NOT BALL_HAS_XDR_U_HYPER)
	CHECK_LIBRARY_EXISTS(${XDR_INT_LIBRARY} xdr_u_hyper "" BALL_HAS_XDR_U_HYPER)
ENDIF()

IF(BALL_HAS_XDR_U_HYPER)
	MESSAGE(STATUS "Looking for 64-bit XDR type for xdr_u_hyper")

	# Try out possible types
	FOREACH(XDR_UINT64_TYPE u_quad_t u_longlong_t "unsigned long long" __uint64_t)
		CHECK_CXX_SOURCE_COMPILES("${XDR_TEST_HEADER}
					   ${XDR_UINT64_TYPE} q;
					   xdr_u_hyper(x, &q);
					   ${XDR_TEST_FOOTER}" XDR_64BIT_TYPE)

		IF (XDR_64BIT_TYPE)
			SET(BALL_XDR_UINT64_TYPE ${XDR_UINT64_TYPE} CACHE STRING "Unsigned 64bit type for xdr_u_hyper" FORCE)
			BREAK()
		ENDIF()
	ENDFOREACH()

	IF (NOT XDR_64BIT_TYPE)
		SET(BALL_XDR_UINT64_TYPE OFF)
		MESSAGE(WARNING "Could not identify an appropriate type for XDR_64BIT_TYPE.")
	ENDIF()
ELSE()
	CHECK_TYPE_SIZE("unsigned long long int" SUPPORTS_64_BIT)
	IF(SUPPORTS_64_BIT)
		SET(BALL_U_QUAD_TYPE "unsigned long long int")
	ELSE()
		MESSAGE(WARNING "Could not identify an 64 bit unsigned type (long long).")
	ENDIF()
ENDIF()
