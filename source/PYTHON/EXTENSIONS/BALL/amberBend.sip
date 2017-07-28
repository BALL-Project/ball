class AmberBend
	: ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/AMBER/amberBend.h>
%End
	public:
	AmberBend();
	AmberBend(ForceField&);
	AmberBend(const AmberBend&);
	virtual ~AmberBend();
	virtual bool setup() throw(TooManyErrors);
	virtual double updateEnergy();
	virtual void updateForces();
};
