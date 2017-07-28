class AmberNonBonded
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberNonBonded.h>
	using namespace BALL::MolmecSupport;
%End
	public:
	AmberNonBonded();
	AmberNonBonded(ForceField&);
	AmberNonBonded(const AmberNonBonded&);
	~AmberNonBonded();
	virtual void clear();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
	virtual void update() throw(TooManyErrors);
	virtual double getElectrostaticEnergy() const;
	virtual double getVdwEnergy() const;
	virtual PairListAlgorithmType determineMethodOfAtomPairGeneration();
};

