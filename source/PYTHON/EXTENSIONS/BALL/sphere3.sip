class Sphere3
{
%TypeHeaderCode
	#include <BALL/MATHS/sphere3.h>
%End
	public:
	Sphere3();
	Sphere3(const Sphere3&);
	Sphere3(const Vector3&, float);
	~Sphere3();

	void swap(Sphere3&);
	void set(const Sphere3&);
	void set(const Vector3&, float);
	void get(Sphere3&) const;
	void get(Vector3&, float) const;
	bool operator == (const Sphere3&) const;
	bool operator != (const Sphere3&) const;
	bool has(const Vector3&, bool on_surface = false) const;
	bool isEmpty() const;
	bool isValid() const;

	Vector3 p;
	float radius;
};
