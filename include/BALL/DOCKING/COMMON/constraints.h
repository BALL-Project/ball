// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_COMMON_CONSTRAINTS
#define BALL_DOCKING_COMMON_CONSTRAINTS


#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_MATHS_PLANE3_H
#include <BALL/MATHS/plane3.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#include <BALL/DATATYPE/hashGrid.h>
#endif


namespace BALL
{
	class DockingAlgorithm;

	namespace VIEW
	{
		class DockRLDialog;
	}

	class ScoringFunction;

	class BALL_EXPORT Constraint
	{
		public:
			Constraint();

			virtual ~Constraint();

			virtual double calculateScore(AtomContainer* mol) = 0;

			/** Set a name for this ReferenceArea. This way, the score summary printed by the ScoringFunction can be analyzed more easily */
			virtual void setName(String name);

			String getName();

			String getType();

			virtual void setScoringFunction(ScoringFunction* sf);

		protected:
			String name_;

			/** Stores the type - 'ReferenceArea' or 'PharmacophoreConstraint' - of this Constraint for text output purposes. */
			String type_;

			/** penalty value that is to be added to the calculated energy if the desired constraints are not fullfilled. */
			double penalty_;
			ScoringFunction* scoring_function_;
	};

	/** ReferenceAreas are cuboids specified by the user for the examined target structure. A given number of ligand atoms or a given fraction of the ligand atoms should be located within these areas, so that docking is optimized in this way. */
	class BALL_EXPORT ReferenceArea
		: public Constraint
	{
		public:
			ReferenceArea();

			~ReferenceArea();
			/** Constructor. Creates a ReferenceArea from 4 given points, that describe a cuboid.\n
			Of those four given points, p1 should be located on the same edge as p2, p3 on the same egde as p2 and p4 on the same edge as p3.  */
			ReferenceArea(Vector3& p1, Vector3& p2,  Vector3& p3, Vector3& p4, bool atom_fraction, double atoms, double penalty);

			/** Constructor. Creates a ReferenceArea from 2 points that describe one side of the cuboid and the the desired expansion of the cubiod along the x- and y-axes of a right handed coordinate system (after transformation) */
			ReferenceArea(Vector3& p1, Vector3& p2, int y_expansion, int z_expansion, bool atom_fraction, double atoms, double penalty);

			/** Constructor that creates a ReferenceArea describing the area occupied by a given AtomContainer */
			ReferenceArea(const AtomContainer* sys, bool atom_fraction, double atoms, double penalty);

			ReferenceArea(list<const AtomContainer*>& container_list, const list<String>& interaction_types, double desired_interaction_score, double penalt);

			/** calculates a penalty for the given AtomContainer */
			double calculateScore(AtomContainer* mol);

			/** returns the number of atoms located within this ReferenceAreas as calculated by the last call of getScore() (respectively ScoringFunction::updateScore() )  */
			double getContainedAtoms();

			/** Creates a molecule with four atoms located on the bonders of the cube that this ReferenceArea describes.\n
			This can be used for visualization purposes.\n
			@return the created System if this ReferenceArea contains a valid ref-area description; else NULL is returned. */
			System* createBoundaryMolecule();

			/** Enlarge this ReferenceArea by the given amount in all directions */
			void enlarge(double angstroem);

			/** Sets the number resp. fraction of atoms of a ligand that should be contained in this ReferenceArea */
			void setNumberDesiredAtoms(double d);

			void setName(String name);

			void setScoringFunction(ScoringFunction* sf);

		private:
			double countContainedAtoms(AtomContainer* sys);

			/** transformation matrix. It is used to rotate the specified cuboid in such a way that it is aligned with the global coordinate system */
			Matrix4x4 T_;

			double min_x_, max_x_, min_y_, max_y_, min_z_, max_z_;

			/** the number of atoms of a reference ligand within this ReferenceArea respectively the fractions of atoms of a reference ligand within this ReferenceArea */
			double ref_atoms_;

			/** if == 1, ReferenceArea.atoms indicates the desired fraction of ligand atoms that should be located within this area */
			bool atom_fraction_;

			/** the number of atoms located within this ReferenceAreas as calculated by the last call of getScore() (respectively ScoringFunction::updateScore() )  */
			double contained_atoms_;

			vector<Vector3> input_points_;

			/** the number of atoms of the reference ligand */
			Size number_reflig_atoms_;

			/** If set to true, the fraction of ligand-atoms contained within this ReferenceArea is calculated with respect to the number of atoms of the reference-ligand, if the current ligand has more atoms than the reference ligand.
			This is useful, because a much larger ligand obviously needs more space and might therefore not fit completely into the binding pocket (as the reference ligand did). */
			bool scale_by_size_;

			friend class BALL::DockingAlgorithm;
			friend class BALL::VIEW::DockRLDialog;
	};

	/** PharmacophoreConstraint allow to define specific intermolecular interactions that should exist between receptor and ligand.\n
	If those interactions do not exist or are too weak (i.e. have a score that is not good enough), a penalty score is calculated and used by ScoringFunction::updateScore() if this PharmacophoreConstraint has been added to ScoringFunction::constraints.\n
	In order to be able to evaluate the ligand's interactions with the specified atoms, the pairwise interactions have to be stored. Use ScoringFunction::enableStoreInteractions() to allow this. */
	class BALL_EXPORT PharmacophoreConstraint
		: public Constraint
	{
		public:
			PharmacophoreConstraint(list<const AtomContainer*>& container_list, const list<String>& interaction_types, double desired_interaction_score, double penalty);

			PharmacophoreConstraint(const AtomContainer* container, const list<String>& interaction_types, double desired_interaction_score, double penalty);

			PharmacophoreConstraint(const PharmacophoreConstraint& phC);

			~PharmacophoreConstraint();

			/** Constructor for initializing a PharmacophoreConstraint with a list of residue-IDs. \n
			If using this constructor, the ScoringFunction for this object must be set by setScoringFunction() _before_ calling calculateScore() for the first time. If the receptor that is used by this ScoringFunction is not a Protein or does not have residues for all the specified IDs, setScoringFunction() will throw an exception. */
			PharmacophoreConstraint(vector<String>& residue_IDs, const list<String>& interaction_types, double desired_interaction_score, double penalty);

			void setScoringFunction(ScoringFunction* sf);

			/** calculates a penalty for the given AtomContainer */
			double calculateScore(AtomContainer* mol);

			/** returns the interaction-score between the ligand and the desired receptor-atoms as calculated during the last call of calculateScore() */
			double getInteractionScore();

			bool usesReceptorResidues();

			AtomContainer* getGridContainer();

			const list<String>* getInteractionTypes();

			const list<const AtomContainer*>* getInteractionPartners();

		private:
			/** only used in case of creation of an object of this class by constructor PharmacophoreConstraint(vector<String>& residue_IDs ...) */
			vector<String> residue_IDs_;
			vector<String> chain_IDs_;

			list<const AtomContainer*> interaction_partners_;
			list<String> interaction_types_;
			double desired_interaction_score_;
			double interaction_score_;
			bool uses_receptor_residues_;

			/** Contains copies of all relevant receptor-atoms in case of grid-based ScoringFunctions */
			AtomContainer grid_container_;

			/** Saves the ID of the relevant ScoreGridSet in case of grid-based ScoringFunctions */
			unsigned int gridSetID_;

			friend class BALL::DockingAlgorithm;
			friend class BALL::VIEW::DockRLDialog;
	};
}

#endif // BALL_DOCKING_COMMON_CONSTRAINTS
