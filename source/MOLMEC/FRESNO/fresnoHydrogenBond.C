// $Id: fresnoHydrogenBond.C,v 1.1.2.1 2002/02/14 17:02:55 anker Exp $
// Molecular Mechanics: Fresno force field, hydrogen bond component

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoHydrogenBond.h>

using namespace std;

namespace BALL
{

	FresnoHydrogenBond::FresnoHydrogenBond()
		throw()
		:	ForceFieldComponent(),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("Fresno Hydrogen Bond");
	}


	FresnoHydrogenBond::FresnoHydrogenBond(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName("Fresno Hydrogen Bond");
	}


	FresnoHydrogenBond::FresnoHydrogenBond(const FresnoHydrogenBond& fhb, bool deep)
		throw()
		:	ForceFieldComponent(fhb, deep),
			possible_hydrogen_bonds_(fhb.possible_hydrogen_bonds_),
			h_bond_distance_lower_(fhb.h_bond_distance_lower_),
			h_bond_distance_upper_(fhb.h_bond_distance_upper_),
			h_bond_angle_lower_(fhb.h_bond_angle_lower_),
			h_bond_angle_upper_(fhb.h_bond_angle_upper_)
	{
	}


	FresnoHydrogenBond::~FresnoHydrogenBond()
		throw()
	{
		clear();
	}


	void FresnoHydrogenBond::clear()
		throw()
	{
		possible_hydrogen_bonds_.clear();
		h_bond_distance_lower_ = 0.0;
		h_bond_distance_upper_ = 0.0;
		h_bond_angle_lower_ = 0.0;
		h_bond_angle_upper_ = 0.0;
		// ?????
		// ForceFieldComponent does not comply to the OCI
		// ForceFieldComponent::clear();
	}


	bool FresnoHydrogenBond::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoHydrogenBond::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// clear the vector of possible hydrogen bonds
		possible_hydrogen_bonds_.clear();

		// ?????
		// we should check whether force_field is a FresnoFF, because we need
		// the fresno types

		System* system = force_field->getSystem();

		FresnoFF* fff = dynamic_cast<FresnoFF*>(force_field);

		// ?????
		// the following should be done with some proper parameter parsing
		h_bond_distance_lower_ = 0.25;
		h_bond_distance_upper_ = 0.65;
		h_bond_angle_lower_ = 30;
		h_bond_angle_upper_ = 80;
		ideal_hbond_length_ = 1.85;
		ideal_hbond_angle_ = 180;

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
				if ((type_A == FresnoFF::HBOND_ACCEPTOR)
						|| (type_A == FresnoFF::HBOND_DONOR)
						|| (type_A == FresnoFF::HBOND_ACCEPTOR_DONOR))
				{
					for (B_it = B->beginAtom(); +B_it; ++B_it)
					{
						// Do we really need to store *all* possible hydrogen bonds?
						type_B = fresno_types[&*B_it];
						if (((type_B == FresnoFF::HBOND_ACCEPTOR)
									&& ((type_A == FresnoFF::HBOND_DONOR)
										|| (type_A == FresnoFF::HBOND_ACCEPTOR_DONOR)))
								|| ((type_B == FresnoFF::HBOND_DONOR)
									&& ((type_A == FresnoFF::HBOND_ACCEPTOR)
										|| (type_A == FresnoFF::HBOND_ACCEPTOR_DONOR)))
								|| ((type_B == FresnoFF::HBOND_ACCEPTOR_DONOR)
									&& ((type_A == FresnoFF::HBOND_ACCEPTOR)
										|| (type_A == FresnoFF::HBOND_DONOR))
										|| (type_A == FresnoFF::HBOND_ACCEPTOR_DONOR)))
						{
							possible_hydrogen_bonds_.push_back(pair<const Atom*, const Atom*>(&*A_it, &*B_it));
							// DEBUG
							cout << "found possible HB: " 
								<< A_it->getFullName() << ":" << B_it->getFullName()
								<< " (length: " 
								<< (A_it->getPosition() - B_it->getPosition()).getLength() 
								<< " A) " 
								<< &*A_it << ":" << &*B_it
								<< endl;
							// /DEBUG
						}
					}
				}
			}
		}

		// DEBUG
		cout << "FresnoHydrogenBond setup statistics:" << endl;
		cout << "Found " << possible_hydrogen_bonds_.size() 
			<< " possible hydrogen bonds" << endl << endl;
		// /DEBUG

		return true;

	}


	double FresnoHydrogenBond::updateEnergy()
		throw()
	{

		double E = 0.0;
		double val = 0.0;
		double tmp;
		double distance;
		double angle;
		const Atom* atom1;
		const Atom* atom2;
		Vector3 h_bond;
		Vector3 h_connection;

		// iterate over all possible hydrogen bond pairs
		::vector< pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_hydrogen_bonds_.begin();
			it != possible_hydrogen_bonds_.end();
			++it)
		{
			atom1 = it->first;
			atom2 = it->second;

			// h_bond is the vector of the hbond

			h_bond = atom2->getPosition() - atom1->getPosition();
			distance = ideal_hbond_length_ - h_bond.getLength();

			// DEBUG
			cout << "HB distance deviation: " << distance << endl;
			// /DEBUG

			// if the distance is too large, the product of g1 and g2 is zero, so
			// we can skip the rest

			if (distance <= h_bond_distance_upper_)
			{
				// calculate g1

				tmp = MolmecSupport::calculateFresnoHelperFunction(distance,
						h_bond_distance_lower_, h_bond_distance_upper_);

				// calculate the angle of the hbond. It is necessary to find out
				// which one of the atoms is the actual hydrogen in order to
				// calculate the vector of the connection (in contrast to h bond)
				// of the hydrogen to the molecule it is attached to

				if (atom1->getElement().getSymbol() == "H")
				{
					h_connection = atom1->getBond(0)->getPartner(*atom1)->getPosition()
						- atom1->getPosition();
				}
				else
				{
					// PARANOIA
					if (atom2->getElement().getSymbol() != "H")
					{
						Log.error() << "FresnoHydrogenBond::updateEnergy(): "
						// cerr << "FresnoHydrogenBond::updateEnergy(): "
							<< "black magic: hydrogen bond without hydrogens:" << endl
							<< atom1->getFullName() << ":" << atom2->getFullName()
							<< endl;
						continue;
					}
					// /PARANOIA
					h_connection = atom2->getPosition()
						- atom2->getBond(0)->getPartner(*atom1)->getPosition();
				}

				// angle is the angle of the h bond
				angle = ideal_hbond_angle_ - h_bond.getAngle(h_connection);
				// DEBUG
				cout << "HB angle deviation: " << angle << endl;
				// /DEBUG

				// if angle is too large, skip the rest
				if (angle <= h_bond_angle_upper_)
				{
					tmp *= MolmecSupport::calculateFresnoHelperFunction(angle,
						h_bond_angle_lower_, h_bond_angle_upper_);
					// DEBUG
					cout << "tmp: " << tmp << endl;
					// /DEBUG
					val += tmp;
				}
			}
		}

		E = val;
		return E;
		
	}


	void FresnoHydrogenBond::updateForces()
		throw()
	{
	}


}
