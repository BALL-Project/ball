// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Box
	: GeometricObject,
		Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/box.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

  Box() throw();
  Box(const Box&) throw();
  Box(const Vector3&, const Vector3& = Vector3( 0, 1, 0), const Vector3& = Vector3(-1, 0, 0), float = 1) throw();
  bool isValid() const throw();
//  const Box& operator = (const Box&) throw();
  void clear() throw();
//  void dump(std::ostream& = std::cout, Size = 0) const throw();
  void set(const Box&) throw();
  ~Box() throw();
};
