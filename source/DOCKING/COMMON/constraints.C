// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/constraints.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/KERNEL/molecularInteractions.h>

using namespace std;

namespace BALL
{
	Constraint::Constraint()
	{
		scoring_function_ = NULL;
		name_ = "";
		type_ = "";
	}

	Constraint::~Constraint()
	{

	}

	void Constraint::setName(String name)
	{
		name_ = name;
	}

	BALL::String Constraint::getName()
	{
		return name_;
	}

	BALL::String Constraint::getType()
	{
		return type_;
	}

	void Constraint::setScoringFunction(ScoringFunction* sf)
	{
		scoring_function_ = sf;
	}

	ReferenceArea::ReferenceArea()
	{
		T_.setIdentity();
		ref_atoms_ = 0;
		name_ = "";
		scale_by_size_ = false;
	}


	ReferenceArea::ReferenceArea(Vector3& p1, Vector3& p2,  Vector3& p3, Vector3& p4, bool atom_fraction, double atoms, double penalty)
	{
		type_ = "ReferenceArea";
		name_ = "";
		scale_by_size_ = false;
		number_reflig_atoms_ = 0;
		ref_atoms_ = atoms;
		atom_fraction_ = atom_fraction;
		penalty_ = penalty;
		T_.setIdentity();

		input_points_.push_back(p1);
		input_points_.push_back(p2);
		input_points_.push_back(p3);
		input_points_.push_back(p4);

		double d12 = p1.getDistance(p2);
		double d23 = p2.getDistance(p3);
		Vector3 g0(0, 0, 0);
		Vector3 g1(d12, 0, 0);
		Vector3 g2(d12, d23, 0);

		T_ = StructureMapper::matchPoints(p1, p2, p3, g0, g1, g2);

		//now set min and max values for each axis...
		min_x_ = 1e100; min_y_ = 1e100; min_z_ = 1e100;
		max_x_ = -1e100; max_y_ = -1e100; max_z_ = -1e100;
		Vector3 tmp = T_*p1;
		if (tmp[0] < min_x_) min_x_ = tmp[0];
		if (tmp[0] > max_x_) max_x_ = tmp[0];
		if (tmp[1] < min_y_) min_y_ = tmp[1];
		if (tmp[1] > max_y_) max_y_ = tmp[1];
		if (tmp[2] < min_z_) min_z_ = tmp[2];
		if (tmp[2] > max_z_) max_z_ = tmp[2];
		tmp = T_*p2;
		if (tmp[0] < min_x_) min_x_ = tmp[0];
		if (tmp[0] > max_x_) max_x_ = tmp[0];
		if (tmp[1] < min_y_) min_y_ = tmp[1];
		if (tmp[1] > max_y_) max_y_ = tmp[1];
		if (tmp[2] < min_z_) min_z_ = tmp[2];
		if (tmp[2] > max_z_) max_z_ = tmp[2];
		tmp = T_*p3;
		if (tmp[0] < min_x_) min_x_ = tmp[0];
		if (tmp[0] > max_x_) max_x_ = tmp[0];
		if (tmp[1] < min_y_) min_y_ = tmp[1];
		if (tmp[1] > max_y_) max_y_ = tmp[1];
		if (tmp[2] < min_z_) min_z_ = tmp[2];
		if (tmp[2] > max_z_) max_z_ = tmp[2];
		tmp = T_*p4;
		if (tmp[0] < min_x_) min_x_ = tmp[0];
		if (tmp[0] > max_x_) max_x_ = tmp[0];
		if (tmp[1] < min_y_) min_y_ = tmp[1];
		if (tmp[1] > max_y_) max_y_ = tmp[1];
		if (tmp[2] < min_z_) min_z_ = tmp[2];
		if (tmp[2] > max_z_) max_z_ = tmp[2];
	}


