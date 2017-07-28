class RegularData1D
{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularData1D.h>
%End
	public:
	RegularData1D() throw();
	RegularData1D(const RegularData1D&) throw();
	~RegularData1D() throw();
	virtual void clear() throw();
	bool operator == (const RegularData1D&) const throw();
	float operator [] (Position) const throw(IndexOverflow);
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

	Size getSize() const throw();
	double getOrigin() const throw();
	double getDimension() const throw();
	void setOrigin(double) throw();
	void setDimension(double) throw();
	void resize(Size) throw();
};
