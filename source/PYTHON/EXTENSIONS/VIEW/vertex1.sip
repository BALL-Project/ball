class Vertex
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/vertex1.h>
%End
	public:
	Vertex() throw();
	Vertex(const Vertex&) throw();
	void clear() throw();
	void set(const Vertex&) throw();
	void swap(Vertex&) throw();
	void setVertex(const Vector3&) throw();
	void setVertex(const float, const float, const float) throw();
	Vector3& getVertex() throw();
	void setVertexAddress(const Vector3&) throw();
	void setDefaultVertexAddress() throw();
	Vector3* getVertexAddress() const throw();
	bool isDefaultVertexAddress() const throw();
	bool isValid() const throw();
};
