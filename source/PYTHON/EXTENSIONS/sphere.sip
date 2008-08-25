// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Sphere 
  : GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/sphere.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Sphere& operator = (const Sphere& sphere) throw();
  Sphere() throw();
  Sphere(const Sphere&) throw();
  Vector3* getPositionAddress() throw();
  bool isDefaultPositionAddress() throw();
  bool isValid() const throw();
  const Vector3& getPosition() const throw();
  float getRadius() const throw();
  void clear() throw();
  void set(const Sphere&) throw();
  void setPosition(const Vector3&) throw();
  void setPositionAddress(const Vector3&) throw();
  void setRadius(float) throw();
  void swap(Sphere&) throw();
  ~Sphere() throw();
};
