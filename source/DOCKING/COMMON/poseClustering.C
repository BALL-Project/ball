// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/FORMAT/lineBasedFile.h>

// TEST
#include <BALL/MATHS/angle.h>

using namespace std;
using namespace Eigen;

namespace BALL
{
	const String PoseClustering::Option::CLUSTER_METHOD  = "cluster_method";
	const Index  PoseClustering::Default::CLUSTER_METHOD = PoseClustering::CLINK_DEFAYS;

	const String PoseClustering::Option::RMSD_LEVEL_OF_DETAIL  = "rmsd_level_of_detail";
	const Index  PoseClustering::Default::RMSD_LEVEL_OF_DETAIL = PoseClustering::C_ALPHA;

	const String PoseClustering::Option::RMSD_THRESHOLD = "pose_clustering_rmsd_threshold";
  const float PoseClustering::Default::RMSD_THRESHOLD = 3;

	const String PoseClustering::Option::RMSD_TYPE = "pose_clustering_rmsd_type";
	const Index PoseClustering::Default::RMSD_TYPE = PoseClustering::SNAPSHOT_RMSD;

	// TODO: change name to USE_CENTER_OF_MASS_CLUSTERING
	const String PoseClustering::Option::USE_CENTER_OF_MASS_PRECLINK = "pose_clustering_use_center_of_mass_preclink";
	const bool   PoseClustering::Default::USE_CENTER_OF_MASS_PRECLINK = false;

	PoseClustering::PoseClustering()
	{
		setDefaultOptions();
	}


	PoseClustering::PoseClustering(ConformationSet* poses, float rmsd)
	{
		options[Option::RMSD_THRESHOLD] = rmsd;
		current_set_ = poses;
	}

	PoseClustering::PoseClustering(System const& base_system, String transformation_file_name)
	{
		setBaseSystemAndTransformations(base_system, transformation_file_name);
	}

	PoseClustering::~PoseClustering()
	{
	}

	Eigen::Matrix3f PoseClustering::computeCovarianceMatrix(System const& system, Index rmsd_level_of_detail)
	{
		GeometricCenterProcessor center;
		system.apply(center);

		Eigen::Matrix3f covariance_matrix = Matrix3f::Zero();

		Vector3f base_com((Eigen::Vector3f() << center.getCenter().x, center.getCenter().y, center.getCenter().z).finished());

		for (AtomConstIterator at_it = system.beginAtom(); +at_it; ++at_it)
		{
			if (    (rmsd_level_of_detail == ALL_ATOMS)
				  || ((rmsd_level_of_detail == C_ALPHA)  && (at_it->getName() == "CA"))
					|| ((rmsd_level_of_detail == BACKBONE) && (    (at_it->getName() == "CA")
					                                             || (at_it->getName() == "C")
																											 || (at_it->getName() == "O")
																											 || (at_it->getName() == "N")
																											 || (at_it->getName() == "H")))
					|| ((rmsd_level_of_detail == PROPERTY_BASED_ATOM_BIJECTION)
					                                        && (at_it->hasProperty("ATOMBIJECTION_RMSD_SELECTION"))))
			{
				Vector3 const& pos = at_it->getPosition();

				covariance_matrix.selfadjointView<Eigen::Upper>().rankUpdate(Eigen::Vector3f(pos.x, pos.y, pos.z) - base_com);
			}
		}

		covariance_matrix /= system.countAtoms();

		return covariance_matrix;
	}

