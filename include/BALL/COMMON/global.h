// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.h,v 1.9.2.1 2003/01/07 13:17:19 anker Exp $

#ifndef BALL_COMMON_GLOBAL_H
#define BALL_COMMON_GLOBAL_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#include <limits.h>
#include <time.h>

#if defined(BALL_LOG_MEMORY) && !defined(BALL_COMMON_MEMORY_H)
#	include <BALL/COMMON/memory.h>
#endif

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
#endif

namespace BALL
{
	 
	typedef int (*ComparatorType)(const void *, const void *);


	#ifndef BALL_HAS_STDINT_H
	/**	@name Type aliases defined by BALL.
			These predefined types are used in BALL for portability and
			comprehensibility.
			{\bf Definition:} \URL{BALL/COMMON/global.h}
	*/
	//@{

	/**	Distance type.
			Use this type to represent distances in indices. Signed.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
	*/
	typedef BALL_INDEX_TYPE	Distance; 

	/**	Handle type.
			Use this type to represent {\bf handles}. Handles are used
			for the non-ambiguous identification of objects (e.g. object derived
      from \Ref{Object}). Handles are unsigned.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
  */
	typedef BALL_SIZE_TYPE 	Handle;

	/**	Index type.
			Use this type to represent indices (e.g. in strings or other sequences).
			Theses indices may be signed, contrary to the \Ref{Size} type.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
	*/
	typedef BALL_INDEX_TYPE	Index;

	/**	Size type.
			Use this type to represent sizes of containers, sequences or alike.
			Variables of type Size are unsigned.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
	*/
	typedef BALL_SIZE_TYPE 	Size;

	/**	Time type.
			Use this type to represent a point in time
			(as a replaecement for time_t).
			\\
			{\bf Size:} platform dependent\\
			{\bf not persistent}
	*/
	typedef time_t 	Time;

	/**	HashIndex type.
			Use this type to access the result of a hash functions. All hash functions
			defined in namespace \Ref{Hashing} return a HashIndex. HashIndex is unsigned.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
	*/
	typedef	BALL_SIZE_TYPE	HashIndex;

	/**	Position type.
			Use this type to represent positions (e.g. in a container) that
			cannot be negative (contrary to \Ref{Index}).
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
	*/
	typedef	BALL_SIZE_TYPE	Position;

	/**	Real type.
			Use this type to represent standard floating point numbers.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
	*/
	typedef float Real;

	/**	Double-precision real type.
			Use this type to represent double precision floating point numbers.
			\\
			{\bf Size:} 64 bit\\
			{\bf persistent}
	*/
	typedef double DoubleReal;

	/**	Unnamed property type.
			Use this type to represent (unnamed) properties.
			\\
			{\bf Size:} 32 bit\\
			{\bf persistent}
			@see PropertyManager
	*/
	typedef BALL_SIZE_TYPE Property;

	/**	Error code property type.
			Use this type to represent (signed) error codes. \\
			{\bf Size:} 32 bit\\
			{\bf persistent}
			@see PropertyManager
	*/
	typedef BALL_INDEX_TYPE ErrorCode;


	/**	Byte type.
			Use this type to represent byte data (8 bit length).
			A Byte is always unsigned.
			\\
			{\bf Size:} 8 bit\\
			{\bf persistent}
	*/
	typedef	unsigned char Byte;

	/**	Pointer-sized unsigned int type.
			This type holds unsigned 64 bit integer numbers and is used to store pointers
			in a portable fashion (for both 32-bit and 64-bit systems).
			\\
			{\bf Size:} 64 bit\\
			{\bf persistent}
	*/
	typedef BALL_ULONG64_TYPE PointerSizeUInt;

	/**	Pointer-sized signed int type.
			This type holds unsigned 64 bit numbers and is used to store pointers
			in a portable fashion (for both 32-bit and 64-bit systems).
			\\
			{\bf Size:} 64 bit\\
			{\bf persistent}
	*/
	typedef BALL_LONG64_TYPE PointerSizeInt;

	#else
		// the ISO C99 definitions
		typedef int32_t	Distance; 
		typedef uint32_t	Handle;
		typedef int32_t	Index;
		typedef uint32_t	Size;
		typedef time_t	Time;
		typedef	uint32_t	HashIndex;
		typedef	uint32_t	Position;
		typedef float Real;
		typedef double DoubleReal;
		typedef uint32_t Property;
		typedef int32_t ErrorCode;
		typedef	uint8_t Byte;
		typedef uint64_t PointerSizeInt;
		typedef int64_t	 PointerSizeUInt;

	#endif

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
