class Bit
{
%HeaderCode
	#include <BALL/DATATYPE/bitVector.h>
	using namespace BALL;
%End
  public:
  Bit() throw();
  Bit(const Bit& /bit/) throw();
	// BAUSTELLE: folgende Funktionen scheinen für sip die gleich Signatur zu
	// haben:
  // Bit(BitVector* /bitvector/, Index /index/ = 0) 
  // throw(NullPointer);
  // Bit(const BitVector* /bitvector/, Index /index/ = 0) 
  // throw(NullPointer, IndexUnderflow, IndexOverflow);
  ~Bit() throw();
  // operator bool() const throw(NullPointer);
  // const Bit& operator = (const Bit& bit) throw();
  // const Bit& operator = (bool bit) 
  //   throw(NullPointer, IllegalOperation);
  virtual void clear() throw();
  bool operator == (const Bit& /bit/) const throw();
  // bool operator == (bool bit) const throw(NullPointer);
	// bool operator != (const Bit& bit) const throw();
  // bool operator != (bool bit) const throw(NullPointer);
};

class BitVector
{
%HeaderCode
	#include <BALL/DATATYPE/bitVector.h>
	using namespace BALL;
	using namespace BALL::Exception;
	typedef unsigned char BlockType;
%End
	public:
	BitVector() throw();
	BitVector(Size /size/) throw(OutOfMemory);
	BitVector(const BitVector& /bit_vector/) 
		throw(OutOfMemory);
	BitVector(const char* /bit_string/)
		throw(OutOfMemory);
	~BitVector() throw();
	void clear() throw();
	void set(const BitVector& /bit_vector/)
		throw(OutOfMemory);
	void set(const char* /bit_string/)
		throw(OutOfMemory);
	// BitVector& operator = (const BitVector& /bit_vector/)
	//	throw(OutOfMemory);
	// const BitVector& operator = (const char *bit_string)
	// 	throw(OutOfMemory);
	void get(BitVector& /bitvector/) const
		throw(OutOfMemory);
	BitVector operator () (Index /first/, Index /last/) const
		throw(IndexUnderflow, IndexOverflow);
	void setSize(Size /size/, bool /keep/ = true)
		throw(OutOfMemory);
	Size getSize() const throw();
	Size countValue(bool /value/) const throw();
	// BlockType* getBitSet() throw();
	// const BlockType* getBitSet() const throw();
	Bit operator [] (Index /index/)
		throw(IndexUnderflow, OutOfMemory);
	bool operator [] (Index /index/) const
		throw(IndexUnderflow, IndexOverflow);
	void setBit(Index /index/, bool /value/ = true)
		throw(IndexUnderflow, OutOfMemory);
	bool getBit(Index /index/)
		throw(IndexUnderflow, OutOfMemory);
	bool getBit(Index /index/) const
		throw(IndexUnderflow, IndexOverflow);
	void toggleBit(Index /index/)
		throw(IndexUnderflow, OutOfMemory);
	void fill(bool /value/ = true, Index /first/ = 0 , Index /last/ = -1)
		throw(IndexUnderflow, OutOfMemory);
	void toggle(Index /first/ = 0, Index /last/ = -1)
		throw(IndexUnderflow, OutOfMemory);
	void setUnsignedChar(unsigned char /bit_pattern/) throw();
	unsigned char getUnsignedChar() const throw();
	void setUnsignedShort(unsigned short /bit_pattern/) throw();
	unsigned short getUnsignedShort() const throw();
// BAUSTELLE: unsigned int scheint sip Probleme zu machen
//	void setUnsignedInt(unsigned int /bit_pattern/) throw();
//	unsigned int getUnsignedInt() const throw();
	void setUnsignedLong(unsigned long /bit_pattern/) throw();
	unsigned long getUnsignedLong() const throw();
	void bitwiseOr(const BitVector& /bit_vector/) throw(OutOfMemory);
	void bitwiseXor(const BitVector& /bit_vector/) throw(OutOfMemory);
	void bitwiseAnd(const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector operator | (const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector& operator |= (const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector operator & (const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector& operator &= (const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector operator ^ (const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector& operator ^= (const BitVector& /bit_vector/) throw(OutOfMemory);
	BitVector operator ~ () throw(OutOfMemory);
	bool operator == (const BitVector& /bit_vector/) const throw();
	// bool operator != (const BitVector& /bit_vector/) const throw();
	bool isAnyBit(bool /value/, Index /first/ = 0, Index /last/ = -1) const
		throw(IndexUnderflow, IndexOverflow);
	bool isEveryBit(bool /value/, Index /first/ = 0, Index /last/ = -1) const
		throw(IndexUnderflow, IndexOverflow);
	bool isValid() const throw();
	// friend std::istream& operator >> (std::istream& s, BitVector& /bit_vector/)
	//	throw(OutOfMemory);
	// friend std::ostream& operator << (std::ostream& s, const BitVector& /bit_vector/)
	//	throw();
	// virtual void read(std::istream& s)
	// 	throw(OutOfMemory);
	// virtual void write(std::ostream& s) const throw();
	// virtual void write(PersistenceManager& pm) const throw();
	// virtual bool read(PersistenceManager& pm)
	//	throw(OutOfMemory);
};
