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
#include <queue>

#include <boost/version.hpp>

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adj_list_serialize.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/set.hpp>
#include <boost/serialization/variant.hpp>

using namespace std;
using namespace Eigen;

namespace BALL
{
	const String PoseClustering::Option::CLUSTER_METHOD  = "cluster_method";
	const Index  PoseClustering::Default::CLUSTER_METHOD = PoseClustering::CLINK_DEFAYS;

	const String PoseClustering::Option::RMSD_LEVEL_OF_DETAIL  = "rmsd_level_of_detail";
	const Index  PoseClustering::Default::RMSD_LEVEL_OF_DETAIL = PoseClustering::C_ALPHA;

	const String PoseClustering::Option::DISTANCE_THRESHOLD = "pose_clustering_rmsd_threshold";
  const float PoseClustering::Default::DISTANCE_THRESHOLD = 3;

	const String PoseClustering::Option::RMSD_TYPE = "pose_clustering_rmsd_type";
	const Index PoseClustering::Default::RMSD_TYPE = PoseClustering::SNAPSHOT_RMSD;

	const String PoseClustering::Option::RUN_PARALLEL  = "pose_clustering_run_parallel";
	const bool   PoseClustering::Default::RUN_PARALLEL = true;


	PoseClustering::PoseClustering()
		: current_set_(0),
			has_rigid_transformations_(false),
			delete_conformation_set_(false)
	{
		setDefaultOptions();
	}


	PoseClustering::PoseClustering(ConformationSet* poses, float rmsd)
	{
		options[Option::DISTANCE_THRESHOLD] = rmsd;
		current_set_ = poses;

		storeSnapShotReferences_();

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


	std::vector<std::set<Index> > PoseClustering::extractClustersForThreshold(float threshold, Size min_size)
	{
//	cout << " extract clusters for dist = " << threshold << endl;

		clusters_.clear();
		cluster_representatives_.clear();
		cluster_scores_.clear();

		// for debug purposes, we might have to reset our cluster id
#ifdef POSECLUSTERING_DEBUG
		BGL_FORALL_VERTICES(current_vertex, cluster_tree_, ClusterTree)
		{
			cluster_tree_[current_vertex].current_cluster_id = -1;
		}
#endif

		std::stack<ClusterTreeNode> stack;
		ClusterTreeNode current_node;

		ClusterTreeNode cluster_tree_root = cluster_tree_[boost::graph_bundle];

		if (cluster_tree_[cluster_tree_root].merged_at < threshold)
		{
			clusters_.push_back(collectClusterBelow_(cluster_tree_root));
			cluster_scores_.push_back(cluster_tree_[cluster_tree_root].merged_at);

#ifdef POSECLUSTERING_DEBUG 
			cluster_tree_[cluster_tree_root].current_cluster_id = clusters_.size()-1;
#endif
		}
		else
		{
			stack.push(cluster_tree_root);

			while (!stack.empty())
			{
				current_node = stack.top();
			  stack.pop();

				BGL_FORALL_ADJ(current_node, child, cluster_tree_, ClusterTree)
				{
					// is child itself a leaf?
					if (   (cluster_tree_[child].merged_at < threshold)
						  || (out_degree(child, cluster_tree_) == 0) )
					{
						clusters_.push_back(collectClusterBelow_(child));
						cluster_scores_.push_back(cluster_tree_[child].merged_at);

#ifdef POSECLUSTERING_DEBUG 
						cluster_tree_[child].current_cluster_id = clusters_.size()-1;
#endif
					}
					else
					{
						stack.push(child);
					}
				}
			}
		}

		if (min_size > 1)
		{
			std::vector< std::set<Index> >::iterator iter = clusters_.begin();
			std::vector< float >::iterator score_iter = cluster_scores_.begin();

			while (iter != clusters_.end())
			{
        if ((*iter).size() < min_size)
				{
					 iter = clusters_.erase(iter);
					 score_iter = cluster_scores_.erase(score_iter);
        }
        else
        {
           ++iter;
					 ++score_iter;
        }
			}
		}

		return clusters_;
	}


	// up to n best clusters
	std::vector<std::set<Index> > PoseClustering::extractNBestClusters(Size n)
	{
//cout << "extractNBestClusters  n=" << n << "/" << getNumberOfPoses() << endl;

		// we cannot rely on getNumberOfPoses() as we may have been given a tree...
		/*
		if (n > getNumberOfPoses())
			throw(Exception::OutOfRange(__FILE__, __LINE__));
		*/

		clusters_.clear();
		cluster_representatives_.clear();
		cluster_scores_.clear();

		ClusterTreeNodeComparator comp(cluster_tree_);
		std::priority_queue< ClusterTreeNode, std::vector<ClusterTreeNode>, ClusterTreeNodeComparator >  prio(comp);
		ClusterTreeNode current_node;

		ClusterTreeNode cluster_tree_root = cluster_tree_[boost::graph_bundle];

		prio.push(cluster_tree_root);

		// read ahead
		current_node = prio.top();

		// first we look out for the first n best clusters
		// we traverse the tree from the root in descending order
		// of merged_at values
		while ( (prio.size() + clusters_.size() < n ) &&
					 (!prio.empty()) //&& (out_degree(current_node, cluster_tree_) > 0)
					)
		{
			prio.pop();

			// check all children
			BGL_FORALL_ADJ(current_node, child, cluster_tree_, ClusterTree)
			{
				// no leaf, traverse further
				if (out_degree(child, cluster_tree_) > 0)
				{
					prio.push(child);
				}
				else // a leaf --> store as "cluster"
				{
					//std::set<Index> new_cluster = collectClusterBelow_(child);
					std::set<Index> new_cluster;
					new_cluster.insert(*(cluster_tree_[child].poses.begin()));
					clusters_.push_back(new_cluster);
					cluster_scores_.push_back(cluster_tree_[current_node].merged_at);
				}
			}
			current_node = prio.top();
		}
//cout << "found clusters: " << clusters_.size() << " size prio: " << prio.size() << " check min_size filter..." << endl;

		// then we collect all clusters that pass the min_size filter
		// --> we may end up with less than n clusters
		while (!prio.empty())
		{
			// no leaves have been added to the priority queue
			std::set<Index> new_cluster = collectClusterBelow_(prio.top());

/*for (std::set<Index>::iterator s_it = new_cluster.begin(); s_it !=  new_cluster.end(); ++s_it)
	cout << *s_it << " ";
cout << endl;
*/
			if (new_cluster.size() > 0) //= min_size)
			{
				clusters_.push_back(new_cluster);
				cluster_scores_.push_back(cluster_tree_[prio.top()].merged_at);
			}
			prio.pop();
//cout << clusters_.size() << " " << prio.size() << endl;
		}

//cout << "after min size filter we finally get: " << clusters_.size() << endl;

		return clusters_;
	}


	std::vector<std::set<Index> > PoseClustering::filterClusters(Size min_size)
	{
//cout << "Got " << clusters_.size() << " clusters and min_size= " << min_size << endl;
		std::vector< std::set<Index> >   temp_clusters;
		std::vector< float >             temp_cluster_scores;

//int c_counter = 0;
		for (Size i = 0; i < clusters_.size(); i++)
		{
			if (clusters_[i].size() >= min_size)
			{
/*
c_counter++;
cout << "  ++ " << c_counter << " (" << clusters_[i].size() << "): ";
for (std::set<Index>::iterator s_it = clusters_[i].begin(); s_it !=  clusters_[i].end(); ++s_it)
	cout << *s_it << " ";
cout << endl;
*/

				temp_clusters.push_back(clusters_[i]);
				temp_cluster_scores.push_back(cluster_scores_[i]);
			}
		}
		swap(clusters_, temp_clusters);
		swap(cluster_scores_, temp_cluster_scores);
		cluster_representatives_.clear();

		return clusters_;
	}

	void PoseClustering::setConformationSet(ConformationSet* new_set, bool precompute_atombijection)
	{
		if (delete_conformation_set_)
			delete current_set_;
		has_rigid_transformations_ = false;
		transformations_.clear();

		current_set_ = new_set;
		base_system_ = new_set->getSystem();
		storeSnapShotReferences_();

		if (precompute_atombijection)
			precomputeAtomBijection_();
	}


	void PoseClustering::setBaseSystemAndPoses(System const& base_system, std::vector<PosePointer> const& poses)
	{
		base_system_ = base_system;
		poses_ = poses;

		// make sure we don't have old transformations or poses flying around
		transformations_.clear();
		if (delete_conformation_set_)
		{
			delete current_set_;
		}
		current_set_ = 0;
	}


	void PoseClustering::setBaseSystemAndTransformations(System const& base_system, String transformation_file_name)
	{
		// clear any potential pointers to old poses	
		if (delete_conformation_set_)
		{
			delete current_set_;
		}
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
//cout << " compute() t=" <<  true << "/f=" << false << " r="  << has_rigid_transformations_ << " t=" << rmsd_type << endl;

		Size num_poses = getNumberOfPoses();

		if (num_poses == 0)
		{
			Log.warn() << "Given input is empty (getNumberOfPoses() == 0)! Nothing to cluster, abort." << endl;

			return false;
		}

		// first, we should figure out if we have been fed the right type of input for the choice of rmsd...
		// TODO: center of mass distance should really not only be defined for snapshots...
		Index rmsd_type = options.getInteger(Option::RMSD_TYPE);

		if ( (rmsd_type == SNAPSHOT_RMSD) || (rmsd_type == CENTER_OF_MASS_DISTANCE) )
		{
			// obviously, we need snapshots
			if (poses_[0].snap == 0)
			{
				// but we don't have them... so let's produce some...
				convertTransformations2Snaphots();
			}
		}
		else if (rmsd_type == RIGID_RMSD)
		{
			// obviously, we need transforms
			if (poses_[0].trafo == 0)
			{
				// but we don't have them... so let's produce some...
				convertSnaphots2Transformations();
			}
		}

		// precompute the atom bijection, that handles the 
		// option RMSD_LEVEL_OF_DETAIL
		precomputeAtomBijection_();

		bool result = false;

		// decide which algorithm to apply
		switch (options.getInteger(Option::CLUSTER_METHOD))
		{
			case TRIVIAL_COMPLETE_LINKAGE:
				result = trivialCompute_();
				break;
			case SLINK_SIBSON:
			case CLINK_DEFAYS:
				result = linearSpaceCompute_();
				break;
			case NEAREST_NEIGHBOR_CHAIN_WARD:
				result = nearestNeighborChainCompute_();
				break;
			case CLINK_ALTHAUS:
				result = althausCompute_();
				break;
			default:
				Log.error() << "Unknown parameter for option CLUSTER_METHOD " << options.get(Option::CLUSTER_METHOD) << endl;
				result = false;
		}

		return result;
	}


	bool PoseClustering::readTransformationsFromFile_(String filename)
	{
		poses_.clear();
		transformations_.clear();

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

				RigidTransformation rd;

				rd.translation << fields[6].toFloat(), fields[10].toFloat(), fields[14].toFloat();

				rd.rotation << fields[ 3].toFloat(),  fields[ 4].toFloat(), fields[ 5].toFloat(),
				               fields[ 7].toFloat(),  fields[ 8].toFloat(), fields[ 9].toFloat(),
											 fields[11].toFloat(),  fields[12].toFloat(), fields[13].toFloat();

				transformations_.push_back(rd);
			}
			else
			{
//				Log.info() << "Skipped line " << current_transformation << " while reading file " << filename << endl;
			}
		}

