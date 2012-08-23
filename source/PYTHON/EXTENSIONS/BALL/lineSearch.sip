// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class LineSearch 
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
	using namespace BALL;
%End
	public:
	//    BALL_CREATE(LineSearch)
	LineSearch();	
	LineSearch(EnergyMinimizer&);
	// LineSearch(const LineSearch&);
	virtual ~LineSearch() throw();
	// LineSearch& operator = (const LineSearch&);
	void setAlpha(double);
	void setBeta(double);
	double getAlpha() const;
	double getBeta() const;
	Size getMaxSteps() const;
	void setMaxSteps(Size);
	void setLowerBound(double);
	double getLowerBound() const;
	void setXTol(double);
	double getXTol() const;
	void setBracketedFlag(bool);
	bool isBracketed() const;
	void setMinimizer(EnergyMinimizer&);
	virtual void takeStep(double&, double&, double&, double&, double&, 
								 double&, double&, double, double, double, double);
	virtual bool minimize(double&, bool keep_gradient = false);
};
