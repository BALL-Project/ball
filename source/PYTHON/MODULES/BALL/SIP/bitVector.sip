class Bit
{
%HeaderCode
	#include <BALL/DATATYPE/bitVector.h>
	using namespace BALL;
%End
  public:
  Bit(const BitVector& /bitvector/, Index /index/ = 0);
  // operator bool() const;
  // Bit& operator = (const Bit& /bit/);
  // Bit& operator = (bool /bit/);
  // bool operator == (const Bit& /bit/) const;
  // bool operator == (bool /bit/) const;
  // bool operator != (const Bit& /bit/) const;
  // bool operator != (bool /bit/) const;
};

class BitVector
{
%HeaderCode
	#include <BALL/DATATYPE/bitVector.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(BitVector)
 	// typedef unsigned char BlockType;
  static const Size BlockSize;
  BitVector();
  BitVector(Size /size/);
  BitVector(const BitVector& /bit_vector/, bool /deep/ = true);
  BitVector(const char* /bit_string/);
  // virtual ~BitVector();
  void clear();
  void set(const BitVector& /bit_vector/, bool /deep/ = true);
  void set(const char* /bit_string/);
  // BitVector& operator = (const BitVector& bit_vector);
  // BitVector& operator = (const char *bit_string);
  void get(BitVector& /bitvector/, bool /deep/ = true) const;
  void swap(BitVector& /bitvector/);
  // BitVector operator () (Index /first/,Index /last/) const;
  void setSize(Size /size/, bool /keep/ = true);
  Size getSize() const;
  Size countValue(bool /value/) const;
  Index getMaxIndex() const;
  // BlockType* getBitSet();
  // const BlockType* getBitSet() const;
  // Bit operator [] (Index /index/);
  // bool operator [] (Index /index/) const;
  void setBit(Index /index/, bool /value/ = true);
  bool getBit(Index /index/) const;
  bool getBit(Index /index/);
  void toggleBit(Index /index/);
  void fill(bool /value/ = true, Index /first/ = 0 , Index /last/ = -1);
  void toggle(Index /first/ = 0, Index /last/ = -1);
  // void setUnsignedChar(unsigned char /bit_pattern/);
  // unsigned char getUnsignedChar() const;
  // void setUnsignedShort(unsigned short /bit_pattern/);
  // unsigned short getUnsignedShort() const;
  // void setUnsignedInt(/unsigned/ int /bit_pattern/);
  // unsigned int getUnsignedInt() const;
  // void setUnsignedLong(/unsigned/ long /bit_pattern/);
  // unsigned long getUnsignedLong() const;
  void bitwiseOr(const BitVector& /bit_vector/);
  void bitwiseXor(const BitVector& /bit_vector/);
  void bitwiseAnd(const BitVector& /bit_vector/);
  // BitVector operator | (const BitVector& bit_vector);
  // BitVector& operator |= (const BitVector& bit_vector);
  // BitVector operator & (const BitVector& bit_vector);
  // BitVector& operator &= (const BitVector& bit_vector);
  // BitVector operator ^ (const BitVector& bit_vector);
  // BitVector& operator ^= (const BitVector& bit_vector);
  // BitVector operator ~ ();
  // bool operator == (const BitVector& bit_vector) const;
  // bool operator != (const BitVector& bit_vector) const;
  bool isAnyBit(bool /value/, Index /first/ = 0, Index /last/ = -1) const;
  bool isEveryBit(bool /value/, Index /first/ = 0, Index /last/ = -1) const;
  bool isValid() const;
  // friend std::istream& operator >> (std::istream& s, BitVector& bit_vector);
  // friend std::ostream& operator << (std::ostream& s, const BitVector& bit_vector);
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
  // virtual void write(PersistenceManager& pm) const;
  // virtual bool read(PersistenceManager& pm);
};
