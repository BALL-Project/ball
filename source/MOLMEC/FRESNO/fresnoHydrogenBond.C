// $Id: fresnoHydrogenBond.C,v 1.1.2.15 2004/06/16 16:29:19 anker Exp $
// Molecular Mechanics: Fresno force field, hydrogen bond component

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoHydrogenBond.h>

#define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{

	FresnoHydrogenBond::FresnoHydrogenBond()
		throw()
		:	ForceFieldComponent(),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("Fresno HydrogenBond");
	}


	FresnoHydrogenBond::FresnoHydrogenBond(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("Fresno HydrogenBond");
	}


	FresnoHydrogenBond::FresnoHydrogenBond(const FresnoHydrogenBond& fhb)
		throw()
		:	ForceFieldComponent(fhb),
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

    Options& options = force_field->options;

		factor_ 
			= options.setDefaultReal(FresnoFF::Option::HB,
					FresnoFF::Default::HB);
		ideal_hbond_length_ 
			= options.setDefaultReal(FresnoFF::Option::HB_IDEAL_LENGTH,
					FresnoFF::Default::HB_IDEAL_LENGTH);
		ideal_hbond_angle_
			= options.setDefaultReal(FresnoFF::Option::HB_IDEAL_ANGLE,
					FresnoFF::Default::HB_IDEAL_ANGLE);
		h_bond_distance_lower_
			= options.setDefaultReal(FresnoFF::Option::HB_DIST_LOWER,
					FresnoFF::Default::HB_DIST_LOWER);
		h_bond_distance_upper_
			= options.setDefaultReal(FresnoFF::Option::HB_DIST_UPPER,
					FresnoFF::Default::HB_DIST_UPPER);
		h_bond_angle_lower_
			= options.setDefaultReal(FresnoFF::Option::HB_ANG_LOWER,
					FresnoFF::Default::HB_ANG_LOWER);
		h_bond_angle_upper_
			= options.setDefaultReal(FresnoFF::Option::HB_ANG_UPPER,
					FresnoFF::Default::HB_ANG_UPPER);
		Size verbosity
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

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
					for (B_it = B->beginAtom(); +B_it; ++B_it)
					{
						if (fresno_types.has(&*B_it))
						{
							if ((fresno_types[&*B_it] == FresnoFF::HBOND_ACCEPTOR_DONOR)
									|| (fresno_types[&*B_it] == FresnoFF::HBOND_ACCEPTOR))
							{
								possible_hydrogen_bonds_.push_back(pair<const Atom*, const Atom*>(&*A_it, &*B_it));
								if (verbosity >= 90)
								{
									Log.info() << "found possible HB: " 
										<< A_it->getBond(0)->getPartner(*A_it)->getFullName() 
										<< "---"
										<< A_it->getFullName() << "..." << B_it->getFullName()
										<< " (length: " 
										<< (A_it->getPosition() - B_it->getPosition()).getLength() 
										<< " A) " 
										<< endl;
								}
							}
						}
					}
				}
			}
		}

		for (B_it = B->beginAtom(); +B_it; ++B_it)
		{
			if (fresno_types.has(&*B_it))
			{
				if (fresno_types[&*B_it] == FresnoFF::HBOND_HYDROGEN)
				{
					for (A_it = A->beginAtom(); +A_it; ++A_it)
					{
						if (fresno_types.has(&*A_it))
						{
							if ((fresno_types[&*A_it] == FresnoFF::HBOND_ACCEPTOR_DONOR)
									|| (fresno_types[&*A_it] == FresnoFF::HBOND_ACCEPTOR))
							{
								possible_hydrogen_bonds_.push_back(pair<const Atom*, const Atom*>(&*B_it, &*A_it));
								if (verbosity >= 90)
								{
									Log.info() << "found possible HB: " 
										<< B_it->getBond(0)->getPartner(*B_it)->getFullName() << "-"
										<< B_it->getFullName() << "..." << A_it->getFullName()
										<< " (length: " 
										<< (B_it->getPosition() - A_it->getPosition()).getLength() 
										<< " A) " 
										<< endl;
								}
							}
						}
					}
				}
			}
		}

		if (verbosity > 8)
		{
			Log.info() << "FresnoHydrogenBond setup statistics:" << endl;
			Log.info() << "Found " << possible_hydrogen_bonds_.size() 
				<< " possible hydrogen bonds" << endl << endl;
		}

		return true;

	}


	double FresnoHydrogenBond::updateEnergy()
		throw()
	{

		#ifdef DEBUG
		Molecule debug_molecule;
		#endif

		Size verbosity 
			= getForceField()->options.getInteger(FresnoFF::Option::VERBOSITY);

		energy_ = 0.0;
		float val = 0.0;
		float distance;
		float angle;
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
			acceptor = it->second;

			// h_bond is the vector of the hbond

			h_bond = acceptor->getPosition() - hydrogen->getPosition();
			distance = fabs(ideal_hbond_length_ - h_bond.getLength());

			// if the distance is too large, the product of g1 and g2 is zero, so
			// we can skip the rest

			if (distance <= h_bond_distance_upper_)
			{
				// calculate g1

				val = ((FresnoFF*)getForceField())->base_function->calculate(distance,
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
					val *= ((FresnoFF*)getForceField())->base_function->calculate(angle,
							h_bond_angle_lower_, h_bond_angle_upper_);
#ifdef DEBUG
					Atom* atom_ptr_H = new Atom();
					atom_ptr_H->setElement(PTE[Element::Fe]);
					atom_ptr_H->setName("H");
					atom_ptr_H->setPosition(hydrogen->getPosition());
					atom_ptr_H->setCharge(val);

					Atom* atom_ptr_acceptor = new Atom();
					atom_ptr_acceptor->setElement(PTE[Element::Fe]);
					atom_ptr_acceptor->setName("ACC");
					atom_ptr_acceptor->setPosition(acceptor->getPosition());
					atom_ptr_acceptor->setCharge(val);

					Atom* atom_ptr_donor = new Atom();
					atom_ptr_donor->setElement(PTE[Element::Fe]);
					atom_ptr_donor->setName("DON");
					atom_ptr_donor->setPosition(hydrogen->getBond(0)->getPartner(*hydrogen)->getPosition());
					atom_ptr_donor->setCharge(val);

					atom_ptr_H->createBond(*atom_ptr_acceptor);
					atom_ptr_H->createBond(*atom_ptr_donor);

					debug_molecule.insert(*atom_ptr_H);
					debug_molecule.insert(*atom_ptr_acceptor);
					debug_molecule.insert(*atom_ptr_donor);
#endif

					// Print all single energy cotributions
					if (verbosity >= 0)
					{
						Atom* donor = it->first->getBond(0)->getPartner(*it->first);
						Log.info() << "HB: " << val << " " 
							<< donor->getFullName() << "-"
							<< it->first->getFullName();
						if (it->first->getResidue() != 0)
						{
							Log.info() << "[" << it->first->getResidue()->getID()
								<< "]";
						}
						Log.info() << "..."
							<< it->second->getFullName();
						if (it->second->getResidue() != 0)
						{
							Log.info() << "[" << it->second->getResidue()->getID()
								<< "]";
						}
						Log.info() << " (delta d " << distance
							<< ", delta phi " << angle << ")"
							<< endl;
					}
					energy_ += val;
				}
			}
		}

		energy_ = factor_ * energy_;

		if (verbosity > 0)
		{
			Log.info() << "HB: energy is " << energy_ << endl;
		}
		
#ifdef DEBUG
		HINFile debug_file("HB_debug.hin", std::ios::out);
		debug_file << debug_molecule;
		debug_file.close();
#endif

		return energy_;
	}


	void FresnoHydrogenBond::updateForces()
		throw()
	{
	}


}
