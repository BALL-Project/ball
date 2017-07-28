class Angle
{
%TypeHeaderCode
	#include <BALL/MATHS/angle.h>
%End
	public:

	enum Range
	{
		RANGE__UNLIMITED = 0,
		RANGE__UNSIGNED  = 1,
		RANGE__SIGNED    = 2
	};

	Angle();
	Angle(const Angle&);
	Angle(float, bool radian = true);
	~Angle();
	void swap(Angle&);
	void set(float, bool radian = true);
	void set(const Angle&);
	float toRadian() const;
	float toDegree() const;
	void normalize(Range);
	void negate();
	Angle& operator += (const Angle&);
	Angle& operator += (float);
	Angle operator + (const Angle&);
	Angle& operator -= (const Angle&);
	Angle& operator -= (float);
	Angle operator - (const Angle&);
	Angle& operator *= (const Angle&);
	Angle& operator *= (float);
	Angle& operator /= (const Angle&);
	Angle& operator /= (float);
	Angle operator / (const Angle&);
	bool operator == (const Angle&) const;
	bool operator != (const Angle&) const;
	bool operator < (const Angle&) const;
	bool operator < (float) const;
	bool operator <= (const Angle&) const;
	bool operator >= (const Angle&) const;
	bool operator > (const Angle&) const;
	bool isEquivalent(Angle) const;
	bool isValid () const;

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(sipCpp->toRadian()).c_str());
%End
};
