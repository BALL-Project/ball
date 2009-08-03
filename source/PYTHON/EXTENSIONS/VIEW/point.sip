// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

namespace VIEW
{
class Point 
  : GeometricObject,
    Vertex
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/point.h>
	using namespace BALL;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Point& operator = (const Point& point) throw();
  Point() throw();
  Point(const VIEW::Point&) throw();
  bool isValid() const throw();
  void clear() throw();
  void set(const VIEW::Point&) throw();
  void swap(VIEW::Point&) throw();
  ~Point() throw();
};
};
