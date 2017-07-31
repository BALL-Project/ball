class Disc
	: GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/disc.h>
%End
	public:
	Disc() throw();
	Disc(const Circle3&) throw();
	Disc(const Disc&) throw();
	bool isValid() const throw();
	const Circle3& getCircle() const throw();
	void clear() throw();
	void set(const Disc&) throw();
	void setCircle(const Circle3&) throw();
	void swap(Disc&) throw();
	~Disc() throw();
};
