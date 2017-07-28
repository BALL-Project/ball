class Bit
{
%TypeHeaderCode
	#include <BALL/DATATYPE/bitVector.h>
%End

	public:
	Bit() throw();
	Bit(const Bit&) throw();
	~Bit() throw();
	virtual void clear() throw();
	bool operator == (const Bit&) const throw();
	bool operator == (bool) const throw(NullPointer);
	bool operator != (const Bit&) const throw();
	bool operator != (bool) const throw(NullPointer);
};

class BitVector
{
%TypeHeaderCode
	#include <BALL/DATATYPE/bitVector.h>
	using BlockType = unsigned char;
%End

	public:
	BitVector() throw();
	BitVector(Size) throw(OutOfMemory);
	BitVector(const BitVector&) throw(OutOfMemory);
	BitVector(const char*) throw(OutOfMemory);
	~BitVector() throw();
	void clear() throw();
	void set(const BitVector&) throw(OutOfMemory);
	void set(const char*) throw(OutOfMemory);
	void get(BitVector&) const throw(OutOfMemory);
	BitVector operator () (Index, Index) const
		throw(IndexUnderflow, IndexOverflow);
%MethodCode
	sipRes = new BitVector(sipCpp->operator() (*a0, *a1));
%End

	void setSize(Size, bool keep = true) throw(OutOfMemory);
	Size getSize() const throw();
	Size countValue(bool) const throw();
	bool operator [] (Index) const throw(IndexUnderflow, IndexOverflow);
%MethodCode
	sipRes = sipCpp->operator[](*a0);
%End

	void setBit(Index, bool value = true) throw(IndexUnderflow, OutOfMemory);
	bool getBit(Index) const throw(IndexUnderflow, IndexOverflow);
	void toggleBit(Index) throw(IndexUnderflow, OutOfMemory);
	void fill(bool value = true, Index first = 0 , Index last = -1)
		throw(IndexUnderflow, OutOfMemory);
	void toggle(Index first = 0, Index last = -1) throw(IndexUnderflow, OutOfMemory);
	void setUnsignedChar(unsigned char) throw();
	unsigned char getUnsignedChar() const throw();
	void setUnsignedShort(unsigned short) throw();
	unsigned short getUnsignedShort() const throw();
	void setUnsignedLong(unsigned long) throw();
	unsigned long getUnsignedLong() const throw();
	void bitwiseOr(const BitVector&) throw(OutOfMemory);
	void bitwiseXor(const BitVector&) throw(OutOfMemory);
	void bitwiseAnd(const BitVector&) throw(OutOfMemory);
	BitVector operator | (const BitVector&) throw(OutOfMemory);
	BitVector& operator |= (const BitVector&) throw(OutOfMemory);
	BitVector operator & (const BitVector&) throw(OutOfMemory);
	BitVector& operator &= (const BitVector&) throw(OutOfMemory);
	BitVector operator ^ (const BitVector&) throw(OutOfMemory);
	BitVector& operator ^= (const BitVector&) throw(OutOfMemory);
	bool operator == (const BitVector&) const throw();
	bool operator != (const BitVector&) const throw();
	bool isAnyBit(bool, Index first = 0, Index last = -1) const
		throw(IndexUnderflow, IndexOverflow);
	bool isEveryBit(bool, Index first = 0, Index last = -1) const
		throw(IndexUnderflow, IndexOverflow);
	bool isValid() const throw();
};
