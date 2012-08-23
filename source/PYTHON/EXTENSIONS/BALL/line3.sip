// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Line3
{
%TypeHeaderCode
	#include <BALL/MATHS/line3.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Line3)
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
  // Line3& operator = (const Line3&);
  void get(Line3&);
  void get(Vector3&,Vector3&, Form form = FORM__PARAMETER) const;
  void normalize();
  bool operator ==(const Line3&) const;
  bool operator != (const Line3&) const;
  bool has(const Vector3&) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  Vector3 p;
  Vector3 d;
};
