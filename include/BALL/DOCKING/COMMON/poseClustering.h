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
#include <boost/variant.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <set>
#include <iostream>

#ifdef BALL_HAS_TBB
# include <tbb/parallel_reduce.h>
# include <tbb/blocked_range.h>
#endif

//#define POSECLUSTERING_DEBUG 1
#undef POSECLUSTERING_DEBUG

namespace BALL
{
	/** Pose Clustering 
	    \ingroup DockingMiscellaneous
	 */

  /** \brief Computation of clusters of docking poses.
	
	    This class computes clusters of docking poses given as a 
			conformation set using a complete linkage algorithm.

			The class assumes the following setup 
			   - a pairwise rigid protein-protein docking 
				 - all receptor-ligand poses have already been mapped onto each other
					 such that the "receptors" are kept fixed
				 - the given pose set only contains the "ligands"
			
			We offer several algorithms via the option CLUSTER_METHOD:
			
		   - TRIVIAL_COMPLETE_LINKAGE: a naive implementation, that guarantees an optimal final partition.
			 - CLINK_DEFAYS as described in 
			            D. Defays: An efficient algorithm for a complete link method. 
                  The Computer Journal. 20, 4, British Computer Society, 1977, p. 364-366.
				 Please note that this implementation does not guarantee to find the best final clustering!
			 - NEAREST_NEIGHBOR_CHAIN_WARD as described in
			            Murtagh, Fionn (1983): "A survey of recent advances in hierarchical clustering algorithms", 
			            The Computer Journal 26 (4): 354–359	
				 Note that this algorithm computes a full clustering.
			 - SLINK_SIBSON as described in 
			            R. Sibson: SLINK: an optimally efficient algorithm for the single-link cluster method. 
                  The Computer Journal. 16, 1, British Computer Society, 1973, p. 30-34

		  
		  The scope of the scoring (the atoms to be considered) can be defined via the option RMSD_LEVEL_OF_DETAIL.
			If the option is set to PROPERTY_BASED_ATOM_BIJECTION, arbitrary sets of atoms, e.g. binding pockets, 
			can be used by assigning property named "ATOMBIJECTION_RMSD_SELECTION" to the respective atoms in the 
			reference system.
			See also BALL::Expression.

			The minimal rmsd or ward distance between the final clusters can be defined via option DISTANCE_THRESHOLD.
			In order to relate RMSD and ward distance, we use sqrt(ward_dist / number_of_selected_atoms)
			for threshold extraction.

			The nearest neighbor chain ward clustering in principle computes a full clustering. 
			Option DISTANCE_THRESHOLD gives a ward distance that is automatically used to extract clusters. 
			Further extractions with different thresholds are possible.

			The complete linkage algorithms guarantee a minimal cluster distance (max RMSD between all pairs
		 	of two clusters), specified with option DISTANCE_THRESHOLD.

			The initial poses can be given as ConformationSet or as transformation file, i.e. translation and 
			rotation of each pose.
			Depending on this choice, the option RMSD_TYPE has to be set to SNAPSHOT_RMSD or RIGID_RMSD.
			If RMSD_TYPE is set to CENTER_OF_MASS_DISTANCE, the option RMSD_LEVEL_OF_DETAIL will be ignored.

			By setting the option RUN_PARALLEL to true, the user can request parallel execution. This will be performed
			if the execution environment is enabled (BALL_HAS_TBB), and if the algorithm supports it.
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
				static const String DISTANCE_THRESHOLD;

				/** the level of detail when computing the RMSD
				*/
				static const String RMSD_LEVEL_OF_DETAIL;

				/** the computation type of the cluster distance meassure
				*/
				static const String RMSD_TYPE;

