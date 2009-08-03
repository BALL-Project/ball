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
	LineSearch(EnergyMinimizer& /minimizer/);
	// LineSearch(const LineSearch& /line_search/);
	virtual ~LineSearch() throw();
	// LineSearch& operator = (const LineSearch& /rhs/);
	void setAlpha(double /alpha/);
	void setBeta(double /beta/);
	double getAlpha() const;
	double getBeta() const;
	Size getMaxSteps() const;
	void setMaxSteps(Size /steps/);
	void setLowerBound(double /lbound/);
	double getLowerBound() const;
	void setXTol(double /xtol/);
	double getXTol() const;
	void setBracketedFlag(bool /bracktd/);
	bool isBracketed() const;
	void setMinimizer(EnergyMinimizer& /minimizer/);
	virtual void takeStep(double& /st_a/, double& /f_a/, double& /g_a/, double& /st_b/, double& /f_b/, 
								 double& /g_b/, double& /stp/, double /f/, double /g/, double /minstp/, double /maxstp/);
	virtual bool minimize(double& /stp/, bool /keep_gradient/ = false);
};
