// $Id: surface.sip,v 1.1 2001/09/03 16:53:17 oliver Exp $

class Surface
{
%HeaderCode
	#include <BALL/MATHS/surface.h>
	using namespace BALL;
%End
  public:
	//  BALL_CREATE_NODEEP(Surface)
  //struct Triangle
  //{
  // Index v1;
  // Index v2;
  // Index v3;
  //};
  Surface();
  Surface(const Surface& /surface/);
  ~Surface();
  void clear();
  void set(const Surface& /box/);
	//  const Surface& operator = (const Surface& box);
  void get(Surface& /box/) const;
  void readMSMSFile(const String& /vert_filename/, const String& /face_filename/);
  float getArea() const;
  // bool operator == (const Surface& box) const;
  // bool operator != (const Surface& box) const;
  bool isValid() const;
  // vector<Vector3>  vertex;
  // vector<Vector3>  normal;
  // vector<Triangle> triangle;
};
