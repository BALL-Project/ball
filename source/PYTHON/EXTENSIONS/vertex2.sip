// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex2.sip,v 1.1 2003/11/18 15:06:06 amoll Exp $
//

class Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/vertex2.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	Vector3& getVertex1() throw();
	Vector3& getVertex2() throw();
	Vector3* getVertex1Address() const throw();
	Vector3* getVertex2Address() const throw();
	Vertex2() throw();
	Vertex2(const Vertex2& ) throw();
	bool isDefaultVertex1Address() const throw();
	bool isDefaultVertex2Address() const throw();
	bool isDefaultVertexAddresses() const throw();
	bool isValid() const throw();
	const Vector3& getVertex1() const throw();
	const Vector3& getVertex2() const throw();
//	const Vertex2& operator = (const Vertex2& ) throw();
	void clear() throw();
//	void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
	void getVertex1(float& , float& , float& ) const throw();
	void getVertex1(Vector3& ) const throw();
	void getVertex2(float& , float& , float& ) const throw();
	void getVertex2(Vector3& ) const throw();
	void getVertices(float& , float& , float& , float& , float& , float& ) throw();
	void getVertices(Vector3& , Vector3& ) throw();
	void set(const Vertex2& ) throw();
	void setDefaultVertex1Address() throw();
	void setDefaultVertex2Address() throw();
	void setDefaultVertexAddresses() throw();
	void setVertex1(const float , const float , const float ) throw();
	void setVertex1(const Vector3& ) throw();
	void setVertex1Address(const Vector3& ) throw();
	void setVertex2(const float , const float , const float ) throw();
	void setVertex2(const Vector3& ) throw();
	void setVertex2Address(const Vector3& ) throw();
	void setVertexAddresses(const Vector3& , const Vector3& ) throw();
	void setVertices(const float , const float , const float , const float , const float , const float ) throw();
	void setVertices(const Vector3& , const Vector3& ) throw();
	void swap(Vertex2& ) throw();
	~Vertex2() throw();
};