	void PoseClustering::setBaseSystemAndTransformations(System const& base_system, String transformation_file_name)
	{
		rmsd_level_of_detail_ = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		base_system_ = System(base_system);

		covariance_matrix_ = computeCovarianceMatrix(base_system, rmsd_level_of_detail_);


		/// TEST TEST TEST
		/// TODO: move to test
		/*
		TransformationProcessor tp;
		Matrix4x4 bm;
		bm.setTranslation(-center.getCenter());
		tp.setTransformation(bm);

		System S(base_system);
		S.apply(tp);

		System S2(base_system);
		S2.apply(tp);
		
		Vector3 axis(3., 1., 4.);
		axis.normalize();

		bm.setRotation(Angle(2.19), axis);
		bm.m14 = 1.0; bm.m24 = 2.0; bm.m34 = 4.5;

		Eigen::Vector3f t(1.0, 2.0, 4.5);
		t*=0.;
		Eigen::Matrix3f m;
		m << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;

		translations_.push_back(t);
		rotations_.push_back(m);

		tp.setTransformation(bm);
		S.apply(tp);

		bm.setIdentity();
		translations_.push_back(Eigen::Vector3f(0.07, 1., 3.1));
		translations_.push_back(Eigen::Vector3f(0, 0, 0));

		axis = Vector3(1., 2., 1.);
		axis.normalize();
		
		bm.setRotation(Angle(1.), axis);
		bm.m14 = 0.07; bm.m24 = 1.; bm.m34 = 3.1;
		m << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;
		rotations_.push_back(m);
		cout << "rigid rmsd: " << getRMSD_(0, 1, PoseClustering::RIGID_RMSD) << endl;

		tp.setTransformation(bm);
		S2.apply(tp);

		StructureMapper mapme(S, S2);
		cout << "atom rmsd: " << mapme.calculateRMSD() << endl;
		rotations_.clear();

		exit(1);
		*/
		/// END TEST END TEST END TEST

		bool success = readTransformationsFromFile(transformation_file_name);
	}


	bool PoseClustering::compute()
	{
	cout << " Call Compute () " << endl;
		if (!current_set_)
		{
			Log.info() << "No valid ConformationSet given." << endl;
			return false;
		}

		int rmsd_type = options.getInteger(Option::RMSD_TYPE);
		if (rmsd_type == PoseClustering::RIGID_RMSD)
		{
			num_poses_ = translations_.size();
		}
		else
		{
			if (current_set_->size()>1)
				num_poses_ = current_set_->size();
			else
			{
				Log.info() << "Given ConformationSet is empty! Nothing to cluster, abort." << endl;
				return false;
			}
		}

		// precompute the atom bijection
		rmsd_level_of_detail_ = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		system_i_ = System(current_set_->getSystem());
		system_j_ = System(current_set_->getSystem());

		StructureMapper mapper(system_i_, system_j_);

		switch (rmsd_level_of_detail_)
		{
			case PoseClustering::C_ALPHA:
				atom_bijection_.assignCAlphaAtoms(system_i_, system_j_);
				break;
			case PoseClustering::BACKBONE:
				atom_bijection_.assignBackboneAtoms(system_i_, system_j_);
				break;
			case PoseClustering::ALL_ATOMS:
				mapper.calculateDefaultBijection();
				atom_bijection_ = mapper.getBijection();
				break;
			case PoseClustering::PROPERTY_BASED_ATOM_BIJECTION:
				atom_bijection_.assignAtomsByProperty(system_i_, system_j_);
				break;
			case PoseClustering::HEAVY_ATOMS:
			default:
				Log.info() << "Option RMSDLevelOfDetaill::HEAVY_ATOMS not yet implemented" << endl;
		}

		// do we have a preclink scenario 
		if (options.getBool(Option::USE_CENTER_OF_MASS_PRECLINK))
		{
			// cluster the centers of gravity
			return centerOfGravityPreCluster_();
		}

		// decide which algorithm to apply
		if (options.getInteger(Option::CLUSTER_METHOD) ==  PoseClustering::TRIVIAL_COMPLETE_LINKAGE)
		{
			return trivialCompute_();
		}
		else if (   (options.getInteger(Option::CLUSTER_METHOD) == PoseClustering::SLINK_SIBSON)
					 || (options.getInteger(Option::CLUSTER_METHOD) == PoseClustering::CLINK_DEFAYS))
		{
			return linearSpaceCompute_(options.getInteger(Option::RMSD_TYPE));
		}
		else
		{
			Log.error() << "Unknown parameter for option CLUSTER_METHOD " << options.get(Option::CLUSTER_METHOD) << endl;
			return false;
		}

		return true;
	}


