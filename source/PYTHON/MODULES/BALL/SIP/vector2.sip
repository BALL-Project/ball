// $Id: vector2.sip,v 1.2 2001/08/01 01:28:39 oliver Exp $

//Vector2 operator * (float scalar, const Vector2& vector)
//  throw();
//        
//std::istream& operator >> (std::istream& s, Vector2& vector)
//  throw();
//        
//std::ostream& operator << (std::ostream& s, const Vector2& vector)
//  throw();
        
class Vector2
	:  PersistentObject
{
%HeaderCode
        #include <BALL/MATHS/vector2.h>
        using namespace BALL;
        using namespace BALL::Exception;
%End
	public:
	//BALL_CREATE(Vector2)

	Vector2() throw();
	Vector2(float /value/)
		throw();
	Vector2(float /vx/, float /vy/)
		throw();
	Vector2(const Vector2& /vector/)
		throw();
	~Vector2()
		throw();
	virtual void clear()
		throw();

	//virtual void persistentWrite
	//	(PersistenceManager& /pm/, const char* /name/ = 0) const
	//	throw();

	//virtual void persistentRead(PersistenceManager& /pm/)
	//	throw();

	void set(float /value/)
		throw();
	void set(float /vx/, float /vy/)
		throw();
	void set(const Vector2& /vector/)
		throw();
	//const Vector2& operator = (const Vector2& /v/)
	//	throw();
	//const Vector2& operator = (float /value/)
	//	throw();
	//const Vector2& operator = (const T* ptr)
	//				throw(NullPointer);
	float getLength() const
		throw();
	float getSquareLength() const
		throw();
	Vector2& normalize()
		throw(DivisionByZero);
	Vector2& negate()
		throw();
	static const Vector2& getZero()
		throw();
	static const Vector2& getUnit()
		throw();
	// float& operator [] (Position /position/)
	//	throw(IndexOverflow);
	float operator [] (Position /position/) const
		throw(IndexOverflow);

	const Vector2& operator + () const
		throw();
	Vector2 operator - () const
		throw();
	Vector2 operator + (const Vector2& /vector/) const
		throw();
	Vector2 operator - (const Vector2& /vector/) const
		throw();
	Vector2& operator += (const Vector2& /vector/)
		throw();
	Vector2& operator -= (const Vector2& /vector/)
		throw();
	Vector2 operator * (float /scalar/) const
		throw();
	Vector2& operator *= (float /scalar/)
		throw();
	Vector2 operator / (float /lambda/) const
		throw(DivisionByZero);
	Vector2& operator /= (float /lambda/)
		throw(DivisionByZero);
	float operator * (const Vector2& /vector/) const
		throw();

	float getDistance(const Vector2& /vector/) const
		throw();
	float getSquareDistance(const Vector2& /vector/) const
		throw();

	bool operator == (const Vector2& /vector/) const
		throw();
	//bool operator != (const Vector2& /vector/) const
	//	throw();
	bool isZero() const
		throw();
	bool isOrthogonalTo(Vector2& /vector/) const
		throw();

	//void dump(std::ostream& s = std::cout, Size depth = 0) const
	//				throw();
	bool isValid() const
		throw();

	float x;
	float y;

	PyMethods:
  __str__
%MemberCode
  Vector2* ptr;
  if ((ptr = (Vector2*)sipGetCppPtr((sipThisType*)a0,sipClass_Vector2)) == NULL)
    return NULL;
 
  static String tmp;
  tmp = "(";
  tmp += String(ptr->x) + " ";
  tmp += String(ptr->y) + ")";
  return PyString_FromString(tmp.c_str());
%End};
