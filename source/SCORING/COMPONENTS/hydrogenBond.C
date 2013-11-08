// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/hydrogenBond.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/DATATYPE/hashMap.h>

#include <BALL/SYSTEM/timer.h>

using namespace BALL;
using namespace std;

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


HydrogenBond::HydrogenBond(Mode mode)
	throw()
	:	ScoringComponent(),
		possible_hydrogen_bonds_()
{
	// set component name
	setName("HydrogenBond");
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	mode_ = mode;
	type_name_ = "HB";
	charge_evaluation_enabled_ = false;

	if (mode_ == LIGAND_HYDROGENS)
	{
		gridable_ = false;
	}
}


HydrogenBond::HydrogenBond(ScoringFunction& sf, Mode mode)
	throw()
	:	ScoringComponent(sf),
		possible_hydrogen_bonds_()
{
	// set component name
	setName("HydrogenBond");
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	mode_ = mode;
	type_name_ = "HB";
	charge_evaluation_enabled_ = false;

	if (mode_ == LIGAND_HYDROGENS)
	{
		gridable_ = false;
	}
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
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	mode_ = hb.mode_;

	if (mode_ == LIGAND_HYDROGENS)
	{
		gridable_ = false;
	}
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

	delete receptor_fresno_types_;
	delete ligand_fresno_types_;
}


void HydrogenBond::enableChargeEvaluation(bool b)
{
	charge_evaluation_enabled_ = b;
	if (charge_evaluation_enabled_) gridable_ = false;
	else
	{
		if (mode_ == LIGAND_HYDROGENS) gridable_ = false;
		else gridable_ = true;
	}
}


bool HydrogenBond::setup()
{
	Timer timer;
	timer.start();

	ScoringFunction* scoring_function = getScoringFunction();
	if (scoring_function == 0)
	{
		Log.error() << "HydrogenBond::setup(): "
			<< "component not bound to scoring function." << std::endl;
		return false;
	}

	// clear the vector of possible hydrogen bonds
	possible_hydrogen_bonds_.clear();

	Options options = getScoringFunction()->getOptions();

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
	verbosity_
		 = options.setDefaultInteger(HydrogenBond::Option::VERBOSITY,
				HydrogenBond::Default::VERBOSITY);

	delete receptor_fresno_types_;
	receptor_fresno_types_ = new FresnoTypes(getScoringFunction()->getReceptor());

	setupLigand();

	timer.stop();
	Log.info() << "HydrogenBond::setup(): "
		<< timer.getCPUTime() << " s" << std::endl;

	return true;
}


void HydrogenBond::setupLigand()
{
	delete ligand_fresno_types_;
	ligand_fresno_types_ = new FresnoTypes(getScoringFunction()->getLigand());
}


Size HydrogenBond::getType(Atom* atom)
{
	HashMap<const Atom*, Size>::const_iterator it = receptor_fresno_types_->getTypeMap()->find(atom);
	if (it != receptor_fresno_types_->getTypeMap()->end())
	{
		return it->second;
	}
	it = ligand_fresno_types_->getTypeMap()->find(atom);
	if (it != ligand_fresno_types_->getTypeMap()->end())
	{
		return it->second;
	}
	return FresnoTypes::UNKNOWN;
}


