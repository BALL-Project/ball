// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/polarity.h>
#include <BALL/SCORING/COMPONENTS/fresnoTypes.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/SYSTEM/timer.h>


using namespace std;
using namespace BALL;


const double Polarity::POL_ES_THRESHOLD = 0.2;
const double Polarity::LIP_ES_THRESHOLD = 0.1;


Polarity::Polarity(ScoringFunction& sf)
	:	ScoringComponent(sf)
{
	setName("Polarity");
	type_name_ = "pol";
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	gridable_ = false;
	atom_pairwise_ = 0;
	setup();
}


Polarity::Polarity(const Polarity& bp)
	:	ScoringComponent(bp)
{
	setName("Polarity");
	type_name_ = "pol";
	receptor_fresno_types_ = 0;
	ligand_fresno_types_ = 0;
	gridable_ = false;
	atom_pairwise_ = 0;
	setup();
}


Polarity::~Polarity()
{
	delete receptor_fresno_types_;
	delete ligand_fresno_types_;
}


void Polarity::clear()
{
}


bool Polarity::setup()
{
	ScoringFunction* sf = getScoringFunction();
	if (sf == 0)
	{
		Log.error() << "Polarity::setup(): "
			<< "component not bound to force field." << endl;
		return false;
	}

	delete receptor_fresno_types_;
	receptor_fresno_types_ = new FresnoTypes(getScoringFunction()->getReceptor());

	setupLigand();

	return true;
}


void Polarity::setupLigand()
{
	delete ligand_fresno_types_;
	ligand_fresno_types_ = new FresnoTypes(getScoringFunction()->getLigand());
}


Size Polarity::getType_(const Atom* atom)
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


void Polarity::update(const std::vector<std::pair<Atom*, Atom*> >& pair_vector)
{
	// nothing to be done here ...
}


bool Polarity::isPolar_(const Atom* atom)
{
	// if atom is polar, it should (besides the correct Fresno type) also have a significant charge
	if (fabs(atom->getCharge()) < POL_ES_THRESHOLD) return false;

	int type = getType_(atom);
	if ( (type == FresnoTypes::POLAR) || (type == FresnoTypes::HBOND_ACCEPTOR)
		|| (type == FresnoTypes::HBOND_DONOR) || (type == FresnoTypes::HBOND_ACCEPTOR_DONOR) || (type == FresnoTypes::HBOND_HYDROGEN) )
	{
		return true;
	}
	else return false;
}


bool Polarity::isLipophilic_(const Atom* atom)
{
	// if atom is lipophilic, it should (besides the correct Fresno type) also have no significant charge
	if (fabs(atom->getCharge()) > LIP_ES_THRESHOLD) return false;

	int type = getType_(atom);
	if (type == FresnoTypes::LIPOPHILIC)
	{
		return true;
	}
	else return false;
}


bool Polarity::isBackboneAtom_(const Atom* atom)
{
	if (atom->getName() == "O" || atom->getName() == "N" || atom->getName() == "H"
		|| atom->getName() == "C" /*|| atom->getName() == "CA"*/)
	{
		return true;
	}

	return false;
}


double Polarity::updateScore()
{
	score_ = 0.0;
	//float val = 0.0;
	//float distance;
	//float R1;
	//float R2;

	const HashGrid3<Atom*>* hashgrid = scoring_function_->getHashGrid();
	int radius = 1;
	Size no_neighbor_cells = (Size)pow((double)(radius*2+1), 3);  // radius of 1 cell == > 3 cells on each axis

	double total_sum = 0;
	AtomPairVector::const_iterator it;
	for (AtomIterator it = scoring_function_->getLigand()->beginAtom(); +it; it++)
	{
		int no_positive_contacts = 0;
		int no_negative_contacts = 0;
		bool ligandatom_is_lipophilic = isLipophilic_(&*it);

		if (!ligandatom_is_lipophilic)
		{
			continue;
		}

		const HashGridBox3<Atom*>* box = hashgrid->getBox(it->getPosition());

		// ligand atom lies outside of grid
		if (!box) continue;

		Position pos_x, pos_y, pos_z;
		hashgrid->getIndices(*box, pos_x, pos_y, pos_z);

		// indices in HashGrid, where the search for interacting target atoms should begin ( != position of ligand atom)
		int i = ((int)pos_x)-radius; if (i < 0){i = 0; }
		int j0 = ((int)pos_y)-radius; if (j0 < 0){j0 = 0; }
		int k0 = ((int)pos_z)-radius; if (k0 < 0){k0 = 0; }
		int x_size = (int)hashgrid->getSizeX();
		int y_size = (int)hashgrid->getSizeY();
		int z_size = (int)hashgrid->getSizeZ();

		for (; i <= pos_x+radius && i < x_size; i++)
		{
			for (int j = j0; j <= pos_y+radius && j < y_size; j++)
			{
				for (int k = k0; k <= pos_z+radius && k < z_size; k++)
				{
					const HashGridBox3<Atom*>* box = hashgrid->getBox(i, j, k);
					if (!box->isEmpty())
					{
						double cell_score = 0;

						for (HashGridBox3 < Atom* > ::ConstDataIterator d_it = box->beginData(); d_it != box->endData(); d_it++)
						{
							if (isBackboneAtom_(*d_it)) continue;
							bool rec_polar = isPolar_(*d_it);
							bool rec_lipophilic = 0;
							if (!rec_polar) rec_lipophilic = isLipophilic_(*d_it);

							if (!rec_polar && ! rec_lipophilic) continue;

							double distance = ((*d_it)->getPosition()-it->getPosition()).getLength();
							if (distance > (*d_it)->getElement().getVanDerWaalsRadius()+it->getElement().getVanDerWaalsRadius()+1.5) continue;

							double val;
							if (distance > 1) val = 1/distance;
							else val = 1;

							// lipophilic--lipophilic interaction; else polar rec. -- lipophilic ligand atom
							if (rec_lipophilic)
							{
								val *= -1;
							}

							cell_score += val;
							total_sum += val;

							if (scoring_function_->storeInteractionsEnabled())
							{
								val = scaleScore(val);
								it->addInteraction(*d_it, "pol", val);
								(*d_it)->addInteraction(&*it, "pol", val);
							}
						}

						if (cell_score < -0.1) no_positive_contacts++;
						else if (cell_score > 0.1) no_negative_contacts++;
					}
					// if there is no neighboring receptor atom, there will be water ...
// 					else if(i!=pos_x||j!=pos_y||k!=pos_z)
// 					{
// 						if(ligandatom_is_lipophilic)
// 						{
// 							no_negative_contacts++;
// 							double scaled_atom_score = 1.0/no_neighbor_cells;
// 							scaleScore(scaled_atom_score);
// 							total_sum += scaled_atom_score;
// 							it->addInteraction("pol",scaled_atom_score);
// 						}
// 					}
				}
			}
		}
		score_ += (no_negative_contacts-no_positive_contacts)/((double)no_neighbor_cells);
		//cout<<it->getFullName()<<" : "<<no_negative_contacts<<", "<<no_positive_contacts<<"  "<<(no_negative_contacts-no_positive_contacts)/((double)no_neighbor_cells)<<endl;
	}

// 	scaleScore();

//	cout<<"polarity: total sum="<<total_sum<<endl;
//	cout<<"polarity: score="<<score_<<endl;

// 	return score_;

	return getScaledScore();
}
