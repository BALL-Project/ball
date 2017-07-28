class Line3
{
%TypeHeaderCode
	#include <BALL/MATHS/line3.h>
%End
	public:

	enum Form
	{
		FORM__PARAMETER  = 0,
		FORM__TWO_POINTS = 1
	};

	Line3();
	Line3(const Line3&);
	Line3(const Vector3&, const Vector3&, Form form = FORM__PARAMETER);
	~Line3();
	void swap(Line3&);
	void set(const Line3&);
	void set(const Vector3&, const Vector3&, Form form = FORM__PARAMETER);
	void get(Line3&);
	void get(Vector3&,Vector3&, Form form = FORM__PARAMETER) const;
	void normalize();
	bool operator ==(const Line3&) const;
	bool operator != (const Line3&) const;
	bool has(const Vector3&) const;
	bool isValid() const;
	Vector3 p;
	Vector3 d;
};
