// $Id: HydrogenBondSlick.C,v 1.4 2006/05/21 17:32:10 anker Exp $
// hydrogen bond component

#include <BALL/SCORING/COMPONENTS/hydrogenBondSlick.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/DATATYPE/hashMap.h>

#include <BALL/SYSTEM/timer.h>

// #define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

namespace BALL
{

	const char* HydrogenBondSlick::Option::HB_IDEAL_LENGTH = "hb_ideal_length";
	const char* HydrogenBondSlick::Option::HB_IDEAL_ANGLE = "hb_ideal_angle";
	const char* HydrogenBondSlick::Option::HB_DIST_LOWER = "hb_dist_lower";
	const char* HydrogenBondSlick::Option::HB_DIST_UPPER = "hb_dist_upper";
	const char* HydrogenBondSlick::Option::HB_ANG_LOWER = "hb_ang_lower";
	const char* HydrogenBondSlick::Option::HB_ANG_UPPER = "hb_ang_upper";
	const char* HydrogenBondSlick::Option::VERBOSITY = "verbosity";

	const float HydrogenBondSlick::Default::HB_IDEAL_LENGTH = 1.85;
	const float HydrogenBondSlick::Default::HB_IDEAL_ANGLE = 180;
	const float HydrogenBondSlick::Default::HB_DIST_LOWER = 0.25;
	const float HydrogenBondSlick::Default::HB_DIST_UPPER = 0.65;
	const float HydrogenBondSlick::Default::HB_ANG_LOWER = 30;
	const float HydrogenBondSlick::Default::HB_ANG_UPPER = 80;
	const Size HydrogenBondSlick::Default::VERBOSITY = 0;


