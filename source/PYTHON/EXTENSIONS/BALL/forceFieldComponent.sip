class ForceFieldComponent
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
%End
	public:
	ForceFieldComponent();
	ForceFieldComponent(ForceField&);
	ForceFieldComponent(const ForceFieldComponent&);
	virtual bool setup() throw(TooManyErrors);
	ForceField* getForceField() const;
	void setForceField(ForceField&);
	void setName(const String&);
	String getName() const;
	virtual double getEnergy() const;
	virtual double updateEnergy();
	virtual void updateForces();
};
