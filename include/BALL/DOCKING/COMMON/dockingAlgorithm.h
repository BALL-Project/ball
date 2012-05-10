/* dockingAlgorithm.h
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

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

#include <BALL/MATHS/matrix44.h>
#include <BALL/SYSTEM/timer.h>
#include <BALL/DOCKING/COMMON/constraints.h>


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


		#ifdef BALL_HAS_VIEW
			const AtomContainer* getVisualizationPose();

			void setVisualizationPose(const double& score);

			double getVisualizationPoseScore();

			bool new_pose_to_be_visualized;

			DISPLAYMODE getDisplayMode();

			void setDisplayMode(DISPLAYMODE display_mode);

			/** Set the maximal number of frames per second for the visualization output. This affects only the number of poses that are written for visualization NOT the actual speed of the docking algorithm itself. */
			void setMaxFps(int no);
		#endif


		protected:

			static void writeSubcategories_(Options& category, ostream& out);

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

		#ifdef BALL_HAS_VIEW
			/** copy of a intermediate pose that is to be visualized.  */
			AtomContainer visualization_pose_;

			double visualization_pose_score_;

			double min_sec_between_visualizations_;
			Timer visualization_timer_;
		#endif
	};

} // namespace BALL

#endif // BALL_DOCKING_COMMON_DOCKINGALGORITHM_H
