// $Id: line3.sip,v 1.2 2001/12/27 00:25:43 oliver Exp $

class Line3
{
%HeaderCode
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
  Line3(const Line3& /line/);
  Line3(const Vector3& /point/, const Vector3& /vector/, Form /form/ = FORM__PARAMETER);
  ~Line3();
  void swap(Line3& /line/);
  void set(const Line3& /line/);
  void set(const Vector3& /point/, const Vector3& /vector/, Form /form/ = FORM__PARAMETER);
  // Line3& operator = (const Line3& /line/);
  void get(Line3& /line/);
  void get(Vector3& /point/,Vector3& /vector/, Form /form/ = FORM__PARAMETER) const;
  void normalize();
  bool operator ==(const Line3& /line/) const;
  // bool operator != (const Line3& line) const;
  bool has(const Vector3& /point/) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  Vector3 p;
  Vector3 d;
};
