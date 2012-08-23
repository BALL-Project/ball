// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
class Bit
{
%TypeHeaderCode
	#include <BALL/DATATYPE/bitVector.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  Bit() throw();
  Bit(const Bit&) throw();
	// BAUSTELLE: folgende Funktionen scheinen für sip die gleich Signatur zu
	// haben:
  // Bit(BitVector*, Index index = 0) 
  // throw(NullPointer);
  // Bit(const BitVector*, Index index = 0) 
  // throw(NullPointer, IndexUnderflow, IndexOverflow);
  ~Bit() throw();
  // operator bool() const throw(NullPointer);
  // const Bit& operator = (const Bit& bit) throw();
  // const Bit& operator = (bool bit) 
  //   throw(NullPointer, IllegalOperation);
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
	using namespace BALL;
	using namespace BALL::Exception;
	typedef unsigned char BlockType;
%End
	public:
	BitVector() throw();
	BitVector(Size) throw(OutOfMemory);
	BitVector(const BitVector&) 
		throw(OutOfMemory);
	BitVector(const char*)
		throw(OutOfMemory);
	~BitVector() throw();
	void clear() throw();
	void set(const BitVector&)
		throw(OutOfMemory);
	void set(const char*)
		throw(OutOfMemory);
	// BitVector& operator = (const BitVector&)
	//	throw(OutOfMemory);
	// const BitVector& operator = (const char *bit_string)
	// 	throw(OutOfMemory);
	void get(BitVector&) const throw(OutOfMemory);
	BitVector operator () (Index, Index) const	throw(IndexUnderflow, IndexOverflow);
%MethodCode
	sipRes = new BitVector(sipCpp->operator() (*a0, *a1));
%End
	void setSize(Size, bool keep = true) throw(OutOfMemory);
	Size getSize() const throw();
	Size countValue(bool) const throw();
	// BlockType* getBitSet() throw();
	// const BlockType* getBitSet() const throw();
	// Bit operator [] (Index)	throw(IndexUnderflow, OutOfMemory);
	bool operator [] (Index) const throw(IndexUnderflow, IndexOverflow);
%MethodCode
	sipRes = sipCpp->operator[](*a0);
%End
	void setBit(Index, bool value = true)
		throw(IndexUnderflow, OutOfMemory);
	bool getBit(Index) const
		throw(IndexUnderflow, IndexOverflow);
	void toggleBit(Index)
		throw(IndexUnderflow, OutOfMemory);
	void fill(bool value = true, Index first = 0 , Index last = -1)
		throw(IndexUnderflow, OutOfMemory);
	void toggle(Index first = 0, Index last = -1)
		throw(IndexUnderflow, OutOfMemory);
	void setUnsignedChar(unsigned char) throw();
	unsigned char getUnsignedChar() const throw();
	void setUnsignedShort(unsigned short) throw();
	unsigned short getUnsignedShort() const throw();
// BAUSTELLE: unsigned int scheint sip Probleme zu machen
//	void setUnsignedInt(unsigned int) throw();
//	unsigned int getUnsignedInt() const throw();
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
	// BitVector operator ~ () throw(OutOfMemory);
	bool operator == (const BitVector&) const throw();
	bool operator != (const BitVector&) const throw();
	bool isAnyBit(bool, Index first = 0, Index last = -1) const
		throw(IndexUnderflow, IndexOverflow);
	bool isEveryBit(bool, Index first = 0, Index last = -1) const
		throw(IndexUnderflow, IndexOverflow);
	bool isValid() const throw();
	// friend std::istream& operator >> (std::istream& s, BitVector&)
	//	throw(OutOfMemory);
	// friend std::ostream& operator << (std::ostream& s, const BitVector&)
	//	throw();
	// virtual void read(std::istream& s)
	// 	throw(OutOfMemory);
	// virtual void write(std::ostream& s) const throw();
	// virtual void write(PersistenceManager& pm) const throw();
	// virtual bool read(PersistenceManager& pm)
	//	throw(OutOfMemory);
};
