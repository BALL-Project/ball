// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tube.sip,v 1.1 2003/11/18 15:06:05 amoll Exp $
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

  Tube() throw();
  Tube(const Tube&) throw();
  bool isValid() const throw();
//  const Tube& operator = (const Tube&) throw();
  void clear() throw();
//  void dump(std::ostream& = std::cout, Size = 0) const throw();
  void set(const Tube&) throw();
	float getLength() const throw();
	float getSquareLength() const throw();
	float getRadius() const throw();
	void setRadius(float) throw();
  ~Tube() throw();
};
