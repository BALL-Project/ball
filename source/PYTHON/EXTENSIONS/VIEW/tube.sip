class Tube
	: GeometricObject,
	  Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/tube.h>
%End
	public:
	float getRadius() const throw();
	Tube() throw();
	Tube(const Tube&) throw();
	bool isValid() const throw();
	float getLength() const throw();
	float getSquareLength() const throw();
	void clear() throw();
	void set(const Tube&) throw();
	void setRadius(float) throw();
	void swap(Tube&) throw();
	~Tube() throw();
};
