// $Id: bitVector.h,v 1.12 2000/07/25 05:53:26 oliver Exp $

#ifndef BALL_DATATYPE_BITVECTOR_H
#define BALL_DATATYPE_BITVECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

#include <string.h>

#define BALL_BLOCK_BITS ((1 * BALL_CHAR_BITS))

#define BALL_BLOCK_MASK (BALL_BLOCK_BITS - 1)

#if   (BALL_BLOCK_BITS == 8)
#  define BALL_BLOCK_SHIFT              3
#  define BALL_BLOCK_ALL_BITS_SET       0xFF
#  define BALL_BLOCK_ALL_BITS_CLEARED   0x00
#elif (BALL_BLOCK_BITS == 16)
#  define BALL_BLOCK_SHIFT              4
#  define BALL_BLOCK_ALL_BITS_SET       0xFFFF
#  define BALL_BLOCK_ALL_BITS_CLEARED   0x0000
#elif (BALL_BLOCK_BITS == 32)
#  define BALL_BLOCK_SHIFT              5
#  define BALL_BLOCK_ALL_BITS_SET       0xFFFFFFFF
#  define BALL_BLOCK_ALL_BITS_CLEARED   0x00000000
#else  
#  define BALL_BLOCK_SHIFT              6
#  define BALL_BLOCK_ALL_BITS_SET       0xFFFFFFFFFFFFFFFF
#  define BALL_BLOCK_ALL_BITS_CLEARED   0x0000000000000000
#endif  

#define BALL_BLOCK_SIZE(bits) (Size)(((bits) + BALL_BLOCK_MASK) >> BALL_BLOCK_SHIFT)


namespace BALL 
{

	class BitVector;

	/**	Bit Class
	*/
	class Bit
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Copy constructor
		*/
		Bit(const BitVector& bitvector, Index index = 0);
		//@}

		/**	@name	Converters
		*/
		//@{
		
		/** Casting operator from Bit to bool.
		*/
		operator bool() const;
		//@}

		/**	Assignment
		*/
		//@{
		
		/** Assignment operator.
				Assign the bool value from a Bit to this instance
		*/
		Bit& operator = (const Bit& bit);

		/** Assignment operator.
				Assign a bool value to this instance
		*/
		Bit& operator = (bool bit);
		//@}

		/**	@name	 Predicates
		*/
		//@{

		/** Equality operator.
				Test if two instances have the same bool value
		*/
		bool operator == (const Bit& bit) const;

		/** Equality operator.
				Test if this instance has the given bool value
		*/
		bool operator == (bool bit) const;

		/** Inequality operator.
				Test if two instances have different bool values
		*/
		bool operator != (const Bit& bit) const;

		/** Inequality operator.
				Test if this instance has not the given bool value
		*/
		bool operator != (bool bit) const;
		//@}

		private:

		Bit();

		// --- ATTRIBUTES

