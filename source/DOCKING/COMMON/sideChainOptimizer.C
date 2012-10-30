// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/sideChainOptimizer.h>
#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/SYSTEM/timer.h>

using namespace std;

namespace BALL
{
	SideChainOptimizer::SideChainOptimizer(ScoringFunction* sf)
	{
		scoring_function_ = sf;
		frag_db_ = new FragmentDB("fragments/Fragments.db");
		rotamer_lib_ = new RotamerLibrary("rotamers/bbind02.May.lib", *frag_db_);
		residues_.clear();

		for (set<Residue*> ::const_iterator it = scoring_function_->flexible_residues_.begin();
			it!=scoring_function_->flexible_residues_.end(); it++)
		{
			residues_.push_back(*it);
		}
	}

	SideChainOptimizer::~SideChainOptimizer()
	{
		delete frag_db_;
		delete rotamer_lib_;
	}


	void SideChainOptimizer::setFlexibleResidues(const set<String>& residues_IDs)
	{
		residues_.clear();
		Protein* protein = dynamic_cast<Protein*>(scoring_function_->getReceptor());
		System* system;
		if (!protein && (system = dynamic_cast < System* > (scoring_function_->getReceptor())))
		{
			protein = dynamic_cast<Protein*>(system->getMolecule(0));
		}
		if (!protein)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "SideChainOptimizer::setFlexibleResidues() error", "Receptor is no Protein!");
		}
		set<Residue*> residue_set;
		for (ResidueIterator it = protein->beginResidue(); +it; it++)
		{
			String id = it->getID();
			if (residues_IDs.find(id) != residues_IDs.end())
			{
				residues_.push_back(&*it);
				residue_set.insert(&*it);
			}
		}
		scoring_function_->setFlexibleResidues(residue_set);
	}


	void SideChainOptimizer::optimize()
	{
		if (residues_.empty())
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "SideChainOptimizer::optimize() error", "No Residues to be optimized! Use setFlexibleResidues() first!");
		}

		Log.level(5)<<"Starting optimization of "<<residues_.size()<<" receptor residues ... "<<endl;
		Timer timer;
		timer.start();
		bool change = false;
		HashGrid3<Atom*>* hashgrid_backup = scoring_function_->hashgrid_;
		scoring_function_->hashgrid_ = scoring_function_->flexible_residues_hashgrid_;

		scoring_function_->ScoringFunction::update();
		scoring_function_->ScoringFunction::updateScore();
		double best_score = scoring_function_->getScore();
		list<pair<const Residue*, const Rotamer*> > changed_residues_list;
		list<pair<String, Size> > new_rotamers; // contains pairs of residue ID, rotamer ID

		for (list < Residue* > ::iterator it = residues_.begin(); it != residues_.end(); it++)
		{
			ResidueRotamerSet* rotamer_set = rotamer_lib_->getRotamerSet(**it);

			list<Vector3> old_positions;
			for (AtomIterator atom_it = (*it)->beginAtom(); +atom_it; atom_it++)
			{
				old_positions.push_back(atom_it->getPosition());
			}

			Size no_rotamers = 0;
			Size no_invalid = 0;
			// Test all rotamers for the current residue
			ResidueRotamerSet::ConstIterator best_rotamer;
			Size best_rotamer_id = 0;
			bool found_new_rotamer = false;
			for (ResidueRotamerSet::ConstIterator it2 = rotamer_set->begin(); it2 != rotamer_set->end(); it2++, no_rotamers++)
			{
				bool assignment_ok = scoring_function_->assignRotamer(*it, rotamer_set, &*it2);

				// residue could not be assigned due to overlaps
				if (!assignment_ok)
				{
					continue;
					no_invalid++;
				}

				scoring_function_->ScoringFunction::update();
				scoring_function_->ScoringFunction::updateScore();
				double score = scoring_function_->getScore();
				if (score < (best_score-0.01))
				{
					best_score = score;
					best_rotamer = it2;
					best_rotamer_id = no_rotamers;
					found_new_rotamer = true;
					change = true;
				}
			}

			if (no_invalid > 0)
			{
				cout<<no_invalid<<" out of "<<no_rotamers<<" rotamer could not be used due to overlaps."<<endl;
			}

			if (found_new_rotamer)
			{
				rotamer_set->setRotamer(**it, *best_rotamer);
				Log.level(5)<<"  new rotamer for "<<(*it)->getFullName()<<(*it)->getID()<<" found. new score = "<<best_score<<endl;
				changed_residues_list.push_back(make_pair(*it, &*best_rotamer));
				new_rotamers.push_back(make_pair((*it)->getID(), best_rotamer_id));
			}
			else
			{
				scoring_function_->resetResiduePositions(*it, old_positions);
				Log.level(5)<<"  no better rotamer for "<<(*it)->getFullName()<<(*it)->getID()<<" found."<<endl;
			}
		}

		String s = "";
		for (list < pair < String, Size > > ::iterator it = new_rotamers.begin(); it != new_rotamers.end(); )
		{
			s += it->first+":"+String(it->second);
			it++;
			if (it != new_rotamers.end()) s += ", ";
		}
		if (s != "")
		{
			scoring_function_->getLigand()->setProperty("receptor_rotamers", s);
		}

		timer.stop();
		Log.level(5)<<"Optimization of receptor residue finished after "<<timer.getClockTime()<<" seconds."<<endl;

		bool use_precalc_flexgrids = true;

		if (change)
		{
			GridBasedScoring* gbs = dynamic_cast<GridBasedScoring*>(scoring_function_);
			if (gbs)
			{
				if (!use_precalc_flexgrids)
				{
					gbs->precalculateGrids(true);
				}
				else
				{
					gbs->loadFlexibleResidueScoreGrids(changed_residues_list);
				}
			}
		}

		scoring_function_->hashgrid_ = hashgrid_backup;
	}


	void SideChainOptimizer::findFlexibleSideChains(set<Residue*>& residues, const double& dist_cutoff, const double& min_B_factor)
	{
		int overlaps = 0;
		double backup_nonbonded_cutoff_2 = scoring_function_->nonbonded_cutoff_2_;
		scoring_function_->nonbonded_cutoff_2_ = pow(dist_cutoff, 2);
		AtomPairVector* pv = scoring_function_->createNonbondedPairVector(scoring_function_->hashgrid_, overlaps, 1, 0, 0);
		scoring_function_->nonbonded_cutoff_2_ = backup_nonbonded_cutoff_2;

		// Add all residues within cutoff distance to any ref ligand atom
		residues.clear();
		for (AtomPairVector::iterator it = pv->begin(); it != pv->end(); it++)
		{
			Residue* residue = it->second->getResidue();
			String name = residue->getName();

			// Ignore small residues
			if (name != "GLY" && name != "ALA"
				&& name!="PRO" && name!="VAL")
			{
				residues.insert(residue);
			}
		}
		delete pv;
		Log.level(10)<<"Found "<<residues.size()<<" residues near reference ligand."<<endl;

		// Check temperature factor for selected residues
		for (set<Residue*> ::iterator it = residues.begin(); it != residues.end(); )
		{
			double b_factor = 0;
			int no_pdbatoms = 0;
			for (AtomIterator atom_it = (*it)->beginAtom(); +atom_it; atom_it++)
			{
				PDBAtom* pdbatom = dynamic_cast<PDBAtom*>(&*atom_it);
				if (!pdbatom) continue;
				b_factor += pdbatom->getTemperatureFactor();
				no_pdbatoms++;
			}
			b_factor /= no_pdbatoms;
			if (b_factor < min_B_factor)
			{
				set<Residue*>::iterator del_it = it;
				it++;
				residues.erase(del_it);
			}
			else it++;
		}
		Log.level(10)<<residues.size()<<" of those residues have a temperature factor >="<<min_B_factor<<endl;

		// Check whether there is at least one other possible rotamer for each selected rotamer, i.e. one that does not produce sterical clashes.
		for (set<Residue*> ::iterator it = residues.begin(); it != residues.end(); )
		{
			ResidueRotamerSet* rotamer_set = rotamer_lib_->getRotamerSet(**it);

			list<Vector3> old_positions;
			for (AtomIterator atom_it = (*it)->beginAtom(); +atom_it; atom_it++)
			{
				old_positions.push_back(atom_it->getPosition());
			}

			Size no_rotamers = 0;
			Size no_valid_rotamers = 0;

			// Test all rotamers for the current residue
			ResidueRotamerSet::ConstIterator best_rotamer;
			for (ResidueRotamerSet::ConstIterator it2 = rotamer_set->begin(); it2 != rotamer_set->end(); it2++, no_rotamers++)
			{
				bool assignment_ok = scoring_function_->assignRotamer(*it, rotamer_set, &*it2);
				if (assignment_ok)
				{
					double rmsd = 0;
					int no_atoms = 0;

					// check whether the applied rotamer if any different for the one observed in the crystal structure
					list<Vector3>::iterator list_it = old_positions.begin();
					for (AtomIterator atom_it = (*it)->beginAtom(); +atom_it; atom_it++, list_it++)
					{
						rmsd = pow(atom_it->getPosition().getDistance(*list_it), 2);
						no_atoms++;
					}
					rmsd /= no_atoms;
					rmsd = sqrt(rmsd);

					if (rmsd > 0.25)
					{
						no_valid_rotamers++;
					}
				}
			}

			scoring_function_->resetResiduePositions(*it, old_positions);

			if (no_valid_rotamers == 0)
			{
				set<Residue*>::iterator del_it = it;
				it++;
				residues.erase(del_it);
			}
			else it++;
		}

		Log.level(10)<<"For "<<residues.size()<<" of those residues, at least one rotamer different from that observed in the crystal structure exists."<<endl;
	}
}
