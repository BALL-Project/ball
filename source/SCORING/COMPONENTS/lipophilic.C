// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/lipophilic.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/FORMAT/HINFile.h>

using namespace std;
using namespace BALL;


const char* Lipophilic::Option::LIPO_R1_OFFSET = "lipo_r1_offset";
const char* Lipophilic::Option::LIPO_R2_OFFSET = "lipo_r2_offset";
const char* Lipophilic::Option::CREATE_INTERACTIONS_FILE
	 = "LIPO_create_interactions_file";
const char* Lipophilic::Option::VERBOSITY = "verbosity";

const float Lipophilic::Default::LIPO_R1_OFFSET = 0.5;
const float Lipophilic::Default::LIPO_R2_OFFSET = 3.0;
const bool Lipophilic::Default::CREATE_INTERACTIONS_FILE = false;
const Size Lipophilic::Default::VERBOSITY = 0;


Lipophilic::Lipophilic(ScoringFunction& sf)
	:	ScoringComponent(sf),
		possible_lipophilic_interactions_(),
		r1_offset_(0.0),
		r2_offset_(0.0)
{
	setName("Fresno Lipophilic");
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	setup();
}


Lipophilic::Lipophilic(const Lipophilic& fl)
	:	ScoringComponent(fl),
		possible_lipophilic_interactions_(fl.possible_lipophilic_interactions_),
		r1_offset_(fl.r1_offset_),
		r2_offset_(fl.r2_offset_)
{
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	setup();
}


Lipophilic::~Lipophilic()
{
	clear();
}


void Lipophilic::clear()
	throw()
{
	possible_lipophilic_interactions_.clear();
	r1_offset_ = 0.0;
	r2_offset_ = 0.0;
	delete ligand_fresno_types_;
	delete receptor_fresno_types_;
}


bool Lipophilic::setup()
{
	Timer timer;
	timer.start();

	ScoringFunction* sf = getScoringFunction();
	if (sf == 0)
	{
		Log.error() << "Lipophilic::setup(): "
			<< "component not bound to force field." << endl;
		return false;
	}

	//clear the vector of lipophilic interactions
	possible_lipophilic_interactions_.clear();

	Options options = sf->getOptions();

	r1_offset_
		 = options.setDefaultReal(Lipophilic::Option::LIPO_R1_OFFSET,
				Lipophilic::Default::LIPO_R1_OFFSET);
	r2_offset_
		 = options.setDefaultReal(Lipophilic::Option::LIPO_R2_OFFSET,
				Lipophilic::Default::LIPO_R2_OFFSET);
	write_interactions_file_
		 = options.setDefaultBool(Lipophilic::Option::CREATE_INTERACTIONS_FILE,
				Lipophilic::Default::CREATE_INTERACTIONS_FILE);
	verbosity_
		 = options.setDefaultInteger(Lipophilic::Option::VERBOSITY,
				Lipophilic::Default::VERBOSITY);

	delete receptor_fresno_types_;
	receptor_fresno_types_ = new FresnoTypes(getScoringFunction()->getReceptor());

	setupLigand();

	timer.stop();
	Log.info() << "Lipophilic::setup(): "
		<< timer.getCPUTime() << std::endl;

	return true;
}


void Lipophilic::setupLigand()
{
	delete ligand_fresno_types_;
	ligand_fresno_types_ = new FresnoTypes(getScoringFunction()->getLigand());
}


bool Lipophilic::isTypeKnown(Atom* atom)
{
	if (receptor_fresno_types_->getTypeMap()->has(atom)) return true;
	if (ligand_fresno_types_->getTypeMap()->has(atom)) return true;
	return false;
}


Size Lipophilic::getType(Atom* atom)
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
	return 0;
}



