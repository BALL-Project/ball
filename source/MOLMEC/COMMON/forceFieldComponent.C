// $Id: forceFieldComponent.C,v 1.1 1999/08/26 08:02:45 oliver Exp $


#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL 
{

	// default constructor 
	ForceFieldComponent::ForceFieldComponent(void)
	{
		name_ = "GenericForceFieldComponent";
		force_field_ = 0;
		energy_ = 0;
	}

	// constructor 
	ForceFieldComponent::ForceFieldComponent(ForceField* force_field)
	{
		force_field_ = force_field;
		name_ = "GenericForceFieldComponent";
		energy_ = 0;	
	}

	// copy constructor 
	ForceFieldComponent::ForceFieldComponent(const ForceFieldComponent& force_field_component, bool /* clone_deep */)
	{
		name_ = force_field_component.name_;
		force_field_ = force_field_component.force_field_;
		energy_ = force_field_component.energy_;
	}


	// destructor
	ForceFieldComponent::~ForceFieldComponent(void)
	{
	}

	// setup
	bool ForceFieldComponent::setup(void)
	{
		return true;
	}

	// Set the name of the component
	void ForceFieldComponent::setName(const String& name)
	{
		name_ = name;
	}

	// Return the name of the component
	String ForceFieldComponent::getName( ) const
	{
		return name_;
	}

	// Return a pointer to the force field
	ForceField* ForceFieldComponent::getForceField(void) const
	{
		return force_field_;
	}

	// Set the force field to force_field 
	void ForceFieldComponent::setForceField(ForceField* force_field)
	{
		force_field_ = force_field;
	}

	float ForceFieldComponent::getEnergy(void) const
	{
		return energy_;
	}

	void ForceFieldComponent::updateForces(void)
	{
	}

	float ForceFieldComponent::updateEnergy(void)
	{
		return 0;
	}


} // namespace BALL
