// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DATATYPE_BITVECTOR_H
#define BALL_DATATYPE_BITVECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif


#include <cstring>

#define BALL_BLOCK_BITS 8
#define BALL_BLOCK_MASK (BALL_BLOCK_BITS - 1)
#define BALL_BLOCK_SHIFT              3
#define BALL_BLOCK_ALL_BITS_SET       0xFF
#define BALL_BLOCK_ALL_BITS_CLEARED   0x00


#define BALL_BLOCK_SIZE(bits) (Size)(((bits) + BALL_BLOCK_BITS - 1) >> BALL_BLOCK_SHIFT)


namespace BALL 
{
	class BitVector;

	/**	Bit Class. This class represents a bit within a BitVector.
			
			@see BitVector. 
    	\ingroup  DatatypeMiscellaneous
	*/
	class BALL_EXPORT Bit
	{
		public:

		/**	@name Exceptions
		*/
		//@{

		/** Exception thrown if a file could not be processed right.
		*/
		class BALL_EXPORT IllegalOperation
			: public Exception::GeneralException
		{
			public:
			IllegalOperation(const char* file, int line);
		};

		//@}


		/**	@name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(Bit)

		/** Default constructor 
		*/
		Bit();

		/**	Copy constructor
		*/
		Bit(const Bit& bit);

		/** Detailed constructor.
				For use with nonconst bitvector.
				The bitvector can be resized by accessing bits out of the bitvectors range.
				@exception NullPointer if bitvector is equal to 0
		*/
		Bit(BitVector* bitvector, Index index = 0);

		/** Detailed constructor.
				For use with const bitvector.
				@exception NullPointer if bitvector is equal to 0
				@exception IndexUnderflow if index is too small
				@exception IndexOverflow	if index is greater than the size of bitvector
		*/
		Bit(const BitVector* const bitvector, Index index = 0);

		/** Destructor
		*/
		virtual ~Bit();
		//@}


		/**	@name	Converters
		*/
		//@{
		
		/** Casting operator from Bit to bool.
				@exception NullPointer
		*/
		operator bool() const;

		//@}
		/**	Assignment
		*/
		//@{
		
		/** Assignment operator.
				Assign the position from a Bit to this instance
		*/
		Bit& operator = (const Bit& bit);

		/** Assignment operator.
				Assign a bool value to this instance.
				The bit in the bitvector is set to the given value.
				@exception Exception::IllegalOperation if instance points to a const bitvector
				@exception Exception::NullPointer
		*/
		Bit& operator = (const bool bit);

		/** Clear method 
		*/
		virtual void clear();

		//@}
		/**	@name	 Predicates
		*/
		//@{

		/** Equality operator.
				Test if two instances have the same position in a bitvector
		*/
		bool operator == (const Bit& bit) const;

		/** Equality operator.
				Test if this instance has the given bool value
				@exception Exception::NullPointer if this bitvector is not correctly initialized
		*/
		bool operator == (bool bit) const;

		/** Inequality operator.
				Test if two instances point to different positions.
		*/
		bool operator != (const Bit& bit) const;

		/** Inequality operator.
				Test if this instance has not the given bool value
				@exception Exception::NullPointer if this bitvector is not correctly initialized
		*/
		bool operator != (bool bit) const;

		//@}

		private:

		// --- ATTRIBUTES

		BitVector* 	bitvector_;
		Index 			index_;
		bool				bitvector_muteable_;
	};


	/**	Bit vector class.
			Indices may be given as negative arguments: start from the end
			-1 therefore means the last bit.
			Some functions resize the instance if a index greater than the
			size of the instance is given.
			Other functions throw exception in this case.

    	\ingroup  DatatypeMiscellaneous
	*/
	class BALL_EXPORT BitVector
	{
		public:

		BALL_CREATE(BitVector)


		/**	@name	Type Definitions and Constants
		*/
		//@{

		///
		typedef unsigned char BlockType;
		///
		typedef std::vector<BlockType>	VectorType;
		///
		static const Size BlockSize;

		//@}
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor
		*/
		BitVector();

		/**	Detailed constructor
		 *  @exception Exception::OutOfMemory if a BitVector with length size could not be allocated
		 */
		BitVector(Size size);

		/** Copy constructor
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		 */
		BitVector(const BitVector& bit_vector);

		/** Detailled constructor from an array of char.
		 *	{\em Caveat:} the array of char has to be zero-terminated!
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		 */
		BitVector(const char* bit_string);

		/**	Destructor
		*/
		virtual ~BitVector();

		/**	Clear method.
				This method will set the size of this instance to 0.
		*/
		void clear();

		//@}
		/**	@name	Assignment 
		*/
		//@{

		/** Assignment from an other BitVector instance.
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		 */
		void set(const BitVector& bit_vector);

