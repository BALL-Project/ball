// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex1.sip,v 1.1 2003/11/24 23:38:44 amoll Exp $
//

class Vertex 
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/vertex1.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const Vertex& operator = (const Vertex& v) throw();
  Vector3& getVertex() throw();
  Vector3* getVertexAddress() const throw();
  Vertex() throw();
  Vertex(const Vertex&) throw();
  bool isDefaultVertexAddress() const throw();
  bool isValid() const throw();
  const Vector3& getVertex() const throw();
  void clear() throw();
  void getVertex(Vector3&) const throw();
  void getVertex(float&, float&, float&) const throw();
  void set(const Vertex&) throw();
  void setDefaultVertexAddress() throw();
  void setVertex(const Vector3&) throw();
  void setVertex(const float, const float, const float) throw();
  void setVertexAddress(const Vector3&) throw();
  void swap(Vertex&) throw();
  ~Vertex() throw();
};
