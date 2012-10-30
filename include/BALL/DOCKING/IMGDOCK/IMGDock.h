// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_IMGDOCK_IMGDOCK_H
#define BALL_DOCKING_IMGDOCK_IMGDOCK_H

#ifndef BALL_DOCKING_COMMON_DOCKINGALGORITHEM_H
#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MATH_MATRIX44_H
#include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_SCORING_COMMON_SCORINGFUNCTION_H
#include <BALL/SCORING/COMMON/scoringFunction.h>
#endif

#ifndef BALL_DOCKING_COMMON_STATICLIGANDFRAGMENT_H
#include <BALL/DOCKING/COMMON/staticLigandFragment.h>
#endif

#ifndef BALL_DOCKING_COMMON_SIDECHAINOPTIMIZER_H
#include <BALL/DOCKING/COMMON/sideChainOptimizer.h>
#endif

#include <map>


namespace BALL
{
	/** Iterative Multi-Greedy Docking  */
	class BALL_EXPORT IMGDock : public DockingAlgorithm
	{
		public:
			/**	@name	Constructors&Destructors  */
			//@{
			IMGDock(System& receptor, System& ligand, Options& new_options);

			IMGDock(System& receptor, System& ligand);

			IMGDock(System& receptor, System& ligand, string config_file);

			~IMGDock();
			//@}

			/**	@name Constant Definitions
			*/
			//@{
			struct Option
			{
				static const char* STEP_WIDTH;
				static const char* NO_SOLUTIONS;
				static const char* GLOBAL_ROTATION;
				static const char* POST_OPTIMIZATION_STEPS;
				static const char* POST_OPTIMIZATION_STEP_WIDTH;
				static const char* MIN_INHIBITOR_ATOMS;
				static const char* SCORING_TYPE;
				static const char* ITERATIONS;
				static const char* DECREASE_STEPWIDTH;
				static const char* SUPERPOSE_LIGAND;
			};

			struct Default
			{
				static int STEP_WIDTH;
				static int NO_SOLUTIONS;
				static bool GLOBAL_ROTATION;
				static int POST_OPTIMIZATION_STEPS;
				static double POST_OPTIMIZATION_STEP_WIDTH;
				static int MIN_INHIBITOR_ATOMS;
				static String SCORING_TYPE;
				static int ITERATIONS;
				static bool DECREASE_STEPWIDTH;
				static bool SUPERPOSE_LIGAND;
			};

			/**	@name	Accessors  */
			//@{
			void setup(System& receptor, System& ligand, Options& new_options);

			void setup(System& receptor, System& ligand);

			void start();

			/** tries to dock a given ligand and return the obtained energy ( = internal conformation energy of the ligand + binding free energy) */
			double dockLigand(AtomContainer& ligand, bool verbose = 0);

			/** returns the energy of current conformation of the ligand candidate ( = conformation energy+interaction energy), as calculated by the last call of update() */
			double getEnergy();

			/** returns the IMGDock score for the current conformation of the ligand candidate as calculated by the last call of update(); i.e. the interaction+conformation energy scaled ... \n
			-- with respect to the depth of burial of the ligand candidate into the binding pocket \n
			-- and with respect to the number/fraction of ligand atoms that are located within the various reference areas (which are defined by the user) */
			double getScore();

			/** prints information about each fragment of the ligand */
			void printFragments(std::ostream& out = std::cout);

			void printResults(std::ostream& out = std::cout);

			/** rotate a ligand fragment around one of its bonds that connects it to another fragment */
			void rotateLigandFragment(Size fragment, Size bond, int degree);

			void steps(Size no = 1);

			/** updates energy and forces between target and ligand */
			void update();

			void startDock(bool verbose = 0);

			const AtomContainer* getLigand();

			//ConformationSet getConformationSet(Index total_conformations);

			/** stores the default options of this algorithms in the given Option object */
			static void getDefaultOptions(Options& options);
			//@}

			/**	@name	Attributes  */
			//@{
			int rec;
			//@}

		private:
			typedef std::multimap<double, std::vector<int> > PoseList;

			/**	@name	Accessors  */
			//@{
			void rotateLigandFragment(StaticLigandFragment* lf, TMatrix4x4<float>& M, Vector3& origin, int static_fragment);

			/** translates the entire ligand candidate according to the given vector */
			void translateLigand(Vector3& v);

