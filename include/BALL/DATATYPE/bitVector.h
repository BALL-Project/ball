// $Id: bitVector.h,v 1.1 1999/08/26 07:53:13 oliver Exp $

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


namespace BALL {

	class BitVector;

	/**	
	*/
	class Bit
	{
		public:

			// --- CONSTRUCTORS

			Bit(const BitVector& bitvector, Index index = 0);

			// --- CONVERTERS

			operator bool(void) const;

			// --- ASSIGNMENT METHODS

			Bit& operator = (const Bit& bit);

			Bit& operator = (bool );

			// --- PREDICATES

			bool operator == (const Bit& bit) const;

			bool operator == (bool bit) const;

			bool operator != (const Bit& bit) const;

			bool operator != (bool bit) const;

		protected:

		private:

			Bit(void);

			// --- ATTRIBUTES

			BitVector* 	bitvector_;
			Index 			index_;
	};


	/**	Bit vector class 
	*/
	class BitVector
	{
		public:

		BALL_CREATE(BitVector)


		/**	@name	Type Definitions
		*/
		//@{

		///
		typedef unsigned char BlockType;
		//@}

	
		/**	@name	Constructors and Destructors */
		//@{

		///
		BitVector(Size size = BALL_BLOCK_BITS);

		///
		BitVector(const BitVector& bit_vector, bool deep = true);

		///
		BitVector(const char *bit_string);

		///
		virtual ~BitVector(void);

		///
		void clear(void);
		//@}


		/**	@name	Assignment */
		//@{

		///
		void set(const BitVector& bit_vector, bool deep = true);

		///
		void set(const char *bit_string);

		///
		BitVector& operator = (const BitVector& bit_vector);

		///
		BitVector& operator = (const char *bit_string);

		///
		void get(BitVector& bitvector, bool deep = true) const;

		///
		void swap(BitVector& bitvector);
		//@}

		/**	@name	Accessors */
		//@{

		///
		BitVector operator () (Index first,Index last) const;

		///
		void setSize(Size size, bool keep = true);

		///
		Size getSize() const;

		///
		Size countValue(bool value) const;

		///
		Index getMaxIndex(void) const;
	
		///
		BlockType* getBitSet(void);

		///
		const BlockType* getBitSet(void) const;

		///
		Bit operator []	(Index index)
		{
			block_(index);
	
			return Bit(*this, index);
		}
	
		///
		bool operator [] (Index index) const
		{
			validateIndex_(index);

			return ((BitVector *)this)->getBit(index);
		}

		///
		const char *get(char *bit_string_buffer, Size buffer_size, Index first = 0, Index last = -1) const;

		///
		void setBit(Index index, bool value = true)
		{
			validateIndex_(index);

			if (value == true)
			{
				bitset_[block_(index)] |= mask_(index);
			} else {
				bitset_[block_(index)] &= ~mask_(index);
			}
		}

		///
		bool getBit(Index index) const
		{
			validateIndex_(index);

			BitVector* const	const_this = const_cast<BitVector *>(this);

			return const_this->getBit(index);
		}

		///
		bool getBit(Index index)
		{
			return (bool)((bitset_[block_(index)] & mask_(index)) != 0);
		}

		///
		void toggleBit(Index index)
		{
			validateIndex_(index);

			bitset_[block_(index)] ^= mask_(index);
		}

		///
		void fill(bool value = true, Index first = 0 , Index last = -1);

		///
		void toggle(Index first = 0, Index last = -1);

		///
		void setUnsignedChar(unsigned char bit_pattern);

		///
		unsigned char getUnsignedChar(void) const;

		///
		void setUnsignedShort(unsigned short bit_pattern);

		///
		unsigned short getUnsignedShort(void) const;

		///
		void setUnsignedInt(unsigned int bit_pattern);

		///
		unsigned int getUnsignedInt(void) const;

		///
		void setUnsignedLong(unsigned long bit_pattern);

		///
		unsigned long getUnsignedLong(void) const;

		///
		void or(const BitVector& bit_vector);

		///
		void xor(const BitVector& bit_vector);

		///
		void and(const BitVector& bit_vector);

		///
		BitVector operator | (const BitVector& bit_vector);

		///
		BitVector &operator |= (const BitVector& bit_vector);

		///
		BitVector operator & (const BitVector& bit_vector);

		///
		BitVector &operator &= (const BitVector& bit_vector);

		///
		BitVector operator ^(const BitVector& bit_vector);

		///
		BitVector &operator ^= (const BitVector& bit_vector);

		///
		BitVector operator ~(void);
		//@}

		/**	@name	Predicates */
		//@{

		///
		bool operator ==(const BitVector& bit_vector) const;

		///
		bool operator != (const BitVector& bit_vector) const;

		///
		bool isAnyBit(bool value, Index first = 0, Index last = -1) const;

		///
		bool isEveryBit(bool value, Index first = 0, Index last = -1) const;

		//@}


		/**	@name	Debugging and Diagnostics */
		//@{

		///
		bool isValid(void) const;
		//@}


		/**	@name	Storers */
		//@{

		///
		friend istream& operator >> (istream& s, BitVector& bit_vector);

		///
		friend ostream& operator << (ostream& s, const BitVector& bit_vector);

		///
		virtual void read(istream& s);

		///
		virtual void write(ostream& s) const;

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

		BlockType mask_(Index index) const
		{
			return (1 << (index & BALL_BLOCK_MASK));
		}

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