		BitVector* 	bitvector_;
		Index 			index_;
	};


	/**	Bit vector class 
			Indices may be given as negative arguments: start from the end
			-1 therefore means the last bit.
			Some functions resize the instance if a index greater than the
			size of the instance is given.
			Other functions throw exception in this case.
	*/
	class BitVector
	{
		public:

		BALL_CREATE(BitVector)


		/**	@name	Type Definitions and Constants
		*/
		//@{

		///
		typedef unsigned char BlockType;
			
		static const Size BlockSize;
		//@}

	
		/**	@name	Constructors and Destructors */
		//@{

		/**	Default constructor
		*/
		BitVector();

		/**	Detailled constructor
		*/
		BitVector(Size size);

		/** Copy constructor
		*/
		BitVector(const BitVector& bit_vector, bool deep = true);

		///
		BitVector(const char* bit_string);

		/**	Destructor
		*/
		virtual ~BitVector();

		/**	Clear method
		*/
		void clear();
		//@}


		/**	@name	Assignment */
		//@{

		/// Assignment from an other BitVector instance.
		void set(const BitVector& bit_vector, bool deep = true);

		/** Assignment from a char string.
				If a char is different from '0', it is interpreted as true.
		*/
		void set(const char* bit_string);

		/// Assignment from an other BitVector instance.
		BitVector& operator = (const BitVector& bit_vector);

		/** Assignment from a char string.
				If a char is different from '0', it is interpreted as true.
		*/
		BitVector& operator = (const char *bit_string);

		/// Assignment to an other BitVector.
		void get(BitVector& bitvector, bool deep = true) const;

		/// Swapping of two BitVector instances.
		void swap(BitVector& bitvector);
		//@}

		/**	@name	Accessors */
		//@{

		/** Return a sub-BitVector.
				A new BitVector is created an filled with elements of this instance.
				@param first, the index of the first element to be copied
				@param last, the index of the last element to be copied
				@return BitVector a partial copy of this instance
		*/
		BitVector operator () (Index first,Index last) const;

		/** Set the size of this instance.
				@param size the new size
				@param keep ???
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
		BlockType* getBitSet();

		/** Return a constant pointer to the values of this instance.
				@return unsigned char*	a constant pointer to the values of this instance
		*/
		const BlockType* getBitSet() const;

		/**	Random access to the components.
				If the given index is greater than the size of this instance, this BitVector
				is increased to the given index.
				@return Bit a Bit pointing to the given element of this instance
		*/
		Bit operator []	(Index index);

		/**	Constant random access to the components.
				If the given index is greater than the size of this instance an exception is thrown.
				@return bool the value of the given index
		*/
		bool operator [] (Index index) const;

		/** Set the given Bit.
				The element at the given position is set to the given value.
				If the given index is greater than the size of this instance, this BitVector
				is increased to the given index.
				@param index the index of the element
				@param value the value to be set
		*/
		void setBit(Index index, bool value = true);

		/** Get the value of an element.
				If the given index is greater than the size of this instance, this BitVector
				is increased to the given index.
				@param index the index of the element
				@return bool the value of the element
		*/
		bool getBit(Index index);

		/** Get the value of an element.
				If the given index is greater than the size of this instance an exception is thrown.
				@param index the index of the element
				@return bool the value of the element
		*/
		bool getBit(Index index) const;

		/** Toggle the value of an element.
				If the given index is greater than the size of this instance an exception is thrown.
				@param index the index of the element
		*/
		void toggleBit(Index index);

		/** Fill a part of this instance with a given value.
				If a given index is greater than the size of this instance an exception is thrown.
				@param value the value to be set
				@param first the starting position
				@param last the last element to be filled
		*/
		void fill(bool value = true, Index first = 0 , Index last = -1);

		/** Toggle the values of a part of this instance.
				If a given index is greater than the size of this instance an exception is thrown.
				@param value the value to be set
				@param first the starting position
				@param last the last element to be filled
		*/
		void toggle(Index first = 0, Index last = -1);

		/// ???
		void setUnsignedChar(unsigned char bit_pattern);

		/// ???/
		unsigned char getUnsignedChar() const;

		/// ???/
		void setUnsignedShort(unsigned short bit_pattern);

		/// ???/
		unsigned short getUnsignedShort() const;

		/// ???/
		void setUnsignedInt(unsigned int bit_pattern);

		/// ???/
		unsigned int getUnsignedInt() const;

		/// ???/
		void setUnsignedLong(unsigned long bit_pattern);

		/// ???/
		unsigned long getUnsignedLong() const;

		///
		void bitwiseOr(const BitVector& bit_vector);

		///
		void bitwiseXor(const BitVector& bit_vector);

		///
		void bitwiseAnd(const BitVector& bit_vector);

		/** Or Operator.
				Creates a new BitVector object and fills it with the result of
				{\em this or bit_Vector).
		*/
		BitVector operator | (const BitVector& bit_vector);

		/** Compute {\em this or bit_vector}.
				The result is saved in this instance.
		*/
		BitVector& operator |= (const BitVector& bit_vector);

		/** And Operator.
				Creates a new BitVector object and fills it with the result of
				{\em this and bit_Vector).
		*/
		BitVector operator & (const BitVector& bit_vector);

		/** Compute {\em this and bit_vector}.
				The result is saved in this instance.
		*/
		BitVector& operator &= (const BitVector& bit_vector);

		/** Xor Operator.
				Creates a new BitVector object and fills it with the result of
				{\em this Xor bit_Vector).
		*/
		BitVector operator ^ (const BitVector& bit_vector);

		/** Compute {\em this xor bit_vector}.
				The result is saved in this instance.
		*/
		BitVector& operator ^= (const BitVector& bit_vector);

		/** Negate Operator.
				Creates a new BitVector object and fills it with the negate 
				result of this instance.
		*/
		BitVector operator ~ ();
		//@}

		/**	@name	Predicates */
		//@{

		/// Equality operator
		bool operator == (const BitVector& bit_vector) const;

		/// Inequality operator
		bool operator != (const BitVector& bit_vector) const;

		/** Test if any bit in a given range has the given value.
				@param value the value to look for
				@param first the index to start searching
				@param last the index to stop searching
		*/
		bool isAnyBit(bool value, Index first = 0, Index last = -1) const;

		/** Test if every bit in a given range has the given value
				@param value the value to look for
				@param first the index to start searching
				@param last the index to stop searching
		*/
		bool isEveryBit(bool value, Index first = 0, Index last = -1) const;

		//@}

		/**	@name	Debugging and Diagnostics */
		//@{

		/** Test if this instance is valid.
		*/
		bool isValid() const;
		//@}


		/**	@name	Storers */
		//@{

		/**	Input- Operator.
				Reads the values of of type {\em bool} from an istream.
		*/
		friend std::istream& operator >> (std::istream& s, BitVector& bit_vector);

		/**	Output- Operator.
				Writes the values of type {\em bool} to an ostream.
		*/
		friend std::ostream& operator << (std::ostream& s, const BitVector& bit_vector);

		/**	Read the values of of type {\em bool} from an istream.
		*/
		virtual void read(std::istream& s);

		/**	Write the values of of type {\em bool} to an ostream.
		*/
		virtual void write(std::ostream& s) const;

		/**	Storable write method.
		*/
		virtual void write(PersistenceManager& pm) const;

		/**	Storable read method.
		*/
		virtual bool read(PersistenceManager& pm);

		//@}

		protected:
	
		void validateIndex_(Index& index) const;

		void validateRange_(Index& first, Index& last) const;


		private:
	
		Index block_(Index index);

		BlockType mask_(Index index) const;

		// --- ATTRIBUTES

		Size 				size_;
		Size 				block_size_;
		BlockType* 	bitset_;
		bool 				resizable_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/bitVector.iC>
#	endif

} //namespace BALL


#endif // BALL_DATATYPE_BITVECTOR_H
