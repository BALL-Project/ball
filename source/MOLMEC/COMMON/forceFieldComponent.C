// $Id: forceFieldComponent.C,v 1.2 1999/12/04 18:34:31 oliver Exp $


#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL 
{

	// default constructor 
	ForceFieldComponent::ForceFieldComponent()
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
	ForceFieldComponent::~ForceFieldComponent()
	{
	}

	// setup
	bool ForceFieldComponent::setup()
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
	ForceField* ForceFieldComponent::getForceField() const
	{
		return force_field_;
	}

	// Set the force field to force_field 
	void ForceFieldComponent::setForceField(ForceField* force_field)
	{
		force_field_ = force_field;
	}

	float ForceFieldComponent::getEnergy() const
	{
		return energy_;
	}

	void ForceFieldComponent::updateForces()
	{
	}

	float ForceFieldComponent::updateEnergy()
	{
		return 0;
	}


} // namespace BALL
