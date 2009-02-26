// $Id: metal.C,v 1.3 2006/05/21 17:50:08 anker Exp $
// 

#include <BALL/SCORING/COMPONENTS/metal.h>

using namespace std;

namespace BALL
{

	const char* ChemScoreMetal::Option::METAL_R1_OFFSET = "metal_r1_offset";
	const char* ChemScoreMetal::Option::METAL_R2_OFFSET = "metalbp_r2_offset";
	const char* ChemScoreMetal::Option::CREATE_INTERACTIONS_FILE
		= "metal_create_interactions_file";
	const char* ChemScoreMetal::Option::VERBOSITY = "verbosity";

	const float ChemScoreMetal::Default::METAL_R1_OFFSET = 0.5;
	const float ChemScoreMetal::Default::METAL_R2_OFFSET = 3.0;
	const bool ChemScoreMetal::Default::CREATE_INTERACTIONS_FILE = false;
	const Size ChemScoreMetal::Default::VERBOSITY = 0;

	ChemScoreMetal::ChemScoreMetal()
		
		:	ScoringComponent()
			possible_metal_interactions_(),
			r1_(0.0),
			r2_(0.0)
	{
		// set component name
		setName("ChemScore Metal");
	}


	ChemScoreMetal::ChemScoreMetal(ScoringFunction& sf)
		
		:	ScoringComponent(sf),
			possible_metal_interactions_(),
			r1_(0.0),
			r2_(0.0)
	{
		// set component name
		setName("ChemScore Metal");
	}


	ChemScoreMetal::ChemScoreMetal(const ChemScoreMetal& csm)
		
		:	ScoringComponent(csm),
			possible_metal_interactions_(csm.possible_metal_interactions_),
			r1_(csm.r1_),
			r2_(csm.r2_)
	{
	}


	ChemScoreMetal::~ChemScoreMetal()
		
	{
		clear();
	}


	void ChemScoreMetal::clear()
		
	{
		possible_metal_interactions_.clear();
		r1_ = 0.0;
		r2_ = 0.0;
	}


	bool ChemScoreMetal::setup()
		
	{
		// clear the vector of buried polar interactions
		possible_metal_interactions_.clear();
    Options& options = force_field->options;

		r1_
			= options.setDefaultReal(ChemScoreMetal::Option::METAL_R1_OFFSET
					ChemScoreMetal::Default::METAL_R1_OFFSET);
		r2_
			= options.setDefaultReal(ChemScoreMetal::Option::METAL_R2_OFFSET
					ChemScoreMetal::Default::METAL_R2_OFFSET);
		write_interactions_file 
			= options.setDefaultBool(ChemScoreMetal::Option::CREATE_INTERACTIONS_FILE
					ChemScoreMetal::Default::CREATE_INTERACTIONS_FILE);
		verbosity_
			= options.setDefaultInteger(ChemScoreMetal::Option::VERBOSITY,
					ChemScoreMetal::Default::VERBOSITY);

		FresnoTypes fresno_types_class(*this);
		fresno_types = fresno_types_class.getTypeMap();

		// quadratic run time. not nice.

		const Molecule* receptor = getScoringFunction()->getReceptor();
		const Molecule* ligand = getScoringFunction()->getLigand();

		AtomConstIterator rec_it = receptor->beginAtom();
		AtomConstIterator lig_it;
		
		for (; +rec_it; ++rec_it)
		{
			if (fresno_types.has(&*rec_it))
			{
				if (fresno_types[&*rec_it] == SlickFF::METAL)
				{
					for (lig_it = ligand->beginAtom(); +lig_it; ++lig_it)
					{
						Size lig_type = fresno_types[&*lig_it];
						if ((lig_type == SlickFF::HBOND_ACCEPTOR)
									|| (lig_type == SlickFF::HBOND_ACCEPTOR_DONOR))
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

		if (verbosity_ >= 90)
		{
			Log.info() << "ChemScoreMetal setup statistics:" << endl;
			Log.info() << "Found " << possible_metal_interactions_.size() 
				<< " possible metal interactions" << endl << endl;
		}

		return true;

	}


	double ChemScoreMetal::updateEnergy()
		
	{

		energy_ = 0.0;
		float val = 0.0;
		float distance;
		float R1;
		float R2;
		const Atom* atom1;
		const Atom* atom2;

		Size verbosity 
			= getForceField()->options.setDefaultInteger(SlickFF::Option::VERBOSITY,
					SlickFF::Default::VERBOSITY);

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
				val = ((SlickFF*)getForceField())->base_function->calculate(distance, R1, R2);

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
		
	{
	}


}
