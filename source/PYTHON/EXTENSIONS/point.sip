// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: point.sip,v 1.1 2003/11/24 22:46:32 amoll Exp $
//

class Point 
  : GeometricObject,
    Vertex
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/point.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Point& operator = (const Point& point) throw();
  Point() throw();
  Point(const Point&) throw();
  bool isValid() const throw();
  void clear() throw();
  void set(const Point&) throw();
  void swap(Point&) throw();
  ~Point() throw();
};
