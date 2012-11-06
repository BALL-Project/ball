// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_POSECLUSTERING_H
#define BALL_DOCKING_POSECLUSTERING_H

#ifndef BALL_DATATYPE_OPTIONS_H
	#include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
  #include <BALL/DOCKING/COMMON/conformationSet.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
# include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

#ifndef BALL_STRUCTURE_ATOMBIJECTION_H
# include <BALL/STRUCTURE/atomBijection.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
  #include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
  #include <BALL/DATATYPE/string.h>
#endif

#include <Eigen/Core>
#include <boost/shared_ptr.hpp>
#include <set>

namespace BALL
{
	/** Pose Clustering 
	    \ingroup DockingMiscellaneous
	 */

  /** \brief Computation of clusters of docking poses.
	
	    This class computes clusters of docking poses given as a 
			conformation set using a complete linkage algorithm.

			The class assumes
			   - a pairwise protein-protein docking 
				 - all receptor-ligand poses have already been mapped onto each other
				 such that "receptors" are kept fixed
				 - the conformation set only contains the "ligands".
			
			The complete linkage algorithm guarantees a minimal cluster
		 	distance (max RMSD between all pairs of two clusters) of RMSD_THRESHOLD.

	*/
  class BALL_EXPORT PoseClustering
	{
		public:
			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/* * the clustering method
				*/
				//static const String CLUSTER_METHOD;

				/** the threshold for minimal required cluster distance
				*/
				static const String RMSD_THRESHOLD;

				/** the level of detail when computing the RMSD
				*/
				static const String RMSD_LEVEL_OF_DETAIL;
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				//static const String CLUSTER_METHOD;
				static const float RMSD_THRESHOLD;
				static const Index RMSD_LEVEL_OF_DETAIL;
			};

			enum BALL_EXPORT RMSDLevelOfDetail
			{
				C_ALPHA, //=0
				HEAVY_ATOMS,
				BACKBONE,
				ALL_ATOMS,
				PROPERTY_BASED_ATOM_BIJECTION
			};


			BALL_CREATE(PoseClustering);

			/**	Constructors and Descructor */
			//@{

			/// Default constructor. 
			PoseClustering();

			/// Detailed constructor.
			/// (TODO: really pass a pointer here?)
			PoseClustering(ConformationSet* poses, float rmsd);

			///
			virtual ~PoseClustering();
			//@}


			/** @name	operation methods */
			//@{

			/** start method.
			*/
			bool compute();

			//@}

			/**	@name	Access methods
			*/
			//@{

			/// 
			void setConformationSet(ConformationSet* new_set)
			{
				current_set_               = new_set;
			}

			/// 
			const ConformationSet* getConformationSet() const {return  current_set_;}

			/// 
			ConformationSet* getConformationSet() {return  current_set_;}

			///
			const System& getSystem() const;

			///
			System& getSystem();

			///
			Size getNumberOfClusters() const {return clusters_.size();}

			/// return indices of all SnapShots of the original ConformationSet assigned to cluster i
			const std::set<Index>& getCluster(Index i) const;

			///
			std::set<Index>& getCluster(Index i);

			///
			Size getClusterSize(Index i) const;

			/// TODO statt System&  besser nur den Index des Median im Orginal ConformationSet?
			//const System& getClusterRepresentative(Index i) const;

			/// TODO statt System& besser nur den Index des Median im Orginal ConformationSet?
			//System& getClusterRepresentative(Index i);

			/// 
			boost::shared_ptr<System> getClusterRepresentative(Index i) const;

			/// returns cluster i as ConformationSet
			boost::shared_ptr<ConformationSet> getClusterConformationSet(Index i) const;

			/// returns a ConformationSet containing one structure per cluster
			boost::shared_ptr<ConformationSet> getReducedConformationSet() const;

			//@}

			/** @name Public Attributes
			*/
			//@{
			/// options
			Options options;

			/** reset the options to default values
			*/
			void setDefaultOptions();
			//@}

		protected:

			// distance between cluster i and cluster j
			float getRMSD_(Index i, Index j);

			//
			float getRMSD_(System* si, System* sj);

			void printCluster_(Index i);

			void printClusters_();

			///
			Eigen::MatrixXd                 pairwise_scores_;

			/// the ConformationSet we wish to cluster
			ConformationSet*                current_set_;

			/// the clusters
			std::vector<std::set<Index> >   clusters_;

			/// the RMSD definition used for clustering
			Index                           rmsd_level_of_detail_;

	}; //class PoseClustering
} //namesspace BALL

#endif // BALL_DOCKING_POSECLUSTERING_H
