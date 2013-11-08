// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/diffGridBasedScoring.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>


using namespace BALL;


DiffGridBasedScoring::DiffGridBasedScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: GridBasedScoring(receptor, ligand, options)
{
	all_ligand_nonbonded_ = 0;
}


DiffGridBasedScoring::DiffGridBasedScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: GridBasedScoring(receptor, hashgrid_origin, options)
{
	all_ligand_nonbonded_ = 0;
}


DiffGridBasedScoring::~DiffGridBasedScoring()
{
	delete all_ligand_nonbonded_;
}


void DiffGridBasedScoring::setLigand(AtomContainer* sys)
{
	ScoringFunction::setSecondMolecule(*sys);
	//force_field_->setup(ligand_, options_); // calls ForceField::setup() and setup() of each component
}


// overloads ScoringFunction::update() so that precalculated receptor-ligand ScoringComponents will not be updated (because grid will be used instead)
void DiffGridBasedScoring::update()
{
	overlaps_ = 0;
	ligand_intramol_overlaps_ = 0;
	AtomPairVector* ligand_nonbonded = NULL;
	bool update_ligand_nonbonded = 0;

	// Make sure to update pairlists for all atoms, including those belonging to flexible residues.
	HashGrid3<Atom*>* backup_grid = 0;
	if (!flexible_residues_.empty())
	{
		backup_grid = hashgrid_;
		hashgrid_ = all_residues_hashgrid_;
	}

	// Check for ligand intramolecular atom clashes
	if (all_ligand_nonbonded_ != NULL)
	{
		// all_ligand_nonbonded_ contains only interfragment pairs
		if (static_ligand_fragments_.size() > 0)
		{
			ligand_intramol_overlaps_ += checkForAtomOverlaps(all_ligand_nonbonded_);
		}
		else
		{
			ligand_nonbonded = createLigandNonbondedPairVector(0, ligand_intramol_overlaps_);
		}
	}
	if (all_ligand_nonbonded_ == NULL) // calculate ligand nonbonded pairs
	{
		ligand_nonbonded = createLigandNonbondedPairVector(0, ligand_intramol_overlaps_);
		update_ligand_nonbonded = 1;
	}


	AtomPairVector empty_vector(0);
	AtomPairVector* receptor_ligand = NULL; // will be used only if there are receptor-ligand components that could not be precalculated

	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if (!(*it)->isEnabled()) continue;

		if ((*it)->isLigandIntraMolecular())
		{
			if (update_ligand_nonbonded)
			{
				(*it)->update(*ligand_nonbonded);
			}
			else if (!(*it)->isAtomPairwise())
			{
				(*it)->update(empty_vector);
			}
		}
		else
		{
			if ((*it)->isGridable())
			{
				(*it)->update(empty_vector);
			}
			else
			{
				if ((*it)->isAtomPairwise())
				{
					if (receptor_ligand == NULL)
					{
						receptor_ligand = createNonbondedPairVector(hashgrid_, overlaps_, 1);
					}
					(*it)->update(*receptor_ligand);
				}
				else
				{
					(*it)->update(empty_vector);
				}
			}
		}
	}

	delete ligand_nonbonded;
	delete receptor_ligand;

	if (!flexible_residues_.empty())
	{
		hashgrid_ = backup_grid;
	}
}



void DiffGridBasedScoring::testOverlaps(Vector3& position, HashGrid3 < Atom* > * hashg)
{
	Atom a; a.setPosition(position);
	a.setType(1);
	a.setTypeName("C");
	Element e = PTE_::getElement("C");

	a.setElement(e);
	Molecule m;
	setLigand(&m);

	int overlaps = 0;
	if (hashg == NULL) hashg = hashgrid_;
	AtomPairVector* nonbonded_pairs = createNonbondedPairVector(hashg, overlaps, 1);

	std::cout<<e.getVanDerWaalsRadius()<<std::endl;
	std::cout<<overlaps<<" atom overlaps were found at the given position"<<std::endl;
	std::cout<<"neighboring_target_atoms_ = "<<neighboring_target_atoms_<<std::endl;

// 	update();
// 	updateScore();
//
// 	cout<<"ScoreGridSet value at given position: "<<grid_sets_[0]->getGridScore(4,position,0)<<endl;
// 	cout<<"neighboring atoms according to grid: "<<grid_sets_[0]->getGridScore(1,position,0)<<endl;
//
//
// 	update();
// 	updateScore();
// 	cout<<"score = "<<getScore()<<endl;
	delete nonbonded_pairs;
}



