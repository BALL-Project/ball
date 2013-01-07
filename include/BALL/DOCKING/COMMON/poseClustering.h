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

			We offer two algorithms via the option CLUSTER_METHOD:
			 - SLINK_SIBSON as described in 
			            R. Sibson: SLINK: an optimally efficient algorithm for the single-link cluster method. 
                  The Computer Journal. 16, Nr. 1, British Computer Society, 1973, S. 30-34
			 - CLINK_DEFAYS as described in 
									D. Defays: An efficient algorithm for a complete link method. 
                  The Computer Journal. 20, Nr. 4, British Computer Society, 1977, S. 364-366. 
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
				/** the clustering method
				*/
				static const String CLUSTER_METHOD;

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
				static const Index CLUSTER_METHOD;
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

			enum BALL_EXPORT ClusterMethod
			{
				TRIVIAL_COMPLETE_LINKAGE,
				SLINK_SIBSON,
				CLINK_DEFAYS
			};

			BALL_CREATE(PoseClustering);

			/**	Constructors and Destructor */
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
				current_set_ = new_set;
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

			void printClusterRMSDs();

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

			// trivial complete linkage implementation
			// with O(n^2) space request
			bool trivialCompute_();

			// space efficient (SLINK or CLINK) clustering
			bool linearSpaceCompute_();

			//	implementation of a single linkage clustering as described in 
			//       R. Sibson: SLINK: an optimally efficient algorithm for the single-link cluster method. 
			//       The Computer Journal. 16, Nr. 1, British Computer Society, 1973, S. 30-34
			void slinkInner_(int current_level);

			// implementation of a complete linkage clustering as described in 
			// 					D. Defays: An efficient algorithm for a complete link method. 
      //          The Computer Journal. 20, Nr. 4, British Computer Society, 1977, S. 364-366. 
			void clinkInner_(int current_level);

			// distance between cluster i and cluster j
			float getRMSD_(Index i, Index j);

			//
			float getRMSD_();

			//
			void printCluster_(Index i);

			//
			void printClusters_();

			///
			Eigen::MatrixXd                 pairwise_scores_;

			/// the ConformationSet we wish to cluster
			ConformationSet*                current_set_;

			/// the clusters
			std::vector<std::set<Index> >   clusters_;

			/// the RMSD definition used for clustering
			Index                           rmsd_level_of_detail_;

			// ------ data structures for slink and clink 

			// stores the distance at which this indexed element has longer 
			// the largest index of its cluster
			vector<double> lambda;

			// the index of the cluster representative at merge-time 
			// (element with largest index)
			vector<int>    pi;

			vector<double> mu;

			// We cache the atom bijection for faster
			// RMSD computation; this is possible, since the system topology does
			// not change
			AtomBijection   atom_bijection_;

			System system_i_;
			System system_j_;

	}; //class PoseClustering
} //namesspace BALL

#endif // BALL_DOCKING_POSECLUSTERING_H