		// now, store the transformations as poses (we cannot do this in the former loop, as the vector
		// may be resized, which would change the pointers)
		for (Position i=0; i<transformations_.size(); ++i)
		{
			poses_.push_back(PosePointer(&(transformations_[i])));
		}

		return true;
	}


	bool PoseClustering::trivialCompute_()
	{
		// in the beginning each pose is a cluster
		// this causes O(n^2) space!!	
		Size num_poses = getNumberOfPoses();
		pairwise_scores_.resize(num_poses, num_poses);

		// clean up old clusters
		clusters_.clear();
		cluster_representatives_.clear();
		cluster_scores_.clear();

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

		// in the beginning each pose is a cluster itself
		for (Size i=0; i<num_poses; i++)
		{
			// add as new cluster
			set<Index> new_set;
			new_set.insert((Index) i);
			clusters_.push_back(new_set);
			cluster_scores_.push_back(0.);

			// and add it into the tree
			ClusterTreeNode v = boost::add_vertex(cluster_tree_);
			cluster_tree_[v].poses.insert((Index) i);
			cluster_tree_[v].size = 1;

			// TODO: continue with the cluster tree... add center if required ...
			// compute the rmsd
			if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
			{
				poses_[i].snap->applySnapShot(system_i_);
			}

			pairwise_scores_(i,i) = 0;
			for (Size j=i+1; j<num_poses; j++)
			{
				if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
				{
					poses_[j].snap->applySnapShot(system_j_);
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
		// gets lower than the DISTANCE_THRESHOLD
		float rmsd_cutoff = options.getReal(Option::DISTANCE_THRESHOLD);
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

			// the max rmsd between all pairs in the new cluster 
			// is the clusters score
			cluster_scores_[min_cluster_a_idx] = min_max_cluster_dist;

			// mark as merged
			min_cluster_b->clear();
			pairwise_scores_(min_cluster_b_idx, min_cluster_b_idx) = std::numeric_limits<float>::max();

//cout << "found a new cluster by merging " << min_cluster_a_idx << " with " << min_cluster_b_idx << " with rmsd " << min_max_cluster_dist <<  " / " << rmsd_cutoff << endl;
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

			for (Size i=0; i<num_poses; i++)
			{
				// skip already seen clusters
				if (clusters_[i].size() > 0)
				{
					for (Size j=i+1; j<num_poses; j++)
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
	//cout << "new min " << min_max_cluster_dist << " btw " << i << " " << j << endl;
							}
						}
					}
				}
			}
		}

		// collect all final clusters
		std::vector< std::set<Index> >   temp_clusters;
		std::vector< float >             temp_cluster_scores;

		for (Size i = 0; i < clusters_.size(); i++)
		{
			if (clusters_[i].size() > 0)
			{
				temp_clusters.push_back(clusters_[i]);
				temp_cluster_scores.push_back(cluster_scores_[i]);
			}
		}

		swap(clusters_, temp_clusters);
		swap(cluster_scores_, temp_cluster_scores);

