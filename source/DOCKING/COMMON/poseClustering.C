// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/poseClustering.h>

#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/FORMAT/lineBasedFile.h>

// TEST
//#include <BALL/MATHS/angle.h>

#include <stack>

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/graphviz.hpp>

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

	const String PoseClustering::Option::USE_CENTER_OF_MASS_PRECLINK = "pose_clustering_use_center_of_mass_preclink";
	const bool   PoseClustering::Default::USE_CENTER_OF_MASS_PRECLINK = false;

	//const String PoseClustering::Option::FULL_CLUSTER_DENDOGRAM = "pose_clustering_full_cluster_dendogram";
	//const bool   PoseClustering::Default::FULL_CLUSTER_DENDOGRAM = false;

	PoseClustering::PoseClustering()
		: current_set_(0),
			has_rigid_transformations_(false),
			delete_conformation_set_(false)
	{
		setDefaultOptions();
	}


	PoseClustering::PoseClustering(ConformationSet* poses, float rmsd)
	{
		options[Option::RMSD_THRESHOLD] = rmsd;
		current_set_ = poses;
		has_rigid_transformations_ = false;
		delete_conformation_set_   = false;

		base_system_ = poses->getSystem();
	}

	PoseClustering::PoseClustering(System const& base_system, String transformation_file_name)
	{
		has_rigid_transformations_ = false;
		delete_conformation_set_   = false;

		setBaseSystemAndTransformations(base_system, transformation_file_name);
	}

	PoseClustering::~PoseClustering()
	{
		if (delete_conformation_set_)
			delete current_set_;
	}


	Eigen::Matrix3f PoseClustering::computeCovarianceMatrix(System const& system, Index rmsd_level_of_detail)
	{
		Eigen::Matrix3f covariance_matrix = Matrix3f::Zero();

		Vector3f base_com(0, 0, 0);

		// note: we need two loops: one to compute the center of mass, the other for the covariance matrix
		Size num_points = 0;
		for (AtomConstIterator at_it = system.beginAtom(); +at_it; ++at_it)
		{
			if (!isExcludedByLevelOfDetail_(&*at_it, rmsd_level_of_detail))
			{
				Vector3 const& pos = at_it->getPosition();
				base_com += ((Eigen::Vector3f() << pos.x, pos.y, pos.z).finished());
				++num_points;
			}
		}

		base_com /= num_points;

		for (AtomConstIterator at_it = system.beginAtom(); +at_it; ++at_it)
		{
			if (!isExcludedByLevelOfDetail_(&*at_it, rmsd_level_of_detail))
			{
				Vector3 const& pos = at_it->getPosition();

				covariance_matrix.selfadjointView<Eigen::Upper>().rankUpdate(Eigen::Vector3f(pos.x, pos.y, pos.z) - base_com);
			}
		}

		covariance_matrix /= num_points;

		// for efficiency, we have only computed the upper triangle so far
		covariance_matrix(1,0) = covariance_matrix(0,1);
		covariance_matrix(2,0) = covariance_matrix(0,2);
		covariance_matrix(2,1) = covariance_matrix(1,2);

		return covariance_matrix;
	}


	void PoseClustering::setConformationSet(ConformationSet* new_set)
	{
		if (delete_conformation_set_)
			delete current_set_;
		has_rigid_transformations_ = false;

		current_set_ = new_set;
	}


	void PoseClustering::setBaseSystemAndTransformations(System const& base_system, String transformation_file_name)
	{
		// clear any potential pointers to old poses	
		if (delete_conformation_set_)
			delete current_set_;
		current_set_ = NULL;

		has_rigid_transformations_ = true;

		rmsd_level_of_detail_ = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		base_system_ = System(base_system);

		base_conformation_.takeSnapShot(base_system_);

		covariance_matrix_ = computeCovarianceMatrix(base_system, rmsd_level_of_detail_);

		readTransformationsFromFile_(transformation_file_name);
	}


	bool PoseClustering::compute()
	{
		int rmsd_type = options.getInteger(Option::RMSD_TYPE);

		if (has_rigid_transformations_ && (rmsd_type != PoseClustering::RIGID_RMSD))
			//!current_set_ && (rmsd_type != PoseClustering::RIGID_RMSD))
		{
			Log.info() << "No valid ConformationSet given." << endl;
			return false;
		}

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

		// precompute the atom bijection, that handles the 
		// option RMSD_LEVEL_OF_DETAIL
		precomputeAtomBijection_();

		// do we have a pre clustering scenario 
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
		else if (options.getInteger(Option::CLUSTER_METHOD) == NEAREST_NEIGHBOR_CHAIN_WARD)
		{
			return nearestNeighborChainCompute_();
		}
		else
		{
			Log.error() << "Unknown parameter for option CLUSTER_METHOD " << options.get(Option::CLUSTER_METHOD) << endl;
			return false;
		}

		return true;
	}


	bool PoseClustering::readTransformationsFromFile_(String filename)
	{
		translations_.clear();
		rotations_.clear();
		if (delete_conformation_set_)
			delete current_set_;

		current_set_ = NULL;

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
				translations_.push_back((Vector3f() << fields[6].toFloat(), fields[10].toFloat(), fields[14].toFloat()).finished());
				rotations_.push_back((Matrix3f() << fields[3].toFloat(),   fields[4].toFloat(),  fields[5].toFloat(),
				                                    fields[7].toFloat(),   fields[8].toFloat(),  fields[9].toFloat(),
																		        fields[11].toFloat(),  fields[12].toFloat(), fields[13].toFloat()).finished());

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
		num_poses_ = getNumberOfPoses();
		pairwise_scores_.resize(num_poses_, num_poses_);

		// clean up old clusters
		clusters_.clear();

		// reset the cluster tree
		cluster_tree_.clear();

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

			// and add it into the tree
			ClusterTreeNode v = boost::add_vertex(cluster_tree_);
			cluster_tree_[v].poses.insert((Index) i);
			cluster_tree_[v].size = 1;

			// TODO: continue with the cluster tree... add center if required ...

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
			for (Size j=0; j < clusters_.size(); j++)
			{
				if ((min_cluster_a_idx != (Index)j) && (clusters_[j].size() > 0))
				{
					hit = true;
					float rmsd = getClusterRMSD_(min_cluster_a_idx, j, rmsd_type);
					pairwise_scores_(min_cluster_a_idx, j) = rmsd;
					pairwise_scores_(j, min_cluster_a_idx) = rmsd;
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


	bool PoseClustering::nearestNeighborChainCompute_()
	{
		// we might need the number of atoms to which our RMSDs apply later on
		// this number depends on the choice of RMSD_LEVEL_OF_DETAIL
		Index rmsd_level_of_detail = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		number_of_selected_atoms_ = 0;
		for (AtomConstIterator at_it = current_set_->getSystem().beginAtom(); +at_it; ++at_it)
		{
			if (!isExcludedByLevelOfDetail_(&*at_it, rmsd_level_of_detail))
			{
				++number_of_selected_atoms_;
			}
		}

		// clean up old clusters
		clusters_.clear();

		// reset the cluster tree
		cluster_tree_.clear();

		num_poses_ = getNumberOfPoses();
		Index rmsd_type = options.getInteger(PoseClustering::Option::RMSD_TYPE);

		float min_cluster_dist = std::numeric_limits<float>::max();

		// the active clusters (as nodes in the cluster tree)
		std::set<ClusterTreeNode> active_clusters;

		// stack of clusters (as nodes in the cluster tree) storing nearest neighbor chains
		std::stack<ClusterTreeNode> cluster_stack;

		// in the beginning each pose is a cluster itself
		for (Size i=0; i<num_poses_; i++)
		{
			// add it as a new cluster to the tree
			ClusterTreeNode v = boost::add_vertex(cluster_tree_);
			cluster_tree_[v].poses.insert((Index) i);
			cluster_tree_[v].size = 1;
			cluster_tree_[v].merged_at = 0.;

			active_clusters.insert(v);
		}

cout << active_clusters.size() << " " << cluster_stack.size() << std::endl;

		initWardDistance_(rmsd_type);

		ClusterTreeNode current_cluster, nearest_cluster;

		// while there is more than one final cluster
		while (active_clusters.size() > 1)
		{
cout << " Stack size: " << cluster_stack.size() <<  endl;
			min_cluster_dist = std::numeric_limits<float>::max();
			nearest_cluster = 0;

			// if stack is empty add randomly
			if (cluster_stack.size() == 0)
			{
				// we simply take the first
				cluster_stack.push(*(active_clusters.begin()));
			}
			current_cluster = cluster_stack.top();

			// Compute the distances to all other clusters
			// TODO: can this be guaranteed?
			//      If there may be multiple equal nearest neighbors to a cluster, the algorithm requires a consistent 
			//      tie-breaking rule: for instance, in this case, the nearest neighbor may be chosen, among the clusters 
			//       at equal minimum distance from current_cluster, by numbering the clusters arbitrarily and choosing 
			//       the one with the smallest index
			for (std::set<ClusterTreeNode>::iterator clust_it = active_clusters.begin();
					 clust_it != active_clusters.end(); ++clust_it)
			{
				// check all pairs between all_clusters[*clust_it] and all_clusters[current_cluster]
				if (*clust_it != current_cluster)
				{
					float rmsd = computeWardDistance_(current_cluster, *clust_it, rmsd_type);

					if (BALL_REAL_GREATER(min_cluster_dist, rmsd, 1e-5))
					{
						nearest_cluster = *clust_it;
						min_cluster_dist = rmsd;
	cout << "    ++++ new min:" << rmsd << endl;
					}
				}
			}

			// have we put this cluster to the stack before ?
			// due to the reducibility of Ward's distance, this would imply that the cluster
			// was our predecessor.
			if (cluster_stack.size() == 1) // we don't have a predecessor => just push
			{
				cluster_stack.push(nearest_cluster);
				continue;
			}

			// look at the predecessor
			cluster_stack.pop(); // already saved in current_cluster

			ClusterTreeNode predecessor = cluster_stack.top();
			if (predecessor == nearest_cluster)
			{
				cluster_stack.pop(); // pop the predecessor

				// add a new parent node to the tree
				ClusterTreeNode new_parent = boost::add_vertex(cluster_tree_);

				// add an edge from the parent to both children
				boost::add_edge(new_parent, current_cluster, cluster_tree_);
				boost::add_edge(new_parent, nearest_cluster, cluster_tree_);

				// remember the value at which these were merged
				// NOTE: min_cluster_dist is the Ward distance, which is quadratic in the
				//       rmsds. We can, e.g., store RMSDs of the centroids, or the square root
				//       of the Ward distance. What we *will* store here, is the square root
				//       of the Ward distance per atom, i.e., the square root of the 
				//       squared average distance from the centroid per atom
				cluster_tree_[new_parent].merged_at = sqrt(min_cluster_dist / number_of_selected_atoms_);

				cluster_tree_[new_parent].size =    cluster_tree_[current_cluster].size
					                                + cluster_tree_[nearest_cluster].size;

				// TODO: here, we could decide if we want to merge the sets from our
				//       children and explictly store them => save runtime, pay memory
				//       (this would look similar to the following...)
				/*
				min_cluster_a = &clusters_[current_cluster];
				min_cluster_b = &clusters_[nearest_cluster];

				// merge
				std::set<Index> temp_set;
				std::set_union(min_cluster_a->begin(), min_cluster_a->end(),
				              min_cluster_b->begin(), min_cluster_b->end(),
				              std::inserter(temp_set, temp_set.begin()));
				*/

				// Note: this needs to be done before clearing and swapping!
				updateWardDistance_(new_parent, current_cluster, nearest_cluster, rmsd_type);

				// make the old clusters inactive and add the new one
				active_clusters.erase(current_cluster);
				active_clusters.erase(nearest_cluster);
				active_clusters.insert(new_parent);

				// push that cluster index onto the stack
				cluster_stack.push(current_cluster);
			}
			else
			{
				cluster_stack.push(current_cluster);
				cluster_stack.push(nearest_cluster);
			}
		}

		// copy final clusters into the general cluster data structure
		// collect all final clusters
		// TODO!!!
		/*
		std::vector<std::set<Index> >   temp_clusters;

		for (Size i = 0; i < clusters_.size(); i++)
		{
			if (clusters_[i].size() >0)
				temp_clusters.push_back(clusters_[i]);
		}

		swap(clusters_, temp_clusters);
		*/

		return true;
	}


	void PoseClustering::initWardDistance_(Index rmsd_type)
	{
		Index rmsd_level_of_detail = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		Index num_atoms = 0;

		if (rmsd_type == SNAPSHOT_RMSD)
		{
			// for snapshot-based rmsds, we will need a copy of the system later on
			system_i_ = System(current_set_->getSystem());

			// and the number of atoms used for rmsd computation
			for (AtomConstIterator atom_it = system_i_.beginAtom(); +atom_it; ++atom_it)
			{
				if (!isExcludedByLevelOfDetail_(&(*atom_it), rmsd_level_of_detail))
				{
					++num_atoms;
				}
			}
		}

		if (rmsd_type == CENTER_OF_MASS_DISTANCE)
		{
			// precompute centers of mass
			computeCenterOfMasses_();
		}

		// iterate over all nodes in the graph (for now, only the leaves)
		BGL_FORALL_VERTICES(v, cluster_tree_, ClusterTree)
		{
			// get the index of the snapshot stored in the node
			// (there is exactly one pose in each node, so this works...)
			Index snapshot_index = *(cluster_tree_[v].poses.begin());

			switch (rmsd_type)
			{
				case RIGID_RMSD:
					cluster_tree_[v].center = RigidTransformation(translations_[snapshot_index],
							                                             rotations_[snapshot_index]);
					break;
				case SNAPSHOT_RMSD:
			  {
					(*current_set_)[snapshot_index].applySnapShot(system_i_);

					Eigen::VectorXf center(3*number_of_selected_atoms_);

					// consider the choice of option RMSD_LEVEL_OF_DETAIL
					Index current_atom = 0;
					for (AtomConstIterator atom_it = system_i_.beginAtom(); +atom_it; ++atom_it)
					{
						if (!isExcludedByLevelOfDetail_(&(*atom_it), rmsd_level_of_detail))
						{
							Vector3 const& current_pos = atom_it->getPosition();

							center[3*current_atom  ] = current_pos.x;
							center[3*current_atom+1] = current_pos.y;
							center[3*current_atom+2] = current_pos.z;

							current_atom++;
						}
					}

					cluster_tree_[v].center = center;
					break;
				}
				case CENTER_OF_MASS_DISTANCE:
				{
					// CENTER_OF_MASS_DISTANCE ignores the option RMSD_LEVEL_OF_DETAIL 
					// NOTE: it would look nicer to center the translations, such that com_[0] == 0
					//       however, this is not really necessary, as we just look at differences of
					//       positions, which are independent of this constant shift
					RigidTransformation com_transform;
					(com_transform.translation  << com_[snapshot_index].x,
					                               com_[snapshot_index].y,
																				 com_[snapshot_index].z).finished();

					com_transform.rotation = Eigen::Matrix3f::Zero();

					cluster_tree_[v].center = com_transform;
					break;
				}
				default:
					Log.info() << "initWardDistance_(): unknown rmsd score type " << __FILE__ <<  " " << __LINE__ << endl;
			}
		}
	}


	void PoseClustering::updateWardDistance_(ClusterTreeNode parent, ClusterTreeNode i, ClusterTreeNode j, Index rmsd_type)
	{
		Size cluster_size_a  = cluster_tree_[i].size;
		Size cluster_size_b  = cluster_tree_[j].size;
		Size cluster_size_ab = cluster_tree_[parent].size;

		switch (rmsd_type)
		{
			case RIGID_RMSD:
			{
				RigidTransformation& transform_i = boost::get<RigidTransformation>(cluster_tree_[i].center);
				RigidTransformation& transform_j = boost::get<RigidTransformation>(cluster_tree_[j].center);

				Eigen::Vector3f new_translation = (  (transform_i.translation * cluster_size_a)
																					 + (transform_j.translation * cluster_size_b)
																					)/cluster_size_ab;

				Eigen::Matrix3f new_rotation    = (  (transform_i.rotation * cluster_size_a)
																					 + (transform_j.rotation * cluster_size_b)
																					)/cluster_size_ab;

				cluster_tree_[parent].center = RigidTransformation(new_translation, new_rotation);

				break;
			}
			case SNAPSHOT_RMSD:
			{
				Eigen::VectorXf& cluster_a = boost::get<Eigen::VectorXf>(cluster_tree_[i].center);
				Eigen::VectorXf& cluster_b = boost::get<Eigen::VectorXf>(cluster_tree_[j].center);

				Eigen::VectorXf new_centroid = (cluster_a * cluster_size_a + cluster_b * cluster_size_b) / cluster_size_ab;

				cluster_tree_[parent].center = new_centroid;

				break;
			}
			case CENTER_OF_MASS_DISTANCE:
			{
				RigidTransformation& transform_i = boost::get<RigidTransformation>(cluster_tree_[i].center);
				RigidTransformation& transform_j = boost::get<RigidTransformation>(cluster_tree_[j].center);

				Eigen::Vector3f new_translation = (  (transform_i.translation * cluster_size_a)
																					 + (transform_j.translation * cluster_size_b)
																					)/cluster_size_ab;

				cluster_tree_[parent].center = RigidTransformation(new_translation, Matrix3f::Zero());

				break;
			}
			default:
				Log.info() << "updateWardDistance_(): unknown rmsd score type " << __FILE__ <<  " " << __LINE__ << endl;
		}
	}


	float PoseClustering::computeWardDistance_(ClusterTreeNode i, ClusterTreeNode j, Index rmsd_type)
	{
		Size cluster_size_a = cluster_tree_[i].size;
		Size cluster_size_b = cluster_tree_[j].size;

		float result = -1.;

		switch (rmsd_type)
		{
			// TODO: check if the formulas are correct
			case RIGID_RMSD:
			{
				RigidTransformation& transform_i = boost::get<RigidTransformation>(cluster_tree_[i].center);
				RigidTransformation& transform_j = boost::get<RigidTransformation>(cluster_tree_[j].center);

				result =  pow(getRigidRMSD(transform_i.translation - transform_j.translation,
																	 transform_j.rotation    - transform_j.rotation,
																	 covariance_matrix_), 2) / ( 1./cluster_size_a  + 1./cluster_size_b)
							   * number_of_selected_atoms_;
				break;
			}
			case SNAPSHOT_RMSD:
			{
				Eigen::VectorXf& cluster_a = boost::get<Eigen::VectorXf>(cluster_tree_[i].center);
				Eigen::VectorXf& cluster_b = boost::get<Eigen::VectorXf>(cluster_tree_[j].center);

				result = (cluster_a - cluster_b).squaredNorm() / ( 1./cluster_size_a + 1./cluster_size_b);

				break;
			}
			case CENTER_OF_MASS_DISTANCE:
			{
				RigidTransformation& transform_i = boost::get<RigidTransformation>(cluster_tree_[i].center);
				RigidTransformation& transform_j = boost::get<RigidTransformation>(cluster_tree_[j].center);

				result = (transform_i.translation - transform_j.translation).squaredNorm() / ( 1./cluster_size_a  + 1./cluster_size_b)
				         *number_of_selected_atoms_;
				break;
			}
			default:
				Log.info() << "getWardDistance_(): unknown rmsd score type " << __FILE__ <<  " " << __LINE__ << endl;
		}

		return result;
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


	// precompute the atom bijection
	void PoseClustering::precomputeAtomBijection_()
	{
		int rmsd_type = options.getInteger(Option::RMSD_TYPE);

		rmsd_level_of_detail_ = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		if (rmsd_type == PoseClustering::RIGID_RMSD)
		{
			system_i_ = System(base_system_);
			system_j_ = System(base_system_);
		}
		else
		{
			system_i_ = System(current_set_->getSystem());
			system_j_ = System(current_set_->getSystem());
		}

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
	}


	bool PoseClustering::centerOfGravityPreCluster_()
	{
//cout << " centerOfGravityPreCluster_() alg = " << options.getInteger(Option::CLUSTER_METHOD) << " " << CLINK_DEFAYS << endl;
		// store the old options
		Options old_options = options;

		// run a pre-clustering
		// with basically the same options
		// but center of mass distance as rmsd
		options.set(Option::RMSD_TYPE, CENTER_OF_MASS_DISTANCE);
		// and no additional pre clustering
		options.set(Option::USE_CENTER_OF_MASS_PRECLINK, false);
		compute();

		// store the new clusters
		std::vector<std::set<Index> >   temp_clusters;

		// reset the options
		options = old_options;

		PoseClustering inner_pc;
		Size num_clusters = 0;
		// now iterate over all clusters 
		for (Size i=0; i<getNumberOfClusters(); i++)
		{
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
//cout << "++++++++++++++++++++++++++++++***" << endl;
//				inner_pc.printClusterRMSDs();
				//cout << " cluster " << i << "( " << getClusterSize(i) << " ) was split into " << num_clusters << " clusters." << endl;
//cout << "***++++++++++++++++++++++++++++++***" << endl;
			}
			else
			{
				temp_clusters.push_back(getCluster(i));
			}
			//std::set<Index>& current_cluster = getCluster(i);
		}
		// switch the clusters
		clusters_ = temp_clusters;
		//printClusters();

		return true;
	}


	bool PoseClustering::isExcludedByLevelOfDetail_(Atom const* atom, Index rmsd_level_of_detail)
	{
		bool result = true;
		if (    (rmsd_level_of_detail == ALL_ATOMS)
			  || ((rmsd_level_of_detail == C_ALPHA)  && (atom->getName() == "CA"))
				|| ((rmsd_level_of_detail == BACKBONE) && (    (atom->getName() == "CA")
					                                             || (atom->getName() == "C")
																											 || (atom->getName() == "O")
																											 || (atom->getName() == "N")
																											 || (atom->getName() == "H")))
					|| (   (rmsd_level_of_detail == PROPERTY_BASED_ATOM_BIJECTION)
					    && (atom->hasProperty("ATOMBIJECTION_RMSD_SELECTION"))))
		{
			result = false;
		}
		return result;
	}


	void PoseClustering::printClusterRMSDs()
	{
		Index rmsd_type = options.getInteger(PoseClustering::Option::RMSD_TYPE);

		cout << endl <<  "For method " << options.getInteger(PoseClustering::Option::CLUSTER_METHOD)
		     << " and RMSDtype " << rmsd_type << " we get: " << endl;

		for (Position i=0; i<clusters_.size(); ++i)
		{
			cout << "=======================================" << endl;
			cout << "    Cluster " << i << endl;

			std::set<Index>& current_cluster = clusters_[i];

			cout << "     ";
			for (std::set<Index>::iterator it_k=current_cluster.begin(); it_k!=current_cluster.end(); ++it_k)
			{
				cout << *it_k << "   ";
			}
			cout << endl;

			for (std::set<Index>::iterator it_j=current_cluster.begin(); it_j!=current_cluster.end(); ++it_j)
			{
				if (rmsd_type == SNAPSHOT_RMSD)
				{
					(*current_set_)[*it_j].applySnapShot(system_i_);
				}

				cout << " " <<  *it_j << " | ";

				for (std::set<Index>::iterator it_k=current_cluster.begin(); it_k!=current_cluster.end(); ++it_k)
				{
					if (rmsd_type == SNAPSHOT_RMSD)
					{
						(*current_set_)[*it_k].applySnapShot(system_j_);
					}

					cout << getRMSD_(*it_j, *it_k, rmsd_type) << " ";
				}

				cout << endl;
			}

			cout << "=======================================" << endl << endl;
		} //next cluster
	}

	void PoseClustering::exportClusterTree(std::ostream& out)
	{
		boost::write_graphviz(out, cluster_tree_, ClusterTreeWriter_(&cluster_tree_));
	}

	void PoseClustering::ClusterTreeWriter_::operator() (std::ostream& out, const ClusterTreeNode& v) const
	{
		out << "[label=\"";
		if ((*cluster_tree_)[v].poses.size() > 0)
			out << *((*cluster_tree_)[v].poses.begin());
		else
			out << (*cluster_tree_)[v].merged_at;
		out << "\"]";
	}

	const System& PoseClustering::getSystem() const
	{
		if (current_set_)
		{
			return current_set_->getSystem();
		}
		else
		{
			// TODO: do we have to make sure, that the system is in 
			// the initial conformation state?
			// // apply the reference state
			//base_conformation_.applySnapShot(base_system_);

			return base_system_;
		}
	}


	System& PoseClustering::getSystem()
	{
		if (current_set_)
		{
			return current_set_->getSystem();
		}
		else
		{
			return base_system_;
		}
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

		boost::shared_ptr<System> new_system(new System(getSystem()));

		// as cluster representative we simply take the first cluster member
		// this is a very simple heuristic :-)
		// TODO: find the median?

		// take the first
		Index conf_set_idx = *(clusters_[i].begin());

		if (has_rigid_transformations_ && !current_set_)
		{
			GeometricCenterProcessor center;
			new_system->apply(center);
			Vector3 toOrigin = center.getCenter().negate();

			TranslationProcessor translation;
			translation.setTranslation(toOrigin);
			new_system->apply(translation);

			// apply transformation and rotation	
			TransformationProcessor transformation;
			transformation.setTransformation(Matrix4x4(rotations_[conf_set_idx](0,0), rotations_[conf_set_idx](0,1), rotations_[conf_set_idx](0,2), translations_[conf_set_idx](0),
			                                           rotations_[conf_set_idx](1,0), rotations_[conf_set_idx](1,1), rotations_[conf_set_idx](1,2), translations_[conf_set_idx](1),
			                                           rotations_[conf_set_idx](2,0), rotations_[conf_set_idx](2,1), rotations_[conf_set_idx](2,2), translations_[conf_set_idx](2),
			                                           0, 0, 0, 1));
			new_system->apply(transformation);

			// move back to its original center
			translation.setTranslation(toOrigin.negate());
			new_system->apply(translation);
		}
		else
		{
			SnapShot sn = (*current_set_)[conf_set_idx];

			sn.applySnapShot(*(new_system.get()));
		}
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

//		options.setDefault(PoseClustering::Option::FULL_CLUSTER_DENDOGRAM,
//				               PoseClustering::Default::FULL_CLUSTER_DENDOGRAM);
	}


	boost::shared_ptr<ConformationSet> PoseClustering::getClusterConformationSet(Index i)
	{
		if (i >= (Index)clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		if (current_set_==NULL)
		{
			// originally we were given transformations, not snapshots
			// lets create some
			convertTransformations2Snaphots_();
		}
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


	boost::shared_ptr<ConformationSet> PoseClustering::getReducedConformationSet()
	{
		if (clusters_.size()==0)
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		if (current_set_==NULL)
		{
			// originally we were given transformations, not snapshots
			// lets create some
			convertTransformations2Snaphots_();
		}

		// create the set to be returned
		boost::shared_ptr<ConformationSet> new_set(new ConformationSet());
		new_set->setup(getSystem());
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
	// this method is used by the trivial complete linkage clustering and computes
	// the maximum distance between cluster members
	float PoseClustering::getClusterRMSD_(Index i, Index j, Index rmsd_type)
	{
		// we have to compute the maximal distance
		float rmsd = std::numeric_limits<float>::min();
		float temp_rmsd = std::numeric_limits<float>::max();
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

				if (temp_rmsd > rmsd)
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
			// TODO: we should probably use RMSDMinimizer instead!
			//       or even better, merge StructureMapper and RMSDMinimizer
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


	void PoseClustering::convertTransformations2Snaphots_()
	{
		if (current_set_ != NULL)
		{
			Log.info() << "Warning: previous snapshots are overwritten!" << __FILE__ << " " << __LINE__ << endl;
		}
		else
		{
			// create a new ConformationSet
			current_set_ =  new ConformationSet();
			delete_conformation_set_= true;

			//Note: we cannot use getSystem here, since
			// we use the existence of current_set_ to distinguish
			// the SnapShot from the Transformation setting
			current_set_->setup(base_system_);
		}

		// pre compute some general transformations
		System new_template_system(base_system_);
		// apply the reference state
		base_conformation_.applySnapShot(new_template_system);

		// compute the translation vector to the origin
		GeometricCenterProcessor center;
		new_template_system.apply(center);
		Vector3 toOrigin = center.getCenter().negate();
		// move to origin
		TranslationProcessor translation;
		translation.setTranslation(toOrigin);
		new_template_system.apply(translation);

		TransformationProcessor transformation;

		// create one snapshot for each transformation
		for (Size i=0; i<translations_.size(); i++)
		{
			// create a copy
			System new_system(new_template_system);
			// apply transformation and rotation
			transformation.setTransformation(Matrix4x4(rotations_[i](0,0), rotations_[i](0,1), rotations_[i](0,2), translations_[i](0) - toOrigin.x,
			                                           rotations_[i](1,0), rotations_[i](1,1), rotations_[i](1,2), translations_[i](1) - toOrigin.y,
			                                           rotations_[i](2,0), rotations_[i](2,1), rotations_[i](2,2), translations_[i](2) - toOrigin.z,
			                                           0, 0, 0, 1));

			new_system.apply(transformation);

			// add the conformation
			current_set_->add(0, new_system);
		}
	}


	void PoseClustering::convertSnaphots2Transformations_()
	{
		// just to be sure...
		translations_.clear();
		rotations_.clear();
		num_poses_ = getNumberOfPoses();

		precomputeAtomBijection_();

		System transformed_system(current_set_->getSystem());
		base_system_ = current_set_->getSystem();

		// we iterate over all the snapshots
		for (Position current_snap = 0; current_snap < num_poses_; ++current_snap)
		{
			// apply the snapshot
			(*current_set_)[current_snap].applySnapShot(transformed_system);

			// compute the best mapping of the base system to the transformed one
			RMSDMinimizer::Result transform = RMSDMinimizer::computeTransformation(atom_bijection_);
			Matrix4x4& bm = transform.first;

			// convert the contained matrix to Eigen translation vector & rotation matrix
			Eigen::Vector3f translation;
			translation << bm.m14, bm.m24, bm.m34;

			translations_.push_back(translation);

			Eigen::Matrix3f rotation;
			rotation << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;

			rotations_.push_back(rotation);
		}

		has_rigid_transformations_ = true;
	}


	void PoseClustering::printCluster_(Index i) const
	{
		cout << "++++ cluster " << i << " ++++" << endl;
		std::copy(clusters_[i].begin(), clusters_[i].end(), std::ostream_iterator<Index>(std::cout, " "));
		cout << endl;
		cout << "+++++++++++++++++++" << endl;
	}


	void PoseClustering::printClusters() const
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
		if (delete_conformation_set_)
			delete current_set_;
		has_rigid_transformations_ = false;
		delete_conformation_set_ = false;
		current_set_ = NULL;
		translations_.clear();
		rotations_.clear();
		covariance_matrix_.setZero();
		base_system_.clear();
		base_conformation_.clear();
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
		number_of_selected_atoms_ = 0;
	}
}