	bool PoseClustering::readTransformationsFromFile(String filename)
	{
		LineBasedFile file(filename, std::ios::in);
		vector<String> fields;
		while (file.LineBasedFile::readLine())
		{
			// get the line
			String current_transformation = file.getLine();

			if (current_transformation.countFields(" ") == 19)
			{
				fields.clear();
				current_transformation.split(fields);
				translations_.push_back((Vector3f() << fields[5].toFloat(), fields[9].toFloat(), fields[13].toFloat()).finished());
				rotations_.push_back((Matrix3f() << fields[2].toFloat(),   fields[3].toFloat(),  fields[4].toFloat(),
				                                    fields[6].toFloat(),   fields[7].toFloat(),  fields[8].toFloat(),
																		        fields[10].toFloat(),  fields[11].toFloat(), fields[12].toFloat()).finished());

				//TODO should we build the ConformationSet here? 
			}
			else
			{
//				Log.info() << "Skipped line " << current_transformation << " while reading file " << filename << endl;
			}
		}

		return true;
	}


	bool PoseClustering::trivialCompute_()
	{
		// in the beginning each pose is a cluster
		// this causes O(n^2) space!!	
		pairwise_scores_.resize(num_poses_, num_poses_);

		// clean up old clusters
		clusters_.clear();

		// compute all pairwise RMSDs
		Index rmsd_type = options.getInteger(Option::RMSD_TYPE);
		if (rmsd_type == PoseClustering::CENTER_OF_MASS_DISTANCE)
		{
			computeCenterOfMasses_();
		}

		// for the next step we need to determine the minimal maximal 
		// distance between two clusters
		float min_max_cluster_dist = std::numeric_limits<float>::max();
		Index min_cluster_a_idx = 0;
		Index min_cluster_b_idx = 0;
		set<Index>* min_cluster_a = NULL;
		set<Index>* min_cluster_b = NULL;

		SnapShot sn_i;
		SnapShot sn_j;

		for (Size i=0; i<num_poses_; i++)
		{
			// add as new cluster
			set<Index> new_set;
			new_set.insert((Index) i);
			clusters_.push_back(new_set);

			// compute the rmsd
			if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
			{
				sn_i = (*current_set_)[i];
				sn_i.applySnapShot(system_i_);
			}
			pairwise_scores_(i,i) = 0;
			for (Size j=i+1; j<num_poses_; j++)
			{
				if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
				{
					sn_j = (*current_set_)[j];
					sn_j.applySnapShot(system_j_);
				}
				float rmsd = getRMSD_(i, j, rmsd_type);
				pairwise_scores_(i,j) = rmsd;
				pairwise_scores_(j,i) = rmsd;

				if (rmsd < min_max_cluster_dist)
				{
					min_max_cluster_dist = rmsd;
					// store
					min_cluster_a_idx = i;
					min_cluster_b_idx = j;
				}
			}
		}

		// iterate as long as the maximal cluster distance 
		// gets lower than the RMSD_THRESHOLD
		float rmsd_cutoff = options.getReal(Option::RMSD_THRESHOLD);
		bool hit = (clusters_.size() > 1);
		while ((min_max_cluster_dist < rmsd_cutoff) && hit)
		{
			// Invariant: cluster_a will always be the smaller one
			min_cluster_a = &clusters_[min_cluster_a_idx];
			min_cluster_b = &clusters_[min_cluster_b_idx];

			if (min_cluster_b->size() > min_cluster_a->size())
			{
				//swap
				swap(min_cluster_a_idx, min_cluster_b_idx);
				swap(min_cluster_a, min_cluster_b);
			}

			// merge
			std::set<Index> temp_set;
			std::set_union(min_cluster_a->begin(), min_cluster_a->end(),
                     min_cluster_b->begin(), min_cluster_b->end(),
                     std::inserter(temp_set, temp_set.begin()));

			min_cluster_a->swap(temp_set);

			// mark as merged
			min_cluster_b->clear();
			pairwise_scores_(min_cluster_b_idx, min_cluster_b_idx) = std::numeric_limits<float>::max();

//cout << "found a new cluster by merging " << min_cluster_a_idx << " with " << min_cluster_b_idx << " with rmsd " << min_max_cluster_dist << endl;
//printCluster_(min_cluster_a_idx);

			// did we end up in a single cluster?
			hit = false;
			// and update the pairwise rmsd scores 
			for (set<Index>::iterator conf_it = min_cluster_a->begin();
					conf_it !=  min_cluster_a->end();
					conf_it++)
			{
				for (Size j=0; j < clusters_.size(); j++)
				{
					if ((*conf_it != (Index)j) && (clusters_[j].size() > 0))
					{
						hit = true;
						float rmsd = getClusterRMSD_(*conf_it, j, rmsd_type);
						pairwise_scores_(*conf_it, j) = rmsd;
						pairwise_scores_(j, *conf_it) = rmsd;
					}
				}
			}

			// now find the new minimum		
			min_max_cluster_dist = std::numeric_limits<float>::max();
			min_cluster_b_idx = 0;
			min_cluster_a_idx = 0;
			min_cluster_b = NULL;
			min_cluster_a = NULL;

			for (Size i=0; i<num_poses_; i++)
			{
				// skip already seen clusters
				if (clusters_[i].size() > 0)
				{
					for (Size j=i+1; j<num_poses_; j++)
					{
						// skip already seen clusters
						if (clusters_[j].size() > 0)
						{
							// check if new minimum	
							if (pairwise_scores_(i,j) < min_max_cluster_dist)
							{
								hit = true;
								min_max_cluster_dist = pairwise_scores_(i,j);
								// store
								min_cluster_a_idx = i;
								min_cluster_b_idx = j;
//	cout << "new min " << min_max_cluster_dist << " btw " << i << " " << j << endl;
							}
						}
					}
				}
			}
		}

		// collect all final clusters
		std::vector<std::set<Index> >   temp_clusters;

		for (Size i = 0; i < clusters_.size(); i++)
		{
			if (clusters_[i].size() >0)
				temp_clusters.push_back(clusters_[i]);
		}

		swap(clusters_, temp_clusters);

//cout << "Final num of clusters " << clusters_.size() << endl;
//printClusters();
	//	printClusterRMSDs();

		return true;
	}


