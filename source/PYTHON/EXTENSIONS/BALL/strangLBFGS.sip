class StrangLBFGSMinimizer
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
%End
	public:

	StrangLBFGSMinimizer();
	StrangLBFGSMinimizer(ForceField&);
	StrangLBFGSMinimizer(ForceField&, SnapShotManager*);
	StrangLBFGSMinimizer(ForceField&, SnapShotManager*, const Options&);
	StrangLBFGSMinimizer(ForceField&, const Options&);
	~StrangLBFGSMinimizer();

	virtual bool specificSetup();
	void setMaxNumOfStoredVectPairs(Size);
	Size getMaxNumOfStoredVectPairs() const;
	void setImprovedFlag(bool);
	bool getImprovedFlag() const;
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size iterations = 0, bool restart = false);
	Options options;
};
