// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Disc 
  : GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/disc.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Disc& operator = (const Disc& Disc) throw();
  Disc() throw();
  Disc(const Circle3&) throw();
  Disc(const Disc&) throw();
  bool isValid() const throw();
  const Circle3& getCircle() const throw();
  void clear() throw();
  void set(const Disc&) throw();
  void setCircle(const Circle3&) throw();
  void swap(Disc&) throw();
  ~Disc() throw();
};
