// $Id: bitVector.C,v 1.22 2000/11/27 16:21:18 amoll Exp $

#include <BALL/DATATYPE/bitVector.h>
#include <BALL/MATHS/common.h>

#include <algorithm>

using namespace std;

namespace BALL 
{

	const Size BitVector::BlockSize = BALL_BLOCK_BITS;

	Bit::IllegalOperation::IllegalOperation(const char* file, int line)
		:	GeneralException(file, line)
	{
		message_ = "Trying to modifiy a const bitvector by a bit";
		Exception::globalHandler.setMessage(message_);
	}

	BitVector::BitVector()
		throw()
		:	size_(0),
			block_size_(BALL_BLOCK_SIZE(BlockSize))
	{
		bitset_ = new BlockType[block_size_];

		if (bitset_ == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, size_);
		}

		memset(bitset_, BALL_BLOCK_ALL_BITS_CLEARED, block_size_ << (sizeof(BlockType) - 1));
	}

	BitVector::BitVector(Size size)
		throw(Exception::OutOfMemory)
		:	size_(size),
			block_size_(BALL_BLOCK_SIZE(size))
	{				
		bitset_ = new BlockType[block_size_];

		if (bitset_ == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, size_);
		}

		memset(bitset_, BALL_BLOCK_ALL_BITS_CLEARED, block_size_ << (sizeof(BlockType) - 1));
	}

	BitVector::BitVector(const BitVector& bit_vector, bool /* deep */)
		throw(Exception::OutOfMemory)
		:	size_(bit_vector.size_),
			block_size_(bit_vector.block_size_)
	{
		bitset_ = new BlockType[block_size_];

		if (bitset_ == 0)
		{
			throw Exception::OutOfMemory(__FILE__, __LINE__, size_);
		}

		memcpy(bitset_, bit_vector.bitset_, block_size_ << (sizeof(BlockType) - 1));
	}

	BitVector::BitVector(const char* bit_string)
		throw(Exception::OutOfMemory)
		: size_(BALL_BLOCK_BITS),
			block_size_(BALL_BLOCK_SIZE(BALL_BLOCK_BITS)),
			bitset_(new BlockType[1])
	{
		set(bit_string);
	}

	BitVector::~BitVector()
		throw()
	{
		delete[] bitset_;
		bitset_ = 0;
	}

	void BitVector::set(const BitVector& bit_vector, bool /* deep */)
  	throw(Exception::OutOfMemory)
	{
		if (block_size_ != bit_vector.block_size_) 
		{
			delete[] bitset_;
			bitset_ = new BlockType[bit_vector.block_size_];

			if (bitset_ == 0)
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__, bit_vector.block_size_);
			}

			block_size_ = bit_vector.block_size_;
		}

		memcpy(bitset_, bit_vector.bitset_, block_size_ << (sizeof(BlockType) - 1));
		size_				= bit_vector.size_;
	}

	void BitVector::swap(BitVector& bit_vector)
		throw()
	{
		BALL::Size temp				 = size_;
		BALL::Size temp_block	 = block_size_;
		BlockType* temp_bitset = bitset_;

		size_				= bit_vector.size_;
		block_size_ = bit_vector.block_size_;
		bitset_			= bit_vector.bitset_;

		bit_vector.size_				= temp;
		bit_vector.block_size_  = temp_block;
		bit_vector.bitset_			= temp_bitset;
	}

	void BitVector::validateRange_(Index& first, Index& last) const
	 throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		if (last < 0)
		{
			last += size_;	
		}

		if (first < 0)
		{
			first += size_;
		}
		
		// if the values are out of bounds - throw an exception
		// and leave it...
		if (last < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, last, size_);
		}
		if (first < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__, first, size_);
		}
		if ((Size)last >= size_)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, last, size_);
		}
		if ((Size)first >= size_)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, first, size_);
		}
		
		// now swap, if last < first
		if (last >= first) 
		{
			return;
		}
		Index tmp = last;
		last = first;
		first = tmp;
	}

	void BitVector::validateIndex_(Index& index)
	 throw(Exception::IndexUnderflow, Exception::OutOfMemory)
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		if (index < 0)
		{
			index += size_;
		}

		// if the values are out of bounds - throw an exception
		// leave it...
		if (index < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}

		if ((Size)index >= size_)
		{
			setSize(index + 1);
		}
	}

	void BitVector::validateIndex_(Index& index) const
	 throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		// indices may be given as negative arguments: start from the end
		// -1 therefore means the last bit.
		if (index < 0)
		{
			index += size_;
		}

		// if the values are out of bounds - throw an exception
		// leave it...
		if (index < 0)
		{
			throw Exception::IndexUnderflow(__FILE__, __LINE__);
		}

		if ((Size)index >= size_)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__);
		}
	}

	BitVector BitVector::operator ()(Index first, Index last) const
	 throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		using std::min;

		validateRange_(first, last);

		BitVector temp(last - first + 1);
		Index source = first;
		Index target = 0;
		Position end = min((Position)last, size_ - 1);
		
		for (; (Position)source <= end; source++, target++)
		{
			temp.setBit(target, getBit(source));
		}

		return temp;
	}

	BALL::Size BitVector::countValue(bool value) const
		throw()
	{
		Size size = 0;

		for (Position index = 0; index < size_; index++)
		{
			if (getBit((Index)index) == value)
			{
				size++;
			}
		}

		return size;
	}

	void BitVector::set(const char* bit_string)
	 throw(Exception::OutOfMemory)
	{
		if (bit_string == 0)
		{
			return;
		}

		const char* tmp = bit_string;
		setSize(strlen(bit_string));
		for (Size i = 0; i < size_ ; i++)
		{
			setBit((Index)i, (*tmp != '0'));
			tmp++;
		}
	}

	void BitVector::fill(bool bit, Index first, Index last)
	 throw(Exception::IndexUnderflow, Exception::OutOfMemory)
	{
		if (size_ == 0)
		{
			return;
		}
		validateRange_(first, last);
		for (Index i = first; i <= last; i++) 
		{
			setBit(i, bit);
		}
	}

	void BitVector::toggle(Index first, Index last)
	 throw(Exception::IndexUnderflow, Exception::OutOfMemory)
	{
		validateRange_(first, last);
		for (Index i = first; i <= last; i++)
		{
			toggleBit(i);
		}
	}

	void BitVector::setUnsignedChar(unsigned char bit_pattern)
		throw()
	{
		setSize(BALL_CHAR_BITS, false);
		unsigned char c = bit_pattern;

		// We do this in a loop instead of using a direct cast to avoid
		// problems with differing byte orders (big endian/little endian)
		for (Position i = 0; i < BALL_CHAR_BITS; i++)
		{
			setBit((Index)i, (((int)c & (int)0x1) == 1));
			c = c >> 1;
		}
	}

	unsigned char BitVector::getUnsignedChar() const
		throw()
	{
		unsigned char c = 0;

		// We do this in a loop instead of using a direct cast to avoid
		// problems with differing byte orders (big endian/little endian)
		for (Index i = (Index)Maths::min((Size)BALL_CHAR_BITS, getSize()) - 1; i >= 0; i--)
		{
			c = c << 1;
			if (getBit((Index)i) == true)
			{
				c |= 1;
			}
		}

		return c;
	}

	void BitVector::setUnsignedShort(unsigned short bit_pattern)
		throw()
	{
		setSize(sizeof(unsigned short) * BALL_CHAR_BITS, false);
		unsigned short c = bit_pattern;

		for (Position i = 0; i < sizeof(unsigned short) * BALL_CHAR_BITS; i++)
		{
			setBit((Index)i, (((int)c & (int)0x1) == 1));
			c = c >> 1;
		}
	}

	unsigned short BitVector::getUnsignedShort() const
		throw()
	{
		unsigned short c = 0;
		Index i = (Index)Maths::min((Size)(sizeof(unsigned short) * BALL_CHAR_BITS), getSize()) - 1;
		for (; i >= 0; i--)
		{
			c = c << 1;
			if (getBit((Index)i) == true)
			{
				c |= 1;
			}
		}

		return c;
	}

	void BitVector::setUnsignedInt(unsigned int bit_pattern)
		throw()
	{
		setSize(sizeof(unsigned int) * BALL_CHAR_BITS, false);
		unsigned int c = bit_pattern;

		for (Position i = 0; i < sizeof(unsigned int) * BALL_CHAR_BITS; i++)
		{
			setBit((Index)i, (((int)c & (int)0x1) == 1));
			c = c >> 1;
		}
	}

	unsigned int BitVector::getUnsignedInt() const
		throw()
	{
		unsigned int c = 0;

		Index i = (Index)Maths::min((Size)(sizeof(unsigned int) * BALL_CHAR_BITS), getSize()) - 1;
		for (; i >= 0; i--)
		{
			c = c << 1;
			if (getBit((Index)i) == true)
			{
				c |= 1;
			}
		}

		return c;
	}

	void BitVector::setUnsignedLong(unsigned long bit_pattern)
		throw()
	{
		setSize(sizeof(unsigned long) * BALL_CHAR_BITS, false);
		unsigned long c = bit_pattern;

		for (Position i = 0; i < sizeof(unsigned long) * BALL_CHAR_BITS; i++)
		{
			setBit((Index)i, (((int)c & (int)0x1) == 1));
			c = c >> 1;
		}
	}

	unsigned long BitVector::getUnsignedLong() const
		throw()
	{
		unsigned long c = 0;

		Index i = (Index)Maths::min((Size)(sizeof(unsigned long) * BALL_CHAR_BITS), getSize()) - 1;
		for (; i >= 0; i--)
		{
			c = c << 1;
			if (getBit((Index)i) == true)
			{
				c |= 1;
			}
		}

		return c;
	}

	void BitVector::bitwiseXor(const BitVector& bit_vector)
		throw(Exception::OutOfMemory)
	{
		BALL::Size block_size = block_size_;

		if (block_size_ < bit_vector.block_size_)
		{
			setSize(bit_vector.size_, true);
			block_size = block_size_;
		} 
		else 
		{
			if (block_size_ > bit_vector.block_size_)
			{
				block_size = bit_vector.block_size_;
			}
		}

		BlockType *source = bit_vector.bitset_;
		BlockType *target = bitset_;

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

	void BitVector::bitwiseOr(const BitVector& bit_vector)
		throw(Exception::OutOfMemory)
	{
		BALL::Size block_size = block_size_;

		if (block_size_ < bit_vector.block_size_)
		{
			setSize(bit_vector.size_, true);
			block_size = block_size_;
		}
		else 
		{
			if (block_size_ > bit_vector.block_size_)
			{
				block_size = bit_vector.block_size_;
			}
		}

		BlockType *source = bit_vector.bitset_;
		BlockType *target = bitset_;

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

	void BitVector::bitwiseAnd(const BitVector& bit_vector)
		throw(Exception::OutOfMemory)
	{
		BALL::Size block_size = block_size_;

		if (block_size_ < bit_vector.block_size_)
		{
			setSize(bit_vector.size_, true);
			block_size = block_size_;
		}
		else 
		{
			if (block_size_ > bit_vector.block_size_)
			{
				block_size = bit_vector.block_size_;
			}
		}

		BlockType *source = bit_vector.bitset_;
		BlockType *target = bitset_;

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
		throw()
	{
		if (size_ != bit_vector.size_)
		{
			return false;
		}

		for (Index i = 0; i < (Index)size_; i++) 
		{
			if ((*this)[i] != bit_vector[i])
			{
				return false;
			}
		}
	 
		return true;
	}

	bool BitVector::isAnyBit(bool bit, Index first, Index last) const
	 throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateRange_(first, last);

		Index	i;
		for (i = first; i <= last; i++)
		{
			if ((*this)[i] == bit)
			{
				return true;
			}
		}
	 
		return false;
	}

	bool BitVector::isEveryBit(bool bit, Index first, Index last) const
	 throw(Exception::IndexUnderflow, Exception::IndexOverflow)
	{
		validateRange_(first, last);

		Index	i;
		for (i = first; i <= last ; i++)
		{
			if ((*this)[i] != bit)
			{
				return false;
			}
		}

		return true;
	}

	istream& operator >> (istream& s, BitVector& bit_vector)
		throw(Exception::OutOfMemory)
	{
		bit_vector.read(s);
		return s;
	}

	ostream& operator << (ostream& s, const BitVector& bit_vector)
		throw()
	{
		bit_vector.write(s);
		return s;
	}

	void BitVector::read(istream& s)
		throw(Exception::OutOfMemory)
	{
		Size size = 0;
		s >> size;
		setSize(size, false);
		size--;

		char c;
		s.get(c); // read leading blank
		for (Index i = (Index)size; i >= 0; i--) 
		{
			s.get(c);

			if (c != '0') 
			{
				setBit(i);
			}
		}
	}

	void BitVector::write(ostream& s) const
		throw()
	{
		s << getSize() << ' ';

		for (Index i = (Index)size_ - 1; i >= 0; i--) 
		{
			if (getBit(i) == true)
			{
				s << '1';
			}
			else 
			{
				s << '0';
			}
		}

		s << ' ';
	}

	bool BitVector::read(PersistenceManager& pm)
	  throw(Exception::OutOfMemory)
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
		throw()
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
	 throw(Exception::IndexUnderflow, Exception::OutOfMemory)
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

				if (bitset == 0)
				{
					throw Exception::OutOfMemory(__FILE__, __LINE__, index);
				}

				memset(bitset + block_size_, BALL_BLOCK_ALL_BITS_CLEARED, 
							 (block_size - block_size_) << (sizeof(BlockType) - 1));

				memcpy(bitset, bitset_, 
							 block_size_ << (sizeof(BlockType) - 1));

				delete[] bitset_;

				bitset_			= bitset;
				block_size_ = block_size;
			}
		
			size_ = index + 1; 
		}

		return (index >> BALL_BLOCK_SHIFT);
	}

	void BitVector::setSize(Size size, bool keep) 
	 throw(Exception::OutOfMemory)
	{
		// calculate new block size
		Size	block_size		 = (Size)((size  + BALL_BLOCK_MASK) >> BALL_BLOCK_SHIFT);
		Size	old_block_size = (Size)((size_ + BALL_BLOCK_MASK) >> BALL_BLOCK_SHIFT);

		if (keep) 
		{
			if (block_size == block_size_) 
			{
				size_ = size;
				return;
			} 

			// allocate a new Block
			BlockType*	tmp = new BlockType[block_size];

			if (tmp == 0)
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__, size);
			}

			memset(tmp, BALL_BLOCK_ALL_BITS_CLEARED, block_size << (sizeof(BlockType) - 1));
			memcpy(tmp, bitset_, old_block_size << (sizeof(BlockType) - 1));
			
			delete[] bitset_;	
			bitset_ = tmp;
		} 
		else 
		{
			delete[] bitset_;
			bitset_ = new BlockType[block_size];

			if (bitset_ == 0)
			{
				throw Exception::OutOfMemory(__FILE__, __LINE__, size);
			}

			memset(bitset_, BALL_BLOCK_ALL_BITS_CLEARED,
						 block_size << (sizeof(BlockType) - 1));
		}

		block_size_ = block_size;
		size_ = size;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/DATATYPE/bitVector.iC>
#	endif

} // namespace BALL
