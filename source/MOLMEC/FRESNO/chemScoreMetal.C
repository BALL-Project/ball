// $Id: chemScoreMetal.C,v 1.1.2.1 2002/04/20 12:26:01 anker Exp $
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
									&& (lig_type == FresnoFF::HBOND_ACCEPTOR_DONOR))
						{
							possible_metal_interactions_.push_back(pair<const Atom*,
									const Atom*>(&*rec_it, &*lig_it));
							// DEBUG
							cout << "found possible metal int.: " 
								<< rec_it->getFullName() << "..." << lig_it->getFullName()
								<< " (length: " 
								<< (rec_it->getPosition() - lig_it->getPosition()).getLength() 
								<< " A) " 
								<< endl;
							// /DEBUG
						}
					}
				}
			}
		}

		// DEBUG
		cout << "ChemScoreMetal setup statistics:" << endl;
		cout << "Found " << possible_metal_interactions_.size() 
			<< " possible metal interactions" << endl << endl;
		// /DEBUG

		return true;

	}


	double ChemScoreMetal::updateEnergy()
		throw()
	{

		double E = 0.0;
		double val = 0.0;
		double distance;
		double R1;
		double R2;
		const Atom* atom1;
		const Atom* atom2;

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

				// DEBUG
				cout << "METAL: adding score of " << val
					<< " (distance " << distance << ", R1 " << R1 << ", R2 " << R2 << ")"
					<< endl;
				// /DEBUG

				E += val;
			}
		}
		energy_ = factor_ * E;
		// DEBUG
		cout << "METAL: score is " << E << endl;
		cout << "METAL: energy is " << energy_ << endl;
		// /DEBUG
		return energy_;
	}


	void ChemScoreMetal::updateForces()
		throw()
	{
	}


}
