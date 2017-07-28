class ConjugateGradientMinimizer
	: EnergyMinimizer
{
%TypeHeaderCode
	#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
%End
	public:

	enum UpdateMethod
	{
		POLAK_RIBIERE,
		FLETCHER_REEVES,
		SHANNO
	};

	ConjugateGradientMinimizer();
	ConjugateGradientMinimizer(ForceField&);
	ConjugateGradientMinimizer(ForceField&, SnapShotManager*);
	ConjugateGradientMinimizer(ForceField&, SnapShotManager*, const Options&);
	ConjugateGradientMinimizer(ForceField&, const Options&);
	~ConjugateGradientMinimizer();
	virtual bool specificSetup();
	void setUpdateMethod(UpdateMethod);
	UpdateMethod getUpdateMethod() const;
	virtual double findStep();
	virtual void updateDirection();
	virtual bool minimize(Size iterations = 0, bool restart = false);
};