	ReferenceArea::ReferenceArea(Vector3& p1, Vector3& p2, int y_expansion, int z_expansion, bool atom_fraction, double atoms, double penalty)
	{
		type_ = "ReferenceArea";
		name_ = "";
		scale_by_size_ = false;
		number_reflig_atoms_ = 0;
		ref_atoms_ = atoms;
		atom_fraction_ = atom_fraction;
		penalty_ = penalty;
		T_.setIdentity();

		input_points_.push_back(p1);
		input_points_.push_back(p2);
		input_points_.push_back(p2+Vector3(0, y_expansion, 0));
		input_points_.push_back(p2+Vector3(0, y_expansion, z_expansion));

		Vector3 x0(0, 0, 0);
		Vector3 x1(1, 0, 0);
		Vector3 v1 = p2-p1;

		// create plane out of one edge of the specified cuboid and the global x-axis
		TPlane3<float> plane(v1, x0, x1);

		// calculate the angle by which v1 has to be rotated in order to be aligned with the global x-axis
		Angle a = -x1.getAngle(v1);

		// rotations will have to be done around the normal vector of the calculated plane
		if (a.toDegree() != 0)
		{
			T_.setRotation(a, plane.n);
		}

		//now set min and max values for each axis...
		min_x_ = 1e100; min_y_ = 1e100; min_z_ = 1e100;
		max_x_ = -1e100; max_y_ = -1e100; max_z_ = -1e100;
		Vector3 tmp = T_*p1;
		if (tmp[0] < min_x_) min_x_ = tmp[0];
		if (tmp[0] > max_x_) max_x_ = tmp[0];
		if (tmp[1] < min_y_) min_y_ = tmp[1];
		if (tmp[1] > max_y_) max_y_ = tmp[1];
		if (tmp[2] < min_z_) min_z_ = tmp[2];
		if (tmp[2] > max_z_) max_z_ = tmp[2];
		tmp = T_*p2;
		if (tmp[0] < min_x_) min_x_ = tmp[0];
		if (tmp[0] > max_x_) max_x_ = tmp[0];
		if (tmp[1] < min_y_) min_y_ = tmp[1];
		if (tmp[1] > max_y_) max_y_ = tmp[1];
		if (tmp[2] < min_z_) min_z_ = tmp[2];
		if (tmp[2] > max_z_) max_z_ = tmp[2];

		if (y_expansion < 0)
		{
			min_y_ += y_expansion;
		}
		else
		{
			max_y_ += y_expansion;
		}
		if (z_expansion < 0)
		{
			min_z_ += z_expansion;
		}
		else
		{
			max_z_ += z_expansion;
		}
	}


	ReferenceArea::ReferenceArea(const AtomContainer* sys, bool atom_fraction, double atoms, double penalty)
	{
		type_ = "ReferenceArea";
		name_ = "";
		scale_by_size_ = false;
		number_reflig_atoms_ = 0;
		T_.setIdentity();
		AtomConstIterator it = sys->beginAtom();
		Vector3 p1 = it->getPosition();
		it++;
		if (it == sys->endAtom())
		{
			throw Exception::GeneralException(__FILE__, __LINE__, "ReferenceArea creation error", "Reference ligand must have more than 1 atoms");
		}

		//now set min and max values for each axis...
		min_x_ = 1e100; min_y_ = 1e100; min_z_ = 1e100;
		max_x_ = -1e100; max_y_ = -1e100; max_z_ = -1e100;

		for (AtomConstIterator it = sys->beginAtom(); it != sys->endAtom(); it++)
		{
			Vector3 p = T_ * it->getPosition();
			if (p[0] > max_x_)
			{
				max_x_ = p[0];
			}
			if (p[0] < min_x_)
			{
				min_x_ = p[0];
			}
			if (p[1] > max_y_)
			{
				max_y_ = p[1];
			}
			if (p[1] < min_y_)
			{
				min_y_ = p[1];
			}
			if (p[2] > max_z_)
			{
				max_z_ = p[2];
			}
			if (p[2] < min_z_)
			{
				min_z_ = p[2];
			}
		}

		input_points_.push_back(Vector3(min_x_, min_y_, min_z_));
		input_points_.push_back(Vector3(max_x_, min_y_, min_z_));
		input_points_.push_back(Vector3(max_x_, max_y_, min_z_));
		input_points_.push_back(Vector3(max_x_, max_y_, max_z_));

		atom_fraction_ = atom_fraction;
		ref_atoms_ = atoms;
		penalty_ = penalty;
	}


	ReferenceArea::~ReferenceArea()
	{

	}


	void ReferenceArea::enlarge(double angstroem)
	{
		max_x_ += angstroem;
		min_x_ -= angstroem;
		max_y_ += angstroem;
		min_y_ -= angstroem;
		max_z_ += angstroem;
		min_z_ -= angstroem;

		input_points_.clear();
		input_points_.push_back(Vector3(min_x_, min_y_, min_z_));
		input_points_.push_back(Vector3(max_x_, min_y_, min_z_));
		input_points_.push_back(Vector3(max_x_, max_y_, min_z_));
		input_points_.push_back(Vector3(max_x_, max_y_, max_z_));
	}

