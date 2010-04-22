// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.h,v 1.24 2005/12/23 17:01:39 amoll Exp $
//

#ifndef BALL_COMMON_GLOBAL_H
#define BALL_COMMON_GLOBAL_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#include <limits.h>
#include <time.h>

#ifdef BALL_HAS_BASETSD_H
#	include <basetsd.h>
#endif

// If possible use the ISO C99-compliant header stdint.h
// to define the portable integer types.
#ifdef BALL_HAS_STDINT_H
#	include <stdint.h>
#endif

// Added to avoid warnings with MS Visual Studio .NET
#ifdef BALL_COMPILER_MSVC
#	pragma warning( disable : 4290 )
#	pragma warning( disable : 4251 ) // Warnings about DLL interface 
#	pragma warning( disable : 4275 ) // Warnings about DLL interface (we should do something about them, though)
#endif

#ifdef BALL_COMPILER_MSVC
# define BALL_HIDE
#	ifdef BALL_BUILD_DLL
#		define BALL_EXPORT __declspec(dllexport)
#		define BALL_EXTERN_VARIABLE __declspec(dllexport) extern
#	else
#		define BALL_EXPORT __declspec(dllimport)
#		define BALL_EXTERN_VARIABLE __declspec(dllimport) extern
#	endif
#	ifdef BALL_VIEW_BUILD_DLL
#		define BALL_VIEW_EXPORT __declspec(dllexport)
#	else
#		define BALL_VIEW_EXPORT __declspec(dllimport)
#	endif
#else
# define BALL_EXPORT __attribute((visibility ("default")))
# define BALL_HIDE __attribute((visibility ("hidden")))
# define BALL_VIEW_EXPORT __attribute((visibility ("default")))
# define BALL_EXTERN_VARIABLE extern
#endif

//This declares a BALL_DEPRECATED macro that can be used to mark deprecated api
//It is essentially copied from Qt 4.4.1 but simplified, thus there might be
//some cases where it does not work as expected, yet.
#if defined(BALL_COMPILER_MSVC) && (_MSC_VER >= 1300)
    #define BALL_DEPRECATED __declspec(deprecated)
#elif defined(BALL_COMPILER_GXX) && (BALL_COMPILER_VERSION_MAJOR - 0 > 3 || (BALL_COMPILER_VERSION_MAJOR - 0 == 3 && BALL_COMPILER_VERSION_MINOR - 0 >= 2))
    #define BALL_DEPRECATED __attribute__((deprecated))
#else
    #define BALL_DEPRECATED
#endif

namespace BALL
{

	typedef int (*ComparatorType)(const void *, const void *);

	/**	@name Type aliases defined by BALL.
			These predefined types are used in BALL for portability and
			comprehensibility.
			
	\ingroup Common
	*/
	//@{

	/**	Distance type.
			Use this type to represent distances in indices. Signed.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
	*/
	typedef BALL_INDEX_TYPE	Distance; 

	/**	Handle type.
			Use this type to represent <b>handles</b>. Handles are used
			for the non-ambiguous identification of objects (e.g. object derived
      from  \link Object Object \endlink ). Handles are unsigned.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
  */
	typedef BALL_SIZE_TYPE 	Handle;

	/**	Index type.
			Use this type to represent indices (e.g. in strings or other sequences).
			Theses indices may be signed, contrary to the  \link ::Size Size \endlink  type.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
	*/
	typedef BALL_INDEX_TYPE	Index;

	/**	Size type.
			Use this type to represent sizes of containers, sequences or alike.
			Variables of type Size are unsigned.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
	*/
	typedef BALL_SIZE_TYPE 	Size;

	/**	Time type.
			Use this type to represent a point in time
			(as a replaecement for time_t).
			 \par
			<b>Size:</b> platform dependent \par
			<b>not persistent</b>
	*/
	typedef time_t 	Time;

	/**	HashIndex type.
			Use this type to access the result of a hash functions. HashIndex is unsigned.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
	*/
	typedef	BALL_SIZE_TYPE	HashIndex;

