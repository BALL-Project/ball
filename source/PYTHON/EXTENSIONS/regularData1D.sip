class RegularData1D
{
%HeaderCode
	#include <BALL/DATATYPE/regularData1D.h>
	using namespace BALL;
%End
	public:
	RegularData1D() throw();
	RegularData1D(const RegularData1D& /data/) throw();
	~RegularData1D() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	// const RegularData1D& operator = (const RegularData1D& data) throw();
	// const RegularData1D& operator = (const VectorType& data) throw();
	bool operator == (const RegularData1D& /data/) const throw();
	float operator [] (Position /index/) const throw(IndexOverflow);
	// float& operator [] (Position /index/) throw(IndexOverflow);
	Size getSize() const throw();
	double getLowerBound() const throw();
	double getUpperBound() const throw();
	void setBoundaries(double /upper/, double /lower/) throw();
	void resize(Size /new_size/) throw();
};