				/** flag for requesting parallel execution
				 */
				static const String RUN_PARALLEL;

			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const Index CLUSTER_METHOD;
				static const float DISTANCE_THRESHOLD;
				static const Index RMSD_LEVEL_OF_DETAIL;
				static const Index RMSD_TYPE;
				static const bool  RUN_PARALLEL;
				static const bool USE_CENTER_OF_MASS_PRECLINK;
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
				CLINK_DEFAYS,
				NEAREST_NEIGHBOR_CHAIN_WARD,
				CLINK_ALTHAUS
			};

			class BALL_EXPORT RigidTransformation
			{
				public:
					RigidTransformation() {};

					RigidTransformation(Eigen::Vector3f const& new_trans, Eigen::Matrix3f const& new_rot)
						: translation(new_trans),
						  rotation(new_rot)
					{}

					Eigen::Vector3f translation;
					Eigen::Matrix3f rotation;
			};

			/** Data type for the poses.
			 *  A pose can either be a rigid transformation (translation + rotation), or
			 *  a SnapShot.
			 */
			class BALL_EXPORT PosePointer
			{
				public:
					PosePointer(RigidTransformation const* new_trafo, SnapShot const* new_snap = 0)
						: trafo(new_trafo),
						  snap(new_snap)
					{ }

					PosePointer(SnapShot const* new_snap)
						: trafo(0),
							snap(new_snap)
					{ }

					RigidTransformation const* trafo;
					SnapShot            const* snap;
			};

			class BALL_EXPORT ClusterProperties
			{
				public:
					/** Serialization method
					 */
					template <class Archive>
					void serialize(Archive& ar, const unsigned int version);

					/** The poses contained in this cluster.
					 */
					std::set<Index> poses;

					/** The number of poses contained in this cluster.
					 */
					Size size;

					/** The center of the cluster.
					 *  Depending on the type of transformations we allow,
					 *  this is either stored as a rigid transformation or
					 *  as the 3N-dimensional vector given by the atom
					 *  coordinates. The special case of using only the center
					 *  of mass is achieved by setting the rotation matrix to
					 *  identity.
					 */
					boost::variant<Eigen::VectorXf, RigidTransformation> center;

					/** The value at which this cluster is merged with its sibling.
					 */
					float merged_at;
#ifdef POSECLUSTERING_DEBUG 
					/** The cluster_idx assigend by last call of method extractClustersForThreshold 
					 */
					float current_cluster_id;
#endif
			};

			typedef boost::adjacency_list<boost::vecS,
			                              boost::vecS,
			                              boost::directedS,
			                              ClusterProperties,
																		boost::no_property,
																		unsigned int> ClusterTree;

			typedef ClusterTree::vertex_descriptor ClusterTreeNode;

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

			/// sets the poses to be clustered, the conformation set's reference system will the base system
			void setConformationSet(ConformationSet* new_set, bool precompute_atombijection = false);

			/** Set a vector of PosePointers to be clustered
			 *  Poses (RigidTransformations or SnapShots) can live outside of this class and will not be
			 *  destroyed.
			 */
		  void setBaseSystemAndPoses(System const& base_system, std::vector<PosePointer> const& poses);

			/// reads the poses given as transformations from a file and update the covariance matrix 	
			/// Note: the given system will be taken as reference, e.g. all transformations 
			//       will be applied upon the current conformation
			void setBaseSystemAndTransformations(System const& base_system, String transformation_file_name);

			/// returns the poses to be clustered as ConformationSet
			const ConformationSet* getConformationSet() const {return  current_set_;}

			/// returns the poses to be clustered as ConformationSet
			ConformationSet* getConformationSet() {return  current_set_;}

			/// returns the poses as rigid transformations
			const std::vector<RigidTransformation> & getRigidTransformations() const {return transformations_;}

			/// returns the centers of mass-vector (non-empty only for CENTER_OF_MASS_DISTANCE)
			std::vector<Vector3> & getCentersOfMass() {return com_;}

			/// returns the centers of mass-vector, const version (non-empty only for CENTER_OF_MASS_DISTANCE)
			std::vector<Vector3> const & getCentersOfMass() const {return com_;}

			/// returns the reference pose 
			const System& getSystem() const;

			/// returns the reference pose
			System& getSystem();

			/// returns the number of poses
			Size getNumberOfPoses() const {return poses_.size();}

			/// returns the number of clusters found
			Size getNumberOfClusters() const {return clusters_.size();}

			/// returns indices of all poses assigned to cluster i
			/// Note: enumeration starts with 0
			const std::set<Index>& getCluster(Index i) const;

			/// returns indices of all poses assigned to cluster i
			/// Note: enumeration starts with 0
			std::set<Index>& getCluster(Index i);

			/// returns the size of cluster i
			Size getClusterSize(Index i) const;

			/// returns the score of cluster i
			float getClusterScore(Index i) const;

			/// returns the score between two poses given as systems
			float getScore(const System sys_a, const System sys_b, Options options) const;

			/// returns a reference to the cached AtomBijection
			AtomBijection& getAtomBijection() {return atom_bijection_;}

			/// returns a const reference to the cached AtomBijection
			AtomBijection const& getAtomBijection() const {return atom_bijection_;}

			/// apply a transformation to a given system
			void applyTransformation2System(Index i, System& target_system);

			/// convert the poses to SnapShots
			void convertTransformations2Snaphots();

			/// convert the poses to rigid transformations
			void convertSnaphots2Transformations();

			/// returns the complete linkage RMSD of cluster i
			float computeCompleteLinkageRMSD(Index i, Options options, bool initialize = true);

			/// returns the complete linkage RMSD of a pose set
			//float computeCompleteLinkageRMSD(boost::shared_ptr<ConformationSet> cluster, Option options) const;

			/// returns the pose i as system
			boost::shared_ptr<System> getPose(Index i) const;

			/// returns poses as PosePointer
			std::vector<PosePointer> const&  getPoses() const {return poses_;}

			/// returns the "central cluster" conformation of cluster i as system 
			boost::shared_ptr<System> getClusterRepresentative(Index i);

			/// returns the index of the cluster representative
			Index findClusterRepresentative(Index i);

			/// returns cluster i as ConformationSet
			boost::shared_ptr<ConformationSet> getClusterConformationSet(Index i);

			/// returns a ConformationSet containing one structure per cluster
			boost::shared_ptr<ConformationSet> getReducedConformationSet();

			/** Refine a given clustering.
			 *  This function can be used to refine a precomputed clustering further. An important
			 *  use case would be to pre-cluster using an efficient rmsd implementation (e.g., center
			 *  of mass or rigid rmsd), and then refine the resulting clusters with the general (i.e.,
			 *  snapshot based) rmsd.
			 *
			 *  NOTE: This function requires that clusters have already been computed. In the case of
			 *        a full hierarchical clustering, extractClustersForThreshold or extractNBestClusters
			 *        must have been called previously.
			 *
			 *  @param refined_options The parameters for the refinment step.
			 *
			 */
			bool refineClustering(Options const& refined_options);

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

			/** @name	rigid transformation methods */
			//@{

			/** Compute the root mean square deviation due to a rigid transformation of a point cloud (here, atoms)
			 *  @param t_ab difference vector between the transformations to be compared
			 *  @param M_ab difference of the rotation matrices between the transformations to be compared
			 *  @param covariance_matrix the covariance matrix of the atom positions
			 */
			static float getRigidRMSD(Eigen::Vector3f const& t_ab, Eigen::Matrix3f const& M_ab, Eigen::Matrix3f const& covariance_matrix);

			/** Compute the mean square deviation due to a rigid transformation of a point cloud (here, atoms)
			 *  @param t_ab difference vector between the transformations to be compared
			 *  @param M_ab difference of the rotation matrices between the transformations to be compared
			 *  @param covariance_matrix the covariance matrix of the atom positions
			 */
			static float getSquaredRigidRMSD(Eigen::Vector3f const& t_ab, Eigen::Matrix3f const& M_ab, Eigen::Matrix3f const& covariance_matrix);

			/** Compute the covariance matrix for the given system
			 */
			static Eigen::Matrix3f computeCovarianceMatrix(System const& system, Index rmsd_level_of_detail = C_ALPHA);

			//@}

			/** @name	methods given a full clustering */
			//@{

			/** Extract clusters wrt a threshold if a complete clustering was performed
			 *  Note: the Ward distance does not equal the rmsd. 
			 *  We use threshold = sqrt(ward_dist / number_of_selected_atoms).
			 *  see NEAREST_NEIGHBOR_CHAIN_WARD
			 */
			std::vector<std::set<Index> > extractClustersForThreshold(float threshold, Size min_size = 0);

			/** returns the first up to n clusters if previously a complete clustering was performed
			 *  see NEAREST_NEIGHBOR_CHAIN_WARD
			 */
			std::vector<std::set<Index> > extractNBestClusters(Size n);

			/** filters the current cluster set wrt to a minimal cluster size
			 *  see NEAREST_NEIGHBOR_CHAIN_WARD
			 */
			std::vector<std::set<Index> > filterClusters(Size min_size = 1);

			/** Export the cluster tree to boost::serialize format.
 			*/
			void serializeWardClusterTree(std::ostream& out, bool binary = false);

			/** Import the cluster tree from boost::serialize format.
 			*/
			void deserializeWardClusterTree(std::istream& in, bool binary = false);

			/** Export the cluster tree in graphviz format.
 			*/
			void exportWardClusterTreeToGraphViz(std::ostream& out);

			//@}


			/// print the clusters as set of pose indices
			/// Note: start counting with 0
			void printClusters(std::ostream& out = std::cout) const;

			/// print clusters of pose indices with RMSD between clusters	
			/// Note: start counting with 0
			void printClusterScores(std::ostream& out = std::cout);


		protected:

