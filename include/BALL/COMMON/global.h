// $Id: global.h,v 1.6 2000/12/08 05:47:07 oliver Exp $

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

namespace BALL
{

	typedef int (*ComparatorType)(const void *, const void *);


	/**	@name Type aliases defined by BALL.
			These predefined types are used in BALL for portability and
			comprehensibility.
			{\bf Definition:} \URL{BALL/COMMON/global.h}
	*/
	//@{

	/**	Distance type.
			Use this type to represent distances in indices.
	*/
	typedef int 					Distance; 

	/**	Handle type.
			Use this type to represent {\bf handles}. Handles are used
			for the non-ambiguous identification of objects (e.g. object derived
      from \Ref{Object}).
  */
	typedef unsigned int 	Handle;

	/**	Index type.
			Use this type to represent indices (e.g. in strings or other sequences).
			Theses indices may be signed, contrary to the \Ref{Size} type.
	*/
	typedef BALL_INDEX_TYPE	Index;

	/**	Size type.
			Use this type to represent sizes of containers, sequences or alike.
			Variables of type Size are unsigned.
	*/
	typedef BALL_SIZE_TYPE 	Size;

	/**	Time type.
			Use this type to represent a point in time
			(as a replaecement for time_t).
	*/
	typedef time_t 	Time;

	/**	HashIndex type.
			Use this type to access the result of a hash functions. All hash functions
			defined in namespace \Ref{Hashing} return a HashIndex. HashIndex is unsigned.
	*/
	typedef	BALL_SIZE_TYPE	HashIndex;

	/**	Position type.
			Use this type to represent positions (e.g. in a container) that
			cannot be negative (contrary to \Ref{Index}).
	*/
	typedef	BALL_SIZE_TYPE	Position;

	/**	Real type.
			Use this type to represent standard floating point numbers.
	*/
	typedef float Real;

	/**	Unnamed property type.
			Use this type to represent (unnamed) properties.
			@see PropertyManager
	*/
	typedef int Property;

	/**	Error code property type.
			Use this type to represent error codes.
			@see PropertyManager
	*/
	typedef int ErrorCode;


	/**	Byte type.
			Use this type to represent byte data (8 bit length).
			A Byte is always unsigned.
	*/
	typedef	unsigned char Byte;

	/**	Pointer-sized int type.
			This type holds unsigned 64 bit numbers and is used to store pointers
			in a portable fashion (for both 32bit and 64bit systems).
	*/
	typedef BALL_64BIT_UINT_TYPE PointerInt;

	/**	64 bit unsigned integer type.
			This type is used to read and write pointers to persistent
			objects to ensure compatibility between 64 and 32 bit architectures.
	*/
	typedef BALL_ULONG64_TYPE LongPointerType;
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
