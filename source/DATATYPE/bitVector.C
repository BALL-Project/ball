// $Id: bitVector.C,v 1.3 1999/12/04 18:34:27 oliver Exp $

#include <BALL/DATATYPE/bitVector.h>

#include <algorithm>

namespace BALL {

	Bit::Bit(const BitVector& bit_vector, Index index)
		:	bitvector_((BitVector *)&bit_vector),
			index_(index)
	{
	}

	BitVector::BitVector(BALL::Size size)
		:	size_(size),
			block_size_(BALL_BLOCK_SIZE(size)),
			resizable_(true)
	{
		if (size <= 0) {
			size = 1;
			throw Exception::InvalidRange(__FILE__, __LINE__);
		}
				
		bitset_ = new BlockType[block_size_];

		memset
			(bitset_, 
			 BALL_BLOCK_ALL_BITS_CLEARED, 
			 block_size_ << (sizeof(BlockType) - 1));
	}

	BitVector::BitVector(const BitVector& bit_vector, bool /* deep */)
		:	size_(bit_vector.size_),
			block_size_(bit_vector.block_size_),
			resizable_(bit_vector.resizable_)
	{
		bitset_ = new BlockType[block_size_];

		memcpy
			(bitset_, 
			 bit_vector.bitset_, 
			 block_size_ << (sizeof(BlockType) - 1));
	}

	BitVector::BitVector(const char *bit_string)
		: size_(BALL_BLOCK_BITS),
			block_size_(BALL_BLOCK_SIZE(BALL_BLOCK_BITS)),
			bitset_(new BlockType[1]),
			resizable_(true)
	{
		set(bit_string);
	}

	BitVector::~BitVector()
	{
		delete [] bitset_;
	}


	void BitVector::set(const BitVector& bit_vector, bool /* deep */)
	{
		Size block_size = bit_vector.block_size_;

		if (block_size_ != block_size) 
		{
			delete [] bitset_;
			bitset_ = new BlockType[block_size];
			block_size_ = block_size;
		}

		memcpy(bitset_, bit_vector.bitset_, 
					 block_size << (sizeof(BlockType) - 1));

		size_ = bit_vector.size_;
		resizable_ = bit_vector.resizable_;
	}

	void BitVector::swap(BitVector& bit_vector)
	{
		BALL::Size temp = size_;
		BALL::Size temp_block = block_size_;
		BlockType *temp_bitset = bitset_;
		bool temp_resizable = resizable_;

		size_= bit_vector.size_;
		block_size_ = bit_vector.block_size_;
		bitset_ = bit_vector.bitset_;
		resizable_ = bit_vector.resizable_;

		bit_vector.size_ = temp;
		bit_vector.block_size_ = temp_block;
		bit_vector.bitset_ = temp_bitset;
		bit_vector.resizable_ = temp_resizable;
	}

	void BitVector::validateRange_(Index& first, Index& last) const
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		if (last < 0)
		{
			last = (Index)(size_ - last - 1);
		}
		if (first < 0)
		{
			first = (Index)(size_ - first - 1);
		}
		
		// if the values are out of bounds - throw an exception
		// and leave it...
		if (last < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, last, size_);
		if (first < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, first, size_);
		if ((Size)last > size_)
			throw Exception::IndexOverflow(__FILE__, __LINE__, last, size_);
		if ((Size)first > size_)
			throw Exception::IndexOverflow(__FILE__, __LINE__, first, size_);
		
