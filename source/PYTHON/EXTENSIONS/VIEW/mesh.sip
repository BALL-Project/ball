class Mesh
	: GeometricObject,
	  Surface,
	  MultiColorExtension
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/mesh.h>
	#include <BALL/PYTHON/EXTENSIONS/pyColorRGBAVector.h>
%End
	public:
	Mesh() throw();
	Mesh(const Mesh&) throw();
	bool isValid() const throw();
	void clear() throw();
	void set(const Mesh&) throw();
	void swap(Mesh&) throw();
	~Mesh() throw();
};
