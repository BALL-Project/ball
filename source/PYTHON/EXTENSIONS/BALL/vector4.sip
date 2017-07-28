class Vector4
{
%TypeHeaderCode
	#include <BALL/MATHS/vector4.h>
%End
	public:

	Vector4();
	Vector4(float, float, float, float h = 1);
	Vector4(const Vector4&);
	~Vector4() throw();

	void set(float, float, float, float rh = 1);
	void set(const Vector4&);
	void get(float, float, float, float) const;
	void get(Vector4&) const;
	void swap(Vector4&);
	float getLength() const;
	float getSquareLength() const;
	Vector4& normalize();
	static const Vector4& getZero();
	static const Vector4& getUnit();
	void set(float value = 1);
	float operator [] (Index);
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

	Vector4 operator - () const;
	Vector4& operator += (const Vector4&);
	Vector4& operator -= (const Vector4&);
	Vector4 operator * (float);
	Vector4& operator *= (float);
	Vector4 operator / (float);
	Vector4& operator /= (float);
	float operator * (const Vector4&) const;
	float getDistance(const Vector4&) const;
	float getSquareDistance(const Vector4&) const;
	bool operator == (const Vector4&) const;
	bool operator != (const Vector4&) const;
	bool isOrthogonalTo(Vector4&) const;
	bool isValid() const;

	float x;
	float y;
	float z;
	float h;
};
