class Vector3
{
%TypeHeaderCode
	#include <BALL/MATHS/vector3.h>
%End
	public:

	Vector3();
	Vector3(float);
	Vector3(float, float, float);
	Vector3(const Vector3&);
	Vector3(float, const Angle&, const Angle&);
	~Vector3();

	void set(float);
	void set(float, float, float);
	void set(const Vector3&);
	void set(float, const Angle&, const Angle&);
	void get(Vector3&) const;
	void get(float, Angle&, Angle&) const;
	void swap(Vector3&);
	float getLength() const;
	float getSquareLength() const;
	Vector3& normalize();
	Vector3& negate();
	static const Vector3& getZero();
	static const Vector3& getUnit();

	float operator [] (Index);
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

	Vector3 operator - () const;
	Vector3& operator += (const Vector3&);
	Vector3& operator -= (const Vector3&);
	Vector3& operator /= (float) throw(DivisionByZero);
	Vector3 operator * (float);
	Vector3 operator / (float) throw(DivisionByZero);
	float operator * (const Vector3&) const;
	Vector3 operator % (const Vector3&) const;
	float getDistance(const Vector3&) const;
	float getSquareDistance(const Vector3&) const;
	Angle getAngle(const Vector3&) const;
	Vector3 getOrthogonalProjection(const Vector3&) const;
	static Vector3 getPerpendicularNormalization
		(const Vector3&, const Vector3&, const Vector3&);
	static float getTripleProduct (const Vector3&, const Vector3&, const Vector3&);
	bool operator == (const Vector3&) const;
	bool operator != (const Vector3&) const;
	bool isZero() const;
	bool isOrthogonalTo(Vector3&) const;
	bool isValid() const;

	Vector3 operator + (const Vector3&) const;
	Vector3 operator - (const Vector3&) const;

	float x;
	float y;
	float z;

	SIP_PYOBJECT __str__();
%MethodCode
	static String tmp;
	tmp = "(";
	tmp += String(sipCpp->x) + " ";
	tmp += String(sipCpp->y) + " ";
	tmp += String(sipCpp->z) + ")";
	sipRes = PyString_FromString(tmp.c_str());
%End
};
