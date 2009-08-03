// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class TwoColoredTube 
  : GeometricObject,
    ColorExtension2,
    Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const TwoColoredTube& operator = (const TwoColoredTube& two_colored_tube) throw();
  TwoColoredTube() throw();
  TwoColoredTube(const TwoColoredTube&) throw();
  Vector3 getMiddleVertex() const throw();
  bool isValid() const throw();
  float getLength() const throw();
  float getRadius() const throw();
  void clear() throw();
  void set(const TwoColoredTube&) throw();
  void setRadius(float) throw();
  void swap(TwoColoredTube&) throw();
  ~TwoColoredTube() throw();
};