void DiffGridBasedScoring::updatePrecalculatedScore(Size set)
{
	if (grid_sets_.size() < set)
	{
		String s = "ScoreGridSet no" + String(set) + " has not been defined!";
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "DiffGridBasedScoring::updatePrecalculatedScore() error", s);
	}
	if (grid_sets_[set]->getHashGrid() == NULL)
	{
		String s = "The HashGrid of ScoreGridSet no" + String(set) + " has not been created yet!";
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "DiffGridBasedScoring::updatePrecalculatedScore() error", s);
	}

	/// use ONLY receptor-ligand nonbonded pairs
	int overlaps = 0;
	AtomPairVector* reclig_nonbonded = createNonbondedPairVector(grid_sets_[set]->getHashGrid(), overlaps, 1);

	score_ = 0;
	if (overlaps == 0)  /// explicit check for sterical clashes!
	{
		for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
		{
			if (!(*it)->isEnabled())
			{
				continue;
			}

			if (!(*it)->isLigandIntraMolecular() && (*it)->isGridable())
			{
				(*it)->update(*reclig_nonbonded);
				score_ += (*it)->updateScore();
			}
		}
	}
	else
	{
		score_ = overlaps*1e10;
	}

	delete reclig_nonbonded;
}


// Overloads ScoringFunction::updateScore() so that the grid score is used to asses the current receptor-ligand interaction
double DiffGridBasedScoring::updateScore()
{
	if (!store_interactions_ && hasPharmacophoreConstraints_())
	{
		std::cout<<"[info:] enabling storing interaction with PharmacophoreConstraint-residues"<<std::endl;
		enableStoreInteractionsOnlyForPhContraints();
	}
	if (store_interactions_) clearStoredInteractions_();

	/// receptor-ligand contribution, _including_ penalty for atoms outside the ScoreGrid and scaling of the score accoriding to the depth of burial of the ligand candidate
	result_.interaction_score = calculateGridScore();

	/// ligand conformation contribution
	double conf_energy = 0;
	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); it++)
	{
		if (!(*it)->isEnabled()) continue;

		if ((*it)->isLigandIntraMolecular())
		{
			conf_energy += (*it)->updateScore();
		}

		// rec-lig components that could not be precalculated
		else if (!(*it)->isGridable())
		{
			result_.interaction_score += (*it)->updateScore();
		}
	}

	if (static_ligand_fragments_.size() != 0)
	{
		conf_energy += static_ligand_energy_;
	}

	result_.ligand_conformation = conf_energy;
	if (conf_energy < 0)
	{
		conf_energy *= conformation_scale_;
	}

	score_ = conf_energy + result_.interaction_score;

	// backtransform
	if (exp_energy_stddev_ > 0.01) score_ *= exp_energy_stddev_;
	score_ += exp_energy_mean_;

	/// add penalty if ligand candidate lies outside of some ReferenceAreas (if any have been defined by the user)
	score_ += calculateConstraintsScore();


	/// explicit check for sterical clashes!
	if (overlaps_ > 0)
	{
		score_ += overlaps_*1e10;
		result_.interaction_score += overlaps_*1e10;

	}
	if (ligand_intramol_overlaps_ > 0)
	{
		result_.ligand_conformation += ligand_intramol_overlaps_*1e10;
		score_ += ligand_intramol_overlaps_*1e10;
	}

	return score_;
}
