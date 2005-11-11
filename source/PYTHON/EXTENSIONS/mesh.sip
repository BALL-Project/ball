// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.sip,v 1.2.6.2 2005/11/11 17:21:23 leonhardt Exp $
//

class Mesh 
  : GeometricObject,
    Surface
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/mesh.h>
	#include <BALL/PYTHON/pyColorRGBAVector.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Mesh& operator = (const Mesh& mesh) throw();
  Mesh() throw();
  Mesh(const Mesh&) throw();
  bool isValid() const throw();
  void clear() throw();
  void set(const Mesh&) throw();
  void swap(Mesh&) throw();
  ~Mesh() throw();
//	PyColorRGBAVector getColors();
//	PyColorRGBAVector colors;
};