		/** Assignment from a char string.
				If a char is different from '0', it is interpreted as true.
				{\em Caveat:} the array of char has to be zero-terminated!
		    @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		*/
		void set(const char* bit_string);

		/** Assignment from an other BitVector instance.
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		 */
		BitVector& operator = (const BitVector& bit_vector);

		/** Assignment from a char string.
		 *	If a char is different from '0', it is interpreted as true.
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		 */
		BitVector& operator = (const char *bit_string);

		/** Assignment to an other BitVector.
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		 */
		void get(BitVector& bitvector) const;
		//@}

		/**	@name	Accessors 
		*/
		//@{

		/** Return a sub-BitVector.
				A new BitVector is created and filled with elements of this instance.
				@param first the index of the first element to be copied
				@param last the index of the last element to be copied
				@return BitVector a partial copy of this instance
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		BitVector operator () (Index first, Index last) const;

		/** Set the size of this instance.
	 	 *	@param size the new size
		 *	@param keep ?????
		 *  @exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		*/
		void setSize(Size size, bool keep = true);

		/**	Return the size of this instance.
		*/
		Size getSize() const;

		/** Count a given bool value in this instance.
				@param value the value to be counted
				@return Size the number of the given value
		*/
		Size countValue(bool value) const;

		/** Return a muteable pointer to the values of this instance.
				@return unsigned char*	a muteable pointer to the values of this instance
		*/
		VectorType& getBitSet();

		/** Return a constant pointer to the values of this instance.
				@return unsigned char*	a constant pointer to the values of this instance
		*/
		const VectorType& getBitSet() const;

		/**	Random access to the components.
				If the given index is greater than the size of this instance, this BitVector
				is increased to the given index.
				@return Bit a Bit pointing to the given element of this instance
				@exception Exception::OutOfMemory if the bit could not be allocated
		*/
		Bit operator []	(Index index);

		/**	Constant random access to the components.
				If the given index is greater than the size of this instance an exception is thrown.
				@return bool the value of the given index
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		bool operator [] (Index index) const;

		/** Set the given Bit.
				The element at the given position is set to the given value.
				If the given index is greater than the size of this instance, this BitVector
				is increased to the given index.
				@param index the index of the element
				@param value the value to be set
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		void setBit(Index index, bool value = true);

		/** Get the value of an element.
				If the given index is greater than the size of this instance, this BitVector
				is increased to the given index.
				@param index the index of the element
				@return bool the value of the element
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		bool getBit(Index index);

		/** Get the value of an element (const version).
				If the given index is greater than the size of this instance an exception is thrown.
				No resizing is done.
				@param index the index of the element
				@return bool the value of the element
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		bool getBit(Index index) const;

		/** Toggle the value of an element.
				If the given index is greater than the size of this instance an exception is thrown.
				@param index the index of the element
				@exception Exception::IndexUnderflow
				@exception Exception::OutOfMemory
		*/
		void toggleBit(Index index);

		/** Fill a part of this instance with a given value.
				If a given index is greater than the size of this instance an exception is thrown.
				@param value the value to be set
				@param first the starting position
				@param last the last element to be filled
				@exception Exception::IndexUnderflow
				@exception Exception::OutOfMemory
		*/
		void fill(bool value = true, Index first = 0 , Index last = -1);

		/** Toggle the values of a part of this instance.
				If a given index is greater than the size of this instance an exception is thrown.
				@param value the value to be set
				@param first the starting position
				@param last the last element to be filled
				@exception Exception::IndexUnderflow
				@exception Exception::OutOfMemory
		*/
		void toggle(Index first = 0, Index last = -1);

		/** Set a unsigned char as the bit pattern.
				For example: 22 => 00010110.
				@param bit_pattern the new pattern.
		*/
		void setUnsignedChar(unsigned char bit_pattern);

		/** Get the bit pattern as unsigned char.
				For example: 00010110 => 22.
				@return unsigned char the bit-pattern
		*/
		unsigned char getUnsignedChar() const;

		/** Set a unsigned short as the bit pattern.
				@see setUnsignedChar
		*/
		void setUnsignedShort(unsigned short bit_pattern);

		/** Get the bit pattern as unsigned short.
				@see getUnsignedChar
		*/
		unsigned short getUnsignedShort() const;

		/** Set a unsigned int as the bit pattern.
				@see setUnsignedChar
		*/
		void setUnsignedInt(unsigned int bit_pattern);

		/** Get the bit pattern as unsigned int.
				@see getUnsignedChar
		*/
		unsigned int getUnsignedInt() const;

		/** Set a unsigned long as the bit pattern.
				@see setUnsignedChar
		*/
		void setUnsignedLong(unsigned long bit_pattern);

