// $Id: fresnoLipophilic.C,v 1.1.2.2 2002/03/05 22:56:43 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoLipophilic.h>

using namespace std;

namespace BALL
{

	FresnoLipophilic::FresnoLipophilic()
		throw()
		:	ForceFieldComponent(),
			possible_lipophilic_interactions_(),
			add_to_radii_(0.0),
			add_to_lower_bound_(0.0)
	{
		// set component name
		setName("Fresno Lipophilic");
	}


	FresnoLipophilic::FresnoLipophilic(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			possible_lipophilic_interactions_(),
			add_to_radii_(0.0),
			add_to_lower_bound_(0.0)
	{
		// set component name
		setName("Fresno Lipophilic");
	}


	FresnoLipophilic::FresnoLipophilic(const FresnoLipophilic& fl, bool deep)
		throw()
		:	ForceFieldComponent(fl, deep),
			possible_lipophilic_interactions_(fl.possible_lipophilic_interactions_),
			add_to_radii_(fl.add_to_radii_),
			add_to_lower_bound_(fl.add_to_lower_bound_)
	{
	}


	FresnoLipophilic::~FresnoLipophilic()
		throw()
	{
		clear();
	}


	void FresnoLipophilic::clear()
		throw()
	{
		possible_lipophilic_interactions_.clear();
		add_to_radii_ = 0.0;
		add_to_lower_bound_ = 0.0;
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
	}


	bool FresnoLipophilic::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoLipophilic::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// clear the vector of lipophilic interactions
		possible_lipophilic_interactions_.clear();

		// ?????
		// we should check whether force_field is a FresnoFF, because we need
		// the fresno types

		System* system = force_field->getSystem();
		FresnoFF* fff = dynamic_cast<FresnoFF*>(force_field);

		// ?????
		// the following should be done with some proper parameter parsing
		add_to_radii_ = 0.5;
		add_to_lower_bound_ = 3.0;

		const HashMap<const Atom*, short>& fresno_types = fff->getFresnoTypes();

		// quadratic run time. not nice.

		Molecule* A = system->getMolecule(0);
		Molecule* B = system->getMolecule(1);

		AtomConstIterator A_it = A->beginAtom();
		AtomConstIterator B_it;
		
		Size type_A;
		Size type_B;

		for (; +A_it; ++A_it)
		{
			if (fresno_types.has(&*A_it))
			{
				type_A = fresno_types[&*A_it];
				if (type_A == FresnoFF::LIPOPHILIC)
				{
					for (B_it = B->beginAtom(); +B_it; ++B_it)
					{
						type_B = fresno_types[&*B_it];
						if (type_B == FresnoFF::LIPOPHILIC)
						{
							possible_lipophilic_interactions_.push_back(pair<const Atom*, const Atom*>(&*A_it, &*B_it));
							// DEBUG
							cout << "found possible lipophilic int.: " 
								<< A_it->getFullName() << "..." << B_it->getFullName()
								<< " (length: " 
								<< (A_it->getPosition() - B_it->getPosition()).getLength() 
								<< " A) " 
								<< endl;
							// /DEBUG
						}
					}
				}
			}
		}

		// DEBUG
		cout << "FresnoLipophilic setup statistics:" << endl;
		cout << "Found " << possible_lipophilic_interactions_.size() 
			<< " possible lipophilic interactions" << endl << endl;
		// /DEBUG

		return true;

	}


	double FresnoLipophilic::updateEnergy()
		throw()
	{

		double E = 0.0;
		double val = 0.0;
		double distance;
		double R1;
		double R2;
		const Atom* atom1;
		const Atom* atom2;

		// iterate over all possible hydrogen bond pairs
		::vector< pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_lipophilic_interactions_.begin();
			it != possible_lipophilic_interactions_.end();
			++it)
		{
			atom1 = it->first;
			atom2 = it->second;

			R1 = atom1->getRadius() + atom2->getRadius() + add_to_radii_;
			R2 = R1 + add_to_lower_bound_;

			distance = (atom1->getPosition() - atom2->getPosition()).getLength();

			// if the distance is too large, the product of g1 and g2 is zero, so
			// we can skip the rest

			if (distance <= R2)
			{
				// we could possibly speed up the next step by using the fact that the
				// difference between R1 and R2 is constant
				val = MolmecSupport::calculateFresnoHelperFunction(distance, R1, R2);
				// DEBUG
				cout << "LIPO: adding score of " << val
					<< " (distance " << distance << ", R1 " << R1 << ", R2 " << R2 << ")"
					<< endl;
				// /DEBUG
				
				E += val;
			}
		}

		return E;
	}


	void FresnoLipophilic::updateForces()
		throw()
	{
	}


}