#ifdef BALL_HAS_TBB
			/** A nested class used for parallel execution of the nearest neighbour chain algorithm.
			 */
			class ComputeNearestClusterTask_
			{
				public:
					/// Default constructor.
					ComputeNearestClusterTask_(PoseClustering* parent,
					                           const std::vector<ClusterTreeNode>& active_clusters,
					                           Position current_cluster,
																		 Index rmsd_type);

					/// Splitting constructor.
					ComputeNearestClusterTask_(ComputeNearestClusterTask_& cnct, tbb::split);

					/// Join two partial results
					void join(ComputeNearestClusterTask_ const& cnct);

					/// The minimum computation
					void operator() (const tbb::blocked_range<size_t>& r);

					/// Return the minimum index
					Position getMinIndex() {return my_min_index_;}

					/// Return the minimum value
					float getMinValue() {return my_min_value_;}

				protected:
					// the PoseClustering instance that called us
					PoseClustering* parent_;

					// the array we work on
					const std::vector<ClusterTreeNode>& active_clusters_;

					// the cluster to compare to everything else
					Position current_cluster_;

					// the kind of rmsd computation desired
					Index rmsd_type_;

					// the minimum index in our own block
					Position my_min_index_;

					// the minimum value in our own block
					float my_min_value_;
			};
