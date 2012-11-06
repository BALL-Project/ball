// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/DOCKING/COMMON/poseClustering.h>

using namespace std;

namespace BALL
{
	//const String PoseClustering::Option::CLUSTER_METHOD     = "cluster_method";

	const String PoseClustering::Option::RMSD_LEVEL_OF_DETAIL = "rmsd_level_of_detail";
	const Index PoseClustering::Default::RMSD_LEVEL_OF_DETAIL = PoseClustering::RMSDLevelOfDetail::C_ALPHA;

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
		// naive implementation of Complete-linkage clustering
		// a faster implementation would be  
		// D. Defays (1977). "An efficient algorithm for a complete link method". 
		// The Computer Journal (British Computer Society) 20 (4): 364–366.

		// 
		rmsd_level_of_detail_ = options.getInteger(Option::RMSD_LEVEL_OF_DETAIL);

		if (!current_set_)
		{
			Log.info() << "No valid ConformationSet given." << endl;
			return false;
		}

		// in the beginning each pose is a cluster	
		pairwise_scores_.resize(current_set_->size(), current_set_->size());

		clusters_.clear();

		// compute all pairwise RMSDs

		// for the next step we need to determine the minimal maximal 
		// distance between two clusters
		float min_max_cluster_dist = std::numeric_limits<float>::max();
		Index min_cluster_a_idx = 0;
		Index min_cluster_b_idx = 0;
		set<Index>* min_cluster_a = NULL;
		set<Index>* min_cluster_b = NULL;

		System sys_i(current_set_->getSystem());
		System sys_j(current_set_->getSystem());
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
			sn_i.applySnapShot(sys_i);
			pairwise_scores_(i,i) = 0;
			for (Size j=i+1; j<num_conf; j++)
			{
				sn_j = (*current_set_)[j];
				sn_j.applySnapShot(sys_j);
				float rmsd = getRMSD_(&sys_i, &sys_j);
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
//cout << " \n\nRMSD cutoff: " << rmsd_cutoff  << " " << min_max_cluster_dist  << endl;

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

		return true;
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

	float PoseClustering::getRMSD_(System* si, System* sj)
	{
		float rmsd = 0.;
		StructureMapper mapper(*si, *sj);

		AtomBijection temp_atom_bijection;
		switch (rmsd_level_of_detail_)
		{
			case PoseClustering::RMSDLevelOfDetail::C_ALPHA:
				temp_atom_bijection.assignCAlphaAtoms(*si, *sj);
				rmsd = mapper.calculateRMSD(temp_atom_bijection);
				break;
			case PoseClustering::RMSDLevelOfDetail::BACKBONE:
				temp_atom_bijection.assignBackboneAtoms(*si, *sj);
				rmsd = mapper.calculateRMSD(temp_atom_bijection);
				break;
			case PoseClustering::RMSDLevelOfDetail::ALL_ATOMS:
				mapper.calculateDefaultBijection();
				//temp_atom_bijection = mapper_.getBijection();
				rmsd = mapper.calculateRMSD();
				break;
			case PoseClustering::RMSDLevelOfDetail::PROPERTY_BASED_ATOM_BIJECTION:
				temp_atom_bijection.assignAtomsByProperty(*si, *sj);
				rmsd = mapper.calculateRMSD(temp_atom_bijection);
				break;

			case PoseClustering::RMSDLevelOfDetail::HEAVY_ATOMS:
			default:
				Log.info() << "Option RMSDLevelOfDetaill::HEAVY_ATOMS not yet implemented" << endl;
		}

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
