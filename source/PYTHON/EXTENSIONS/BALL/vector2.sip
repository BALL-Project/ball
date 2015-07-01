// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

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
%TypeHeaderCode
        #include <BALL/MATHS/vector2.h>
        using namespace BALL;
        using namespace BALL::Exception;
%End
	public:
	//BALL_CREATE(Vector2)

	Vector2() throw();
	Vector2(float) throw();
	Vector2(float, float) throw();
	Vector2(const Vector2&) throw();
	~Vector2() throw();
	virtual void clear() throw();

	//virtual void persistentWrite
	//	(PersistenceManager&, const char* name = 0) const
	//	throw();

	//virtual void persistentRead(PersistenceManager&)
	//	throw();

	void set(float) throw();
	void set(float, float) throw();
	void set(const Vector2&) throw();
	//const Vector2& operator = (const Vector2&)
	//	throw();
	//const Vector2& operator = (float)
	//	throw();
	//const Vector2& operator = (const T* ptr)
	//				throw(NullPointer);
	float getLength() const throw();
	float getSquareLength() const throw();
	Vector2& normalize()
		throw(DivisionByZero);
	Vector2& negate() throw();
	static const Vector2& getZero() throw();
	static const Vector2& getUnit() throw();
	// float& operator [] (Position)
	//	throw(IndexOverflow);
	float operator [] (Position) const throw(IndexOverflow);
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

	// ???? const Vector2& operator + () const throw();
	Vector2 operator - () const	throw();
	Vector2 operator + (const Vector2&) const throw();
	Vector2 operator - (const Vector2&) const throw();
	Vector2& operator += (const Vector2&) throw();
	Vector2& operator -= (const Vector2&) throw();
	Vector2 operator * (float) const throw();
	Vector2& operator *= (float) throw();
	Vector2 operator / (float) const	throw(DivisionByZero);
	Vector2& operator /= (float) throw(DivisionByZero);
	float operator * (const Vector2&) const throw();

	float getDistance(const Vector2&) const throw();
	float getSquareDistance(const Vector2&) const throw();

	bool operator == (const Vector2&) const throw();
	bool operator != (const Vector2&) const throw();
	bool isZero() const throw();
	bool isOrthogonalTo(Vector2&) const throw();

	//void dump(std::ostream& s = std::cout, Size depth = 0) const
	//				throw();
	bool isValid() const throw();

	float x;
	float y;

  SIP_PYOBJECT __str__();
%MethodCode
	static String tmp;
	tmp = "(";
	tmp += String(sipCpp->x) + " ";
	tmp += String(sipCpp->y) + ")";
	sipRes = PyString_FromString(tmp.c_str());
%End
};
