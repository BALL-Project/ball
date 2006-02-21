// $Id: buriedPolar.C,v 1.2 2006/02/21 16:14:19 anker Exp $
// Molecular Mechanics: Fresno force field, buried polar component

#include <BALL/SCORING/COMPONENTS/buriedPolar.h>
#include <BALL/SCORING/TYPES/fresnoTypes.h>

#include <BALL/SYSTEM/timer.h>

#define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{

	BuriedPolar::BuriedPolar()
		throw()
		:	ScoringComponent(),
			possible_buried_polar_interactions_(),
			r1_offset_(0.0),
			r2_offset_(0.0)
	{
		// set component name
		setName("Fresno BuriedPolar");
	}


	BuriedPolar::BuriedPolar(ScoringFunction& sf)
		throw()
		:	ScoringComponent(sf),
			possible_buried_polar_interactions_(),
			r1_offset_(0.0),
			r2_offset_(0.0)
	{
		// set component name
		setName("Fresno BuriedPolar");
	}


	BuriedPolar::BuriedPolar(const BuriedPolar& bp)
		throw()
		:	ScoringComponent(bp),
			possible_buried_polar_interactions_(bp.possible_buried_polar_interactions_),
			r1_offset_(bp.r1_offset_),
			r2_offset_(bp.r2_offset_)
	{
	}


	BuriedPolar::~BuriedPolar()
		throw()
	{
		clear();
	}


	void BuriedPolar::clear()
		throw()
	{
		possible_buried_polar_interactions_.clear();
		r1_offset_ = 0.0;
		r2_offset_ = 0.0;
		// ?????
		// ScoringComponent does not comply with the OCI
		// ScoringComponent::clear();
	}


	bool BuriedPolar::setup()
		throw()
	{
		Timer timer;
		timer.start();

		ScoringFunction* sf = getScoringFunction();
		if (sf == 0)
		{
			Log.error() << "BuriedPolar::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// clear the vector of buried polar interactions
		possible_buried_polar_interactions_.clear();

		// ?????
		// we should check whether force_field is a SlickFF, because we need
		// the fresno types

		System* system = getScoringFunction()->getSystem();
		if (system == 0)
		{
			return(false);
		}

    Options& options = getScoringFunction()->options;

		r1_offset_
			= options.setDefaultReal(BuriedPolar::Option::BP_R1_OFFSET,
					BuriedPolar::Default::BP_R1_OFFSET);
		r2_offset_
			= options.setDefaultReal(BuriedPolar::Option::BP_R2_OFFSET,
					BuriedPolar::Default::BP_R2_OFFSET);
		Size verbosity
			= options.setDefaultInteger(BuriedPolar::Option::VERBOSITY,
					BuriedPolar::Default::VERBOSITY);

		FresnoTypes fresno_types_class(*this);
		const HashMap<const Atom*, Size>& fresno_types 
			= fresno_types_class.getTypeMap();

		// quadratic run time. not nice.

		Molecule* A = system->getMolecule(0);
		Molecule* B = system->getMolecule(1);

		AtomConstIterator A_it = A->beginAtom();
		AtomConstIterator B_it;
		
		Size type_A;
		Size type_B;

		for (; +A_it; ++A_it)
		{
			type_A = fresno_types[&*A_it];
			for (B_it = B->beginAtom(); +B_it; ++B_it)
			{
				type_B = fresno_types[&*B_it];
				if ((((type_B == FresnoTypes::POLAR)
								|| (type_B == FresnoTypes::HBOND_ACCEPTOR)
								|| (type_B == FresnoTypes::HBOND_DONOR)
								|| (type_B == FresnoTypes::HBOND_ACCEPTOR_DONOR)
								|| (type_B == FresnoTypes::HBOND_HYDROGEN))
							&& (type_A == FresnoTypes::LIPOPHILIC))
						|| ((type_B == FresnoTypes::LIPOPHILIC)
							&& ((type_A == FresnoTypes::POLAR)
								|| (type_A == FresnoTypes::HBOND_ACCEPTOR)
								|| (type_A == FresnoTypes::HBOND_DONOR)
								|| (type_A == FresnoTypes::HBOND_ACCEPTOR_DONOR)
								|| (type_A == FresnoTypes::HBOND_HYDROGEN))))
				{
					possible_buried_polar_interactions_.push_back(pair<const Atom*, const Atom*>(&*A_it, &*B_it));
					if (verbosity >= 90)
					{
						Log.info() << "found possible buried polar int.: " 
							<< A_it->getFullName() << "..." << B_it->getFullName()
							<< " (length: " 
							<< (A_it->getPosition() - B_it->getPosition()).getLength() 
							<< " A) " 
							<< endl;
					}
				}
			}
		}

		if (verbosity > 8)
		{
			Log.info() << "BuriedPolar setup statistics:" << endl;
			Log.info() << "Found " << possible_buried_polar_interactions_.size() 
				<< " possible buried polar interactions" << endl << endl;
		}

		timer.stop();
		Log.info() << "BuriedPolar::setup(): " << timer.getCPUTime() << " s"
			<< std::endl;

		return(true);

	}


	double BuriedPolar::calculateScore()
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
		float R1;
		float R2;
		const Atom* atom1;
		const Atom* atom2;

		::vector< pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_buried_polar_interactions_.begin();
			it != possible_buried_polar_interactions_.end();
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

				if (verbosity >= 0)
				{
					Log.info() << "BP: " << val << " "
						<< atom1->getFullName() << " " 
						<< endl;
//						<< fresno_types_class.getFresnoTypeString(atom1);
					if (atom1->getResidue() != 0)
					{
						Log.info() << "[" << atom1->getResidue()->getID() << "]";
					}
					Log.info() << "..." << atom2->getFullName() << " "
						<< endl;
//						<< fresno_types_class.getFresnoTypeString(atom2);
					if (atom2->getResidue() != 0)
					{
						Log.info() << "[" << atom2->getResidue()->getID() << "]";
					}
					Log.info() << " (d " << distance << ", R1 " << R1 
						<< ", R2 " << R2 << ")" << endl;
				}

#ifdef DEBUG
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

				debug_molecule.insert(*atom_ptr_L1);
				debug_molecule.insert(*atom_ptr_L2);
#endif
				score_ += val;
			}
		}

#ifdef DEBUG
		HINFile debug_file("BP_debug.hin", std::ios::out);
		debug_file << debug_molecule;
		debug_file.close();
#endif

		if (verbosity > 0)
		{
			Log.info() << "BP: energy is " << score_ << endl;
		}

		timer.stop();
		Log.info() << "BuriedPolar::updateEnergy(): " 
			<< timer.getCPUTime() << " s" << std::endl;

		return score_;

	}

}
