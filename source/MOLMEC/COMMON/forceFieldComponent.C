// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceFieldComponent.C,v 1.12.26.1 2007/03/25 22:00:30 oliver Exp $
//


#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

using namespace std;

namespace BALL 
{

	// default constructor 
	ForceFieldComponent::ForceFieldComponent()
		: force_field_(0),
			energy_(0),
			name_("GenericForceFieldComponent"),
			enabled_(1)
	{
	}

	// constructor 
	ForceFieldComponent::ForceFieldComponent(ForceField& force_field)
		: force_field_(&force_field),
			energy_(0),
			name_("GenericForceFieldComponent"),
			enabled_(true)
	{
	}

	// copy constructor 
	ForceFieldComponent::ForceFieldComponent(const ForceFieldComponent& force_field_component)
		: force_field_(force_field_component.force_field_),
			energy_(force_field_component.energy_),
			name_(force_field_component.name_),
			enabled_(force_field_component.enabled_)
	{
	}


	// destructor
	ForceFieldComponent::~ForceFieldComponent()
	{
	}

	// setup
	bool ForceFieldComponent::setup()
		throw(Exception::TooManyErrors)
	{
		return true;
	}

	// update pair lists - empty!
	void update()
		throw(Exception::TooManyErrors)
	{
	}

	// Set the name of the component
	void ForceFieldComponent::setName(const String& name)
	{
		name_ = name;
	}

	// Return the name of the component
	String ForceFieldComponent::getName() const
	{
		return name_;
	}

	// Return a pointer to the force field
	ForceField* ForceFieldComponent::getForceField() const
	{
		return force_field_;
	}

	// Set the force field to force_field 
	void ForceFieldComponent::setForceField(ForceField& force_field)
	{
		force_field_ = &force_field;
	}

	double ForceFieldComponent::getEnergy() const
	{
		if (!isEnabled()) return 0.;
		return energy_;
	}

	double ForceFieldComponent::updateEnergy()
	{
		return 0.0;
	}

	void ForceFieldComponent::updateForces() 
	{
	}

	void ForceFieldComponent::update()
		throw(Exception::TooManyErrors)
	{
	}

	double ForceFieldComponent::updateScore()
	{
		score_ = updateEnergy();

		/*
		scaleScore();
		return score_;
		*/

		return getScaledScore();
	}

} // namespace BALL
