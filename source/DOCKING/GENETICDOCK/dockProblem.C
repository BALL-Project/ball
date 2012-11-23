// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DOCKING/GENETICDOCK/genes.h>
#include <BALL/DOCKING/GENETICDOCK/evolutionaryDocking.h>
#include <BALL/DOCKING/GENETICDOCK/dockProblem.h>
#include <BALL/CONCEPT/composite.h>

#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace BALL
{
	DockProblem::DockProblem()
	{
	}

	DockProblem::DockProblem(AtomContainer& sys_lig, ScoringFunction* sf, EvolutionaryDocking* docker, bool post_dock_optimization)
	{
		rb_ = NULL;
		post_dock_optimization_ = post_dock_optimization;

		scoring_function_ = sf;
		docker_ = docker;
		setup(sys_lig);

		if (ligand_ != 0)
		{
			scoring_function_->setLigand(sys_lig);
		}

		if (!post_dock_optimization_)
		{
			binding_pocket_center_ = sf->getLigandCenter();
		}
	}

	void DockProblem::setup(AtomContainer& sys_lig)
	{
		redraw_ = false;

		ligand_ = &sys_lig;

		scoring_function_->setLigand(sys_lig);

		/** create rotable bonds class with ligand
		*/
		delete rb_;
		rb_ = new RotateBonds(scoring_function_->getRotatableLigandBonds());

		/** save ligand positions to restore after calculation
		*/
		ligand_positions_.clear();
		AtomIterator it_ligand = ligand_->beginAtom();
		for (Size pos = 0; +it_ligand; ++it_ligand, pos++)
		{
			ligand_positions_.push_back(it_ligand->getPosition());
		}

		double_parameter_.values = vector<double>(rb_->numberBonds() + 3, 0.);

		if (post_dock_optimization_)
		{
			binding_pocket_center_ = scoring_function_->getLigandCenter();
		}
	}


	void DockProblem::connectTo(GeneticAlgorithm* opt)
	{
		/** register double parameter
		*/
		opt->registerParameter(&double_parameter_);

		/** register quaternion parameter
		*/
		opt->registerParameter(&quaternion_parameter_);
	}

	DockProblem::~DockProblem()
	{
		delete rb_;
	}


	double DockProblem::calculate()
	{
		move();

		/** get conformation energy
		*/
		scoring_function_->update();
		scoring_function_->updateScore();

		double score = scoring_function_->getScore();

		if (docker_->getDisplayMode() == DockingAlgorithm::ALL_INTERMEDIATE_POSES)
		{
			docker_->setVisualizationPose(score);
		}

		restore();

		return score;
	}


	void DockProblem::move()
	{
		/** rotate rotatable bonds
		*/
		rb_->rotate(vector<double>(double_parameter_.values.begin() + 3, double_parameter_.values.end()));


		/** move geometric center of molecule to center of binding pocket
		*/
		GeometricCenterProcessor gcp;
		ligand_->apply(gcp);
		Vector3 lig_center = gcp.getCenter();

		TranslationProcessor center_translation(binding_pocket_center_-lig_center);

		ligand_->apply(center_translation);


		/** rotate molecule
		*/
		Matrix4x4 r_matrixq;
		quaternion_parameter_.quat.getRotationMatrix(r_matrixq);
		TransformationProcessor rotationq(r_matrixq);
		ligand_->apply(rotationq);


		/** apply translations
		*/
		TranslationProcessor translation(Vector3(t_origin_.x + double_parameter_.values[0]*t_extension_.x,
				t_origin_.y + double_parameter_.values[1]*t_extension_.y,
				t_origin_.z + double_parameter_.values[2]*t_extension_.z));

		ligand_->apply(translation);
	}


	void DockProblem::restore()
	{
		AtomIterator it = ligand_->beginAtom();
		for (Size pos = 0; pos < ligand_positions_.size(); pos++, ++it)
		{
			it->setPosition(ligand_positions_[pos]);
		}
	}

	bool DockProblem::redraw()
	{
		if (redraw_)
		{
			redraw_ = false;
			return true;
		}

		return false;
	}

	AtomContainer* DockProblem::getLigand()
	{
		move();
		return ligand_;
	}
}