	/**	Position type.
			Use this type to represent positions (e.g. in a container) that
			cannot be negative (contrary to  \link ::Index Index \endlink ).
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
	*/
	typedef	BALL_SIZE_TYPE	Position;

	/**	Real type.
			Use this type to represent standard floating point numbers.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
	*/
	typedef float Real;

	/**	Double-precision real type.
			Use this type to represent double precision floating point numbers.
			 \par
			<b>Size:</b> 64 bit \par
			<b>persistent</b>
	*/
	typedef double DoubleReal;

	/**	Unnamed property type.
			Use this type to represent (unnamed) properties.
			 \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
			@see PropertyManager
	*/
	typedef BALL_SIZE_TYPE Property;

	/**	Error code property type.
			Use this type to represent (signed) error codes.  \par
			<b>Size:</b> 32 bit \par
			<b>persistent</b>
			@see PropertyManager
	*/
	typedef BALL_INDEX_TYPE ErrorCode;


	/**	Byte type.
			Use this type to represent byte data (8 bit length).
			A Byte is always unsigned.
			 \par
			<b>Size:</b> 8 bit \par
			<b>persistent</b>
	*/
	typedef	unsigned char Byte;

	/**	Long unsigned int type.
			This type holds unsigned 64 bit integer numbers and is used to store pointers
			in a portable fashion (for both 32-bit and 64-bit systems).
			 \par
			<b>Size:</b> 64 bit \par
			<b>persistent</b>
	*/
	typedef BALL_ULONG64_TYPE LongSize;

	/**	Long signed int type.
			This type holds unsigned 64 bit numbers and is used to store pointers
			in a portable fashion (for both 32-bit and 64-bit systems).
			 \par
			<b>Size:</b> 64 bit \par
			<b>persistent</b>
	*/
	typedef BALL_LONG64_TYPE LongIndex;

	/** Unsigned int with the same size as a pointer.
			Used for internal conversion issues mostly.
			<b>Size:</b> 32/64 bit (platform dependent)\par
	*/
	typedef BALL_POINTERSIZEUINT_TYPE PointerSizeUInt;

	//@}

	enum ASCII
	{
		ASCII__BACKSPACE        = '\b',
		ASCII__BELL             = '\a',
		ASCII__CARRIAGE_RETURN  = '\r',
		ASCII__HORIZONTAL_TAB   = '\t',
		ASCII__NEWLINE          = '\n',
		ASCII__RETURN           = ASCII__NEWLINE,
		ASCII__SPACE            = ' ',
		ASCII__TAB              = ASCII__HORIZONTAL_TAB,
		ASCII__VERTICAL_TAB     = '\v',

		ASCII__COLON            = ':',
		ASCII__COMMA            = ',',
		ASCII__EXCLAMATION_MARK = '!',
		ASCII__POINT            = '.',
		ASCII__QUESTION_MARK    = '?',
		ASCII__SEMICOLON        = ';'
	};

	static const Distance INVALID_DISTANCE = INT_MIN;
	static const Distance DISTANCE_MIN = (INT_MIN + 1);
	static const Distance DISTANCE_MAX = INT_MAX;

	static const Handle INVALID_HANDLE = INT_MAX;
	static const Handle HANDLE_MIN = 0 ;
	static const Handle HANDLE_MAX = INT_MAX - 1;

	static const Index INVALID_INDEX = -1;
	static const Index INDEX_MIN = 0;
	static const Index INDEX_MAX = INT_MAX;

	static const Position INVALID_POSITION = INT_MAX;
	static const Position POSITION_MIN = 0;
	static const Position POSITION_MAX = INT_MAX - 1;

#	undef SIZE_MAX
	static const Size INVALID_SIZE = INT_MAX;
	static const Size SIZE_MIN = 0;
	static const Size SIZE_MAX = INT_MAX - 1;
	
	
}

#endif // BALL_COMMON_GLOBAL_H