	bool PoseClustering::linearSpaceCompute_(Index rmsd_type)
	{
		float threshold = options.getReal(Option::RMSD_THRESHOLD);

		if (rmsd_type == PoseClustering::CENTER_OF_MASS_DISTANCE)
		{
			computeCenterOfMasses_();
		}

		// we will need arrays pi, lambda, and mu
		lambda_.resize(num_poses_);
		pi_.resize(num_poses_);
		mu_.resize(num_poses_);

//cout << " num poses: " << num_poses_ << endl;

		// and initialize them for the first point
		pi_[0] = 0;
		lambda_[0] = numeric_limits<double>::max();

		for (Size current_level=1; current_level < num_poses_; ++current_level)
		{
			// TEST
			if (current_level % 1000 == 0)
			{
				double percentage = current_level / (double)num_poses_;
				percentage *= 100. * percentage;
//std::cout << current_level << " " << num_poses_ << " " << percentage << std::endl;
			}
			// END TEST

			pi_[current_level] = current_level;
			lambda_[current_level] = numeric_limits<double>::max();

			if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
			{
				(*current_set_)[current_level].applySnapShot(system_i_);
			}

			for (Size j=0; j<current_level; ++j)
			{
				if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
				{
					(*current_set_)[j].applySnapShot(system_j_);
				}

				// note: we don't need the rmsd matrix here - that's the whole point: the algorithm
				//       only requires O(n) data, not (O(n^2))
				mu_[j] = getRMSD_(current_level, j, rmsd_type);
//std::cout << current_level << " <=> " << j << " " << pi_[j] << ": " << mu_[j] << " " << lambda_[j]  << " " << rmsd_type << std::endl;
			}

			if (options.getInteger(Option::CLUSTER_METHOD) == SLINK_SIBSON)
			{
				slinkInner_(current_level);
			}
			else //(options.getInteger(Option::CLUSTER_METHOD) == CLINK_DEFAYS)
			{
				clinkInner_(current_level);
			}
		}
		// convert lambda, pi, and mu to clusters datastructure
		clusters_.clear();

//for (int i=0; i<pi_.size(); ++i)
//	std::cout << i << " " << pi_[i] << " " << lambda_[i] << std::endl;

		// we need a helper for the clusters
		std::vector<std::set<Index> > cluster_helper;

		// first, each point is its own cluster
		for (Position i=0; i < num_poses_; ++i)
		{
			cluster_helper.push_back(std::set<Index>());
			cluster_helper.back().insert(i);
		}

		// now, join clusters if needed
		for (Position current_cluster=0; current_cluster < cluster_helper.size(); ++current_cluster)
		{
			if (BALL_REAL_LESS_OR_EQUAL(lambda_[current_cluster], threshold, 1e-5))
			{
				// merge this cluster with its destination
				cluster_helper[pi_[current_cluster]].insert(cluster_helper[current_cluster].begin(), cluster_helper[current_cluster].end());

				// save some memory
				cluster_helper[current_cluster].clear();
			}
			else
			{
				// this cluster is done => put it into the result
				clusters_.push_back(cluster_helper[current_cluster]);

				// save some memory
				cluster_helper[current_cluster].clear();
			}
		}
//printClusterRMSDs();
		return true;
	}


