class ClippingPlane
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/clippingPlane.h>
%End
	public:

	ClippingPlane() throw();
	ClippingPlane(const ClippingPlane&) throw();
	void clear() throw();
	const Vector3& getNormal() const;
	void setNormal(const Vector3&);
	bool isActive() const;
	void setActive(bool);
	bool isHidden() const;
	void setHidden(bool);
	float getDistance() const;
	const Vector3& getPoint() const;
	void setPoint(const Vector3& );
	void flip();
};
