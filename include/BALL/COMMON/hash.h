// $Id: hash.h,v 1.6 2000/06/07 14:27:21 amoll Exp $

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

	/**	@name	Hashing
	*/
	//@{

	/**	General Hash Function Template.
			This template function provides a simple wrapper
			for the specialized hash functions. It facilitates their use 
			in STL hash associative containers which expect a {\bf Hasher}
			class as template parameter.
	*/
  template <typename T>
  class HashFunction
  {
    public:
		
    bool operator () (const T& t) const
    {
      return Hash(t);
		}
	};

	/**	@name Specialized Hash Functions.
	*/
	//@{
	
	/**
	*/
  extern HashIndex hashPointer(void *const ptr);

	/**
	*/
  extern HashIndex hashString(const char* str);

	/**
	*/
  extern HashIndex hashPJWString(const char* str);

	/**
	*/
  extern HashIndex hashElfString(const char* str);

	/** General default hash function.
			This method should tires to convert the given key to a 
			\Ref{HashIndex} by calling {\tt (HashIndex)key}.
			If the key type {\tt T} is not convertible to \Ref{HashIndex} by
			default, a converter should be defined ({\tt operator HashIndex}).
			@param	key the key to be hashed
			@return	HashIndex the hash index
	*/
	template <typename T>
	inline HashIndex Hash(const T& key)
	{
		return (HashIndex)((PointerInt)key);
	}

	/** String hash function.
			This method is optimized for the hashing of STL strings.
			In fact, it is only an inline wrapper around \Ref{hashString}.
	*/
	inline HashIndex Hash(const string& s)
	{
		return hashString(s.c_str());
	}

	/** String hash function.
			This method is optimized for the hashing of BALL strings.
			In fact, it is only an inline wrapper around \Ref{hashString}.
	*/
	inline HashIndex Hash(const String& s)
	{
		return hashString(s.c_str());
	}

	/** Pointer hash function.
			Use this function to hash pointers to objects.
	*/
	inline HashIndex Hash(void *const& ptr)
	{
		return hashPointer(ptr);
	}

	//@}
		
	/**	Calculate the next prime number.
			This method returns the first prime number that is 
			greater or equal to the number given as the argument.
			Only odd prime numbers are returned, the lowest number returned is 3.
	*/
	HashIndex getNextPrime(HashIndex l);

	//@}

} // namespace BALL

#endif // BALL_COMMON_HASH_H
