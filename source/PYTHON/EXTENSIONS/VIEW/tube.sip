// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Tube 
  : GeometricObject,
    Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/tube.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Tube& operator = (const Tube& tube) throw();
  float getRadius() const throw();
  Tube() throw();
  Tube(const Tube&) throw();
  bool isValid() const throw();
  float getLength() const throw();
  float getSquareLength() const throw();
  void clear() throw();
  void set(const Tube&) throw();
  void setRadius(float) throw();
  void swap(Tube&) throw();
  ~Tube() throw();
};
