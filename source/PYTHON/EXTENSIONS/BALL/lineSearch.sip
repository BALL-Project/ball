class LineSearch
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
%End
	public:
	LineSearch();
	LineSearch(EnergyMinimizer&);
	virtual ~LineSearch() throw();
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
