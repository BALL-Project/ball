class AmberStretch
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberStretch.h>
%End
	public:
	AmberStretch();
	AmberStretch(ForceField&);
	AmberStretch(const AmberStretch&);
	virtual ~AmberStretch();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
	virtual void update() throw(TooManyErrors);
};
