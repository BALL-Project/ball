// $Id: fresnoConstant.C,v 1.1.2.1 2002/03/15 14:48:00 anker Exp $
// Molecular Mechanics: Fresno force field, additive constant

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoConstant.h>

using namespace std;

namespace BALL
{

	FresnoConstant::FresnoConstant()
		throw()
		:	ForceFieldComponent()
	{
		// set component name
		setName("Fresno Constant");
	}


	FresnoConstant::FresnoConstant(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName("Fresno Constant");
	}


	FresnoConstant::FresnoConstant(const FresnoConstant& fd, bool deep)
		throw()
		:	ForceFieldComponent(fd, deep)
	{
	}


	FresnoConstant::~FresnoConstant()
		throw()
	{
		clear();
	}


	void FresnoConstant::clear()
		throw()
	{
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
	}


	bool FresnoConstant::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoConstant::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

    Options& options = force_field->options;

		energy_ 
			= options.setDefaultReal(FresnoFF::Option::CONST,
					FresnoFF::Default::CONST);

		return true;
	}

	double FresnoConstant::updateEnergy()
		throw()
	{
		return energy_;
	}


	void FresnoConstant::updateForces()
		throw()
	{
	}


}
