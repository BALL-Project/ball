// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tube.sip,v 1.3 2003/11/24 23:37:04 amoll Exp $
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
  Real getRadius() const throw();
  Tube() throw();
  Tube(const Tube&) throw();
  bool isValid() const throw();
  float getLength() const throw();
  float getSquareLength() const throw();
  void clear() throw();
  void set(const Tube&) throw();
  void setRadius(Real) throw();
  void swap(Tube&) throw();
  ~Tube() throw();
};
