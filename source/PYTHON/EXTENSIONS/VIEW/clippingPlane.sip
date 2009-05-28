class ClippingPlane 
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/clippingPlane.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	//BALL_CREATE(ClippingPlane)
	ClippingPlane() throw();
  ClippingPlane(const ClippingPlane&) throw();
  //virtual ~ClippingPlane() throw();
//const ClippingPlane& operator = (const ClippingPlane&) throw();
	void clear() throw();
	//HashSet<const Representation*>& getRepresentations();
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
