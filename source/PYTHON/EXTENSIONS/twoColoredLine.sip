// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class TwoColoredLine 
  : GeometricObject,
    ColorExtension2,
    Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const TwoColoredLine& operator = (const TwoColoredLine& two_colored_line) throw();
  TwoColoredLine() throw();
  TwoColoredLine(const TwoColoredLine&) throw();
  Vector3 getMiddleVertex() const throw();
  bool isValid() const throw();
  void clear() throw();
  void set(const TwoColoredLine&) throw();
  void swap(TwoColoredLine&) throw();
  ~TwoColoredLine() throw();
};