#endif

			/** A nested class used for exporting cluster trees to graphviz format
			 */
			class ClusterTreeWriter_
			{
				public:
					ClusterTreeWriter_(ClusterTree const* cluster_tree)
						: cluster_tree_(cluster_tree)
					{ }

					void operator() (std::ostream& out, const ClusterTreeNode& v) const;

				protected:
					ClusterTree const* cluster_tree_;
			};

			/** A nested class for our cluster tree node priorization
			 */
			class ClusterTreeNodeComparator
			{
				public:
					ClusterTreeNodeComparator(ClusterTree& cluster_tree)
						: cluster_tree_(&cluster_tree)
					{}

					bool operator() (ClusterTreeNode const first, ClusterTreeNode const second) const
					{
						float first_value  = (*cluster_tree_)[ first].merged_at;
						float second_value = (*cluster_tree_)[second].merged_at;

						return first_value < second_value;
					}

				protected:
					ClusterTree* cluster_tree_;
			};

			// trivial complete linkage implementation
			// with O(n^2) space request
			bool trivialCompute_();

			// space efficient (SLINK or CLINK) clustering
			bool linearSpaceCompute_();

			//
			bool althausCompute_();

			//	implementation of a single linkage clustering as described in 
			//       R. Sibson: SLINK: an optimally efficient algorithm for the single-link cluster method. 
			//       The Computer Journal. 16, 1, British Computer Society, 1973, p. 30-34
			void slinkInner_(int current_level);

			//  implementation of a complete linkage clustering as described in 
			// 					D. Defays: An efficient algorithm for a complete link method. 
      //          The Computer Journal. 20, 4, British Computer Society, 1977, p. 364-366. 
			void clinkInner_(int current_level);

			// implememtation of the nearest neighbor chain clustering algorithm 
			// as described in:
			//          Murtagh, Fionn (1983): "A survey of recent advances in hierarchical clustering algorithms", 
			//          The Computer Journal 26 (4): 354–359
			bool nearestNeighborChainCompute_();

			void initWardDistance_(Index rmsd_type);

			void updateWardDistance_(ClusterTreeNode parent, ClusterTreeNode i, ClusterTreeNode j, Index rmsd_type);

			float computeWardDistance_(ClusterTreeNode i, ClusterTreeNode j, Index rmsd_type);

			std::set<Index> collectClusterBelow_(ClusterTreeNode const& v);

			// compute the center of masses
			void computeCenterOfMasses_();

			// precompute an atom bijection for faster access
			void precomputeAtomBijection_();

			// check the given atom wrt choice of option RMSD_LEVEL_OF_DETAIL
			bool static isExcludedByLevelOfDetail_(Atom const* atom, Index rmsd_level_of_detail);

			// distance between cluster i and cluster j
			float getClusterRMSD_(Index i, Index j, Index rmsd_type);

			// reads the poses given as transformations from a file
			// Note: the previously given system will be taken
			//       as untransformed reference, e.g. all transformations 
			//       will be applied upon the current conformation
			bool readTransformationsFromFile_(String filename);

			// compute the RMSD between two "poses"  
			float getRMSD_(Index i, Index j, Index rmsd_type);

			// store pointers to the snapshots in the poses vector
			void storeSnapShotReferences_();

			//
			void printCluster_(Index i, std::ostream& out = std::cout) const;

			// 
			void printVariables_(int a, int b, double c, int d, double e, int current_level);

			//
			void clear_();

			// only used by trivial clustering
			Eigen::MatrixXd                  pairwise_scores_;

			/// the ConformationSet we wish to cluster
			ConformationSet*                 current_set_;

			/// the clusters: sets of pose indices 
			std::vector< std::set<Index> >   clusters_;

			std::vector< Index >   cluster_representatives_;

			/// the scores of the clusters 
			std::vector< float >             cluster_scores_;

			/// the RMSD definition used for clustering
			Index                            rmsd_level_of_detail_;


			// ----- unified access to the poses, independent of their type
			//       (the poses are either stored in transformations_, or in current_set_)
			std::vector<PosePointer>         poses_;

			// ----- data structure for transformation input (instead of snapshots)
			std::vector<RigidTransformation> transformations_;

			Eigen::Matrix3f                  covariance_matrix_;

			// TODO: maybe use a const - ptr instead?
			System                           base_system_;

			// the reference state
			SnapShot                         base_conformation_;

			// flag indicating the use of transformation as input
			bool                             has_rigid_transformations_;

			// do we need to delete the conformation set, that was
			// created by converting transformations to snapshots
			bool                             delete_conformation_set_;

			// ------ data structures for slink and clink 

			// stores the distance at which this indexed element has longer 
			// the largest index of its cluster
			std::vector<double>    lambda_;

			// the index of the cluster representative at merge-time 
			// (element with largest index)
			std::vector<int>       pi_;

			std::vector<double>    mu_;


			// ----- data structures for nearest neighbor chain ward
			Size                   number_of_selected_atoms_;

			// ----- data structure for CENTER_OF_GRAVITY_CLINK

			// the geometric center of mass
			std::vector<Vector3>   com_;

			// ----- general data structures

			// We cache the atom bijection for faster
			// RMSD computation; this is possible, since the system topology does
			// not change
			AtomBijection     atom_bijection_;

			// helper dummies to speed up snapshot application
			System            system_i_;
			System            system_j_;

			/// The tree built during hierarchical clustering
			ClusterTree       cluster_tree_;
	}; //class PoseClustering
} //namesspace BALL

#endif // BALL_DOCKING_POSECLUSTERING_H
