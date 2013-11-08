// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMMON_GRIDBASEDSCORING_H
#define BALL_SCORING_COMMON_GRIDBASEDSCORING_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMMON/scoreGridSet.h>
#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <set>
#include <map>

namespace BALL
{
	class ScoreGridSet;
	class Residue;
	class Rotamer;

	class BALL_EXPORT GridBasedScoring : public ScoringFunction
	{
		public:
			struct Option
			{
				/** the resolution of the ScoreGrids in units of Angstroem (not to be confused with ScoringFunction::resolution_, which is the resolution of the _Hash_Grid !!) */
				static const char* SCOREGRID_RESOLUTION;

				/** determines whether or not interpolation should be used in order to calculated the score from the precalculated ScoreGridSets. */
				static const char* SCOREGRID_INTERPOLATION;
			};

			struct Default
			{
				static double SCOREGRID_RESOLUTION;
				static bool SCOREGRID_INTERPOLATION;
			};

			GridBasedScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options);

			GridBasedScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

			virtual ~GridBasedScoring();

			/** creates a new ScoreGridSet and appends it to the existing ones.\n
			See class ScoreGridSet for documentation of the other parameters
			@param angstroem the size of the grid in Angstroem (since this might be more comfortable when supplying a molecule) */
			void defineGridSet(AtomContainer* sys, Vector3& angstroem, double out_of_grid_penalty, bool enforce_grid_boundaries, double resolution, double interaction_no_scale = 1);

			/** create a new ScoreGridSet and appends it to the existing ones.\n
			The size of this ScoreGridSet is automatically set in such a way that it is large enough to encompass all nonbonded interactions with the given AtomContainer (dependend on nonbonded_cutoff_, which is set by use of an Option object in the ScoringFunction constructor) */
			void defineGridSet(AtomContainer* sys, double out_of_grid_penalty, bool enforce_grid_boundaries, double resolution, double interaction_no_scale = 1);

			void defineFlexibleResiduesGridSet();

			void printResult();

			/** sets the atom_types that are to be used */
			void setAtomTypeNames(std::set<String>& types);

			/** precalculate one Grid for each desired AtomType and one Grid for the electrostatic (q1/distance) */
			void precalculateGrids(bool ony_flexRes_grids = false);

			/** saves all previously calculated Grids to the specified file */
			void saveGridSetsToFile(String file, String receptor_name);

			/** restores GridSet from the given file and appends it to the current ScoreGridSets */
			void readAdditionalGridSetFromFile(String file);

			/** deletes all existing ScoreGridSet and creates a new one from a given file */
			void replaceGridSetFromFile(String file);

			/** Load precalculated ScoreGridSets for the given residues from files. \n
			The values stored in those grids are automatically added to the ScoreGridSets that holds the scores for all flexible residues. */
			void loadFlexibleResidueScoreGrids(std::list<std::pair<const Residue*, const Rotamer*> > residue_list);

			void setupReferenceLigand();

			/** defines how the Scores computed for on all ScoreGridSets are to be combined \n
			0 = addition \n
			1 = average \n
			2 = minimum \n
			3 = maximum */
			void setCombineOperation(int i);

			/** Returns the average number of receptor atoms neighboring each ligand atom as determined by the last call of calculateGridScore() (which is called by update()). */
			int getNoNeighboringReceptorAtoms();

			std::vector<ScoreGridSet*>* getScoreGridSets();

			void validateGridSets();

		protected:
			void setup();

			struct GridSetsResult
			{
				/** contains one energy value per GridSet */
				std::vector<double> gridSet_scores;

				/** for each GridSet, it contains the average number of neighbors to each atom of the current ligand */
				std::vector<double> no_neighbors;

				/** for each GridSet, it contains the number of atoms of the current ligand that lie outside of the GridSet*/
				std::vector<Size> no_out_of_grid;

				void setup(Size no_gridSets);
			};

			/** saves the final and all intermediate results of the last call of updateScore() */
			GridSetsResult gridsets_result_;

			/** calculates the score for the interaction between the receptor and the current ligand conformation by use of the precalculated GridSets */
			double calculateGridScore();

			/** creates a new ScoreGridSet for receptor_ according to dimensions/properties of ScoringFunction::hashgrid_ \n
			See class ScoreGridSet for documentation of the other parameters
			@param size the number of grid cell on each axis */
			void defineReceptorGridSet(Vector3& origin, Vector3& size, double out_of_grid_penalty, bool enforce_grid_boundaries, double resolution = 0.5, double interaction_no_scale = 1);

			virtual AtomTypes& getAtomTypes() = 0;

			std::map<String, int>* getAtomTypesMap();

			/** Set the desired type of the given atom.
			 This function can be overloaded when setting the atom's types type is done in a different way, like for example for Gehlhaar's ScoringFunction */
			virtual void setAtomType(Atom* atom, const String& type_name);

			/** Get the atom-type name for the specified grid (as stored in atom_types_map_) */
			String getGridAtomTypeName(int grid_id);

			/* --------- protected members ------------------ */
			/** see setCombineOperation() */
			int combine_operation_;

			/** maps each AtomType that is to be used to the ID of its Grid (position within ScoreGridSet.score_grids) */
			std::map<String, int> atom_types_map_;

			/** stores all calculated GridSets */
			std::vector<BALL::ScoreGridSet*> grid_sets_;

			/** update all nonbonded atom-pair lists and precalculate the score
			@param set the id of the ScoreGrid, whose HashGrid is to be used */
			virtual void updatePrecalculatedScore(Size set) = 0;

			/** the resolution of the ScoreGrids in units of Angstroem (not to be confused with ScoringFunction::resolution_, which is the resolution of the _Hash_Grid !!) */
			double scoregrid_resolution_;

			/** determines whether or not interpolation should be used in order to calculated the score from the precalculated ScoreGridSets. */
			bool scoregrid_interpolation_;

			/** index of the ScoreGridSet for the flexible residues of the receptor (if there are any) */
			int flex_gridset_id_;

			/** Stores one ScoreGridSets for each flexible sidechain. \n
			These ScoreGridSets are not used directly during updateScore() (and therefore they are not put into grid_sets_), but their scores are added to the one ScoreGridSet that holds the score-sums of all flexible residues by loadFlexibleResidueScoreGrids(). */
			std::map<const Residue*, ScoreGridSet*> flex_gridsets_;

			friend class ScoreGridSet;
			friend class PharmacophoreConstraint;
	};
}

#endif // BALL_SCORING_COMMON_GRIDBASEDSCORING_H