	void ReferenceArea::setNumberDesiredAtoms(double d)
	{
		ref_atoms_ = d;
	}

	void ReferenceArea::setName(String name)
	{
		Constraint::setName(name);
		if (name.hasSubstring("PocketDetector"))
		{
			scale_by_size_ = true;
			if (scoring_function_) setScoringFunction(scoring_function_);
		}
		else
		{
			scale_by_size_ = false;
		}
	}


	void ReferenceArea::setScoringFunction(ScoringFunction* sf)
	{
		Constraint::setScoringFunction(sf);
		if (scale_by_size_)
		{
			number_reflig_atoms_ = 0;
			for (AtomConstIterator it = sf->getLigand()->beginAtom(); +it; it++)
			{
				number_reflig_atoms_++;
			}
		}
	}

	double ReferenceArea::countContainedAtoms(AtomContainer* sys)
	{
		double observed = 0;
		Size ligand_atoms = 0;
		for (AtomIterator it = sys->beginAtom(); it != sys->endAtom(); it++)
		{
			ligand_atoms++;
			Vector3 p = T_ * it->getPosition();
			if (p[0] >= min_x_ && p[0] <= max_x_ && p[1] >= min_y_ && p[1] <= max_y_ && p[2] >= min_z_ && p[2] <= max_z_)
			{
				observed++;
			}
		}

		if (atom_fraction_)
		{
			if (scale_by_size_ && ligand_atoms > number_reflig_atoms_)
			{
				if (number_reflig_atoms_ == 0)
				{
					throw Exception::GeneralException(__FILE__, __LINE__, "ReferenceArea error", "No of ref-atoms = 0, use setScoringFunction() first!");
				}
				ligand_atoms = number_reflig_atoms_;
			}
			observed = observed/ligand_atoms;
		}

		return observed;
	}


	double ReferenceArea::calculateScore(AtomContainer* sys)
	{
		if (ref_atoms_ == 0)
		{
			return 0;
		}

		double observed = countContainedAtoms(sys);

		contained_atoms_ = observed;

		double score = ref_atoms_ - observed;
		if (score > 0)
		{
			score *= penalty_;
		}
		else
		{
			return 0;
		}
		return score;
	}

	double ReferenceArea::getContainedAtoms()
	{
		return contained_atoms_;
	}

	BALL::System* ReferenceArea::createBoundaryMolecule()
	{
		if (input_points_.size() < 4) return NULL;

		System* sys = new System;
		Atom* a0 = new Atom;
		a0->setPosition(input_points_[0]);
		Atom* a1 = new Atom;
		a1->setPosition(input_points_[1]);
		Atom* a2 = new Atom;
		a2->setPosition(input_points_[2]);
		Atom* a3 = new Atom;
		a3->setPosition(input_points_[3]);
		a0->createBond(*a1);
		a1->createBond(*a2);
		a2->createBond(*a3);
		Molecule* m = new Molecule;
		m->insert(*a0);
		m->insert(*a1);
		m->insert(*a2);
		m->insert(*a3);
		sys->insert(*m);

		return sys;
	}


	PharmacophoreConstraint::PharmacophoreConstraint(list<const AtomContainer*>& container_list, const list<String>& interaction_types, double desired_interaction_score, double penalty)
	{
		type_ = "PharmacophoreConstraint";
		desired_interaction_score_ = desired_interaction_score;
		interaction_partners_ = container_list;
		interaction_types_ = interaction_types;
		penalty_ = penalty;
		gridSetID_ = -1;

		for (list < const AtomContainer* > ::iterator it = interaction_partners_.begin();
			it!=interaction_partners_.end(); it++)
		{
			const Residue* residue = dynamic_cast<const Residue*>(*it);
			if (residue)
			{
				residue_IDs_.push_back(residue->getID());
				uses_receptor_residues_ = true;
				const Chain* chain = residue->getChain();
				if (chain)
				{
					chain_IDs_.push_back(chain->getName());
				}
				else
				{
					chain_IDs_.push_back("");
				}
			}
			else
			{
				uses_receptor_residues_ = false;
				residue_IDs_.clear();
				break;
			}
		}
	}

