// $Id: fresnoDesolvation.C,v 1.1.2.1 2002/02/14 17:02:55 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoDesolvation.h>

using namespace std;

namespace BALL
{

	FresnoDesolvation::FresnoDesolvation()
		throw()
		:	ForceFieldComponent(),
			fdpb_()
	{
		// set component name
		setName("Fresno Desolvation");
	}


	FresnoDesolvation::FresnoDesolvation(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			fdpb_()
	{
		// set component name
		setName("Fresno Desolvation");
	}


	FresnoDesolvation::FresnoDesolvation(const FresnoDesolvation& fd, bool deep)
		throw()
		:	ForceFieldComponent(fd, deep),
			fdpb_(fd.fdpb_)
	{
	}


	FresnoDesolvation::~FresnoDesolvation()
		throw()
	{
		clear();
	}


	void FresnoDesolvation::clear()
		throw()
	{
		// fdpb_.clear();

		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
	}


	bool FresnoDesolvation::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoDesolvation::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// fdpb.clear();

		// ?????
		// we should check whether force_field is a FresnoFF, because we need
		// the fresno types

		System* system = force_field->getSystem();

		// FresnoFF* fff = dynamic_cast<FresnoFF*>(force_field);

		ProteinPredicate isProtein;

		// see above
		Molecule* receptor = &*system->beginProtein();
		Molecule* ligand = system->getMolecule(0);
		if (ligand == receptor) ligand = system->getMolecule(1);

		// ?????
		// the following should be done with some proper parameter parsing

		System ligand_system;
		ligand_system.insert(*ligand);
		if (fdpb_.setup(ligand_system))
		{
			return true;
		}
		else
		{
			Log.error() << "FresnoDesolvation::setup(): "
				<< "could not setup FDPB solver." << endl;
			return false;
		}

	}

	double FresnoDesolvation::updateEnergy()
		throw()
	{
		fdpb_.solve();
		double E = fdpb_.getReactionFieldEnergy();
		return E;
	}


	void FresnoDesolvation::updateForces()
		throw()
	{
	}


}
