// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/poseClustering.h>
#include <BALL/STRUCTURE/structureMapper.h>

using namespace std;

namespace BALL
{
	const String PoseClustering::Option::CLUSTER_METHOD  = "cluster_method";
	const Index  PoseClustering::Default::CLUSTER_METHOD = PoseClustering::ClusterMethod::CLINK_DEFAYS;

	const String PoseClustering::Option::RMSD_LEVEL_OF_DETAIL  = "rmsd_level_of_detail";
	const Index  PoseClustering::Default::RMSD_LEVEL_OF_DETAIL = PoseClustering::C_ALPHA;

	const String PoseClustering::Option::RMSD_THRESHOLD = "pose_clustering_rmsd_threshold";
  const float PoseClustering::Default::RMSD_THRESHOLD = 3;


	PoseClustering::PoseClustering()
	{
		setDefaultOptions();
	}


	PoseClustering::PoseClustering(ConformationSet* poses, float rmsd)
	{
		options[Option::RMSD_THRESHOLD] = rmsd;
		current_set_ = poses;
	}


	PoseClustering::~PoseClustering()
	{
	}


	bool PoseClustering::compute()
	{
		if (!current_set_)
		{
			Log.info() << "No valid ConformationSet given." << endl;
			return false;
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

		if (options.getInteger(Option::CLUSTER_METHOD) ==  ClusterMethod::TRIVIAL_COMPLETE_LINKAGE)
		{
			return trivialCompute_();
		}
		else if (   (options.getInteger(Option::CLUSTER_METHOD) == ClusterMethod::SLINK_SIBSON)
				     || (options.getInteger(Option::CLUSTER_METHOD) == ClusterMethod::CLINK_DEFAYS))
		{
			return linearSpaceCompute_();
		}
		else
		{
			Log.error() << "Unknown parameter for option CLUSTER_METHOD " << options.get(Option::CLUSTER_METHOD) << endl;
			return false;
		}

		return true;
	}

	bool PoseClustering::trivialCompute_()
	{
		// in the beginning each pose is a cluster
		// this causes O(n^2) space!!	
		pairwise_scores_.resize(current_set_->size(), current_set_->size());

		// clean up old clusters
		clusters_.clear();

		// compute all pairwise RMSDs

		// for the next step we need to determine the minimal maximal 
		// distance between two clusters
		float min_max_cluster_dist = std::numeric_limits<float>::max();
		Index min_cluster_a_idx = 0;
		Index min_cluster_b_idx = 0;
		set<Index>* min_cluster_a = NULL;
		set<Index>* min_cluster_b = NULL;

		Size num_conf = current_set_->size();
		SnapShot sn_i;
		SnapShot sn_j;

		for (Size i=0; i<num_conf; i++)
		{
			// add as new cluster
			set<Index> new_set;
			new_set.insert((Index) i);
			clusters_.push_back(new_set);
			// compute the rmsd
			sn_i = (*current_set_)[i];
			sn_i.applySnapShot(system_i_);
			pairwise_scores_(i,i) = 0;
			for (Size j=i+1; j<num_conf; j++)
			{
				sn_j = (*current_set_)[j];
				sn_j.applySnapShot(system_j_);
				float rmsd = getRMSD_();
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
						float rmsd = getRMSD_(*conf_it, j);
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

			for (Size i=0; i<num_conf; i++)
			{
				// skip already seen clusters
				if (clusters_[i].size() > 0)
				{
					for (Size j=i+1; j<num_conf; j++)
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
//printClusters_();
	//	printClusterRMSDs();

		return true;
	}

	bool PoseClustering::linearSpaceCompute_()
	{
		float threshold = options.getReal(Option::RMSD_THRESHOLD);

		// we will need arrays pi, lambda, and mu
		lambda.resize(current_set_->size());
		pi.resize(current_set_->size());
		mu.resize(current_set_->size());

		// and initialize them for the first point
		pi[0] = 0;
		lambda[0] = numeric_limits<double>::max();

		for (Size current_level=1; current_level < current_set_->size(); ++current_level)
		{
			pi[current_level] = current_level;
			lambda[current_level] = numeric_limits<double>::max();

			(*current_set_)[current_level].applySnapShot(system_i_);

			for (Size j=0; j<current_level; ++j)
			{
				(*current_set_)[j].applySnapShot(system_j_);

				// note: we don't need the rmsd matrix here - that's the whole point: the algorithm
				//       only requires O(n) data, not (O(n^2))
				mu[j] = getRMSD_();
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

		// we need a helper for the clusters
		std::vector<std::set<Index> > cluster_helper;

		// first, each point is its own cluster
		for (Position i=0; i<current_set_->size(); ++i)
		{
			cluster_helper.push_back(std::set<Index>());
			cluster_helper.back().insert(i);
		}

		// now, join clusters if needed
		for (Position current_cluster=0; current_cluster < cluster_helper.size(); ++current_cluster)
		{
			if (BALL_REAL_LESS_OR_EQUAL(lambda[current_cluster], threshold, 1e-5))
			{
				// merge this cluster with its destination
				cluster_helper[pi[current_cluster]].insert(cluster_helper[current_cluster].begin(), cluster_helper[current_cluster].end());

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
			if (BALL_REAL_GREATER_OR_EQUAL(lambda[i], mu[i], 1e-5))
			{
				mu[pi[i]] = min(mu[pi[i]], lambda[i]);
				lambda[i] = mu[i];
				pi[i] = current_level;
			}
			else
			{
				mu[pi[i]] = min(mu[pi[i]], mu[i]);
			}
		}

		for (int i=0; i<current_level; ++i)
		{
			if (BALL_REAL_GREATER_OR_EQUAL(lambda[i], lambda[pi[i]], 1e-5))
				pi[i] = current_level;
		}

	}


	void PoseClustering::clinkInner_(int current_level)
	{
		for (int i=0; i<current_level; ++i)
		{
			if (BALL_REAL_LESS(lambda[i], mu[i], 1e-5))
			{
				mu[pi[i]] = max(mu[pi[i]], mu[i]);
				mu[i] = numeric_limits<double>::max();
			}
		}

		int a = current_level-1;
		for (int i=0; i<current_level; ++i)
		{
			int j = (current_level-1)-i;

			if (BALL_REAL_GREATER_OR_EQUAL(lambda[j], mu[pi[j]], 1e-5))
			{
				if (BALL_REAL_LESS(mu[j], mu[a], 1e-5))
				{
					a = j;
				}
			}
			else
			{
				mu[j] = numeric_limits<double>::max();
			}
		}

		int    b = pi[a];
		double c = lambda[a];

		pi[a] = current_level;
		lambda[a] = mu[a];

		int d;
		double e;

		while ((a < current_level-1) && (b < current_level-1))
		{
			d = pi[b];
			e = lambda[b];
			pi[b] = current_level;
			lambda[b] = c;
			b = d;
			c = e;
		}

		if ((a < current_level - 1) && (b == current_level-1))
		{
			pi[b] = current_level;
			lambda[b] = c;
		}

		for (int i=0; i<current_level; ++i)
		{
			if (pi[pi[i]] == current_level)
			{
				if (BALL_REAL_GREATER_OR_EQUAL(lambda[i], lambda[pi[i]], 1e-5))
					pi[i] = current_level;
			}
		}
	}


	void PoseClustering::printClusterRMSDs()
	{
		for (Position i=0; i<clusters_.size(); ++i)
		{
			std::cout << "Cluster " << i << " for method " << options.getInteger(PoseClustering::Option::CLUSTER_METHOD) << std::endl;

			std::set<Index>& current_cluster = clusters_[i];

			for (std::set<Index>::iterator it_j=current_cluster.begin(); it_j!=current_cluster.end(); ++it_j)
			{
				(*current_set_)[*it_j].applySnapShot(system_i_);

				for (std::set<Index>::iterator it_k=current_cluster.begin(); it_k!=current_cluster.end(); ++it_k)
				{
					(*current_set_)[*it_k].applySnapShot(system_j_);

					std::cout << getRMSD_() << " ";
				}

				std::cout << std::endl;
			}

			std::cout << "=======================================" << std::endl << std::endl;
		}
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


	//TODO shouldnt this method return double?
	// distance between cluster i and cluster j
	float PoseClustering::getRMSD_(Index i, Index j)
	{
		// we have to compute the maximal distance
		float rmsd =  std::numeric_limits<float>::max();
		for (set<Index>::iterator conf_it_i = clusters_[i].begin();
					conf_it_i !=  clusters_[i].end();
					conf_it_i ++)
		{
			for (set<Index>::iterator conf_it_j = clusters_[j].begin();
					conf_it_j !=  clusters_[j].end();
					conf_it_j ++)
			{
				if (pairwise_scores_(*conf_it_i, *conf_it_j) < rmsd)
					rmsd = pairwise_scores_(*conf_it_i, *conf_it_j);
			}
		}

		return rmsd;
	}

	float PoseClustering::getRMSD_()
	{
		float rmsd = 0.;

		StructureMapper mapper(system_i_, system_j_);
		rmsd = mapper.calculateRMSD(atom_bijection_);

		return rmsd;
	}

	void PoseClustering::printCluster_(Index i)
	{
		cout << "++++ cluster " << i << " ++++" << endl;
		std::copy(clusters_[i].begin(), clusters_[i].end(), std::ostream_iterator<Index>(std::cout, " "));
		cout << endl;
		cout << "+++++++++++++++++++" << endl;
	}

	void PoseClustering::printClusters_()
	{
		cout << "\n\n    FINAL CLUSTERS     \n\n" << endl;
		for (Size i = 0; i < clusters_.size(); i++)
		{
			printCluster_(i);
		}
	}
}
