class TwoColoredTube
	: GeometricObject,
	  ColorExtension2,
	  Vertex2
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
%End
	public:
	TwoColoredTube() throw();
	TwoColoredTube(const TwoColoredTube&) throw();
	Vector3 getMiddleVertex() const throw();
	bool isValid() const throw();
	float getLength() const throw();
	float getRadius() const throw();
	void clear() throw();
	void set(const TwoColoredTube&) throw();
	void setRadius(float) throw();
	void swap(TwoColoredTube&) throw();
	~TwoColoredTube() throw();
};