	void PoseClustering::slinkInner_(int current_level)
	{
		for (int i=0; i<current_level; ++i)
		{
			if (BALL_REAL_GREATER_OR_EQUAL(lambda_[i], mu_[i], 1e-5))
			{
				mu_[pi_[i]] = min(mu_[pi_[i]], lambda_[i]);
				lambda_[i]  = mu_[i];
				pi_[i]      = current_level;
			}
			else
			{
				mu_[pi_[i]] = min(mu_[pi_[i]], mu_[i]);
			}
		}

		for (int i=0; i<current_level; ++i)
		{
			if (BALL_REAL_GREATER_OR_EQUAL(lambda_[i], lambda_[pi_[i]], 1e-5))
				pi_[i] = current_level;
		}
	}


	void PoseClustering::clinkInner_(int current_level)
	{
		for (int i=0; i<current_level; ++i)
		{
			if (BALL_REAL_LESS(lambda_[i], mu_[i], 1e-5))
			{
				mu_[pi_[i]] = max(mu_[pi_[i]], mu_[i]);
				mu_[i] = numeric_limits<double>::max();
			}
		}

		// step 4
		int a = current_level-1;

		// step 5
		for (int i=0; i<current_level; ++i)
		{
			int j = (current_level-1)-i;

			if (BALL_REAL_GREATER_OR_EQUAL(lambda_[j], mu_[pi_[j]], 1e-5))
			{
				if (BALL_REAL_LESS(mu_[j], mu_[a], 1e-5))
				{
					a = j;
				}
			}
			else
			{
				mu_[j] = numeric_limits<double>::max();
			}
		}

		//printVariables_(a, -5, -5., -5, -5, current_level);

		// step 6
		int    b = pi_[a];
		double c = lambda_[a];

		pi_[a] = current_level;
		lambda_[a] = mu_[a];

		int d;
		double e;

		//printVariables_(a, b, c, -5, -5, current_level);

		// step 7
		while ((a < current_level-1) && (b < current_level-1))
		{
			d = pi_[b];
			e = lambda_[b];
			pi_[b] = current_level;
			lambda_[b] = c;
			b = d;
			c = e;
		}

		if ((a < current_level - 1) && (b == current_level-1))
		{
			pi_[b] = current_level;
			lambda_[b] = c;
		}

		//printVariables_(a, b, c, d, e, current_level);

		// step 8
		for (int i=0; i<current_level; ++i)
		{
			if (pi_[pi_[i]] == current_level)
			{
				if (BALL_REAL_GREATER_OR_EQUAL(lambda_[i], lambda_[pi_[i]], 1e-5))
					pi_[i] = current_level;
			}
		}

		/*	cout << "------------------------------" << endl;
		printVariables_(a, b, c, d, e, current_level);
		cout << "------------------------------" << endl;
		*/
	}


	void PoseClustering::computeCenterOfMasses_()
	{
		GeometricCenterProcessor center;
		com_.resize(num_poses_);
		for (Size i=0; i < num_poses_; ++i)
		{
			(*current_set_)[i].applySnapShot(system_i_);
			system_i_.apply(center);
			com_[i] = center.getCenter();
		}
	}


