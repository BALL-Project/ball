// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMMON_SCOREGRIDSET
#define BALL_SCORING_COMMON_SCOREGRIDSET

#include <BALL/MATHS/vector3.h>
#include <BALL/SCORING/COMMON/gridBasedScoring.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/DOCKING/COMMON/constraints.h>


namespace BALL
{
	typedef vector<vector<vector<double> > > ScoreGrid;

	class GridBasedScoring;

	/** A ScoreGridSet is a set of grids that describes a specified area with one Grid for each AtomType */
	class BALL_EXPORT ScoreGridSet
	{
		public:
			/** constructor. create a ScoreGridSet with the given given resolution, starting at the specified origin.
			@param size the number of cells on each axis */
			ScoreGridSet(GridBasedScoring* gbs, Vector3& v_origin, Vector3& size, double& resolution);

			ScoreGridSet(GridBasedScoring* gbs);

			/** creates a ScoreGridSet by use of an existing ScoreGridSet. \n
			This can be helpful for using several ScoreGridSets for the same molecule . */
			ScoreGridSet(ScoreGridSet* sgs);

			/** Constructor for using a ScoreGridSet independent of a ScoringFunction. Use it only if you just want to read/write/examine grids without using the grids during scoring/docking. */
			ScoreGridSet();

			~ScoreGridSet();

			/** Add values of the given ScoreGridSet to this ScoreGridSet.
			Cells of the given ScoreGridSet that are locate outside of the range of this ScoreGridSet will be ignored !
			Note that in order to be able to use this function, both ScoreGridSets must have been created for the same atom types! */
			void operator += (ScoreGridSet& sgs);

			/** Substracts values of the given ScoreGridSet from this ScoreGridSet.
			Cells of the given ScoreGridSet that are locate outside of the range of this ScoreGridSet will be ignored !
			Note that in order to be able to use this function, both ScoreGridSets must have been created for the same atom types! */
			void operator -= (ScoreGridSet& sgs);

			/** Reset all grid cells to zero. */
			void clearData();

			ScoreGrid& operator[](int i);

			void binaryWrite(std::ostream& output);

			void binaryRead(std::istream& input);

			void saveToFile(std::ostream& output, String receptor_name);

			void readFromFile(std::istream& input);

			void setHashGrid(HashGrid3<Atom*>* hashgrid);

			HashGrid3<Atom*>* getHashGrid();

			void transform(TMatrix4x4 < float > & t);

			/** moves ( == translates, no rotation) the ScoreGridSet in such a way, that its center will be located at the given destination */
			void moveTo(Vector3& destination);

			/** fetches the score for a given atom position from the specified ScoreGrid.
			@param if set to true, linear interpolation between the neighboring grid cells is done */
			double getGridScore(Size grid, Vector3 position, bool interpolation);

			Size sizeX();

			Size sizeY();

			Size sizeZ();

			Size noGrids();

			/** enables the current ScoreGridSet, so that is is taken into account when calculating scores */
			void enable();

			/** disables the current ScoreGridSet, so that is is not taken into account when calculating scores */
			void disable();

			bool isEnabled();

			/** return the origin of this ScoreGridSet */
			Vector3 getOrigin();

			/** return the geometrical center of this ScoreGridSet */
			Vector3 getCenter();

			/** Converts the content of this object to RegularData3D grids, e.g. for visualization purposes. For each atom type, one RegularData3D grid is created.
			@return a list containing pairs of grid name and RegularData3D*. */
			std::list<std::pair<String, BALL::RegularData3D*> > convertToRegularData3DGrids();

			PharmacophoreConstraint* getPharmacophoreConstraint();

			void setPharmacophoreConstraint(PharmacophoreConstraint* phc);

			void setParameters(bool enforce_grid_boundaries, double out_of_grid_penalty, double interaction_no_scale);

			// -------- public members: -------------
			String name;

			/** the average number of interactions per reference ligand atom */
			Size reference_interactions;

			double interaction_no_scale_;

		private:
			std::map<String, int>* getAtomTypesMap();

			/** Get the atom-type name for the specified grid */
			String getGridAtomTypeName(int grid_id);

			/** are the scores taken from another ScoreGridSet? (which is the case when using several ScoreGridSets for the same molecule, e.g. water, at once) */
			bool is_reference_;

			/** the origin of the grid after all applied transformations */
			Vector3 origin_;

			/** the origin of the grid before any transformations were done */
			Vector3 original_origin_;

			double resolution_;

			/** if set to true, ligand candidate poses which have atoms outside of the grid boundaries are not allowed. Thus the respective pose is rejected without further score calculations as soon as the atom violating this restriction is found */
			bool enforce_grid_boundaries_;

			/** value to be added to the calculated score for each ligand atom that is located outside of this GridSet */
			double out_of_grid_penalty_;

			/** scales the calculated score by a given factor if the interactions/atom for the current ligand are different from reference_interactions \n
			 == > deep burial into the binding pocket results in a better score */

			/** determines whether this ScoreGridSet is currently active and should be takeb into account when calculating scores */
			bool enabled_;

			std::vector<ScoreGrid*>* score_grids_;

			Size size_x;

			Size size_y;

			Size size_z;

			/** has this ScoreGridSet already been transformed? */
			bool transformed_;

			void initializeEmptyGrids(int no = -1);

			/** pointer to the GridBasedScoring object that created the object of this class */
			GridBasedScoring* parent;

			/** was a new HashGrid creates for this GridSet or is the HashGrid of parent used?? */
			bool new_hashgrid_;

			/** transformation matrix that describes the orientation of this grid set according to the global coordinate system */
			TMatrix4x4<float> T_;

			/** the inverse of T_ . It is used to transform positions of ligand atoms back to the original localtion&orientation of the ScoreGridSet, since the ScoreGridSet is never _explicitly_ rotated (for reasons of time complexity) */
			TMatrix4x4<float> T_i_;

			HashGrid3<Atom*>* hashgrid_;

			/** will ONLY be used if this object is not connencted to a parent. Thus, always use getAtomTypesMap() to obtain the correct map */
			std::map<BALL::String, int> atom_types_map_no_parent_;

			PharmacophoreConstraint* pharm_constraint_;

			friend class GridBasedScoring;
	};
}

#endif // BALL_SCORING_COMMON_SCOREGRIDSET