	PharmacophoreConstraint::PharmacophoreConstraint(const AtomContainer* container, const list<String>& interaction_types, double desired_interaction_score, double penalty)
	{
		type_ = "PharmacophoreConstraint";
		desired_interaction_score_ = desired_interaction_score;
		interaction_partners_.push_back(container);
		interaction_types_ = interaction_types;
		penalty_ = penalty;
		gridSetID_ = -1;

		const Residue* residue = dynamic_cast<const Residue*>(container);
		if (residue)
		{
			residue_IDs_.push_back(residue->getID());
			uses_receptor_residues_ = true;
			const Chain* chain = residue->getChain();
			if (chain)
			{
				chain_IDs_.push_back(chain->getName());
			}
			else
			{
				chain_IDs_.push_back("");
			}
		}
		else
		{
			uses_receptor_residues_ = false;
			residue_IDs_.clear();
		}
	}

	PharmacophoreConstraint::PharmacophoreConstraint(vector<String>& residue_IDs, const list<String>& interaction_types, double desired_interaction_score, double penalty)
	{
		type_ = "PharmacophoreConstraint";
		desired_interaction_score_ = desired_interaction_score;
		interaction_types_ = interaction_types;
		penalty_ = penalty;
		residue_IDs_.clear();
		for (Size i=0; i<residue_IDs.size(); i++)
		{
			String residue = residue_IDs[i];
			String chain = "";
			if (residue.hasSubstring(":"))
			{
				chain = residue.before(":");
				residue = residue.after(":");
			}
			residue_IDs_.push_back(residue);
			chain_IDs_.push_back(chain);
		}
		uses_receptor_residues_ = true;
		gridSetID_ = -1;
	}

	PharmacophoreConstraint::PharmacophoreConstraint(const PharmacophoreConstraint& phC)
		: Constraint()
	{
		residue_IDs_ = phC.residue_IDs_;
		interaction_partners_ = phC.interaction_partners_;
		desired_interaction_score_ = phC.desired_interaction_score_;
		interaction_score_ = phC.interaction_score_;
		uses_receptor_residues_ = phC.uses_receptor_residues_;
		gridSetID_ = phC.gridSetID_;
		gridSetID_ = -1;
		for (AtomContainerConstIterator it = phC.grid_container_.beginAtomContainer(); +it; it++)
		{
			const Residue* residue = dynamic_cast<const Residue*>(&*it);
			if (!residue) continue;
			Residue* residue_copy = new Residue;
			*residue_copy = *residue;
			grid_container_.insert(*residue_copy); // do not insert 'residue', since this would remove it from its parent (the receptor) !
		}
	}


	PharmacophoreConstraint::~PharmacophoreConstraint()
	{

	}


	void PharmacophoreConstraint::setScoringFunction(ScoringFunction* sf)
	{
		Constraint::setScoringFunction(sf);
		grid_container_.clear();
		gridSetID_ = -1;

		GridBasedScoring* gbs = dynamic_cast<GridBasedScoring*>(sf);
		if (interaction_partners_.size() == 0 && residue_IDs_.size() > 0)
		{
			const Protein* protein = dynamic_cast<Protein*>(sf->getReceptor());
			if (!protein)
			{
				const System* system = dynamic_cast<System*>(sf->getReceptor());
				if (system && system->getProtein(0))
				{
					protein = dynamic_cast<const Protein*>(system->getProtein(0));
				}

				if (!protein)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "PharmacophoreConstraint creation error", "The receptor that is used by the scoring function is not a protein, thus using residue-based PharmacophoreConstraints is not possible!");
				}
			}

