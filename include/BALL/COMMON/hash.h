// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_HASH_H
#define BALL_COMMON_HASH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

namespace BALL 
{
	/**	@name Specialized Hash Functions.
	*/
	//@{
	
	/**
	*/
  BALL_EXPORT extern HashIndex hashPointer(void *const ptr);

	/**
	*/
  BALL_EXPORT extern HashIndex hashString(const char* str);

	/**
	*/
  BALL_EXPORT extern HashIndex hashPJWString(const char* str);

	/**
	*/
  BALL_EXPORT extern HashIndex hashElfString(const char* str);

	/** General default hash function.
			This method converts a given key to a 
			 \link HashIndex HashIndex \endlink  by calling <tt>(HashIndex)key</tt>.
			If the key type <tt>T</tt> is not convertible to  \link HashIndex HashIndex \endlink  by
			default, a converter should be defined (<tt>operator HashIndex</tt>).
			@param	key the key to be hashed
			@return	HashIndex the hash index
	*/
	template <typename T>
	inline HashIndex Hash(const T& key)
	{
		return static_cast<HashIndex>((PointerSizeUInt)key);
	}

	/** String hash function.
			This method is optimized for the hashing of STL strings.
			In fact, it is only an inline wrapper around  \link hashString hashString \endlink .
	*/
	BALL_EXPORT inline HashIndex Hash(const string& s)
	{
		return hashString(s.c_str());
	}

	/** String hash function.
			This method is optimized for the hashing of BALL strings.
			In fact, it is only an inline wrapper around  \link hashString hashString \endlink .
	*/
	BALL_EXPORT inline HashIndex Hash(const String& s)
	{
		return hashString(s.c_str());
	}

	/** Pointer hash function.
			Use this function to hash pointers to objects.
	*/
	BALL_EXPORT inline HashIndex Hash(void *const& ptr)
	{
		return hashPointer(ptr);
	}

	//@}
		
	//@{
	/**	Calculate the next prime number.
			This method returns the first prime number that is 
			greater or equal to the number given as the argument.
			Only odd prime numbers are returned, the lowest number returned is 3.
	*/
	BALL_EXPORT HashIndex getNextPrime(HashIndex l);

	//@}

} // namespace BALL

#endif // BALL_COMMON_HASH_H
