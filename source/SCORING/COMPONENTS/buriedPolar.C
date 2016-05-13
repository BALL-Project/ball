// Molecular Mechanics: Fresno force field, buried polar component
// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/buriedPolar.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/SYSTEM/timer.h>


using namespace std;
using namespace BALL;


const char* BuriedPolar::Option::BP_R1_OFFSET = "bp_r1_offset";
const char* BuriedPolar::Option::BP_R2_OFFSET = "bp_r2_offset";
const char* BuriedPolar::Option::VERBOSITY = "verbosity";

const float BuriedPolar::Default::BP_R1_OFFSET = 0.5;
const float BuriedPolar::Default::BP_R2_OFFSET = 3.0;
const Size BuriedPolar::Default::VERBOSITY = 0;


BuriedPolar::BuriedPolar(ScoringFunction& sf)

	:	ScoringComponent(sf),
		possible_buried_polar_interactions_(),
		r1_offset_(0.0),
		r2_offset_(0.0)
{
	setName("BuriedPolar");
	type_name_ = "BP";
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	gridable_ = false;
}


BuriedPolar::BuriedPolar(const BuriedPolar& bp)
	:	ScoringComponent(bp),
	possible_buried_polar_interactions_(bp.possible_buried_polar_interactions_),
		r1_offset_(bp.r1_offset_),
		r2_offset_(bp.r2_offset_)
{
	setName("BuriedPolar");
	type_name_ = "BP";
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	gridable_ = false;
}


BuriedPolar::~BuriedPolar()
{
}


void BuriedPolar::clear()
{
	possible_buried_polar_interactions_.clear();
}


bool BuriedPolar::setup()
{
	ScoringFunction* sf = getScoringFunction();
	if (sf == 0)
	{
		Log.error() << "BuriedPolar::setup(): "
			<< "component not bound to force field." << endl;
		return false;
	}

	// clear the vector of buried polar interactions
	possible_buried_polar_interactions_.clear();

	Options options = getScoringFunction()->getOptions();

	r1_offset_
		 = options.setDefaultReal(BuriedPolar::Option::BP_R1_OFFSET,
				BuriedPolar::Default::BP_R1_OFFSET);
	r2_offset_
		 = options.setDefaultReal(BuriedPolar::Option::BP_R2_OFFSET,
				BuriedPolar::Default::BP_R2_OFFSET);

	verbosity_
		 = options.setDefaultInteger(BuriedPolar::Option::VERBOSITY,
				BuriedPolar::Default::VERBOSITY);

	delete receptor_fresno_types_;
	receptor_fresno_types_ = new FresnoTypes(getScoringFunction()->getReceptor());

	setupLigand();

	if (verbosity_ > 2)
	{
		Log.info() << "BuriedPolar setup statistics:" << endl;
		Log.info() << "Found " << possible_buried_polar_interactions_.size()
			<< " possible buried polar interactions" << endl << endl;
	}

	return true;
}


void BuriedPolar::setupLigand()
{
	delete ligand_fresno_types_;
	ligand_fresno_types_ = new FresnoTypes(getScoringFunction()->getLigand());
}


Size BuriedPolar::getType(Atom* atom)
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


bool BuriedPolar::isBackboneAtom(const Atom* atom)
{
	return atom->getName() == "O" || atom->getName() == "N"
	    || atom->getName() == "H" || atom->getName() == "C";
}


void BuriedPolar::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
{
	possible_buried_polar_interactions_.clear();

	for (vector < std::pair < Atom*, Atom* > > ::const_iterator it = pair_vector.begin(); it != pair_vector.end(); it++)
	{
		int type_A = getType(it->first);
		if (type_A == FresnoTypes::UNKNOWN) continue;

		int type_B = getType(it->second);
		if (type_B == FresnoTypes::UNKNOWN || isBackboneAtom(it->second)) continue;

		if ( (((type_B == FresnoTypes::POLAR)
				|| (type_B == FresnoTypes::HBOND_ACCEPTOR)
				|| (type_B == FresnoTypes::HBOND_DONOR)
				|| (type_B == FresnoTypes::HBOND_ACCEPTOR_DONOR)
				|| (type_B == FresnoTypes::HBOND_HYDROGEN))
				&& (type_A == FresnoTypes::LIPOPHILIC))
			/*|| ((type_B == FresnoTypes::LIPOPHILIC)
				&& ((type_A == FresnoTypes::POLAR)
				|| (type_A == FresnoTypes::HBOND_ACCEPTOR)
				|| (type_A == FresnoTypes::HBOND_DONOR)
				|| (type_A == FresnoTypes::HBOND_ACCEPTOR_DONOR)
				|| (type_A == FresnoTypes::HBOND_HYDROGEN)))*/ )
		{
			possible_buried_polar_interactions_.push_back(*it);
			if (verbosity_ >= 10)
			{
				Log.info() << "found possible buried polar int.: "
				<< it->first->getFullName() << "..." << it->second->getFullName()
				<< " (length: "
				<< (it->first->getPosition() - it->second->getPosition()).getLength()
				<< " A) "
				<< endl;
			}
		}
	}
}


double BuriedPolar::updateScore()
{
	score_ = 0.0;
	float val = 0.0;
	float distance;
	float R1;
	float R2;
	Atom* atom1;
	Atom* atom2;

	AtomPairVector::const_iterator it;
	for (it = possible_buried_polar_interactions_.begin(); it != possible_buried_polar_interactions_.end(); ++it)
	{
		atom1 = it->first;
		atom2 = it->second;

		R1 = atom1->getElement().getVanDerWaalsRadius()+atom2->getElement().getVanDerWaalsRadius()-1;
		R2 = R1 + 1.5;

		distance = (atom1->getPosition() - atom2->getPosition()).getLength();

		// if the distance is too large, the product of g1 and g2 is zero, so
		// we can skip the rest
		if (distance <= R2)
		{
			// we could possibly speed up the next step by using the fact that the
			// difference between R1 and R2 is constant
			val = base_function_->calculate(distance, R1, R2);
			if (scoring_function_->storeInteractionsEnabled())
			{
				double scaled_atom_score = val;
				scaled_atom_score = scaleScore(scaled_atom_score);
				atom1->addInteraction(atom2, "pol", scaled_atom_score);
				atom2->addInteraction(atom1, "pol", scaled_atom_score);
			}
			score_ += val;

			if (verbosity_ >= 1)
			{
				Log.info() << "BP: " << val << " "
					<< atom1->getFullName() << " "
					<< endl;
				if (atom1->getResidue() != 0)
				{
					Log.info() << "[" << atom1->getResidue()->getID() << "]";
				}
				Log.info() << "..." << atom2->getFullName() << " "
					<< endl;
				if (atom2->getResidue() != 0)
				{
					Log.info() << "[" << atom2->getResidue()->getID() << "]";
				}
				Log.info() << " (d " << distance << ", R1 " << R1
					<< ", R2 " << R2 << ")" << endl;
			}
		}
	}

	if (verbosity_ > 0)
	{
		Log.info() << "BP: energy is " << score_ << endl;
	}

	return score_;
}