		// now swap, if last < first
		Index tmp = last;
		last = first;
		first = tmp;
	}

	void BitVector::validateIndex_(Index& index) const
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		if (index < 0)
		{
			index = (Index)(size_ - index - 1);
		}

		// if the values are out of bounds - throw an exception
		// leave it...
		if (index < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}

		if ((Size)index > size_)
		{
			const_cast<BitVector*>(this)->setSize(index);
		}
	}

	BitVector BitVector::operator ()(Index first, Index last) const
	{
		using std::min;

		validateRange_(first, last);

		BitVector temp(last - first + 1);
		register Index source = first;
		register Index target = 0;
		Position end = min((Position)last, size_ - 1);
		
		for (; (Position)source <= end; source++, target++)
		{
			temp.setBit(target, getBit(source));
		}

		return temp;
	}


	BALL::Size BitVector::countValue(bool value) const
	{
		register Size size = 0;

		for (register Position index = 0; index < size_; index++)
		{
			if (getBit((Index)index) == value)
			{
				size++;
			}
		}

		return size;
	}

	void BitVector::set(const char *bit_string)
	{
		if (bit_string == 0)
			return;

		register const char *tmp = 0;
		register BALL::Size size;

		for (tmp = bit_string; *tmp != 0; tmp++);

		size = (BALL::Size)(tmp - bit_string);
		
		for (tmp--, size--; tmp >= bit_string; tmp--)
		{
			if (*tmp != '0')
			{
				setBit((Index)(size - (tmp - bit_string)));
			}
		}
	}

	void BitVector::fill(bool bit, Index first, Index last)
	{
		validateRange_(first, last);

		for (register Index i = first; i <= last; i++) {
			setBit(i, bit);
		}
	}

	void BitVector::toggle(Index first, Index last)
	{
		validateRange_(first, last);

		for (register Index i = first; i <= last; i++)
		{
			toggleBit(i);
		}
	}

	void BitVector::setUnsignedChar(unsigned char bit_pattern)
	{
		setSize(BALL_CHAR_BITS, false);

		*((unsigned char *)bitset_) = bit_pattern;
	}

	unsigned char BitVector::getUnsignedChar() const
	{
		return *((unsigned char *)bitset_);
	}

	void BitVector::setUnsignedShort(unsigned short bit_pattern)
	{
		setSize(sizeof(unsigned short) * BALL_CHAR_BITS, false);

		*((unsigned short *)bitset_) = bit_pattern;
	}

	unsigned short BitVector::getUnsignedShort() const
	{
		return *((unsigned short *)bitset_);
	}

	void BitVector::setUnsignedInt (unsigned int bit_pattern)
	{
		setSize(sizeof(unsigned int) * BALL_CHAR_BITS, false);

		*((unsigned int *)bitset_) = bit_pattern;
	}

	unsigned int BitVector::getUnsignedInt() const
	{
		return *((unsigned int *)bitset_);
	}

	void BitVector::setUnsignedLong(unsigned long bit_pattern)
	{
		setSize(sizeof(unsigned long) * BALL_CHAR_BITS, false);

		*((unsigned long *)bitset_) = bit_pattern;
	}

	unsigned long BitVector::getUnsignedLong() const
	{
		return *((unsigned long *)bitset_);
	}

	void BitVector::xor(const BitVector& bit_vector)
	{
		register BALL::Size block_size = block_size_;

		if (block_size_ < bit_vector.block_size_)
		{
			setSize(bit_vector.size_, true);
			
			block_size = block_size_;
		} 
		else if (block_size_ > bit_vector.block_size_)
		{
			block_size = bit_vector.block_size_;
		}

		register BlockType *source = bit_vector.bitset_;
		register BlockType *target = bitset_;

		for (; block_size >= 1; block_size--, target++, source++)
		{
			*target ^= *source;
		}

		if (size_ < bit_vector.size_)
		{
			size_ = bit_vector.size_;
			block_size_ = bit_vector.block_size_;
		}
	}


	void BitVector::or(const BitVector& bit_vector)
	{
		register BALL::Size block_size = block_size_;

		if (block_size_ < bit_vector.block_size_)
		{
			setSize(bit_vector.size_, true);
			
			block_size = block_size_;
		}
		else if (block_size_ > bit_vector.block_size_)
		{
			block_size = bit_vector.block_size_;
		}

		register BlockType *source = bit_vector.bitset_;
		register BlockType *target = bitset_;

		for (; block_size >= 1; block_size--, target++, source++)
		{
			*target |= *source;
		}

		if (size_ < bit_vector.size_)
		{
			size_ = bit_vector.size_;
			block_size_ = bit_vector.block_size_;
		}
	}


	void BitVector::and(const BitVector& bit_vector)
	{
		register BALL::Size block_size = block_size_;

		if (block_size_ < bit_vector.block_size_)
		{
			setSize(bit_vector.size_, true);
			
			block_size = block_size_;
		}
		else if (block_size_ > bit_vector.block_size_)
		{
			block_size = bit_vector.block_size_;
		}

		register BlockType *source = bit_vector.bitset_;
		register BlockType *target = bitset_;

		for (; block_size >= 1; block_size--, target++, source++)
		{
			*target &= *source;
		}

		if (size_ < bit_vector.size_)
		{
			size_ = bit_vector.size_;
			block_size_ = bit_vector.block_size_;
		}
	}


	bool BitVector::operator == (const BitVector& bit_vector) const
	{
		if (size_ != bit_vector.size_)
		{
			return false;
		}

		Index	i;
		for (i = 0; i < (Index)size_; i++) 
		{
			if ((*this)[i] != bit_vector[i])
			{
				return false;
			}
		}
	 
		return true;
	}

	bool BitVector::isAnyBit(bool bit, Index first, Index last) const
	{
		validateRange_(first, last);

		Index	i;
		for (i = first; i <= last; i++) 
			if ((*this)[i] == bit)
				return true;
	 
		return false;
	}

	bool BitVector::isEveryBit(bool bit, Index first, Index last) const
	{
		validateRange_(first, last);

		Index	i;
		for (i = first; i <= last ; i++)
			if ((*this)[i] != bit)
				return false;

		return true;
	}

	istream& operator >> (istream& s, BitVector& bit_vector)
	{
		bit_vector.read(s);

		return s;
	}

	ostream& operator << (ostream& s, const BitVector& bit_vector)
	{
		bit_vector.write(s);

		return s;
	}

	void BitVector::read(istream& s)
	{
		Size size = 0;
		
		s >> size;

		setSize(size, false);

		size--;


		char c;
		s.get(c); // read leading blank
		for (Index i = (Index)size ; i >= 0 ; i--) 
		{
			s.get(c);

			if (c != '0') 
			{
				setBit(i);
			}
		}
	}

	void BitVector::write(ostream& s) const
	{
		s << getSize() << ' ';

		for (Index i = (Index)size_ - 1; i >= 0; i--) {
			if (getBit(i) == true)
			{
				s << '1';
			} else {
				s << '0';
			}
		}

		s << ' ';
	}

	bool BitVector::read(PersistenceManager& pm)
	{
		Size size = 0;
		
		if (!pm.readPrimitive(size, "size"))
		{
			return false;
		}
			
		setSize(size, false);

		size--;

		bool bit;
		for (Index i = (Index)size; i >= 0 && pm.readPrimitive(bit, ""); i--) 
		{
			setBit(i, bit);
		}

		return true;
	}

	void BitVector::write(PersistenceManager& pm) const
	{
		Size size = getSize();
		pm.writePrimitive(size, "size");

		for (Index i = (Index)size_ - 1; i >= 0; i--) 
		{
			bool bit = getBit(i);
			pm.writePrimitive(bit, "");
		}
	}

	Index BitVector::block_(Index index)
	{
		if (index < 0)
		{
			index = (Index)size_ - index + 1;
		}

		if (index < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}
		
		if ((Size)index >= size_)
		{
			BALL::Size block_size = (BALL::Size)((index + BALL_BLOCK_BITS) >> BALL_BLOCK_SHIFT);

			if (block_size > block_size_)
			{
				BlockType* bitset = new BlockType[block_size]; 

				memset(bitset + block_size_, BALL_BLOCK_ALL_BITS_CLEARED, 
							 (block_size - block_size_) << (sizeof(BlockType) - 1));

				memcpy(bitset, bitset_, 
							 block_size_ << (sizeof(BlockType) - 1));

				delete [] bitset_;

				bitset_ = bitset;
				block_size_ = block_size;
			}
		
			size_ = index + 1; 
		}

		return (index >> BALL_BLOCK_SHIFT);
	}

	void BitVector::setSize(Size size, bool keep) 
	{
		// calculate new block size
		Size	block_size			= (Size)((size + BALL_BLOCK_MASK) >> BALL_BLOCK_SHIFT);
		Size	old_block_size	= (Size)((size_ + BALL_BLOCK_MASK) >> BALL_BLOCK_SHIFT);

		if (keep) 
		{
			if (block_size == block_size_) 
			{
				size_ = size;
				return;
			} 

			// allocate a new Block
			BlockType*	tmp = new BlockType[block_size];

			memset(tmp, BALL_BLOCK_ALL_BITS_CLEARED, block_size << (sizeof(BlockType) - 1));
			memcpy(tmp, bitset_, old_block_size << (sizeof(BlockType) - 1));
			
			delete [] bitset_;	
			bitset_ = tmp;

		} else {

			delete [] bitset_;

			bitset_ = new BlockType[block_size];

			memset(bitset_, BALL_BLOCK_ALL_BITS_CLEARED,
						 block_size << (sizeof(BlockType) - 1));
		}
	}
 

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/bitVector.iC>
#	endif

} // namespace BALL
