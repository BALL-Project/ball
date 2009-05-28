// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Line 
  : GeometricObject,
    Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/line.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Line& operator = (const Line& line) throw();
  Line() throw();
  Line(const Line&) throw();
  bool isValid() const throw();
  void clear() throw();
  void set(const Line&) throw();
  void swap(Line&) throw();
  ~Line() throw();
};
