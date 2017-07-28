class Vector2
	: PersistentObject
{
%TypeHeaderCode
	#include <BALL/MATHS/vector2.h>
%End
	public:

	Vector2() throw();
	Vector2(float) throw();
	Vector2(float, float) throw();
	Vector2(const Vector2&) throw();
	~Vector2() throw();

	virtual void clear() throw();
	void set(float) throw();
	void set(float, float) throw();
	void set(const Vector2&) throw();
	float getLength() const throw();
	float getSquareLength() const throw();
	Vector2& normalize() throw(DivisionByZero);
	Vector2& negate() throw();
	static const Vector2& getZero() throw();
	static const Vector2& getUnit() throw();
	float operator [] (Position) const throw(IndexOverflow);
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

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
