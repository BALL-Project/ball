// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_DOCKING_COMMON_DOCKINGALGORITHM_H
#define BALL_DOCKING_COMMON_DOCKINGALGORITHM_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
# include <BALL/DOCKING/COMMON/conformationSet.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_SYSTEM_TIMER_H
#include <BALL/SYSTEM/timer.h>
#endif

#ifndef BALL_DOCKING_COMMON_CONSTRAINTS_H
#include <BALL/DOCKING/COMMON/constraints.h>
#endif


namespace BALL
{

	//namespace Docking
	//{
	class ScoringFunction;
	//}

	/** Interface class for docking algorithms.
			\ingroup Docking
	*/
	class BALL_EXPORT DockingAlgorithm
	{
		public:

			enum DISPLAYMODE
			{
				NO_DISPLAY = 0,
				BEST_INTERMEDIATE_POSES = 1,
				ALL_INTERMEDIATE_POSES = 2,
				NO_INTERMEDIATE_POSES = 3
			};

			DockingAlgorithm();

			DockingAlgorithm(System& receptor, System& ligand, Options& new_options);

			DockingAlgorithm(System& receptor, System& ligand);

			virtual ~DockingAlgorithm();

			/** Attributes
			*/
			Options options;


			/** Operations
			*/

			/** Full setup.
			 *  Assigns systems 1 and two and the options for this DockingAlgorithm.
			 */
			virtual void setup(System& receptor, System& ligand, Options& new_options);

			/** Alternative setup.
			 *  Assigns systems 1 and two without changing the options for this DockingAlgorithm.
			 */
			virtual void setup(System& receptor, System& ligand);

			static void readOptionFile(String filename, Options& output_options, list<Constraint*>& output_constraints, const AtomContainer* ref_ligand = 0);

			static void writeOptionFile(String filename, Options& input_options, list<Constraint*>& input_constraints);

			/** dock the given ligand and return its score */
			//virtual double dockLigand(AtomContainer& ligand, bool verbose = 0) = 0;
			virtual double dockLigand(AtomContainer& ligand, bool verbose = 0);

			/** return the ScoringFunction that is used by this DockingAlgorithm */
			ScoringFunction* getScoringFunction();

			void setScoringFunction(ScoringFunction* scoring) { scoring_function_ = scoring;}

			void processMultiMoleculeFile(string input_filename, string output_filename, double score_cutoff, vector<double>* min_atoms_in_ref_areas = 0, String toolinfo="", String timestamp="");

			void setLigand(AtomContainer* ligand);

			const AtomContainer* getLigand();

			const AtomContainer* getReferenceLigand();

			/** get the name of this docking algorithm */
			const String& getName();

			/**
			*/
			virtual void start();

			/**
			*/
			virtual void pause();

			/**
			*/
			virtual void proceed();

			/**
			*/
			virtual void abort();

			virtual void finish();

			/** resets Algorithm before new dock-run, e.g. sets finished_ to false */
			virtual void reset();

			/**
			*/
			virtual bool hasFinished() const;

			///
			virtual bool wasAborted() const
				{ return abort_;}

			///
			virtual bool wasPaused() const
				{ return pause_;}

			/**
			*/
			virtual float getProgress() const;

			/** Return total_conformations putative complexes, ordered
			 *  according to their rank.
			 *  If total_conformations == 0, the option TOP_N is used
			 */
			virtual ConformationSet getConformationSet(Index total_conformations = 0);

			double calculateRMSD(const AtomContainer& S1, const AtomContainer& S2, int* no_matched_atoms = NULL);

			static Matrix4x4 mapCompounds(const AtomContainer& S1, const AtomContainer& S2, Size& no_matched_heavy_atoms, double& rmsd, double upper_bound, double lower_bound, double tolerance);

			const static String OPTION_FILE_PARAMETER_NAME;

			const AtomContainer* getVisualizationPose();

			void setVisualizationPose(const double& score);

			double getVisualizationPoseScore();

			bool new_pose_to_be_visualized;

			DISPLAYMODE getDisplayMode();

			void setDisplayMode(DISPLAYMODE display_mode);

			/** Set the maximal number of frames per second for the visualization output. This affects only the number of poses that are written for visualization NOT the actual speed of the docking algorithm itself. */
			void setMaxFps(int no);

		protected:

			static void writeSubcategories_(Options& category, std::ostream& out);

			System* system1_;
			System* system2_;

			bool   pause_;
			bool   abort_;
			bool finished_;

			AtomContainer* reference_ligand_;

			/** the name of the docking algorithm */
			String name_;

			AtomContainer* receptor_;

			AtomContainer* ligand_;

			void mapLigandOntoReferenceLigand();

			ScoringFunction* scoring_function_;

			String parameter_filename_;

			/** name of the ScoringFunction to be used */
			String scoring_type_;

			DISPLAYMODE display_mode_;

			/** copy of a intermediate pose that is to be visualized.  */
			AtomContainer visualization_pose_;

			double visualization_pose_score_;

			double min_sec_between_visualizations_;
			Timer visualization_timer_;
	};

} // namespace BALL

#endif // BALL_DOCKING_COMMON_DOCKINGALGORITHM_H