			/** calculates IMGDock score from the current interaction+conforamtion energy */
			void calculateScore(double& interact_energy, double& conf_energy);

			void createLigandFragments();

			/** stores force vectors of ligand atoms to old_forces_ */
			void storeForces();

			/** reassigns the ligand atom forces from old_forces_ */
			void reassignForces();

			/** stores information about each inter-fragment bond in bond_information_  */
			void saveBondInformation();

			void saveBondInformation(AtomPairList* mapped_atoms);

			bool findUnfrozenFragments(int fragment, std::set<int> frozen_fragments, std::set<int>& processed_fragments);

			void postDockOptimization(double step_width, int no_steps);

			void recursionPrint(string line);

			void optimizeRotation(std::vector < int > & conf, PoseList& best_conformations, Size bond, bool ignore_original_angle);

			void displayConformation(const std::vector < int > & conf, const double& energy);

			void applyConformation(const std::vector < int > & conf, bool verbose = 0);

			/** try to find a bond, so that the force vector is orthogonal to the plane spanned by the bond and the fragment center */
			//void findBestFragmentRotation(int fragment, Size& bond, int& degree, map<int>* ignore_bonds);

			/** find the inter-fragment bond that has the largest angle to the currently largest repulsive force vector (i.e. which is orthogonal in the best case)
			@param ignore_bond ID of bond that should be ignored because rotation around it did not decrease energy; \n
			if ignore_bond == -1, no bond is ignored
			@param fragment if fragment == -1, the fragment to rotate is obtained by this function itself, overwriting the value at the given reference; \n
			if fragment != -1, the bond to rotate around is searched within the given fragment
			@param degree angle between largest repulsive force vector and plane build out of inter-fragment bond and atom on which this force vector acts on */
			//void findBestRotation(int& fragment, Size& bond, int& degree, map* ignore_bonds = NULL, bool use_fragment_force = 0);

			/** rotate the entire ligand around the X-, Y-, or Z-axis
			@param a the axis to rotate around, 0: = X-axis, 1: = Y-axis, 2: = Z-axis */
			void rotateLigand(int a, int degree);

			/** sets all rotations back to zero degrees */
			void resetRotations();

			/** saves positions of current ligand to original_atom_positions_ */
			void saveAtomPositions();

			//@}

			/**	@name	Attributes  */
			//@{

			/** the number of times the algorithms tries to successively optimize all inter-fragment bonds */
			int iterations_;

			/** determines whether each ligand is superposed to the reference ligand before the actual docking is started */
			bool superpose_ligand_;

			int no_steps_;

			std::vector<Vector3> original_atom_positions_;

			//AtomContainer* receptor_;

			double score_;

			/** geometrical center of the reference ligand, i.e. the ligand obtained from the cristal structure */
			Vector3 reference_center_;

			/** saves force vectors of ligand atoms, so that after undoing a rotation (because it increased overall energy) the forces need not be calculated anew */
			std::vector < Vector3 > old_forces_;

			/** current rotation angles of all inter-fragment bonds */
			std::vector < int > current_conformation_;

			/** for each entry of current_conformation_, this matrix saves the id of the fragment and the id of the inter-fragment bond of this fragment that this entry corresponds to in one column */
			std::vector < std::vector < int > > bond_information_;

			/** the highest index of a inter-fragment bond that has been rotated so far */
			int max_rotated_pos_;

			/** if set to true (via options), the step-width is reduced by a factor of 2 in each iteration (whereas the number of steps is held constant). */
			bool decrease_stepwidth_;

			/** the size of the angle (in degrees) by which should be rotated around inter-fragment bonds in each step */
			int step_width_;

			Size no_solutions_;

			/** the number of optimization steps that are to be done after docking by use of the function postDockOptimization() */
			int post_optimization_steps_;

			/** the width of each step of post docking optimization in units of Angstroem */
			double post_optimization_step_width_;

			/** determines whether the entire compound should be rotated around the global x-, y-, z-axis before opimizing inter-fragment bonds */
			bool global_rotation_;

			int min_inhibitor_atoms_;

			/** average number of target atoms around each reference ligand atom */
			int av_ref_lig_neighbors_;

			PoseList best_conformations_;

			SideChainOptimizer* sidechain_optimizer_;

			//@}
	};
}

#endif // BALL_DOCKING_IMGDOCK_IMGDOCK_H