		/** Get the bit pattern as unsigned long.
				@see getUnsignedChar
		*/
		unsigned long getUnsignedLong() const;

		/** Compute {\em this or bit_vector}.
				The result is saved in this instance.
		    @exception Exception::OutOfMemory
		*/
		void bitwiseOr(const BitVector& bit_vector);

		/** Compute {\em this xor bit_vector}.
				The result is saved in this instance.
		    @exception Exception::OutOfMemory
		*/
		void bitwiseXor(const BitVector& bit_vector);

		/** Compute {\em this and bit_vector}.
				The result is saved in this instance.
		    @exception Exception::OutOfMemory
		*/
		void bitwiseAnd(const BitVector& bit_vector);

		/** Or Operator.
				Creates a new BitVector object and fills it with the result of
				{\em this or bit_Vector}.
		    @exception Exception::OutOfMemory
		*/
		BitVector operator | (const BitVector& bit_vector);

		/** Compute {\em this or bit_vector}.
				The result is saved in this instance.
		    @exception Exception::OutOfMemory
		*/
		BitVector& operator |= (const BitVector& bit_vector);

		/** And Operator.
				Creates a new BitVector object and fills it with the result of
				{\em this and bit_Vector}.
		    @exception Exception::OutOfMemory
		*/
		BitVector operator & (const BitVector& bit_vector);

		/** Compute {\em this and bit_vector}.
				The result is saved in this instance.
		    @exception Exception::OutOfMemory
		*/
		BitVector& operator &= (const BitVector& bit_vector);

		/** Xor Operator.
				Creates a new BitVector object and fills it with the result of
				{\em this Xor bit_vector}.
		    @exception Exception::OutOfMemory
		*/
		BitVector operator ^ (const BitVector& bit_vector);

		/** Compute {\em this xor bit_vector}.
				The result is saved in this instance.
		    @exception Exception::OutOfMemory
		*/
		BitVector& operator ^= (const BitVector& bit_vector);

		/** Negate Operator.
				Creates a new BitVector object and fills it with the negate 
				result of this instance.
		    @exception Exception::OutOfMemory
		*/
		BitVector operator ~ ();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const BitVector& bit_vector) const;

		/// Inequality operator
		bool operator != (const BitVector& bit_vector) const;

		/** Test if any bit in a given range has the given value.
				@param value the value to look for
				@param first the index to start searching
				@param last the index to stop searching
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		bool isAnyBit(bool value, Index first = 0, Index last = -1) const;

		/** Test if every bit in a given range has the given value
				@param value the value to look for
				@param first the index to start searching
				@param last the index to stop searching
				@exception Exception::IndexUnderflow
				@exception Exception::IndexOverflow
		*/
		bool isEveryBit(bool value, Index first = 0, Index last = -1) const;

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Test if this instance is valid.
		*/
		bool isValid() const;

		//@}
		/**	@name	Storers 
		*/
		//@{

		/**	Input operator.
				Reads the values of type <b>  bool </b> from an istream.
				@exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		*/
		BALL_EXPORT friend std::istream& operator >> (std::istream& s, BitVector& bit_vector);

		/**	Output operator.
				Writes the values of type <b>  bool </b> to an ostream.
		*/
		BALL_EXPORT friend std::ostream& operator << (std::ostream& s, const BitVector& bit_vector);

		/**	Read the values of of type <b>  bool </b> from an istream.
		 *	@exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		*/
		virtual void read(std::istream& s);

		/**	Write the values of of type <b>  bool </b> to an ostream.
		*/
		virtual void write(std::ostream& s) const;

		/**	Storable write method.
		*/
		virtual void write(PersistenceManager& pm) const;

		/**	Storable read method.
		 *	@exception Exception::OutOfMemory if a BitVector with the appropriate size could not be allocated
		*/
		virtual bool read(PersistenceManager& pm);

		//@}

		protected:
	
		// @exception Exception::IndexUnderflow
		// @exception Exception::OutOfMemory
		void validateIndex_(Index& index);

		// @exception Exception::IndexUnderflow
		// @exception Exception::OutOfMemory
		void validateIndex_(Index& index) const;

		// @exception Exception::IndexUnderflow
		// @exception Exception::IndexOverflow
		void validateRange_(Index& first, Index& last) const;


		private:
	
		// @exception Exception::IndexUnderflow
		// @exception Exception::OutOfMemory
		Index block_(Index index);

		// @exception Exception::IndexUnderflow
		// @exception Exception::OutOfMemory
		Index block_(Index index) const;

		BlockType mask_(Index index) const;

		// --- ATTRIBUTES

		Size		size_;
		VectorType	bitset_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/bitVector.iC>
#	endif
} //namespace BALL


#endif // BALL_DATATYPE_BITVECTOR_H
