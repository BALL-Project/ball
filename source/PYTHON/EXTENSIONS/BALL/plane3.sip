class Plane3
{
%TypeHeaderCode
	#include <BALL/MATHS/plane3.h>
%End
	public:
	Plane3();
	Plane3(const Plane3&);
	Plane3(const Vector3&, const Vector3&);
	Plane3(const Vector3&, const Vector3&, const Vector3&);
	Plane3(float, float, float, float);
	~Plane3();
	void swap(Plane3&);
	void set(const Plane3&);
	void set(const Vector3&, const Vector3&);
	void set(const Vector3&, const Vector3&, const Vector3&);
	void get(Plane3&) const;
	void get(Vector3&, Vector3&) const;
	void normalize();
	void hessify();
	bool operator == (const Plane3&) const;
	bool operator != (const Plane3&) const;
	bool has(const Vector3&) const;
	bool has(const Line3&) const;
	bool isValid() const;
};
