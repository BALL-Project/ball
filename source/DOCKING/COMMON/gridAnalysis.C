// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#include <BALL/DOCKING/COMMON/gridAnalysis.h>

using namespace std;

namespace BALL
{
	GridAnalysis::GridAnalysis(ScoringFunction* sf, AtomContainer* probe_group)
		: scoring_function_(sf)
	{
		// use a copy to prevent that moving the original compound invalidates the poses obtained by getBestPoses()
		probe_group_ = *probe_group;
		no_best_poses_ = 0;
		resolution_ = -1;
	}

	void GridAnalysis::enableSavingBestPoses(Size number_best_poses)
	{
		no_best_poses_ = number_best_poses;
	}


	void GridAnalysis::setResolution(const double& resolution)
	{
		resolution_ = resolution;
	}

	RegularData3D* GridAnalysis::calculate()
	{
		// restore the ligand after all calculations, in case the scoring-function is also used for something else...
		AtomContainer* backup_ligand = scoring_function_->getLigand();
		best_poses_.clear();

		scoring_function_->setLigand(probe_group_);

		if (resolution_ < 0) // resolution has not been set manually
		{
			double radius = scoring_function_->getLigandRadius();
			resolution_ = radius/2;
			if (resolution_ < 1) resolution_ = 1;
		}

		const HashGrid3<Atom*>* hashgrid = scoring_function_->getHashGrid();
		origin_ = hashgrid->getOrigin();

		Vector3 hashgrid_resolution = hashgrid->getUnit();
		Size no_x_steps = (Size)((hashgrid->getSizeX()*hashgrid_resolution[0])/resolution_);
		Size no_y_steps = (Size)((hashgrid->getSizeY()*hashgrid_resolution[1])/resolution_);
		Size no_z_steps = (Size)((hashgrid->getSizeZ()*hashgrid_resolution[2])/resolution_);

		Vector3 dimension(no_x_steps*resolution_, no_y_steps*resolution_, no_z_steps*resolution_);
		Vector3 resolution(resolution_, resolution_, resolution_);
		RegularData3D* reg3d = new RegularData3D(origin_, dimension, resolution);

		Size no_atoms = scoring_function_->getNoLigandAtoms();
		bool enable_rotation = 0;
		AtomIterator it = probe_group_.beginAtom();
		if (it == probe_group_.endAtom())
		{
			cerr<<"Error, probe group has no atoms!!"<<endl;
			return 0;
		}
		it++;
		if (it != probe_group_.endAtom()) enable_rotation = 1;

		center_ = scoring_function_->getLigandCenter();

		for (Size x = 0; x < no_x_steps; x++)
		{
			for (Size y = 0; y < no_y_steps; y++)
			{

				for (Size z = 0; z < no_z_steps; z++)
				{
					Vector3 position(origin_[0]+(x+0.5)*resolution_, origin_[1]+(y+0.5)*resolution_, origin_[2]+(z+0.5)*resolution_);

					moveProbeGroup_(position);

					scoring_function_->update();
					double score = scoring_function_->updateScore();
					int best_angle_x = 0;
					int best_angle_y = 0;
					int best_angle_z = 0;

					// if probe-group has more than one atom, enable rotation around all three axes and store minimal score in RegularData3D
					if (enable_rotation && score < 0.75e10*no_atoms)
					{
						for (Size i = 1; i < 36; i++)
						{
							rotateProbeGroup_(0, 10);
							scoring_function_->update();
							double score_i = scoring_function_->updateScore();
							if (score_i < score)
							{
								score = score_i;
								best_angle_x = i*10;
							}
						}
						rotateProbeGroup_(0, -350+best_angle_x);
						for (Size i = 1; i < 36; i++)
						{
							rotateProbeGroup_(1, 10);
							scoring_function_->update();
							double score_i = scoring_function_->updateScore();
							if (score_i < score)
							{
								score = score_i;
								best_angle_y = i*10;
							}
						}
						rotateProbeGroup_(1, -350+best_angle_y);
						for (Size i = 1; i < 36; i++)
						{
							rotateProbeGroup_(2, 10);
							scoring_function_->update();
							double score_i = scoring_function_->updateScore();
							if (score_i < score)
							{
								score = score_i;
								best_angle_z = i*10;
							}
						}

						// rotate back to original orientation
						rotateProbeGroup_(2, -350);
						rotateProbeGroup_(1, -best_angle_y);
						rotateProbeGroup_(0, -best_angle_x);
					}

					RegularData3D::IndexType index(x, y, z);
					reg3d->getData(index) = score;

					if (no_best_poses_ > 0) // if saving best positions is enabled
					{
						if (best_poses_.size() < no_best_poses_ || score < best_poses_.rbegin()->first)
						{
							bool insert = 1;

							list<PoseList::iterator> to_be_deleted;

							for (PoseList::iterator it = best_poses_.begin(); it != best_poses_.end(); it++)
							{
								if (it->second.first.getDistance(position) < 3*resolution_)
								{
									if (score < it->first) // current position is better, so remove the other position
									{
										to_be_deleted.push_back(it);
									}
									else // current position is worse, so do not insert it
									{
										insert = false;
										break;
									}
								}
							}

							if (insert)
							{
								for (list < PoseList::iterator > ::iterator it = to_be_deleted.begin(); it != to_be_deleted.end(); it++)
								{
									best_poses_.erase(*it);
								}

								Vector3 rotation(best_angle_x, best_angle_y, best_angle_z);
								best_poses_.insert(make_pair(score, make_pair(position, rotation)));
							}
						}
						if (best_poses_.size() > no_best_poses_)
						{
							PoseList::iterator it = best_poses_.end();
							it--;
							best_poses_.erase(it);
						}
					}
				}
			}
		}

		scoring_function_->setLigand(*backup_ligand);
		return reg3d;
	}


	list<AtomContainer*> GridAnalysis::getBestPoses()
	{
		list<AtomContainer*> container_list;
		if (best_poses_.size() == 0) return container_list; // return empty list

		AtomContainer probe_group_backup = probe_group_;
		Vector3 center_backup = center_;

		for (PoseList::iterator it = best_poses_.begin();
			it!=best_poses_.end(); it++)
		{
			probe_group_ = probe_group_backup;
			center_ = center_backup;
			moveProbeGroup_(it->second.first);
			rotateProbeGroup_(0, (int)it->second.second[0]);
			rotateProbeGroup_(1, (int)it->second.second[1]);
			rotateProbeGroup_(2, (int)it->second.second[2]);

			AtomContainer* new_container = new AtomContainer;
			*new_container = probe_group_; // copy the probe-group in the assigned pose
			container_list.push_back(new_container);
		}

		probe_group_ = probe_group_backup;
		center_ = center_backup;
		return container_list;
	}


	void GridAnalysis::moveProbeGroup_(const Vector3& destination)
	{
		TMatrix4x4<float> M;
		Vector3 translation_vector = destination-center_;
		M.setTranslation(translation_vector);
		for (AtomIterator it = probe_group_.beginAtom(); +it; it++)
		{
			it->setPosition(M*it->getPosition());
		}
		center_ = destination;
	}


	void GridAnalysis::rotateProbeGroup_(int axis, int degree)
	{
		TMatrix4x4<float> M;
		TAngle<float> angle(degree, false);
		Vector3 axis_vector(axis == 0, axis == 1, axis == 2);
		M.setRotation(angle, axis_vector);

		for (AtomIterator it = probe_group_.beginAtom(); it != probe_group_.endAtom(); it++)
		{
			it->setPosition(M*(it->getPosition()-center_)+center_);
		}
	}
}