	HydrogenBondSlick::HydrogenBondSlick()
		:	ScoringComponent(),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("SLICK HydrogenBond");
	}


	HydrogenBondSlick::HydrogenBondSlick(ScoringFunction& sf)
		:	ScoringComponent(sf),
			possible_hydrogen_bonds_()
	{
		// set component name
		setName("SLICK HydrogenBond");
	}


	HydrogenBondSlick::HydrogenBondSlick(const HydrogenBondSlick& hb)
		:	ScoringComponent(hb),
			possible_hydrogen_bonds_(hb.possible_hydrogen_bonds_),
			h_bond_distance_lower_(hb.h_bond_distance_lower_),
			h_bond_distance_upper_(hb.h_bond_distance_upper_),
			h_bond_angle_lower_(hb.h_bond_angle_lower_),
			h_bond_angle_upper_(hb.h_bond_angle_upper_)
	{
	}


	HydrogenBondSlick::~HydrogenBondSlick()
	{
		clear();
	}


	void HydrogenBondSlick::clear()
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


	bool HydrogenBondSlick::setup()

	{
		Timer timer;
		timer.start();

		ScoringFunction* scoring_function = getScoringFunction();
		if (scoring_function == 0)
		{
			Log.error() << "HydrogenBondSlick::setup(): "
				<< "component not bound to scoring function." << std::endl;
			return false;
		}

		// clear the vector of possible hydrogen bonds
		possible_hydrogen_bonds_.clear();

		Options* options = getScoringFunction()->getOptionsToModify();

		ideal_hbond_length_
			= options->setDefaultReal(HydrogenBondSlick::Option::HB_IDEAL_LENGTH,
					HydrogenBondSlick::Default::HB_IDEAL_LENGTH);
		ideal_hbond_angle_
			= options->setDefaultReal(HydrogenBondSlick::Option::HB_IDEAL_ANGLE,
					HydrogenBondSlick::Default::HB_IDEAL_ANGLE);
		h_bond_distance_lower_
			= options->setDefaultReal(HydrogenBondSlick::Option::HB_DIST_LOWER,
					HydrogenBondSlick::Default::HB_DIST_LOWER);
		h_bond_distance_upper_
			= options->setDefaultReal(HydrogenBondSlick::Option::HB_DIST_UPPER,
					HydrogenBondSlick::Default::HB_DIST_UPPER);
		h_bond_angle_lower_
			= options->setDefaultReal(HydrogenBondSlick::Option::HB_ANG_LOWER,
					HydrogenBondSlick::Default::HB_ANG_LOWER);
		h_bond_angle_upper_
			= options->setDefaultReal(HydrogenBondSlick::Option::HB_ANG_UPPER,
					HydrogenBondSlick::Default::HB_ANG_UPPER);
		Size verbosity
			= options->setDefaultInteger(HydrogenBondSlick::Option::VERBOSITY,
					HydrogenBondSlick::Default::VERBOSITY);


		// Get FresnoTypes
		FresnoTypes rec_fresno_types_class(getScoringFunction()->getReceptor());
		rec_fresno_types_ = *rec_fresno_types_class.getTypeMap();
		FresnoTypes lig_fresno_types_class(getScoringFunction()->getLigand());
		lig_fresno_types_ = *lig_fresno_types_class.getTypeMap();

		AtomContainer* rec = getScoringFunction()->getReceptor();
		AtomContainer* lig = getScoringFunction()->getLigand();
		AtomConstIterator rec_it = rec->beginAtom();
		AtomConstIterator lig_it;

		// two times quadratic run time. not nice.
		for (; +rec_it; ++rec_it)
		{
			if (rec_fresno_types_.has(&*rec_it))
			{
				if (rec_fresno_types_[&*rec_it] == FresnoTypes::HBOND_HYDROGEN)
				{
					for (lig_it = lig->beginAtom(); +lig_it; ++lig_it)
					{
						if (lig_fresno_types_.has(&*lig_it))
						{
							if ((lig_fresno_types_[&*lig_it] == FresnoTypes::HBOND_ACCEPTOR_DONOR)
									|| (lig_fresno_types_[&*lig_it] == FresnoTypes::HBOND_ACCEPTOR))
							{
								possible_hydrogen_bonds_.push_back(std::pair<const Atom*, const Atom*>(&*rec_it, &*lig_it));
								if (verbosity >= 90)
								{
									Log.info() << "found possible HB: "
										<< rec_it->getBond(0)->getPartner(*rec_it)->getFullName()
										<< "---"
										<< rec_it->getFullName() << "..." << lig_it->getFullName()
										<< " (length: "
										<< (rec_it->getPosition() - lig_it->getPosition()).getLength()
										<< " A) "
										<< std::endl;
								}
							}
						}
					}
				}
			}
		}

		for (lig_it = lig->beginAtom(); +lig_it; ++lig_it)
		{
			if (lig_fresno_types_.has(&*lig_it))
			{
				if (lig_fresno_types_[&*lig_it] == FresnoTypes::HBOND_HYDROGEN)
				{
					for (rec_it = rec->beginAtom(); +rec_it; ++rec_it)
					{
						if (rec_fresno_types_.has(&*rec_it))
						{
							if ((rec_fresno_types_[&*rec_it] == FresnoTypes::HBOND_ACCEPTOR_DONOR)
									|| (rec_fresno_types_[&*rec_it] == FresnoTypes::HBOND_ACCEPTOR))
							{
								possible_hydrogen_bonds_.push_back(std::pair<const Atom*, const Atom*>(&*lig_it, &*rec_it));
								if (verbosity >= 90)
								{
									Log.info() << "found possible HB: "
										<< lig_it->getBond(0)->getPartner(*lig_it)->getFullName() << "-"
										<< lig_it->getFullName() << "..." << rec_it->getFullName()
										<< " (length: "
										<< (lig_it->getPosition() - rec_it->getPosition()).getLength()
										<< " A) "
										<< std::endl;
								}
							}
						}
					}
				}
			}
		}

		if (verbosity > 8)
		{
			Log.info() << "HydrogenBondSlick setup statistics:" << std::endl;
			Log.info() << "Found " << possible_hydrogen_bonds_.size()
				<< " possible hydrogen bonds" << std::endl << std::endl;
		}

		timer.stop();
		Log.info() << "HydrogenBondSlick::setup(): "
			<< timer.getCPUTime() << " s" << std::endl;

		return true;
	}


	void HydrogenBondSlick::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
	{
	}


	double HydrogenBondSlick::updateScore()
	{
		Timer timer;
		timer.start();

#ifdef DEBUG
		Molecule debug_molecule;
#endif
		Size verbosity = getScoringFunction()->getOptions().getInteger(Option::VERBOSITY);

		score_ = 0.0;
		float val = 0.0;
		float distance;
		float angle;
		const Atom* hydrogen;
		const Atom* acceptor;
		Vector3 h_bond;
		Vector3 h_connection;

		// iterate over all possible hydrogen bond std::pairs
		vector< std::pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_hydrogen_bonds_.begin(); it != possible_hydrogen_bonds_.end(); ++it)
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
				val = getScoringFunction()->getBaseFunction()->calculate(distance, h_bond_distance_lower_, h_bond_distance_upper_);

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
					Log.error() << "HydrogenBondSlick::updateEnergy(): "
						<< "black magic: hydrogen bond without hydrogens:" << std::endl
						<< hydrogen->getFullName() << ":" << acceptor->getFullName()
						<< std::endl;
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
							<< std::endl;
					}
					score_ += val;
				}
			}
		}

		if (verbosity > 0)
		{
			Log.info() << "HB: energy is " << score_ << std::endl;
		}

#ifdef DEBUG
		HINFile debug_file("HB_debug.hin", std::ios::out);
		debug_file << debug_molecule;
		debug_file.close();
#endif

		timer.stop();
#ifdef DEBUG
		Log.info() << "HydrogenBondSlick::updateEnergy(): "
			<< timer.getCPUTime() << " s" << std::endl;
#endif

		return score_;
	}
}