void Lipophilic::update(const AtomPairVector& pair_vector)
{
	possible_lipophilic_interactions_.clear();

	for (AtomPairVector::const_iterator it = pair_vector.begin(); it != pair_vector.end(); it++)
	{
		if (isTypeKnown(it->first))
		{
			if (getType(it->first) == FresnoTypes::LIPOPHILIC)
			{
				if (isTypeKnown(it->second))
				{
					if (getType(it->second) == FresnoTypes::LIPOPHILIC)
					{
						possible_lipophilic_interactions_.push_back(*it);
						if (verbosity_ >= 10)
						{
							Log.info() << "found possible lipophilic int.: "
									<< it->first->getFullName() << "..." << it->second->getFullName()
									<< " (length: "
									<< (it->first->getPosition() - it->second->getPosition()).getLength()
									<< " A) "
									<< endl;
						}
					}
				}
			}
		}
	}

	if (verbosity_ > 2)
	{
		Log.info() << "Lipophilic update statistics:" << endl;
		Log.info() << "Found " << possible_lipophilic_interactions_.size()
				<< " possible lipophilic interactions" << endl << endl;
	}
}



double Lipophilic::updateScore()
{
	Timer timer;
	timer.start();

	Molecule interactions_molecule;

	score_ = 0.0;
	float val = 0.0;
	float distance;
	float R1;
	float R2;
	const Atom* atom1;
	const Atom* atom2;


	for (AtomPairVector::const_iterator it = possible_lipophilic_interactions_.begin();
		it != possible_lipophilic_interactions_.end(); ++it)
	{
		atom1 = it->first;
		atom2 = it->second;

		R1 = atom1->getRadius() + atom2->getRadius() + r1_offset_;
		R2 = R1 + r2_offset_;

		distance = (atom1->getPosition() - atom2->getPosition()).getLength();

		// if the distance is too large, the product of g1 and g2 is zero, so
		// we can skip the rest

		if (distance <= R2)
		{
			// we could possibly speed up the next step by using the fact that the
			// difference between R1 and R2 is constant
			val = base_function_->calculate(distance, R1, R2);
			if (verbosity_ >= 1)
			{
				Log.info() << "LI: " << val << " "<< atom1->getFullName() << " " << endl;
				if (atom1->getResidue() != 0)
				{
					Log.info() << "[" << atom1->getResidue()->getID() << "]";
				}
				Log.info() << "..." << atom2->getFullName() << " " << endl;
				if (atom2->getResidue() != 0)
				{
					Log.info() << "[" << atom2->getResidue()->getID() << "]";
				}
				Log.info() << " (d " << distance << ", R1 " << R1
					<< ", R2 " << R2 << ")" << endl;
			}

			if (write_interactions_file_ == true)
			{
				Atom* atom_ptr_L1 = new Atom();
				atom_ptr_L1->setElement(atom1->getElement());
				atom_ptr_L1->setName("L1");
				atom_ptr_L1->setPosition(atom1->getPosition());
				atom_ptr_L1->setCharge(val);

				Atom* atom_ptr_L2 = new Atom();
				atom_ptr_L2->setElement(atom2->getElement());
				atom_ptr_L2->setName("L2");
				atom_ptr_L2->setPosition(atom2->getPosition());
				atom_ptr_L2->setCharge(val);

				atom_ptr_L1->createBond(*atom_ptr_L2);

				interactions_molecule.insert(*atom_ptr_L1);
				interactions_molecule.insert(*atom_ptr_L2);
			}

			score_ += val;
		}
	}

	if (write_interactions_file_ == true)
	{
		HINFile debug_file("LIPO_debug.hin", std::ios::out);
		debug_file << interactions_molecule;
		debug_file.close();
	}

	if (verbosity_ > 0)
	{
		Log.info() << "LIPO: score is " << score_ << endl;
	}

	timer.stop();

	if (verbosity_ > 1)
	{
		Log.info() << "Lipophilic::updateScore(): "
			<< timer.getCPUTime() << " s" << std::endl;
	}

	// we want a negative score for a good pose, thus we will use the negative of the value computed above
	score_ *= -1;

	/*
	scaleScore();
	return score_;
	*/

	return getScaledScore();
}
