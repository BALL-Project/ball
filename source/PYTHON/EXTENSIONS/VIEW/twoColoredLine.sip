class TwoColoredLine
	: GeometricObject,
	  ColorExtension2,
	  Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
%End
	public:
	TwoColoredLine() throw();
	TwoColoredLine(const TwoColoredLine&) throw();
	Vector3 getMiddleVertex() const throw();
	bool isValid() const throw();
	void clear() throw();
	void set(const TwoColoredLine&) throw();
	void swap(TwoColoredLine&) throw();
	~TwoColoredLine() throw();
};
