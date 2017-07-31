class SimpleBox
	: SimpleBox3,
	  GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
%End
	public:
	SimpleBox() throw();
	SimpleBox(const SimpleBox&) throw();
	bool isValid() const throw();
	void clear() throw();
	void set(const SimpleBox&) throw();
	~SimpleBox() throw();
};
