class Point
	: GeometricObject,
	  Vertex
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/point.h>
%End
	public:
	Point() throw();
	Point(const VIEW::Point&) throw();
	bool isValid() const throw();
	void clear() throw();
	void set(const VIEW::Point&) throw();
	void swap(VIEW::Point&) throw();
	~Point() throw();
};
