// $Id: fresnoBuriedPolar.C,v 1.1.2.16 2005/01/30 14:00:45 anker Exp $
// Molecular Mechanics: Fresno force field, buried polar component

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoBuriedPolar.h>

#include <BALL/SYSTEM/timer.h>

#define DEBUG 1

#ifdef DEBUG
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{

	FresnoBuriedPolar::FresnoBuriedPolar()
		throw()
		:	ForceFieldComponent(),
			possible_buried_polar_interactions_(),
			r1_offset_(0.0),
			r2_offset_(0.0)
	{
		// set component name
		setName("Fresno BuriedPolar");
	}


	FresnoBuriedPolar::FresnoBuriedPolar(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			possible_buried_polar_interactions_(),
			r1_offset_(0.0),
			r2_offset_(0.0)
	{
		// set component name
		setName("Fresno BuriedPolar");
	}


	FresnoBuriedPolar::FresnoBuriedPolar(const FresnoBuriedPolar& fl)
		throw()
		:	ForceFieldComponent(fl),
			possible_buried_polar_interactions_(fl.possible_buried_polar_interactions_),
			r1_offset_(fl.r1_offset_),
			r2_offset_(fl.r2_offset_)
	{
	}


	FresnoBuriedPolar::~FresnoBuriedPolar()
		throw()
	{
		clear();
	}


	void FresnoBuriedPolar::clear()
		throw()
	{
		possible_buried_polar_interactions_.clear();
		r1_offset_ = 0.0;
		r2_offset_ = 0.0;
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
	}


	bool FresnoBuriedPolar::setup()
		throw()
	{
		Timer timer;
		timer.start();

		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoBuriedPolar::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// clear the vector of buried polar interactions
		possible_buried_polar_interactions_.clear();

		// ?????
		// we should check whether force_field is a FresnoFF, because we need
		// the fresno types

		System* system = force_field->getSystem();

		FresnoFF* fff = dynamic_cast<FresnoFF*>(force_field);

    Options& options = force_field->options;

		factor_
			= options.setDefaultReal(FresnoFF::Option::BP,
					FresnoFF::Default::BP);
		r1_offset_
			= options.setDefaultReal(FresnoFF::Option::BP_R1_OFFSET,
					FresnoFF::Default::BP_R1_OFFSET);
		r2_offset_
			= options.setDefaultReal(FresnoFF::Option::BP_R2_OFFSET,
					FresnoFF::Default::BP_R2_OFFSET);
		Size verbosity
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

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
			type_A = fresno_types[&*A_it];
			for (B_it = B->beginAtom(); +B_it; ++B_it)
			{
				type_B = fresno_types[&*B_it];
				if ((((type_B == FresnoFF::POLAR)
								|| (type_B == FresnoFF::HBOND_ACCEPTOR)
								|| (type_B == FresnoFF::HBOND_DONOR)
								|| (type_B == FresnoFF::HBOND_ACCEPTOR_DONOR)
								|| (type_B == FresnoFF::HBOND_HYDROGEN))
							&& (type_A == FresnoFF::LIPOPHILIC))
						|| ((type_B == FresnoFF::LIPOPHILIC)
							&& ((type_A == FresnoFF::POLAR)
								|| (type_A == FresnoFF::HBOND_ACCEPTOR)
								|| (type_A == FresnoFF::HBOND_DONOR)
								|| (type_A == FresnoFF::HBOND_ACCEPTOR_DONOR)
								|| (type_A == FresnoFF::HBOND_HYDROGEN))))
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
			Log.info() << "FresnoBuriedPolar setup statistics:" << endl;
			Log.info() << "Found " << possible_buried_polar_interactions_.size() 
				<< " possible buried polar interactions" << endl << endl;
		}

		timer.stop();
		Log.info() << "FresnoBuriedPolar::setup(): " << timer.getCPUTime() << " s"
			<< std::endl;

		return(true);

	}


	double FresnoBuriedPolar::updateEnergy()
		throw()
	{

		Timer timer;
		timer.start();

		#ifdef DEBUG
		Molecule debug_molecule;
		#endif

		Size verbosity
			= getForceField()->options.getInteger(FresnoFF::Option::VERBOSITY);

		energy_ = 0.0;
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
				val = ((FresnoFF*)getForceField())->base_function->calculate(distance, R1, R2);

				if (verbosity >= 0)
				{
					Log.info() << "BP: " << val << " "
						<< atom1->getFullName() << " " 
						<< ((FresnoFF*)getForceField())->getFresnoTypeString(atom1);
					if (atom1->getResidue() != 0)
					{
						Log.info() << "[" << atom1->getResidue()->getID() << "]";
					}
					Log.info() << "..." << atom2->getFullName() << " "
						<< ((FresnoFF*)getForceField())->getFresnoTypeString(atom2);
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
				energy_ += val;
			}
		}

#ifdef DEBUG
		HINFile debug_file("BP_debug.hin", std::ios::out);
		debug_file << debug_molecule;
		debug_file.close();
#endif

		energy_ = factor_ * energy_;

		if (verbosity > 0)
		{
			Log.info() << "BP: energy is " << energy_ << endl;
		}

		timer.stop();
		Log.info() << "FresnoBuriedPolar::updateEnergy(): " 
			<< timer.getCPUTime() << " s" << std::endl;

		return energy_;

	}


	void FresnoBuriedPolar::updateForces()
		throw()
	{
	}


}
