// $Id: fresnoHydrogenBond.C,v 1.1.2.4 2002/03/05 22:55:30 anker Exp $
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
			possible_hydrogen_bonds_(),
			already_used_()
	{
		// set component name
		setName("Fresno Hydrogen Bond");
	}


	FresnoHydrogenBond::FresnoHydrogenBond(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			possible_hydrogen_bonds_(),
			already_used_()
	{
		// set component name
		setName("Fresno Hydrogen Bond");
	}


	FresnoHydrogenBond::FresnoHydrogenBond(const FresnoHydrogenBond& fhb, bool deep)
		throw()
		:	ForceFieldComponent(fhb, deep),
			possible_hydrogen_bonds_(fhb.possible_hydrogen_bonds_),
			already_used_(fhb.already_used_),
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
		already_used_.clear();
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

		// two times quadratic run time. not nice.

		Molecule* A = system->getMolecule(0);
		Molecule* B = system->getMolecule(1);

		AtomConstIterator A_it = A->beginAtom();
		AtomConstIterator B_it;

		for (; +A_it; ++A_it)
		{
			if (fresno_types.has(&*A_it))
			{
				if (fresno_types[&*A_it] == FresnoFF::HBOND_HYDROGEN)
				{
					already_used_.insert(pair<const Atom*, bool>(&*A_it, false));
					for (B_it = B->beginAtom(); +B_it; ++B_it)
					{
						if (fresno_types.has(&*B_it))
						{
							if ((fresno_types[&*B_it] == FresnoFF::HBOND_ACCEPTOR_DONOR)
									|| (fresno_types[&*B_it] == FresnoFF::HBOND_ACCEPTOR))
							{
								possible_hydrogen_bonds_.push_back(pair<const Atom*, const Atom*>(&*A_it, &*B_it));
								// DEBUG
								cout << "found possible HB: " 
									<< A_it->getBond(0)->getPartner(*A_it)->getFullName() << "---"
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
		}

		for (; +B_it; ++B_it)
		{
			if (fresno_types.has(&*B_it))
			{
				if (fresno_types[&*B_it] == FresnoFF::HBOND_HYDROGEN)
				{
					already_used_.insert(pair<const Atom*, bool>(&*B_it, false));
					for (A_it = B->beginAtom(); +A_it; ++A_it)
					{
						if (fresno_types.has(&*A_it))
						{
							if ((fresno_types[&*A_it] == FresnoFF::HBOND_ACCEPTOR_DONOR)
									|| (fresno_types[&*A_it] == FresnoFF::HBOND_ACCEPTOR))
							{
								possible_hydrogen_bonds_.push_back(pair<const Atom*, const Atom*>(&*B_it, &*A_it));
								// DEBUG
								cout << "found possible HB: " 
									<< B_it->getBond(0)->getPartner(*B_it)->getFullName() << "-"
									<< B_it->getFullName() << "..." << A_it->getFullName()
									<< " (length: " 
									<< (B_it->getPosition() - A_it->getPosition()).getLength() 
									<< " A) " 
									<< endl;
								// /DEBUG
							}
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

		// clear the already-used-flags
		HashMap<const Atom*, bool>::Iterator au_it = already_used_.begin();
		for (; +au_it; ++au_it)
		{
			au_it->second = false;
		}

		double E = 0.0;
		double val = 0.0;
		double distance;
		double angle;
		const Atom* hydrogen;
		const Atom* acceptor;
		Vector3 h_bond;
		Vector3 h_connection;

		// iterate over all possible hydrogen bond pairs
		::vector< pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_hydrogen_bonds_.begin();
			it != possible_hydrogen_bonds_.end();
			++it)
		{
			hydrogen = it->first;
			// we could check for multiple scoring here, but it would cost a lot
			// of performance. 
			if (already_used_.has(hydrogen))
			{
				if (already_used_[hydrogen] == false)
				{
					acceptor = it->second;

					// h_bond is the vector of the hbond

					h_bond = acceptor->getPosition() - hydrogen->getPosition();
					distance = fabs(ideal_hbond_length_ - h_bond.getLength());

					// if the distance is too large, the product of g1 and g2 is zero, so
					// we can skip the rest

					if (distance <= h_bond_distance_upper_)
					{
						// calculate g1

						val = MolmecSupport::calculateFresnoHelperFunction(distance,
								h_bond_distance_lower_, h_bond_distance_upper_);

						// calculate the angle of the hbond. It is necessary to find out
						// which one of the atoms is the actual hydrogen in order to
						// calculate the vector of the connection (in contrast to h bond)
						// of the hydrogen to the molecule it is attached to

						if (hydrogen->getElement().getSymbol() == "H")
						{
							h_connection = 
								hydrogen->getBond(0)->getPartner(*hydrogen)->getPosition()
								- hydrogen->getPosition();
						}
						// PARANOIA
						else
						{
							cerr << "FresnoHydrogenBond::updateEnergy(): "
								<< "black magic: hydrogen bond without hydrogens:" << endl
								<< hydrogen->getFullName() << ":" << acceptor->getFullName()
								<< endl;
							continue;
						}
						// /PARANOIA

						// angle is the angle of the h bond
						angle = ideal_hbond_angle_ - h_bond.getAngle(h_connection).toDegree();

						// if angle is too large, skip the rest
						if (angle <= h_bond_angle_upper_)
						{
							val *= MolmecSupport::calculateFresnoHelperFunction(angle,
									h_bond_angle_lower_, h_bond_angle_upper_);
							if (already_used_.has(hydrogen))
							{
								already_used_[hydrogen] = true;
							}
							// PARANOIA
							else
							{
								cerr << "FresnoHydrogenBond::setup(): "
									<< "already_used_ doesn't know this hydrogen." << endl;
							}
							// /PARANOIA

							// DEBUG
							cout << "HB: adding score of " << val 
								<< "(distance " << distance
								<< ", angle " << angle << ")"
								<< endl;
							// /DEBUG
							E += val;
						}
					}
				}
			}
		}
		return E;
	}


	void FresnoHydrogenBond::updateForces()
		throw()
	{
	}


}
