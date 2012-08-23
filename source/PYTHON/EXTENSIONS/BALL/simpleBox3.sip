// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class SimpleBox3
{
%TypeHeaderCode
	#include <BALL/MATHS/simpleBox3.h>
	using namespace BALL;
%End
	public:
  SimpleBox3();
  SimpleBox3(SimpleBox3&);
  SimpleBox3(Vector3&, Vector3&);
  SimpleBox3
   (float, float, float,
    float, float, float);

  void set(SimpleBox3&);
  void set(Vector3&, Vector3&);
  void set
   (float, float, float,
    float, float, float);
  // SimpleBox3& operator = (SimpleBox3& box);
  void get(SimpleBox3&) const;
  void get(Vector3&, Vector3&) const;
  void get(float, float, float, float, float, float) const;
  void swap(SimpleBox3&);
  float getSurface() const;
  float getVolume() const;
  float getWidth() const;
  float getHeight() const;
  float getDepth() const;
  void join(SimpleBox3&);
  bool operator == (const SimpleBox3&) const;
  bool operator != (SimpleBox3&) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  Vector3 a;
  Vector3 b;


  SIP_PYOBJECT __str__();
%MethodCode
	 static String tmp;
	 tmp = "SimpleBox3 { (";
	 tmp += String(sipCpp->a.x) + " ";
	 tmp += String(sipCpp->a.y) + " ";
	 tmp += String(sipCpp->a.z) + ") (";
	 tmp += String(sipCpp->b.x) + " ";
	 tmp += String(sipCpp->b.y) + " ";
	 tmp += String(sipCpp->b.z) + ") }";
	 sipRes = PyString_FromString(tmp.c_str());
%End
};
