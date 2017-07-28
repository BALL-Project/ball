class SteepestDescentMinimizer
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
%End
	public:
	SteepestDescentMinimizer();
	SteepestDescentMinimizer(ForceField&);
	SteepestDescentMinimizer(ForceField&, SnapShotManager*);
	SteepestDescentMinimizer(ForceField&, SnapShotManager*, const Options&);
	SteepestDescentMinimizer(ForceField&, const Options&);
	~SteepestDescentMinimizer();

	virtual bool specificSetup();
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size iterations = 0, bool resume = false);
};