			bool found_static_residue = false;
			vector<String>::iterator it2 = chain_IDs_.begin();
			for (vector<String>::iterator it = residue_IDs_.begin(); it != residue_IDs_.end(); it++, it2++)
			{
				const Residue* residue = 0;
				if (*it2 == "") residue = protein->getResidueByID(*it);
				else
				{
					const Chain* chain = 0;
					for (ChainConstIterator ch_it = protein->beginChain(); +ch_it; ch_it++)
					{
						if (ch_it->getName() == *it2)
						{
							chain = &*ch_it;
							break;
						}
					}
					if (chain)
					{
						for (ResidueConstIterator res_it = chain->beginResidue(); +res_it; res_it++)
						{
							if (res_it->getID() == *it)
							{
								residue = &*res_it;
								break;
							}
						}
					}
				}
				if (!residue)
				{
					String m = "A residue with ID '"+*it+"' could not be found ";
					if (*it2 != "" )
					{
						m += "within a chain named '";
						m += *it2;
						m += "' !";
					}
					else m += "in the receptor!";
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "PharmacophoreConstraint creation error", m);
				}
				interaction_partners_.push_back(residue);
				if (gbs)
				{
					if (scoring_function_->flexible_residues_.find(const_cast < Residue* > (residue)) == scoring_function_->flexible_residues_.end())
					{
						found_static_residue = true;
						Residue* residue_copy = new Residue;
						*residue_copy = *residue;
						grid_container_.insert(*residue_copy); // do not insert 'residue', since this would remove it from its parent (the receptor) !
					}
					else
					{
						throw BALL::Exception::GeneralException(__FILE__, __LINE__, "PharmacophoreConstraint error", "Using flexible residues for PharmacophoreConstraint is not supported yet!");
					}
				}
			}

			if (gbs && found_static_residue)
			{
				gridSetID_ = gbs->getScoreGridSets()->size();
				Options options = scoring_function_->getOptions();
				double score_resolution = options.setDefaultReal(GridBasedScoring::Option::SCOREGRID_RESOLUTION, GridBasedScoring::Default::SCOREGRID_RESOLUTION);
				int hashgrid_size = options.setDefaultInteger(ScoringFunction::Option::HASHGRID_SIZE, ScoringFunction::Default::HASHGRID_SIZE);
				double hash_resolution = options.setDefaultReal(ScoringFunction::Option::HASHGRID_RESOLUTION, ScoringFunction::Default::HASHGRID_RESOLUTION);
				Vector3 angstroem(hashgrid_size*hash_resolution, hashgrid_size*hash_resolution, hashgrid_size*hash_resolution);
				gbs->defineGridSet(&grid_container_, angstroem, 0, 0, score_resolution, 0);

				// make sure that the new grid-set will only be used by PharmacophoreConstraint::calculateScore(), not by each call of ScoringFunction::update()/updateScore().
				(*gbs->getScoreGridSets())[gridSetID_]->disable();
				(*gbs->getScoreGridSets())[gridSetID_]->setPharmacophoreConstraint(this);
			}
		}


	}

	bool PharmacophoreConstraint::usesReceptorResidues()
	{
		return uses_receptor_residues_;
	}

	double PharmacophoreConstraint::calculateScore(AtomContainer* /*container*/)
	{
		if (!scoring_function_)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "PharmacophoreConstraint::calculateScore() error", "No ScoringFunction is set for this PharmacophoreConstraint!");
		}

		interaction_score_ = 0;
		if (interaction_partners_.size() > 0)
		{
			for (list < const AtomContainer* > ::iterator c_it = interaction_partners_.begin();
			c_it!=interaction_partners_.end(); c_it++)
			{
				for (AtomConstIterator a_it = (*c_it)->beginAtom(); +a_it; a_it++)
				{
					if (!a_it->interactions) continue;
					interaction_score_ += a_it->interactions->getInteractionEnergy(interaction_types_);
				}
			}

			GridBasedScoring* gbs = dynamic_cast<GridBasedScoring*>(scoring_function_);
			if (gbs != 0)
			{
				// enable only the relevant grid-set ...
				vector<ScoreGridSet*>* gridsets = gbs->getScoreGridSets();
				vector<bool> enabled_backup(gridsets->size());
				for (Size i = 0; i < gridsets->size(); i++)
				{
					enabled_backup[i] = (*gridsets)[i]->isEnabled();
					if (i == gridSetID_) 
					{
						(*gridsets)[i]->enable();
					}
					else 
					{
						(*gridsets)[i]->disable();
					}
				}

				// ... calculate the score ...
				interaction_score_ += gbs->calculateGridScore();

				// ... and reset enabled-states of all score-gridsets
				for (Size i = 0; i < gridsets->size(); i++)
				{
					if (enabled_backup[i]) (*gridsets)[i]->enable();
					else (*gridsets)[i]->disable();
				}
			}

			if (!gbs) Log<<"PharmacophoreConstraint interaction-score : "<<interaction_score_<<endl;

			double score = 0;
			if (interaction_score_ > desired_interaction_score_)
			{
				double factor = fabs((interaction_score_-desired_interaction_score_)/desired_interaction_score_);
				if (factor < 1) score = factor*penalty_;
				else score = penalty_;
			}
			return score;
		}
		else return 0;
	}


	BALL::AtomContainer* PharmacophoreConstraint::getGridContainer()
	{
		return &grid_container_;
	}

	const list<BALL::String>* PharmacophoreConstraint::getInteractionTypes()
	{
		return &interaction_types_;
	}

	const list<const BALL::AtomContainer*>* PharmacophoreConstraint::getInteractionPartners()
	{
		return &interaction_partners_;
	}
}