	bool PoseClustering::centerOfGravityPreCluster_()
	{
cout << " centerOfGravityPreCluster_() alg = " << options.getInteger(Option::CLUSTER_METHOD) << " " << CLINK_DEFAYS << endl;
		// store the old options
		Options old_options = options;

		// run a pre-clustering
		// with basically the same options
		// but center of mass distance as rmsd
		options.set(Option::RMSD_TYPE, CENTER_OF_MASS_DISTANCE);
		// and no additional pre clustering
		options.set(Option::USE_CENTER_OF_MASS_PRECLINK, false);
		compute();

		// precompute all center of masses
		//computeCenterOfMasses_();
		// run the CLINK machinery
		//linearSpaceCompute_(CENTER_OF_MASS_DISTANCE);

		// store results

cout << " found " << getNumberOfClusters() << "  geometric center clusters." << endl;
printClusters();
printClusterRMSDs();
cout <<  endl << endl << " Weiter gehts " << endl;

		// store the new clusters
		std::vector<std::set<Index> >   temp_clusters;

		// reset the options
		options = old_options;

		PoseClustering inner_pc;
		Size num_clusters = 0;
		// now iterate over all clusters 
		for (Size i=0; i<getNumberOfClusters(); i++)
		{
cout << "       " << i << endl;
			if (getClusterSize(i)>1)
			{
				// temporarily convert the set to a vector for quicker access
				std::vector<Index> current_cluster_vector(getClusterSize(i));
				Position current_cluster=0;
				for (std::set<Index>::iterator clust_it = clusters_[i].begin(); clust_it != clusters_[i].end(); ++clust_it, current_cluster++)
				{
					current_cluster_vector[current_cluster] = *clust_it;
				}

				boost::shared_ptr<ConformationSet> current_conformation_set = getClusterConformationSet(i);

				// run a clustering separately on each pre cluster as described by the options
				inner_pc.clear_();

				// set the original Option::RMSD_TYPE
				inner_pc.options = options;

				inner_pc.options.set(Option::USE_CENTER_OF_MASS_PRECLINK, false);

				inner_pc.setConformationSet(&*current_conformation_set);
				inner_pc.compute();

				// add what we have found
				num_clusters = inner_pc.getNumberOfClusters();
				for (Size j=0; j<num_clusters; ++j)
				{
					// convert the indices
					std::set<Index> temp_indices = inner_pc.getCluster(j);
					std::set<Index> converted_indices;

					for (std::set<Index>::iterator ind_it = temp_indices.begin(); ind_it != temp_indices.end(); ++ind_it)
					{
						converted_indices.insert(current_cluster_vector[*ind_it]);
					}

					temp_clusters.push_back(converted_indices);
					//cout << "    " << i << " " << j << ": " << inner_pc.getClusterSize(j) << std::endl;
				}
cout << "++++++++++++++++++++++++++++++***" << endl;
				inner_pc.printClusterRMSDs();
				//cout << " cluster " << i << "( " << getClusterSize(i) << " ) was split into " << num_clusters << " clusters." << endl;
cout << "***++++++++++++++++++++++++++++++***" << endl;
			}
			else
			{
				temp_clusters.push_back(getCluster(i));
			}
			//std::set<Index>& current_cluster = getCluster(i);
		}
		// switch the clusters
		clusters_ = temp_clusters;
		printClusters();

		return true;
	}


	void PoseClustering::printClusterRMSDs()
	{
		cout << endl <<  "For method " << options.getInteger(PoseClustering::Option::CLUSTER_METHOD)
		     << " and RMSDtype " << options.getInteger(PoseClustering::Option::RMSD_TYPE) << " we get: " << endl;

		for (Position i=0; i<clusters_.size(); ++i)
		{
			cout << "=======================================" << endl;
			cout << "    Cluster " << i << endl;

			std::set<Index>& current_cluster = clusters_[i];

			for (std::set<Index>::iterator it_j=current_cluster.begin(); it_j!=current_cluster.end(); ++it_j)
			{
				(*current_set_)[*it_j].applySnapShot(system_i_);

				for (std::set<Index>::iterator it_k=current_cluster.begin(); it_k!=current_cluster.end(); ++it_k)
				{
					(*current_set_)[*it_k].applySnapShot(system_j_);

					cout << getRMSD_(*it_j, *it_k, options.getInteger(Option::RMSD_TYPE)) << " ";
				}

				cout << endl;
			}

			cout << "=======================================" << endl << endl;
		} //next cluster
	}

	const System& PoseClustering::getSystem() const
	{
		return current_set_->getSystem();
	}


	System& PoseClustering::getSystem()
	{
		return current_set_->getSystem();
	}