//cout << "Final num of clusters " << clusters_.size() << endl;
//printClusters();
//printClusterScores();

		return true;
	}


	bool PoseClustering::linearSpaceCompute_()
	{
		Size num_poses = getNumberOfPoses();

		float threshold = options.getReal(Option::DISTANCE_THRESHOLD);

		Index rmsd_type = options.getInteger(Option::RMSD_TYPE);
		if (rmsd_type == PoseClustering::CENTER_OF_MASS_DISTANCE)
		{
			computeCenterOfMasses_();
		}

		// we will need arrays pi, lambda, and mu
		lambda_.resize(num_poses);
		pi_.resize(num_poses);
		mu_.resize(num_poses);

//cout << " num poses: " << num_poses << endl;

		// and initialize them for the first point
		pi_[0] = 0;
		lambda_[0] = numeric_limits<double>::max();

		for (Size current_level=1; current_level < num_poses; ++current_level)
		{
			// TEST
			if (current_level % 1000 == 0)
			{
				double percentage = current_level / (double)num_poses;
				percentage *= 100. * percentage;
std::cout << current_level << " " << num_poses << " " << percentage << std::endl;
			}
			// END TEST

			pi_[current_level] = current_level;
			lambda_[current_level] = numeric_limits<double>::max();

			if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
			{
				poses_[current_level].snap->applySnapShot(system_i_);
			}

			for (Size j=0; j<current_level; ++j)
			{
				if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
				{
					poses_[j].snap->applySnapShot(system_j_);
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
		cluster_representatives_.clear();
		cluster_scores_.clear();

//for (int i=0; i<pi_.size(); ++i)
//	std::cout << i << " " << pi_[i] << " " << lambda_[i] << std::endl;

		// we need a helper for the clusters
		std::vector<std::set<Index> > cluster_helper;
		std::vector<float> score_helper(num_poses);

		// first, each point is its own cluster
		for (Position i=0; i < num_poses; ++i)
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

				// store the value at which this was merged
				score_helper[pi_[current_cluster]] = lambda_[current_cluster];
//		std::cout << "merging into " << pi_[current_cluster] << " with " << lambda_[current_cluster] << std::endl;
				// save some memory
				cluster_helper[current_cluster].clear();
			}
			else
			{
				// this cluster is done => put it into the result
				clusters_.push_back(cluster_helper[current_cluster]);
				// save some memory
				cluster_helper[current_cluster].clear();

				cluster_scores_.push_back(score_helper[current_cluster]);
			}
		}
//printClusterScores();
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

	bool PoseClustering::althausCompute_()
	{
		return false;
	}

	bool PoseClustering::nearestNeighborChainCompute_()
	{
		Size num_poses = getNumberOfPoses();

		// we might need the number of atoms to which our ward distances apply later on
		// this number depends on the choice of RMSD_LEVEL_OF_DETAIL
		Index rmsd_level_of_detail = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);
		number_of_selected_atoms_ = 0;
		for (AtomConstIterator at_it = getSystem().beginAtom(); +at_it; ++at_it)
		{
			if (!isExcludedByLevelOfDetail_(&*at_it, rmsd_level_of_detail))
			{
				++number_of_selected_atoms_;
			}
		}
		// clean up old clusters
		clusters_.clear();
		cluster_representatives_.clear();
		cluster_scores_.clear();

		// reset the cluster tree
		cluster_tree_.clear();

		Index rmsd_type = options.getInteger(PoseClustering::Option::RMSD_TYPE);

		bool run_parallel = options.getBool(PoseClustering::Option::RUN_PARALLEL);

		float min_cluster_dist = std::numeric_limits<float>::max();

		// the active clusters (as nodes in the cluster tree)
		std::vector<ClusterTreeNode> active_clusters;
		active_clusters.reserve(num_poses);

		// stack of clusters (as indices into the active_cluster-vector) storing nearest neighbor chains
		std::deque<Index> cluster_stack;

		// in the beginning each pose is a cluster itself
		for (Size i=0; i<num_poses; i++)
		{
			// add it as a new cluster to the tree
			ClusterTreeNode v = boost::add_vertex(cluster_tree_);
			cluster_tree_[v].poses.insert((Index) i);
			cluster_tree_[v].size = 1;
			cluster_tree_[v].merged_at = 0.;
#ifdef POSECLUSTERING_DEBUG 
			cluster_tree_[v].current_cluster_id = -1;
#endif

			active_clusters.push_back(v);
		}

//cout << active_clusters.size() << " " << cluster_stack.size() << std::endl;

		initWardDistance_(rmsd_type);

		Index current_cluster, nearest_cluster;

		Size num_active_clusters = num_poses;

		// while there is more than one final cluster
		while (num_active_clusters > 1)
		{
			// TEST
			if (num_active_clusters % 1000 == 0)
			{
				cout << "active clusters: " << num_active_clusters << " (" << 100.*num_active_clusters / (double)num_poses << ")" <<endl;
			}
			// END TEST

			min_cluster_dist = std::numeric_limits<float>::max();
			nearest_cluster = num_active_clusters - 1;

			// if stack is empty add randomly
			if (cluster_stack.empty())
			{
				// we simply take the first
				cluster_stack.push_back(0);
			}
			current_cluster = cluster_stack.back();

			// Compute the distances to all other clusters
			// TODO: can this be guaranteed?
			//      If there may be multiple equal nearest neighbors to a cluster, the algorithm requires a consistent 
			//      tie-breaking rule: for instance, in this case, the nearest neighbor may be chosen, among the clusters 
			//       at equal minimum distance from current_cluster, by numbering the clusters arbitrarily and choosing 
			//       the one with the smallest index
#ifdef BALL_HAS_TBB
			if (run_parallel)
			{
//				std::cout << "Parallel computation...";
				ComputeNearestClusterTask_ root_task(this, active_clusters, current_cluster, rmsd_type);
				tbb::parallel_reduce(tbb::blocked_range<size_t>(0, num_active_clusters), root_task);
//				std::cout << "...done." << std::endl;

				nearest_cluster = root_task.getMinIndex();
				min_cluster_dist = root_task.getMinValue();
			}
			else
			{
#endif
			for (Index i=0; i<(Index)num_active_clusters; ++i)
			{
				// check all pairs between all_clusters[*clust_it] and all_clusters[current_cluster]
				if (i != current_cluster)
				{
					float rmsd = computeWardDistance_(active_clusters[current_cluster], active_clusters[i], rmsd_type);

					// for stability, make sure that "really" smaller cases are always taken...
					if (BALL_REAL_LESS(rmsd, min_cluster_dist, 1e-5))
					{
						nearest_cluster = i;
						min_cluster_dist = rmsd;
					}
				}
			}
#ifdef BALL_HAS_TBB
			}
#endif

			// have we put this cluster to the stack before ?
			// due to the reducibility of Ward's distance, this would imply that the cluster
			// was our predecessor.
			if (cluster_stack.size() == 1) // we don't have a predecessor => just push
			{
				cluster_stack.push_back(nearest_cluster);
				continue;
			}

			// look at the predecessor
			cluster_stack.pop_back(); // already saved in current_cluster

			Index predecessor = cluster_stack.back();
			if (predecessor == nearest_cluster)
			{
				cluster_stack.pop_back(); // pop the predecessor

				ClusterTreeNode current_node = active_clusters[current_cluster];
				ClusterTreeNode nearest_node = active_clusters[nearest_cluster];

				// add a new parent node to the tree
				ClusterTreeNode new_parent = boost::add_vertex(cluster_tree_);

				// make this parent the new root of the cluster tree
				// (the final root will be the last parent we add)
				cluster_tree_[boost::graph_bundle] = new_parent;

				// add an edge from the parent to both children
				boost::add_edge(new_parent, current_node, cluster_tree_);
				boost::add_edge(new_parent, nearest_node, cluster_tree_);

				// remember the value at which these were merged
				// NOTE: min_cluster_dist is the Ward distance, which is quadratic in the
				//       rmsds. We can, e.g., store RMSDs of the centroids, or the square root
				//       of the Ward distance. What we *will* store here, is the square root
				//       of the Ward distance per atom, i.e., the square root of the 
				//       squared average distance from the centroid per atom
				cluster_tree_[new_parent].merged_at = sqrt(min_cluster_dist / number_of_selected_atoms_);

				cluster_tree_[new_parent].size =    cluster_tree_[current_node].size
					                                + cluster_tree_[nearest_node].size;
#ifdef POSECLUSTERING_DEBUG 
			 cluster_tree_[new_parent].current_cluster_id = -1;
#endif
//cout << cluster_tree_[new_parent].size << " " << num_poses << std::endl;
//cout << " merged at: " << cluster_tree_[new_parent].merged_at << endl;

				// TODO: here, we could decide if we want to merge the sets from our
				//       children and explictly store them => save runtime, pay memory
				//       (this would look similar to the following...)
				/*
				min_cluster_a = &clusters_[current_node];
				min_cluster_b = &clusters_[nearest_node];

				// merge
				std::set<Index> temp_set;
				std::set_union(min_cluster_a->begin(), min_cluster_a->end(),
				              min_cluster_b->begin(), min_cluster_b->end(),
				              std::inserter(temp_set, temp_set.begin()));
				*/

				// Note: this needs to be done before clearing and swapping!
				updateWardDistance_(new_parent, current_node, nearest_node, rmsd_type);

				// make the old clusters inactive and add the new one
				// 
				// in order to avoid unnecessary erases from the middle of the vector, we
				// do the following:
				//
				// =====x====y=====k
				//
				// x and y are supposed to be erased, and we want to attach a new element behind k.
				// instead, we now put k in x, and the new thing in y, and remove the last element => O(1)
				//
				// NOTE: this might invalidate our consistent tie breaking rule!
				//
				active_clusters[std::max(current_cluster, nearest_cluster)] = active_clusters.back();
				active_clusters[std::min(current_cluster, nearest_cluster)] = new_parent;

				// stupidly, we have to update all references to the formerly last element in the stack
				for (std::deque<Index>::iterator s_it = cluster_stack.begin(); s_it != cluster_stack.end(); ++s_it)
				{
					if (*s_it == (Index)(num_active_clusters-1))
					{
						*s_it = std::max(current_cluster, nearest_cluster);
					}
				}

				active_clusters.pop_back();

				num_active_clusters--;

				cluster_stack.push_back(std::min(current_cluster, nearest_cluster));
			}
			else
			{
				cluster_stack.push_back(current_cluster);
				cluster_stack.push_back(nearest_cluster);
			}
		}

		// we automatically extract clusters if a reasonable threshold is provided.
		// distance threshold equal zero prohibits that to save runtime 
		float threshold = options.getReal(Option::DISTANCE_THRESHOLD);
		if (BALL_REAL_GREATER_OR_EQUAL(threshold, 0.0, 1e-5))
			extractClustersForThreshold(threshold);

		return true;
	}


	void PoseClustering::initWardDistance_(Index rmsd_type)
	{
		Index rmsd_level_of_detail = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		Index num_atoms = 0;

		if (rmsd_type == SNAPSHOT_RMSD)
		{
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
				{
					cluster_tree_[v].center = *(poses_[snapshot_index].trafo);
					break;
				}
				case SNAPSHOT_RMSD:
			  {
					poses_[snapshot_index].snap->applySnapShot(system_i_);

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
			case RIGID_RMSD:
			{
				RigidTransformation& transform_i = boost::get<RigidTransformation>(cluster_tree_[i].center);
				RigidTransformation& transform_j = boost::get<RigidTransformation>(cluster_tree_[j].center);

				result =  getSquaredRigidRMSD(transform_i.translation - transform_j.translation,
																	    transform_i.rotation    - transform_j.rotation,
																	    covariance_matrix_) / ( 1./cluster_size_a  + 1./cluster_size_b)
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


	std::set<Index> PoseClustering::collectClusterBelow_(ClusterTreeNode const& v)
	{
		// collect the respective leaves
		std::set<Index> result;

		// is v itself a leaf?
		if (out_degree(v, cluster_tree_) == 0)
		{
			result.insert(*(cluster_tree_[v].poses.begin()));
		}
		else
		{
			std::stack<ClusterTreeNode> stack;
			stack.push(v);
			ClusterTreeNode current_node;
			while (!stack.empty())
			{
				current_node = stack.top();
				stack.pop();

				BGL_FORALL_ADJ(current_node, child, cluster_tree_, ClusterTree)
				{
					if (out_degree(child, cluster_tree_) == 0) // we found a leaf
					{
						// currently, each leaf represents one single pose
						result.insert(*(cluster_tree_[child].poses.begin()));
					}
					else // something inbetween
					{
						stack.push(child);
					}
				}
			}
		}

		return result;
	}


	void PoseClustering::computeCenterOfMasses_()
	{
		GeometricCenterProcessor center;

		com_.resize(getNumberOfPoses());

		for (Size i=0; i<com_.size(); ++i)
		{
			poses_[i].snap->applySnapShot(system_i_);
			system_i_.apply(center);
			com_[i] = center.getCenter();
		}
	}


	// precompute the atom bijection
	void PoseClustering::precomputeAtomBijection_()
	{
		Index rmsd_level_of_detail = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		system_i_ = System(base_system_);
		system_j_ = System(base_system_);

		StructureMapper mapper(system_i_, system_j_);

		switch (rmsd_level_of_detail)
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


	bool PoseClustering::refineClustering(Options const& refined_options)
	{
		if (getNumberOfClusters() == 0)
		{
			// ok, nothing to do here...
			return true;
		}

		// this is going to be a little cumbersome... hence, to simplify handling of the member
		// variables, we will use a new PoseClustering instance 
		PoseClustering inner_pc;

		// in principle, things are simple: we iterate over all existing clusters, perform a new
		// clustering, and store the results
		//
		// in reality, things are less simple: for each of the inner clustering calls, we need
		// to generate suitable input without having too many copies of the poses or transformations
		// flying around at any point in time to avoid memory issues; also, indices will have to
		// be mapped back and forth
		Size num_curr_clusters = 0;

		// store the inner clusters temporarily
		std::vector< std::set<Index> > temp_clusters;
		std::vector< float >           temp_cluster_scores;

		// now iterate over all clusters 
		for (Size i=0; i<getNumberOfClusters(); i++)
		{
//cout << "\n Inner compute no " << i << " of " <<  getNumberOfClusters() << " #members:" << getClusterSize(i) << endl;

			// if the cluster has only one element, there is no need to split it
			if (getClusterSize(i)<=1)
			{
				temp_clusters.push_back(getCluster(i));
				temp_cluster_scores.push_back(getClusterScore(i));
			}
			else // multiple-element cluster
			{
				// temporarily convert the poses to be re-clustered to index set 0 ... getClusterSize(i)
				std::vector<Index>        current_cluster_vector(getClusterSize(i));
				std::vector<PosePointer>  current_pose_vector;

				Position temp_idx = 0;

				for (std::set<Index>::iterator clust_it = clusters_[i].begin();
						 clust_it != clusters_[i].end();
						 ++clust_it, temp_idx++)
				{
					current_cluster_vector[temp_idx] = *clust_it;
					current_pose_vector.push_back(poses_[*clust_it]);
				}

				// run a clustering separately on each pre cluster as described by the options
				inner_pc.clear_();

				// set the refinement options 
				inner_pc.options = refined_options;

				// set the poses for the inner pc
				inner_pc.setBaseSystemAndPoses(base_system_, current_pose_vector);
//cout << "       # given poses: " << current_pose_vector.size() << endl;

				inner_pc.compute();

				// add each cluster of the current inner run
				num_curr_clusters = inner_pc.getNumberOfClusters();
//cout << "                split into " << num_curr_clusters << " inner clusters " << endl;
				for (Size j=0; j<num_curr_clusters; ++j)
				{
					// convert the indices back to original pose indices
					std::set<Index> temp_indices = inner_pc.getCluster(j);
					std::set<Index> converted_indices;

					for (std::set<Index>::iterator ind_it = temp_indices.begin(); ind_it != temp_indices.end(); ++ind_it)
					{
						converted_indices.insert(current_cluster_vector[*ind_it]);
					}

					// add to the temp set 
					temp_clusters.push_back(converted_indices);
					temp_cluster_scores.push_back(inner_pc.getClusterScore(j));
				}
//cout << "++++++++++++++++++++++++++++++***" << endl;
//				inner_pc.printClusterScores();
//cout << " cluster " << i << "( " << getClusterSize(i) << " ) was split into " << num_curr_clusters 
//      << " clusters." << endl;
//cout << "***++++++++++++++++++++++++++++++***" << endl;
			}
		}

		// switch the clusters
		clusters_       = temp_clusters;
		cluster_scores_ = temp_cluster_scores;
		cluster_representatives_.clear();

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


	void PoseClustering::printClusterScores(std::ostream& out)
	{
		Index rmsd_type   = options.getInteger(Option::RMSD_TYPE);
#ifdef POSECLUSTERING_DEBUG 
		Index cluster_alg = options.getInteger(Option::CLUSTER_METHOD);
#endif
		/*
		cout << endl <<  "For method " << options.getInteger(Option::CLUSTER_METHOD)
		     << " and RMSDtype " << rmsd_type
				 << " and cutoff " << options.getInteger(Option::DISTANCE_THRESHOLD)
				 << " we get: " << endl;
				 */

		for (Position i=0; i<clusters_.size(); ++i)
		{
			out << "=======================================" << endl;
			out << "=   Cluster " << i << " (cluster score: " << getClusterScore(i) << ")" << endl;

#ifdef POSECLUSTERING_DEBUG 
			if (cluster_alg == NEAREST_NEIGHBOR_CHAIN_WARD)
			{
				// find the cluster node with this id
				BGL_FORALL_VERTICES(current_node, cluster_tree_, ClusterTree)
				{
					if (cluster_tree_[current_node].current_cluster_id == i)
					{
						out << "Merged at Ward distance: " << cluster_tree_[current_node].merged_at << std::endl;
					}
				}
			}
#endif

			std::set<Index>& current_cluster = clusters_[i];

			out << "     ";
			for (std::set<Index>::iterator it_k=current_cluster.begin(); it_k!=current_cluster.end(); ++it_k)
			{
				out << *it_k << "   ";
			}
			out << endl;

			for (std::set<Index>::iterator it_j=current_cluster.begin(); it_j!=current_cluster.end(); ++it_j)
			{
				if (rmsd_type == SNAPSHOT_RMSD)
				{
					poses_[*it_j].snap->applySnapShot(system_i_);
				}

				out << " " <<  *it_j << " | ";

				for (std::set<Index>::iterator it_k=current_cluster.begin(); it_k!=current_cluster.end(); ++it_k)
				{
					if (rmsd_type == SNAPSHOT_RMSD)
					{
						poses_[*it_k].snap->applySnapShot(system_j_);
					}

					out << getRMSD_(*it_j, *it_k, rmsd_type) << " ";
				}

				out << endl;
			}

			out << "=======================================" << endl << endl;
		} //next cluster
	}

	void PoseClustering::exportWardClusterTreeToGraphViz(std::ostream& out)
	{
		boost::write_graphviz(out, cluster_tree_, ClusterTreeWriter_(&cluster_tree_));
	}

	void PoseClustering::serializeWardClusterTree(std::ostream& out, bool binary)
	{
		if (binary)
		{
			boost::archive::binary_oarchive oa(out);
			oa << cluster_tree_;
		}
		else
		{
			boost::archive::text_oarchive oa(out);
			oa << cluster_tree_;
		}
	}

	void PoseClustering::deserializeWardClusterTree(std::istream& in, bool binary)
	{
		if (binary)
		{
			boost::archive::binary_iarchive ia(in);
			ia >> cluster_tree_;
		}
		else
		{
			boost::archive::text_iarchive ia(in);
			ia >> cluster_tree_;
		}

		// unfortunately, old versions of boost don't serialize / deserialize graph_bundle properties... *sigh*
		// so, be nice to them...
#if BOOST_VERSION < 105100
		// iterate over all nodes in the graph and find the one which has no in_edges
		// this would be much simpler if boost would store in_degrees for directed graphs... we don't want a bidirectional graph, though
		HashMap<ClusterTreeNode, Size> in_degrees;
		BGL_FORALL_VERTICES(current_vertex, cluster_tree_, ClusterTree)
		{
			boost::graph_traits<ClusterTree>::out_edge_iterator e, e_end;

			for (boost::tie(e, e_end) = boost::out_edges(current_vertex, cluster_tree_); e != e_end; ++e)
			{
				ClusterTreeNode target = boost::target(*e, cluster_tree_);
				if (in_degrees.find(target) == in_degrees.end())
				{
					in_degrees[target] = 1;
				}
				else
				{
					in_degrees[target]++;
				}
			}
		}

		// now, iterate over the tree again to find the one node without a parent
		BGL_FORALL_VERTICES(current_vertex, cluster_tree_, ClusterTree)
		{
			if (in_degrees[current_vertex] == 0)
			{
				cluster_tree_[boost::graph_bundle] = current_vertex;
				break;
			}
		}
#endif

	}


#ifdef BALL_HAS_TBB
	PoseClustering::ComputeNearestClusterTask_::ComputeNearestClusterTask_(PoseClustering* parent,
	                                           const std::vector<ClusterTreeNode>& active_clusters, 
					                                   Position current_cluster, Index rmsd_type)
		: parent_(parent),
		  active_clusters_(active_clusters),
			current_cluster_(current_cluster),
			rmsd_type_(rmsd_type),
			my_min_value_(std::numeric_limits<float>::max())
	{
	}

	PoseClustering::ComputeNearestClusterTask_::ComputeNearestClusterTask_(PoseClustering::ComputeNearestClusterTask_& cnct, tbb::split)
	  : parent_(cnct.parent_),
		  active_clusters_(cnct.active_clusters_),
		  current_cluster_(cnct.current_cluster_),
			rmsd_type_(cnct.rmsd_type_),
			my_min_value_(std::numeric_limits<float>::max())
	{
	}

	void PoseClustering::ComputeNearestClusterTask_::join(ComputeNearestClusterTask_ const& cnct)
	{
		if (my_min_value_ > cnct.my_min_value_)
		{
			my_min_value_ = cnct.my_min_value_;
			my_min_index_ = cnct.my_min_index_;
		}
	}

	void PoseClustering::ComputeNearestClusterTask_::operator() (const tbb::blocked_range<size_t>& r)
	{
		// we use a local variable to squeeze out a little more efficiency, if possible
		float    min_value = my_min_value_;
		Position min_index = my_min_index_;

		size_t end = r.end();
		for (size_t i = r.begin(); i != end; ++i)
		{
			// check all pairs between all_clusters[*clust_it] and all_clusters[current_cluster]
			if (i != current_cluster_)
			{
				float rmsd = parent_->computeWardDistance_(active_clusters_[current_cluster_], active_clusters_[i], rmsd_type_);

				// for stability, make sure that "really" smaller cases are always taken...
				if (BALL_REAL_LESS(rmsd, min_value, 1e-5))
				{
					min_index = i;
					min_value = rmsd;
				}
			}
		}

		my_min_index_ = min_index;
		my_min_value_ = min_value;
	}
#endif

	template <class Archive>
	void PoseClustering::ClusterProperties::serialize(Archive& ar, const unsigned int /*version*/)
	{
		ar & poses;
		ar & size;
		//		TODO: handle serialization of eigen matrix
//		ar & center;
		ar & merged_at;
	}

	void PoseClustering::ClusterTreeWriter_::operator() (std::ostream& out, const ClusterTreeNode& v) const
	{
		out << "[label=\"";
		if ((*cluster_tree_)[v].poses.size() > 0)
			out << *((*cluster_tree_)[v].poses.begin());
		else
		{
			out << (*cluster_tree_)[v].merged_at;
#ifdef POSECLUSTERING_DEBUG
		  out	<< "\t" << (*cluster_tree_)[v].current_cluster_id;
#endif
		}
		out << "\"]";
	}

	const System& PoseClustering::getSystem() const
	{
		return base_system_;
	}


	System& PoseClustering::getSystem()
	{
		return base_system_;
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


	float PoseClustering::getClusterScore(Index i) const
	{
		if (i >= (Index)cluster_scores_.size())
				throw(Exception::OutOfRange(__FILE__, __LINE__));

		return cluster_scores_[i];
	}


	float PoseClustering::getScore(System sys_a, System sys_b, Options temp_options) const
	{
		// basically reimplement getRMSD_(Index i, j, rmsd_type)
		// but more general
		float rmsd = 0.;
		Index rmsd_type            = temp_options.getInteger(Option::RMSD_TYPE);
		Index rmsd_level_of_detail = temp_options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		// compute an atombijection
		// Note: we cannot use precomputeAtomBijection as we might have 
		// different systems sys_a and sys_b given
		AtomBijection temp_atom_bijection;
		StructureMapper mapper(sys_a, sys_b);

		switch (rmsd_level_of_detail)
		{
			case PoseClustering::C_ALPHA:
				temp_atom_bijection.assignCAlphaAtoms(sys_a, sys_b);
				break;
			case PoseClustering::BACKBONE:
				temp_atom_bijection.assignBackboneAtoms(sys_a, sys_b);
				break;
			case PoseClustering::ALL_ATOMS:
				mapper.calculateDefaultBijection();
				temp_atom_bijection = mapper.getBijection();
				break;
			case PoseClustering::PROPERTY_BASED_ATOM_BIJECTION:
				temp_atom_bijection.assignAtomsByProperty(sys_a, sys_b);
				break;
			case PoseClustering::HEAVY_ATOMS:
			default:
				Log.info() << "Option RMSDLevelOfDetaill::HEAVY_ATOMS not yet implemented" << endl;
		}

		// center of masses
		GeometricCenterProcessor center;
		sys_a.apply(center);
		Vector3 com_a = center.getCenter();

		sys_b.apply(center);
		Vector3 com_b = center.getCenter();

		if (rmsd_type == PoseClustering::RIGID_RMSD)
		{
			// we will also need the covariance matrix
			Eigen::Matrix3f temp_covariance_matrix = computeCovarianceMatrix(sys_a, rmsd_level_of_detail);

			RMSDMinimizer::Result transform = RMSDMinimizer::computeTransformation(temp_atom_bijection);
			Matrix4x4& bm = transform.first;

			// convert the contained matrix to Eigen translation vector & rotation matrix
			RigidTransformation rd_a;

			// unfortunately, we cannot directly use the translation vector contained in the matrix:
			// the way the transformation is set up, we will first move the second protein in the origin,
			// then rotate it, and finally move it to the center of mass of the first protein. for our
			// RIGID_RMSD-computation, however, we need the difference of the centers of mass, without
			// the influence of the rotation

			rd_a.rotation << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;

			Eigen::Vector3f eigen_com_a;
			eigen_com_a << com_a.x, com_a.y, com_a.z;

			Eigen::Vector3f eigen_com_b;
			eigen_com_b << com_b.x, com_b.y, com_b.z;

			Eigen::Vector3f t_ab = eigen_com_a - eigen_com_b;
			Eigen::Matrix3f M_ab = rd_a.rotation - Eigen::Matrix3f::Identity();

			rmsd = getRigidRMSD(t_ab, M_ab, temp_covariance_matrix);
		}
		else if (rmsd_type == PoseClustering::SNAPSHOT_RMSD)
		{
			rmsd = mapper.calculateRMSD(temp_atom_bijection);
		}
		else if (rmsd_type == PoseClustering::CENTER_OF_MASS_DISTANCE)
		{
			// just query the center distance
			rmsd = com_a.getDistance(com_b);
		}

		return rmsd;
	}


	float PoseClustering::computeCompleteLinkageRMSD(Index i, Options temp_options, bool initialize)
	{
		if (i >= (Index)cluster_scores_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		// we have to compute the maximal RMSD between all pairs in the cluster i

		float rmsd = 0.;
		Index rmsd_type            = temp_options.getInteger(Option::RMSD_TYPE);

		// do we have to recompute the atombijection?
		if (initialize)
		{
			// prepare whatever has to be prepared...
			if ( (rmsd_type == SNAPSHOT_RMSD) || (rmsd_type == CENTER_OF_MASS_DISTANCE) )
			{
				// obviously, we need snapshots
				if (poses_[0].snap == 0)
				{
					// but we don't have them... so let's produce some...
					convertTransformations2Snaphots();
				}
			}
			else if (rmsd_type == RIGID_RMSD)
			{
				// obviously, we need transforms
				if (poses_[0].trafo == 0)
				{
					// but we don't have them... so let's produce some...
					convertSnaphots2Transformations();
				}
			}

			// precompute the atom bijection, that handles the 
			// option RMSD_LEVEL_OF_DETAIL
			precomputeAtomBijection_();
		}

		// now do the pairwise checking
		for (set<Index>::iterator conf_it_i = clusters_[i].begin();
				 conf_it_i != clusters_[i].end(); conf_it_i++)
		{
			poses_[*conf_it_i].snap->applySnapShot(system_i_);

			for (set<Index>::iterator conf_it_j = conf_it_i; conf_it_j != clusters_[i].end(); conf_it_j++)
			{
				if (conf_it_i != conf_it_j)
				{
					if (rmsd_type == SNAPSHOT_RMSD)
					{
						poses_[*conf_it_j].snap->applySnapShot(system_j_);
					}

					float temp_rmsd = getRMSD_(*conf_it_i, *conf_it_j, rmsd_type);
					rmsd = std::max(rmsd, temp_rmsd);
				}
			}
		}

		return rmsd;
	}


	boost::shared_ptr<System> PoseClustering::getPose(Index i) const
	{
		if (i >= (Index)poses_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		boost::shared_ptr<System> new_system(new System(getSystem()));

		if (has_rigid_transformations_)
		{
			GeometricCenterProcessor center;
			new_system->apply(center);
			Vector3 toOrigin = center.getCenter().negate();

			TranslationProcessor translation;
			translation.setTranslation(toOrigin);
			new_system->apply(translation);

			// apply transformation and rotation	
			RigidTransformation const& rigid_trafo = *(poses_[i].trafo);
			Eigen::Matrix3f     const& rd_rotation    = rigid_trafo.rotation;
			Eigen::Vector3f     const& rd_translation = rigid_trafo.translation;

			TransformationProcessor transformation;
			transformation.setTransformation(Matrix4x4(rd_rotation(0,0), rd_rotation(0,1), rd_rotation(0,2), rd_translation(0),
			                                           rd_rotation(1,0), rd_rotation(1,1), rd_rotation(1,2), rd_translation(1),
			                                           rd_rotation(2,0), rd_rotation(2,1), rd_rotation(2,2), rd_translation(2),
			                                           0, 0, 0, 1));
			new_system->apply(transformation);

			// move back to its original center
			translation.setTranslation(toOrigin.negate());
			new_system->apply(translation);
		}
		else
		{
			poses_[i].snap->applySnapShot(*(new_system.get()));
		}
		return new_system;
	}


	Index PoseClustering::findClusterRepresentative(Index i)
	{
		if (i >= (Index)clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		Index rmsd_type = options.getInteger(Option::RMSD_TYPE);

		// as cluster representative we simply compute the pose that 
		// has the smallest deviation from all other cluster members

		// take the first
		//Index conf_set_idx = *(clusters_[i].begin());

		//std::set<Index> current_cluster = clusters_[i];

		Index current_best = std::numeric_limits<Index>::max();
		float min_distance = std::numeric_limits<float>::max();

		for (set<Index>::iterator current_pose = clusters_[i].begin(); current_pose != clusters_[i].end(); current_pose++)
		{
			float sum_dist = 0;
			for (set<Index>::iterator to_test_pose = clusters_[i].begin(); to_test_pose != clusters_[i].end(); to_test_pose++)
			{
				if (*current_pose != *to_test_pose)
				{
					sum_dist += getRMSD_(*current_pose, *to_test_pose, rmsd_type);
				}
			}
			if (min_distance > sum_dist)
			{
				min_distance  = sum_dist;
				current_best = *current_pose;
			}
		}
		return current_best;
	}


	boost::shared_ptr<System> PoseClustering::getClusterRepresentative(Index i)
	{
		if (i >= (Index)clusters_.size())
			throw(Exception::OutOfRange(__FILE__, __LINE__));

		return getPose(findClusterRepresentative(i));
	}


	void PoseClustering::setDefaultOptions()
	{
		options.setDefault(PoseClustering::Option::CLUSTER_METHOD,
		                   PoseClustering::Default::CLUSTER_METHOD);

		options.setDefaultReal(PoseClustering::Option::DISTANCE_THRESHOLD,
		                       PoseClustering::Default::DISTANCE_THRESHOLD);

		options.setDefault(PoseClustering::Option::RMSD_LEVEL_OF_DETAIL,
		                   PoseClustering::Default::RMSD_LEVEL_OF_DETAIL);

		options.setDefault(PoseClustering::Option::RMSD_TYPE,
		                   PoseClustering::Default::RMSD_TYPE);

		options.setDefault(PoseClustering::Option::RUN_PARALLEL,
				               PoseClustering::Default::RUN_PARALLEL);

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
			convertTransformations2Snaphots();
		}
		// create a new ConformationSet
		boost::shared_ptr<ConformationSet> new_set(new ConformationSet());

		new_set->setup(base_system_);

		new_set->setParent(current_set_);

		for (set<Index>::iterator conf_it = clusters_[i].begin();
				 conf_it != clusters_[i].end(); conf_it++)
		{
			System conf(base_system_);

			poses_[*conf_it].snap->applySnapShot(conf);

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
			convertTransformations2Snaphots();
		}

		cluster_representatives_.clear();
		cluster_representatives_.resize(clusters_.size());

		// create the set to be returned
		boost::shared_ptr<ConformationSet> new_set(new ConformationSet());
		new_set->setup(getSystem());
		new_set->setParent(current_set_);

		// iterate over all clusters
		for (Size i=0; i<clusters_.size(); i++)
		{
			System conf(new_set->getSystem());

			// get the cluster Representative
			Index current_representative = findClusterRepresentative(i);
			poses_[current_representative].snap->applySnapShot(conf);
			cluster_representatives_[i] = current_representative;
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
					Log.info() << "getRMSD() for current option setting not yet implemented! "
					           << __FILE__ <<  " " << __LINE__ << endl;
				}
//				std::cout << "duddeldumm: " << *conf_it_i << " " << *conf_it_j << " " << temp_rmsd << std::endl;
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

	float PoseClustering::getSquaredRigidRMSD(Eigen::Vector3f const& t_ab, Eigen::Matrix3f const& M_ab, Eigen::Matrix3f const& covariance_matrix)
	{
		// based on the given transformation we can simply compute the rmsd
		// 
		//              2 |-----------------------------------------------------------
		//              - |              2    1    (   t           /        t \   )
		//                | || t_(a,b) ||  +  - tr ( M   M  * sum (x_i x_i     )  )
		// rmsd(a,b) =    |                   n    (           i   \          /   )
		//                |

		return t_ab.squaredNorm() + ((M_ab.transpose() * M_ab) * covariance_matrix.selfadjointView<Eigen::Upper>()).trace();
	}


	// compute the RMSD between two "poses", 
	//    i.e. snapshots, transformations, or center of masses
	float PoseClustering::getRMSD_(Index i, Index j, Index rmsd_type)
	{
		float rmsd = 0.;

		if (rmsd_type == PoseClustering::RIGID_RMSD)
		{
			RigidTransformation const& trafo_i = *(poses_[i].trafo);
			RigidTransformation const& trafo_j = *(poses_[j].trafo);

			Eigen::Vector3f t_ab = (trafo_j.translation - trafo_i.translation);
			Eigen::Matrix3f M_ab = (trafo_j.rotation    - trafo_i.rotation);

			rmsd = getRigidRMSD(t_ab, M_ab, covariance_matrix_);

//			std::cout << t_ab << " " << M_ab << " " << rmsd << std::endl << covariance_matrix_ << std::endl;

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

//		std::cout << "rmsd between " << i << " " << j << ": " << rmsd << std::endl;
		return rmsd;
	}

	void PoseClustering::storeSnapShotReferences_()
	{
		// make sure that we have a sensible default state
		poses_.clear();

		for (Position i=0; i<current_set_->size(); ++i)
		{
			poses_.push_back(&((*current_set_)[i]));
		}
	}


	void PoseClustering::applyTransformation2System(Index i, System& target_system)
	{
		// Please note, for efficiency reasons this code is copied 
		// from function convertTransformations2Snaphots() below

		if (i < (Index) poses_.size())
		{
			// compute the translation vector to the origin
			GeometricCenterProcessor center;
			target_system.apply(center);
			Vector3 toOrigin = center.getCenter().negate();

			// move to origin
			TranslationProcessor translation;
			translation.setTranslation(toOrigin);
			target_system.apply(translation);

			TransformationProcessor transformation;

			RigidTransformation const& rigid_trafo = *(poses_[i].trafo);

			Eigen::Vector3f const&     rd_translation = rigid_trafo.translation;
			Eigen::Matrix3f const&     rd_rotation    = rigid_trafo.rotation;

			// apply transformation and rotation
			// NOTE: transformations given as input are always relative to the original pose
			//       thus substraction of toOrigin is correct and more efficient than
			//       the intuitive action sequence 
			//             <toOrigin, pose rotate, negation of toOrigin, pose translation> !

			transformation.setTransformation(Matrix4x4(rd_rotation(0,0), rd_rotation(0,1), rd_rotation(0,2), rd_translation(0) - toOrigin.x,
						rd_rotation(1,0), rd_rotation(1,1), rd_rotation(1,2), rd_translation(1) - toOrigin.y,
						rd_rotation(2,0), rd_rotation(2,1), rd_rotation(2,2), rd_translation(2) - toOrigin.z,
						0, 0, 0, 1));

			target_system.apply(transformation);
		}
		else
		{
			throw(Exception::OutOfRange(__FILE__, __LINE__));
		}
	}


	void PoseClustering::convertTransformations2Snaphots()
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
		for (Size i=0; i<poses_.size(); i++)
		{
			RigidTransformation const& rigid_trafo = *(poses_[i].trafo);

			Eigen::Vector3f const&     rd_translation = rigid_trafo.translation;
			Eigen::Matrix3f const&     rd_rotation    = rigid_trafo.rotation;

			// create a copy
			System new_system(new_template_system);

			// apply transformation and rotation
			// NOTE: transformations given as input are always relative to the original pose
			//       thus substraction of toOrigin is correct!
			transformation.setTransformation(Matrix4x4(rd_rotation(0,0), rd_rotation(0,1), rd_rotation(0,2), rd_translation(0) - toOrigin.x,
			                                           rd_rotation(1,0), rd_rotation(1,1), rd_rotation(1,2), rd_translation(1) - toOrigin.y,
			                                           rd_rotation(2,0), rd_rotation(2,1), rd_rotation(2,2), rd_translation(2) - toOrigin.z,
			                                           0, 0, 0, 1));

			new_system.apply(transformation);

			// add the conformation
			current_set_->add(0, new_system);
		}

		for (Position i=0; i<current_set_->size(); ++i)
		{
			poses_[i].snap = &((*current_set_)[i]);
		}
	}


	void PoseClustering::convertSnaphots2Transformations()
	{
		// just to be sure...
		transformations_.clear();

		// we will also need the covariance matrix
		covariance_matrix_ = computeCovarianceMatrix(base_system_, options.getInteger(Option::RMSD_LEVEL_OF_DETAIL));

		Size num_poses = getNumberOfPoses();

		system_i_ = base_system_;
		system_j_ = base_system_;
		poses_[0].snap->applySnapShot(system_i_);

		// TODO: return and parameters?
		precomputeAtomBijection_();
		computeCenterOfMasses_();

		// we iterate over all the snapshots
		for (Position current_snap = 0; current_snap < num_poses; ++current_snap)
		{
			// apply the snapshot (we compute all transformations relative to system_i_)
			poses_[current_snap].snap->applySnapShot(system_j_);

			// compute the best mapping of the base system to the transformed one
			RMSDMinimizer::Result transform = RMSDMinimizer::computeTransformation(atom_bijection_);
			Matrix4x4& bm = transform.first;

			// convert the contained matrix to Eigen translation vector & rotation matrix
			RigidTransformation rd;

			// unfortunately, we cannot directly use the translation vector contained in the matrix:
			// the way the transformation is set up, we will first move the second protein in the origin,
			// then rotate it, and finally move it to the center of mass of the first protein. for our
			// RIGID_RMSD-computation, however, we need the difference of the centers of mass, without
			// the influence of the rotation

			rd.rotation << bm.m11, bm.m12, bm.m13, bm.m21, bm.m22, bm.m23, bm.m31, bm.m32, bm.m33;

			Vector3 current_com = com_[current_snap] - com_[0];
			rd.translation << current_com.x, current_com.y, current_com.z;

			transformations_.push_back(rd);
		}

		for (Position i=0; i<poses_.size(); ++i)
		{
			poses_[i].trafo = &(transformations_[i]);
		}

		has_rigid_transformations_ = true;
	}


	void PoseClustering::printCluster_(Index i, std::ostream& out) const
	{
		out << "++++ cluster " << i << " score " << getClusterScore(i) << " members "  << clusters_[i].size();

		if (cluster_representatives_.size() <= i)
				out  << " rep " << cluster_representatives_[i];
	  out << " ++++" << endl;

		std::copy(clusters_[i].begin(), clusters_[i].end(), std::ostream_iterator<Index>(out, " "));
		out << endl;
		out << "+++++++++++++++++++" << endl;
	}


	void PoseClustering::printClusters(std::ostream& out) const
	{
		out << "+\n+\n+    FINAL CLUSTERS     \n+\n+" << endl;
		for (Size i = 0; i < clusters_.size(); i++)
		{
			printCluster_(i, out);
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
		poses_.clear();
		transformations_.clear();
		covariance_matrix_.setZero();
		base_system_.clear();
		base_conformation_.clear();
		clusters_.clear();
		cluster_representatives_.clear();
		cluster_scores_.clear();
		//rmsd_level_of_detail_;
		lambda_.clear();
		pi_.clear();
		mu_.clear();
		com_.clear();
		//atom_bijection_;
		//system_i_;
		//system_j_;
		number_of_selected_atoms_ = 0;
	}
}
