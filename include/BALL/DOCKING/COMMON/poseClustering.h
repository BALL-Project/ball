// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_POSECLUSTERING_H
#define BALL_DOCKING_POSECLUSTERING_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
# include <BALL/DOCKING/COMMON/conformationSet.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
# include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

#ifndef BALL_STRUCTURE_ATOMBIJECTION_H
# include <BALL/STRUCTURE/atomBijection.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#       include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
# include <BALL/MATHS/matrix44.h>
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
   // TODO add TIRVIAL_COMPLETE_LINKAGE

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

				/** the computation type of the cluster distance meassure
				*/
				static const String RMSD_TYPE;

				/** flag indicating the use of a geometric center based pre-clink run
				 */
				static const String USE_CENTER_OF_MASS_PRECLINK;
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const Index CLUSTER_METHOD;
				static const float RMSD_THRESHOLD;
				static const Index RMSD_LEVEL_OF_DETAIL;
				static const Index RMSD_TYPE;
				static const bool USE_CENTER_OF_MASS_PRECLINK; //TODO setDefaults!! //rename 
				static const String INDEX_PROPERTY_NAME;
			};

			enum BALL_EXPORT RMSDType
			{
				SNAPSHOT_RMSD,
				RIGID_RMSD,
				CENTER_OF_MASS_DISTANCE
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

			/// PoseClustering for a given set of rigid transformations of a base structure
			PoseClustering(System const& base_system, String transformation_file_name);

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
			void setBaseSystemAndTransformations(System const& base_system, String transformation_file_name);

			/// reads the poses given as transformations from a file
			bool readTransformationsFromFile(String filename);

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

			/** Compute the root mean square deviation due to a rigid transformation of a point cloud (here, atoms)
			 *  @param t_ab difference vector between the transformations to be compared
			 *  @param M_ab difference of the rotation matrices between the transformations to be compared
			 *  @param covariance_matrix the covariance matrix of the atom positions
			 */
			static float getRigidRMSD(Eigen::Vector3f const& t_ab, Eigen::Matrix3f const& M_ab, Eigen::Matrix3f const& covariance_matrix);

			/** Compute the covariance matrix for the given system
			 */
			static Eigen::Matrix3f computeCovarianceMatrix(System const& system, Index rmsd_level_of_detail = C_ALPHA);

			void printClusters();


		protected:

			// trivial complete linkage implementation
			// with O(n^2) space request
			bool trivialCompute_();

			// space efficient (SLINK or CLINK) clustering
			bool linearSpaceCompute_(Index rmsd_type);

			//	implementation of a single linkage clustering as described in 
			//       R. Sibson: SLINK: an optimally efficient algorithm for the single-link cluster method. 
			//       The Computer Journal. 16, Nr. 1, British Computer Society, 1973, S. 30-34
			void slinkInner_(int current_level);

			// implementation of a complete linkage clustering as described in 
			// 					D. Defays: An efficient algorithm for a complete link method. 
      //          The Computer Journal. 20, Nr. 4, British Computer Society, 1977, S. 364-366. 
			void clinkInner_(int current_level);

			// compute the center of masses
			void computeCenterOfMasses_();

			// run a pre clink on the centers of gravity 
			bool centerOfGravityPreCluster_();

			// distance between cluster i and cluster j
			float getClusterRMSD_(Index i, Index j, Index rmsd_type);

			//
			float getRMSD_(Index i, Index j, Index rmsd_type);

			//
			void printCluster_(Index i);

			void printVariables_(int a, int b, double c, int d, double e, int current_level);

			//
			void clear_();

			// only used by trivial clustering
			Eigen::MatrixXd                 pairwise_scores_;

			/// the ConformationSet we wish to cluster
			ConformationSet*                current_set_;

			// ----- data structures for transformation input (instead of snapshots)
			std::vector<Eigen::Vector3f>    translations_;
			std::vector<Eigen::Matrix3f>    rotations_;

			Eigen::Matrix3f                 covariance_matrix_;

			System                          base_system_;

			/// the clusters
			std::vector<std::set<Index> >   clusters_;

			/// the RMSD definition used for clustering
			Index                           rmsd_level_of_detail_;

			// flag indicating the use of transformation as input
			// TODO switch to option
			//bool                            input_type_transformation_;

			// ------ data structures for slink and clink 

			// stores the distance at which this indexed element has longer 
			// the largest index of its cluster
			std::vector<double>    lambda_;

			// the index of the cluster representative at merge-time 
			// (element with largest index)
			std::vector<int>       pi_;

			std::vector<double>    mu_;


			// ----- data structure for CENTER_OF_GRAVITY_CLINK
			// the geometric center of mass
			std::vector<Vector3>   com_;

			// ----- generell datastructures


			// We cache the atom bijection for faster
			// RMSD computation; this is possible, since the system topology does
			// not change
			AtomBijection     atom_bijection_;

			System            system_i_;
			System            system_j_;

			// The number of poses to cluster
			Size              num_poses_;

	}; //class PoseClustering
} //namesspace BALL

#endif // BALL_DOCKING_POSECLUSTERING_H