	const set<Index>& PoseClustering::getCluster(Index i) const
	{
		if (i >= (Index) clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		return clusters_[i];
	}


	set<Index>& PoseClustering::getCluster(Index i)
	{
		if (i >= (Index) clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		return clusters_[i];
	}


	Size PoseClustering::getClusterSize(Index i) const
	{
		if (i >= (Index)clusters_.size())
				throw(Exception::OutOfRange(__FILE__, __LINE__));

		return clusters_[i].size();
	}


	boost::shared_ptr<System> PoseClustering::getClusterRepresentative(Index i) const
	{
		if (i >= (Index)clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		boost::shared_ptr<System> new_system(new System(current_set_->getSystem()));

		// TODO translate cluster i to conformationsetindex
		// as cluster representative we simply take the first cluster member
		// this is a very simple heuristic :-)
		// TODO: find the median?

		// we simply take the first
		Index conf_set_idx = *(clusters_[i].begin());

		SnapShot sn = (*current_set_)[conf_set_idx];

		sn.applySnapShot(*(new_system.get()));
		return new_system;
	}


	void PoseClustering::setDefaultOptions()
	{
		options.setDefault(PoseClustering::Option::CLUSTER_METHOD,
		                   PoseClustering::Default::CLUSTER_METHOD);

		options.setDefaultReal(PoseClustering::Option::RMSD_THRESHOLD,
		                       PoseClustering::Default::RMSD_THRESHOLD);

		options.setDefault(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL,
		                   PoseClustering::Default::RMSD_LEVEL_OF_DETAIL);

		options.setDefault(PoseClustering::Option::RMSD_TYPE,
		                   PoseClustering::Default::RMSD_TYPE);

		options.setDefault(PoseClustering::Option::USE_CENTER_OF_MASS_PRECLINK,
				               PoseClustering::Default::USE_CENTER_OF_MASS_PRECLINK);
	}


	boost::shared_ptr<ConformationSet> PoseClustering::getClusterConformationSet(Index i) const
	{
		if (i >= (Index)clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		// create a new ConformationSet
		boost::shared_ptr<ConformationSet> new_set(new ConformationSet());

		new_set->setup(current_set_->getSystem());

		new_set->setParent(current_set_);

		for (set<Index>::iterator conf_it = clusters_[i].begin();
				 conf_it != clusters_[i].end(); conf_it++)
		{
			System conf(new_set->getSystem());

			SnapShot sn = (*current_set_)[*conf_it];
			sn.applySnapShot(conf);
			new_set->add(0, conf);
		}

		return new_set;
	}


	boost::shared_ptr<ConformationSet> PoseClustering::getReducedConformationSet() const
	{
		if (clusters_.size()==0)
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		// create a new ConformationSet
		boost::shared_ptr<ConformationSet> new_set(new ConformationSet());

		new_set->setup(current_set_->getSystem());
		new_set->setParent(current_set_);

		// iterate over all clusters
		for (Size i=0; i<clusters_.size(); i++)
		{
			// and take the first  
			System conf(new_set->getSystem());
			SnapShot sn = (*current_set_)[*clusters_[i].begin()];
			sn.applySnapShot(conf);
			new_set->add(0, conf);
		}

		return new_set;
	}


	// distance between cluster i and cluster j
	// this method is used by the trivial clustering
	float PoseClustering::getClusterRMSD_(Index i, Index j, Index rmsd_type)
	{
		// we have to compute the maximal distance
		float rmsd = std::numeric_limits<float>::max();
		float temp_rmsd = std::numeric_limits<float>::min();
		Index cluster_method = options.getInteger(Option::CLUSTER_METHOD);

		for (set<Index>::iterator conf_it_i = clusters_[i].begin();
				conf_it_i !=  clusters_[i].end();
				conf_it_i ++)
		{
			for (set<Index>::iterator conf_it_j = clusters_[j].begin();
					conf_it_j !=  clusters_[j].end();
					conf_it_j ++)
			{
				if (rmsd_type == PoseClustering::CENTER_OF_MASS_DISTANCE)
				{
					temp_rmsd = com_[*conf_it_i].getDistance(com_[*conf_it_j]);
				}
				else if (cluster_method == TRIVIAL_COMPLETE_LINKAGE)
				{
					//if (pairwise_scores_(*conf_it_i, *conf_it_j) < rmsd)
					temp_rmsd = pairwise_scores_(*conf_it_i, *conf_it_j);
				}
				else if (   (cluster_method == PoseClustering::CLINK_DEFAYS)
						     || (cluster_method == PoseClustering::SLINK_SIBSON))
				{
					Log.info() << " This should not happen!! " << __FILE__ << " " << __LINE__<< endl;
				}
				else
				{
					Log.info() << "getRMSD() for current option setting not yet implemented!" << __FILE__ <<  " " << __LINE__ << endl;
				}

				if (temp_rmsd < rmsd)
					rmsd = temp_rmsd;
			}
		}
		return rmsd;
	}


	float PoseClustering::getRigidRMSD(Eigen::Vector3f const& t_ab, Eigen::Matrix3f const& M_ab, Eigen::Matrix3f const& covariance_matrix)
	{
		// based on the given transformation we can simply compute the rmsd
		// 
		//              2 |-----------------------------------------------------------
		//              - |              2    1    (   t           /        t \   )
		//                | || t_(a,b) ||  +  - tr ( M   M  * sum (x_i x_i     )  )
		// rmsd(a,b) =    |                   n    (           i   \          /   )
		//                |

		return sqrt(t_ab.squaredNorm() + ((M_ab.transpose() * M_ab) * covariance_matrix.selfadjointView<Eigen::Upper>()).trace());
	}

	// compute the RMSD between two "poses", 
	//    i.e. snapshots, transformations, or center of masses
	float PoseClustering::getRMSD_(Index i, Index j, Index rmsd_type)
	{
		float rmsd = 0.;

		if (rmsd_type == PoseClustering::RIGID_RMSD)
		{
			Eigen::Vector3f t_ab = (translations_[j] - translations_[i]);
			Eigen::Matrix3f M_ab = (rotations_[j] - rotations_[i]);

			rmsd = getRigidRMSD(t_ab, M_ab, covariance_matrix_);

//			std::cout << t_ab.squaredNorm() << " " << rmsd << std::endl;

		}
		else if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
		{
			// we are were given Conformations that have been applied to sys_i and sys_j
			StructureMapper mapper(system_i_, system_j_);
			rmsd = mapper.calculateRMSD(atom_bijection_);
		}
		else if (rmsd_type == PoseClustering::CENTER_OF_MASS_DISTANCE)
		{
			// just query the center distance
			rmsd = com_[i].getDistance(com_[j]);
		}

		//std::cout << "rmsd between " << i << " " << j << ": " << rmsd << std::endl;
		return rmsd;
	}


	void PoseClustering::printCluster_(Index i)
	{
		cout << "++++ cluster " << i << " ++++" << endl;
		std::copy(clusters_[i].begin(), clusters_[i].end(), std::ostream_iterator<Index>(std::cout, " "));
		cout << endl;
		cout << "+++++++++++++++++++" << endl;
	}

	void PoseClustering::printClusters()
	{
		cout << "\n\n    FINAL CLUSTERS     \n\n" << endl;
		for (Size i = 0; i < clusters_.size(); i++)
		{
			printCluster_(i);
		}
	}


	void PoseClustering::printVariables_(int a, int b, double c, int d, double e, int current_level)
	{
		cout << "***************************" << endl;
		cout << " a:" << a << " b:" << b << " c:" << c << " d:" << d << " e:" << e << " cl: " << current_level << endl;
		cout << " pi:" << endl;
		for (int i=0; i < current_level +1; ++i)
		{
			cout << "(i," << pi_[i] <<  ") ";
		}
		cout << endl;
		cout << " lambda:" << endl;
		for (int i=0; i < current_level+1; ++i)
		{
			cout << "(i," << lambda_[i] <<  ") ";
		}
		cout << endl;
		cout << " mu:" << endl;
		for (int i=0; i < current_level; ++i)
		{
			cout << "(i," << mu_[i] <<  ") ";
		}
		cout << endl;

		cout << "******************" << endl;
	}


	void PoseClustering::clear_()
	{
		// Note: options should not be reset
		pairwise_scores_.setZero();
		current_set_ = NULL;
		translations_.clear();
		rotations_.clear();
		covariance_matrix_.setZero();
		base_system_.clear();
		clusters_.clear();
		//rmsd_level_of_detail_; 
		lambda_.clear();
		pi_.clear();
		mu_.clear();
		com_.clear();
		//atom_bijection_;
		//system_i_;
		//system_j_;
		num_poses_ = 0;
	}
}
