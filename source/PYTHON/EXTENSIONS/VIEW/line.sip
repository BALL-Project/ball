class Line
	: GeometricObject,
	  Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/line.h>
%End
	public:
	Line() throw();
	Line(const Line&) throw();
	bool isValid() const throw();
	void clear() throw();
	void set(const Line&) throw();
	void swap(Line&) throw();
	~Line() throw();
};
