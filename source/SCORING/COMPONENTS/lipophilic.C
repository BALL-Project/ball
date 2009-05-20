// $Id: lipophilic.C,v 1.3 2006/05/21 17:38:40 anker Exp $
// lipophilic component

#include <BALL/SCORING/COMPONENTS/lipophilic.h>
#include <BALL/SCORING/TYPES/fresnoTypes.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/FORMAT/HINFile.h>

using namespace std;

namespace BALL
{

	const char* Lipophilic::Option::LIPO_R1_OFFSET = "lipo_r1_offset";
	const char* Lipophilic::Option::LIPO_R2_OFFSET = "lipo_r2_offset";
	const char* Lipophilic::Option::CREATE_INTERACTIONS_FILE 
		= "LIPO_create_interactions_file"; 
	const char* Lipophilic::Option::VERBOSITY = "verbosity";

	const float Lipophilic::Default::LIPO_R1_OFFSET = 0.5;
	const float Lipophilic::Default::LIPO_R2_OFFSET = 3.0;
	const bool Lipophilic::Default::CREATE_INTERACTIONS_FILE = false;
	const Size Lipophilic::Default::VERBOSITY = 0;


	Lipophilic::Lipophilic()
		
		:	ScoringComponent(),
			possible_lipophilic_interactions_(),
			r1_offset_(0.0),
			r2_offset_(0.0)
	{
		// set component name
		setName("Fresno Lipophilic");
	}


	Lipophilic::Lipophilic(ScoringFunction& sf)
		
		:	ScoringComponent(sf),
			possible_lipophilic_interactions_(),
			r1_offset_(0.0),
			r2_offset_(0.0)
	{
		// set component name
		setName("Fresno Lipophilic");
	}


	Lipophilic::Lipophilic(const Lipophilic& fl)
		
		:	ScoringComponent(fl),
			possible_lipophilic_interactions_(fl.possible_lipophilic_interactions_),
			r1_offset_(fl.r1_offset_),
			r2_offset_(fl.r2_offset_)
	{
	}


	Lipophilic::~Lipophilic()
		
	{
		clear();
	}


	void Lipophilic::clear()
		
	{
		possible_lipophilic_interactions_.clear();
		r1_offset_ = 0.0;
		r2_offset_ = 0.0;
		// ?????
		// ScoringComponent does not comply with the OCI
		// ScoringComponent::clear();
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

		// clear the vector of lipophilic interactions
		possible_lipophilic_interactions_.clear();

    Options& options = sf->options;

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

		FresnoTypes fresno_types_class(*this);
		const HashMap<const Atom*, Size>& fresno_types 
			= fresno_types_class.getTypeMap();

		// quadratic run time. not nice.

		Molecule* A = getScoringFunction()->getReceptor();
		Molecule* B = getScoringFunction()->getLigand();

		AtomConstIterator A_it = A->beginAtom();
		AtomConstIterator B_it;
		
		Size type_A;
		Size type_B;

		for (; +A_it; ++A_it)
		{
			if (fresno_types.has(&*A_it))
			{
				type_A = fresno_types[&*A_it];
				if (type_A == FresnoTypes::LIPOPHILIC)
				{
					for (B_it = B->beginAtom(); +B_it; ++B_it)
					{
						type_B = fresno_types[&*B_it];
						if (type_B == FresnoTypes::LIPOPHILIC)
						{
							possible_lipophilic_interactions_.push_back(pair<const Atom*, const Atom*>(&*A_it, &*B_it));
							if (verbosity_ >= 10)
							{
								Log.info() << "found possible lipophilic int.: " 
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

		if (verbosity_ > 2)
		{
			Log.info() << "Lipophilic setup statistics:" << endl;
			Log.info() << "Found " << possible_lipophilic_interactions_.size() 
				<< " possible lipophilic interactions" << endl << endl;
		}

		timer.stop();
		Log.info() << "Lipophilic::setup(): " 
			<< timer.getCPUTime() << std::endl;

		return(true);

	}


	double Lipophilic::calculateScore()
		
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

		// iterate over all possible hydrogen bond pairs
		::vector< pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_lipophilic_interactions_.begin();
			it != possible_lipophilic_interactions_.end();
			++it)
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
				val = getScoringFunction()->getBaseFunction()->calculate(distance, R1, R2);
				if (verbosity_ >= 1)
				{
					Log.info() << "LI: " << val << " "
						<< atom1->getFullName() << " " 
						<< endl;
//						<< ((SlickFF*)getForceField())->getFresnoTypeString(atom1);
					if (atom1->getResidue() != 0)
					{
						Log.info() << "[" << atom1->getResidue()->getID() << "]";
					}
					Log.info() << "..." << atom2->getFullName() << " "
						<< endl;
//						<< ((SlickFF*)getForceField())->getFresnoTypeString(atom2);
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
			Log.info() << "LIPO: energy is " << score_ << endl;
		}

		timer.stop();

		if (verbosity_ > 1)
		{
			Log.info() << "Lipophilic::updateEnergy(): "
				<< timer.getCPUTime() << " s" << std::endl;
		}

		return(score_);
	}

}
