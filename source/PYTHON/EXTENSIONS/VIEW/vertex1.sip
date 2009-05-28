// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Vertex 
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/vertex1.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  Vertex() throw();
  Vertex(const Vertex&) throw();
  //virtual ~Vertex() throw();
  void clear() throw();
  void set(const Vertex&) throw();
//  const Vertex& operator = (const Vertex& v) throw();
  void swap(Vertex&) throw();
  void setVertex(const Vector3&) throw();
  void setVertex(const float, const float, const float) throw();

  Vector3& getVertex() throw();
  // const Vector3& getVertex() const throw();

  // void getVertex(Vector3&) const throw();
  // void getVertex(float&, float&, float&) const throw();
  void setVertexAddress(const Vector3&) throw();
  void setDefaultVertexAddress() throw();
  Vector3* getVertexAddress() const throw();
  bool isDefaultVertexAddress() const throw();
  bool isValid() const throw();
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};