// If intermolecular H-bonds are to be evaluated, the _first_ atom of each pair must be a ligand atom and the second one a receptor atom.
// This is automatically done this way by ScoringFunction::createNonbondedPairVector()
void HydrogenBond::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
{
	possible_hydrogen_bonds_.clear();

	for (vector < std::pair < Atom*, Atom* > > ::const_iterator it = pair_vector.begin(); it != pair_vector.end(); it++)
	{
		// is there exactly one hydrogen? (tested here for speed-up only)
		bool h1 = (it->first->getElement().getSymbol() == "H");
		bool h2 = (it->second->getElement().getSymbol() == "H");
		if ( (!h1 && !h2) || (h1 && h2) )
		{
			continue;
		}

		// is there a hydrogen that is part of the ligand?
		bool ligand_hydrogen = h1;

		// HydrogenBonds with ligand-hydrogens cannot be precalculated (no donor-atom)
		if (mode_ == RECEPTOR_HYDROGENS && ligand_hydrogen)
		{
			continue;
		}

		// Scores for HydrogenBonds with receptor-hydrogens have already been precalculated by different HydrogenBond-component and are part of the score-grids
		else if (mode_ == LIGAND_HYDROGENS && !ligand_hydrogen)
		{
			continue;
		}

		int first_type = getType(it->first);
		int second_type = getType(it->second);
		if (first_type == FresnoTypes::UNKNOWN || second_type == FresnoTypes::UNKNOWN)
		{
			continue;
		}

		if ( first_type == FresnoTypes::HBOND_HYDROGEN
			&& (second_type == FresnoTypes::HBOND_ACCEPTOR_DONOR
			|| second_type == FresnoTypes::HBOND_ACCEPTOR) )

		{
			possible_hydrogen_bonds_.push_back(*it);
		}

		else if ( second_type == FresnoTypes::HBOND_HYDROGEN
			&& (first_type == FresnoTypes::HBOND_ACCEPTOR_DONOR
			|| first_type == FresnoTypes::HBOND_ACCEPTOR) )

		{
			possible_hydrogen_bonds_.push_back(make_pair(it->second, it->first));
		}
	}

	if (verbosity_ > 8)
	{
		Log.info() << "HydrogenBond update() statistics:" << std::endl;
		Log.info() << "Found " << possible_hydrogen_bonds_.size()
				<< " possible hydrogen bonds" << std::endl << std::endl;
	}
}


double HydrogenBond::updateScore()
{

#ifdef DEBUG
	Timer timer;
	timer.start();
	Molecule debug_molecule;
#endif

	Size verbosity
		 = getScoringFunction()->getOptions().getInteger(Option::VERBOSITY);

	score_ = 0.0;
	float val = 0.0;
	float distance;
	float angle;
	const Atom* hydrogen;
	const Atom* acceptor;
	Vector3 h_bond;
	Vector3 h_connection;

	// iterate over all possible hydrogen bond std::pairs
	vector<pair<const Atom*, const Atom*> >::const_iterator it;
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
			val = base_function_->calculate(distance,
				h_bond_distance_lower_, h_bond_distance_upper_);

			/// Use partial charges for detection of strong hydrogen-bonds.
			/// Maximal charge-difference will result in score being multiplied by three.
			if (charge_evaluation_enabled_)
			{
				double charge1 = hydrogen->getCharge();
				double charge2 = acceptor->getCharge();
				if ((charge1 < 0 && charge2 > 0) || (charge1 > 0 && charge2 < 0))
				{
					double abs_charge_diff = fabs(charge2-charge1);
					if (abs_charge_diff > 2) abs_charge_diff = 2;
					if (abs_charge_diff > 1) abs_charge_diff = 1+(1-abs_charge_diff)*2;
					val *= abs_charge_diff;
					cout<<"hb factor = "<<abs_charge_diff<<endl;
				}
			}

			// If this component is to be precalculated for a grid,
			/// use distance only if probe-atom is a hydrogen.
			/// If probe-atom is donor atom, angle-dependend score can be precalculated.
			if (gridable_ && hydrogen->countBonds() == 0)
			{
				score_ += val;
				continue;
			}

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
			else // PARANOIA
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "HydrogenBond::updateScore() error!", "black magic: hydrogen bond without hydrogens");
			}


			// angle is the angle of the h bond
			angle = ideal_hbond_angle_ - h_bond.getAngle(h_connection).toDegree();

			// if angle is too large, skip the rest
			if (angle <= h_bond_angle_upper_)
			{
				val *= base_function_->calculate(angle,
					h_bond_angle_lower_, h_bond_angle_upper_);

				if (scoring_function_->storeInteractionsEnabled())
				{
					// negative score for good pose ...
					double scaled_atom_score = -val;
					scaled_atom_score = scaleScore(scaled_atom_score);
					Atom* a1 = const_cast<Atom*>(hydrogen);
					Atom* a2 = const_cast<Atom*>(acceptor);
					a1->addInteraction(acceptor, "HB", scaled_atom_score);
					a2->addInteraction(hydrogen, "HB", scaled_atom_score);
				}

				score_ += val;
			}
		}
	}

	if (verbosity > 0)
	{
		Log.info() << "HB: energy is " << score_ << std::endl;
	}

	// we want a negative score for a good pose, thus we will use the negative of the value computed above
	score_ *= -1;

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}


