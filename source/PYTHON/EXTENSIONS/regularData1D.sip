// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: regularData1D.sip,v 1.4 2003/08/26 09:18:22 oliver Exp $
//

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
//	virtual void destroy() throw();
	// const RegularData1D& operator = (const RegularData1D& data) throw();
	// const RegularData1D& operator = (const VectorType& data) throw();
	bool operator == (const RegularData1D& /data/) const throw();
	float operator [] (Position /index/) const throw(IndexOverflow);
	// float& operator [] (Position /index/) throw(IndexOverflow);
	Size getSize() const throw();
	double getOrigin() const throw();
	double getDimension() const throw();
	void setOrigin(double /upper/) throw();
	void setDimension(double /upper/) throw();
	void resize(Size /new_size/) throw();
};


