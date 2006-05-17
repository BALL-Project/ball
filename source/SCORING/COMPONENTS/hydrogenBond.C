// $Id: hydrogenBond.C,v 1.3 2006/05/17 12:50:46 anker Exp $
// hydrogen bond component

#include <BALL/SCORING/COMPONENTS/hydrogenBond.h>
#include <BALL/SCORING/TYPES/fresnoTypes.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/DATATYPE/hashMap.h>

#include <BALL/SYSTEM/timer.h>

// #define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{

	const char* HydrogenBond::Option::HB_IDEAL_LENGTH = "hb_ideal_length";
	const char* HydrogenBond::Option::HB_IDEAL_ANGLE = "hb_ideal_angle";
	const char* HydrogenBond::Option::HB_DIST_LOWER = "hb_dist_lower";
	const char* HydrogenBond::Option::HB_DIST_UPPER = "hb_dist_upper";
	const char* HydrogenBond::Option::HB_ANG_LOWER = "hb_ang_lower";
	const char* HydrogenBond::Option::HB_ANG_UPPER = "hb_ang_upper";
	const char* HydrogenBond::Option::VERBOSITY = "verbosity";

	const float HydrogenBond::Default::HB_IDEAL_LENGTH = 1.85;
	const float HydrogenBond::Default::HB_IDEAL_ANGLE = 180;
	const float HydrogenBond::Default::HB_DIST_LOWER = 0.25;
	const float HydrogenBond::Default::HB_DIST_UPPER = 0.65;
	const float HydrogenBond::Default::HB_ANG_LOWER = 30;
	const float HydrogenBond::Default::HB_ANG_UPPER = 80;
	const Size HydrogenBond::Default::VERBOSITY = 0;


	HydrogenBond::HydrogenBond()
		throw()
		:	ScoringComponent(),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("SLICK HydrogenBond");
	}


	HydrogenBond::HydrogenBond(ScoringFunction& sf)
		throw()
		:	ScoringComponent(sf),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("SLICK HydrogenBond");
	}


	HydrogenBond::HydrogenBond(const HydrogenBond& hb)
		throw()
		:	ScoringComponent(hb),
			possible_hydrogen_bonds_(hb.possible_hydrogen_bonds_),
			h_bond_distance_lower_(hb.h_bond_distance_lower_),
			h_bond_distance_upper_(hb.h_bond_distance_upper_),
			h_bond_angle_lower_(hb.h_bond_angle_lower_),
			h_bond_angle_upper_(hb.h_bond_angle_upper_)
	{
	}


	HydrogenBond::~HydrogenBond()
		throw()
	{
		clear();
	}


	void HydrogenBond::clear()
		throw()
	{
		possible_hydrogen_bonds_.clear();
		h_bond_distance_lower_ = 0.0;
		h_bond_distance_upper_ = 0.0;
		h_bond_angle_lower_ = 0.0;
		h_bond_angle_upper_ = 0.0;
		// ?????
		// ScoringComponent does not comply to the OCI
		// ScoringComponent::clear();
	}


	bool HydrogenBond::setup()
		throw()
	{

		Timer timer;
		timer.start();

		ScoringFunction* scoring_function = getScoringFunction();
		if (scoring_function == 0)
		{
			Log.error() << "HydrogenBond::setup(): "
				<< "component not bound to scoring function." << endl;
			return false;
		}

		// clear the vector of possible hydrogen bonds
		possible_hydrogen_bonds_.clear();

    Options& options = getScoringFunction()->options;

		ideal_hbond_length_ 
			= options.setDefaultReal(HydrogenBond::Option::HB_IDEAL_LENGTH,
					HydrogenBond::Default::HB_IDEAL_LENGTH);
		ideal_hbond_angle_
			= options.setDefaultReal(HydrogenBond::Option::HB_IDEAL_ANGLE,
					HydrogenBond::Default::HB_IDEAL_ANGLE);
		h_bond_distance_lower_
			= options.setDefaultReal(HydrogenBond::Option::HB_DIST_LOWER,
					HydrogenBond::Default::HB_DIST_LOWER);
		h_bond_distance_upper_
			= options.setDefaultReal(HydrogenBond::Option::HB_DIST_UPPER,
					HydrogenBond::Default::HB_DIST_UPPER);
		h_bond_angle_lower_
			= options.setDefaultReal(HydrogenBond::Option::HB_ANG_LOWER,
					HydrogenBond::Default::HB_ANG_LOWER);
		h_bond_angle_upper_
			= options.setDefaultReal(HydrogenBond::Option::HB_ANG_UPPER,
					HydrogenBond::Default::HB_ANG_UPPER);
		Size verbosity
			= options.setDefaultInteger(HydrogenBond::Option::VERBOSITY,
					HydrogenBond::Default::VERBOSITY);

		verbosity = 100;

		FresnoTypes fresno_types_class(*this);
		// const HashMap<const Atom*, Size>& fresno_types 
		//	= fresno_types_class.getTypeMap();
		fresno_types = fresno_types_class.getTypeMap();

		// two times quadratic run time. not nice.

		Molecule* A = getScoringFunction()->getReceptor();
		Molecule* B = getScoringFunction()->getLigand();

		AtomConstIterator A_it = A->beginAtom();
		AtomConstIterator B_it;

		for (; +A_it; ++A_it)
		{
			if (fresno_types.has(&*A_it))
			{
				if (fresno_types[&*A_it] == FresnoTypes::HBOND_HYDROGEN)
				{
					for (B_it = B->beginAtom(); +B_it; ++B_it)
					{
						if (fresno_types.has(&*B_it))
						{
							if ((fresno_types[&*B_it] == FresnoTypes::HBOND_ACCEPTOR_DONOR)
									|| (fresno_types[&*B_it] == FresnoTypes::HBOND_ACCEPTOR))
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
				if (fresno_types[&*B_it] == FresnoTypes::HBOND_HYDROGEN)
				{
					for (A_it = A->beginAtom(); +A_it; ++A_it)
					{
						if (fresno_types.has(&*A_it))
						{
							if ((fresno_types[&*A_it] == FresnoTypes::HBOND_ACCEPTOR_DONOR)
									|| (fresno_types[&*A_it] == FresnoTypes::HBOND_ACCEPTOR))
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
			Log.info() << "HydrogenBond setup statistics:" << endl;
			Log.info() << "Found " << possible_hydrogen_bonds_.size() 
				<< " possible hydrogen bonds" << endl << endl;
		}

		timer.stop();
		Log.info() << "HydrogenBond::setup(): "
			<< timer.getCPUTime() << " s" << std::endl;

		return(true);

	}


	const HashMap<const Atom*, Size>& HydrogenBond::getFresnoTypes()
		throw()
	{
		return(fresno_types);
	}


	double HydrogenBond::calculateScore()
		throw()
	{

		Timer timer;
		timer.start();

#ifdef DEBUG
		Molecule debug_molecule;
#endif

		Size verbosity 
			= getScoringFunction()->options.getInteger(Option::VERBOSITY);

		score_ = 0.0;
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

				val = getScoringFunction()->getBaseFunction()->calculate(distance,
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
					cerr << "HydrogenBond::updateEnergy(): "
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
					val *= getScoringFunction()->getBaseFunction()->calculate(angle,
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

					// Print all single energy contributions
					if (verbosity >= 100)
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
					score_ += val;
				}
			}
		}

		if (verbosity > 0)
		{
			Log.info() << "HB: energy is " << score_ << endl;
		}
		
#ifdef DEBUG
		HINFile debug_file("HB_debug.hin", std::ios::out);
		debug_file << debug_molecule;
		debug_file.close();
#endif

		timer.stop();
#ifdef DEBUG
		Log.info() << "HydrogenBond::updateEnergy(): "
			<< timer.getCPUTime() << " s" << std::endl;
#endif

		return(score_);
	}

}
