// $Id: chemScoreMetal.C,v 1.1.2.3 2002/11/22 15:11:26 anker Exp $
// 

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/chemScoreMetal.h>

using namespace std;

namespace BALL
{

	ChemScoreMetal::ChemScoreMetal()
		throw()
		:	ForceFieldComponent(),
			possible_metal_interactions_(),
			r1_(0.0),
			r2_(0.0)
	{
		// set component name
		setName("ChemScore Metal");
	}


	ChemScoreMetal::ChemScoreMetal(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			possible_metal_interactions_(),
			r1_(0.0),
			r2_(0.0)
	{
		// set component name
		setName("ChemScore Metal");
	}


	ChemScoreMetal::ChemScoreMetal(const ChemScoreMetal& csm, bool deep)
		throw()
		:	ForceFieldComponent(csm, deep),
			possible_metal_interactions_(csm.possible_metal_interactions_),
			r1_(csm.r1_),
			r2_(csm.r2_)
	{
	}


	ChemScoreMetal::~ChemScoreMetal()
		throw()
	{
		clear();
	}


	void ChemScoreMetal::clear()
		throw()
	{
		possible_metal_interactions_.clear();
		r1_ = 0.0;
		r2_ = 0.0;
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
	}


	bool ChemScoreMetal::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "ChemScoreMetal::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

		// clear the vector of buried polar interactions
		possible_metal_interactions_.clear();

		// ?????
		// we should check whether force_field is a FresnoFF, because we need
		// the fresno types

		System* system = force_field->getSystem();

		FresnoFF* fff = dynamic_cast<FresnoFF*>(force_field);

    Options& options = force_field->options;

		factor_
			= options.setDefaultReal(FresnoFF::Option::METAL,
					FresnoFF::Default::METAL);
		r1_
			= options.setDefaultReal(FresnoFF::Option::METAL_R1,
					FresnoFF::Default::METAL_R1);
		r2_
			= options.setDefaultReal(FresnoFF::Option::METAL_R2,
					FresnoFF::Default::METAL_R2);

		Size verbosity 
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		const HashMap<const Atom*, short>& fresno_types = fff->getFresnoTypes();

		// quadratic run time. not nice.

		const Molecule* receptor = &*system->beginProtein();
		const Molecule* ligand = system->getMolecule(0);
		if (ligand == receptor) ligand = system->getMolecule(1);

		AtomConstIterator rec_it = receptor->beginAtom();
		AtomConstIterator lig_it;
		
		for (; +rec_it; ++rec_it)
		{
			if (fresno_types.has(&*rec_it))
			{
				if (fresno_types[&*rec_it] == FresnoFF::METAL)
				{
					for (lig_it = ligand->beginAtom(); +lig_it; ++lig_it)
					{
						Size lig_type = fresno_types[&*lig_it];
						if ((lig_type == FresnoFF::HBOND_ACCEPTOR)
									|| (lig_type == FresnoFF::HBOND_ACCEPTOR_DONOR))
						{
							possible_metal_interactions_.push_back(pair<const Atom*,
									const Atom*>(&*rec_it, &*lig_it));
							if (verbosity >= 90)
							{
								Log.info() << "found possible metal int.: " 
									<< rec_it->getFullName() << "..." << lig_it->getFullName()
									<< " (length: " 
									<< (rec_it->getPosition() - lig_it->getPosition()).getLength() 
									<< " A) " 
									<< endl;
							}
						}
					}
				}
			}
			// PARANOIA
			else
			{
				Log.error() << "ChemScoreMetal::updateEnergy() "
					<< "atom not found in the fresno type list." << endl;
			}
			// /PARANOIA
		}

		if (verbosity >= 90)
		{
			Log.info() << "ChemScoreMetal setup statistics:" << endl;
			Log.info() << "Found " << possible_metal_interactions_.size() 
				<< " possible metal interactions" << endl << endl;
		}

		return true;

	}


	double ChemScoreMetal::updateEnergy()
		throw()
	{

		energy_ = 0.0;
		float val = 0.0;
		float distance;
		float R1;
		float R2;
		const Atom* atom1;
		const Atom* atom2;

		Size verbosity 
			= getForceField()->options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		::vector< pair<const Atom*, const Atom*> >::const_iterator it;
		for (it = possible_metal_interactions_.begin();
			it != possible_metal_interactions_.end();
			++it)
		{
			atom1 = it->first;
			atom2 = it->second;

			R1 = r1_;
			R2 = r2_;

			distance = (atom1->getPosition() - atom2->getPosition()).getLength();

			// if the distance is too large, the product of g1 and g2 is zero, so
			// we can skip the rest

			if (distance <= R2)
			{
				// we could possibly speed up the next step by using the fact that the
				// difference between R1 and R2 is constant
				val = MolmecSupport::calculateFresnoHelperFunction(distance, R1, R2);

				if (verbosity >= 90)
				{
					Log.info() << "METAL: adding score of " << val
						<< " (distance " << distance << ", R1 " << R1 << ", R2 " 
						<< R2 << ")" << endl;
				}

				energy_ += val;
			}
		}
		energy_ = factor_ * energy_;
		if (verbosity > 0)
		{
			Log.info() << "METAL: energy is " << energy_ << endl;
		}
		return energy_;
	}


	void ChemScoreMetal::updateForces()
		throw()
	{
	}


}
