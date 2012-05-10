// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scoringFunction.h,v 1.2 2006/05/21 18:15:28 anker Exp $

#ifndef BALL_SCORING_COMMON_SCORINGFUNCTION_H
#define BALL_SCORING_COMMON_SCORINGFUNCTION_H

#include <set>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/DOCKING/COMMON/staticLigandFragment.h>
#include <BALL/DOCKING/COMMON/constraints.h>
#include <BALL/SCORING/COMMON/baseFunction.h>
#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/STRUCTURE/residueRotamerSet.h>

namespace BALL
{
	typedef std::list<std::pair<Atom*, Atom*> > AtomPairList;
	typedef std::vector<std::pair<Atom*, Atom*> > AtomPairVector;

	class ScoringComponent;

	class BALL_EXPORT ScoringFunction
	{

		friend class PharmacophoreConstraint;
		friend class SideChainOptimizer;

		public:

		struct Option
		{

			/**
			*/
			static const char* VERBOSITY;

			/**
			*/
			static const char* BASE_FUNCTION_TYPE;

			/**
			*/
			static const char* HASHGRID_RESOLUTION;

			/**
			*/
			static const char* HASHGRID_SIZE;

			/**
			*/
			static const char* NONBONDED_CUTOFF;

			/**
			*/
			static const char* CONFORMATION_SCALE;

			/**
			*/
			static const char* ALL_LIG_NONB_PAIRS;

			/**
			*/
			static const char* USE_STATIC_LIG_FRAGMENTS;

			/**
			*/
			static const char* IGNORE_H_CLASHES;

			/**
			*/
			static const char* ALLOWED_INTERMOL_OVERLAP;

			/**
			*/
			static const char* ALLOWED_INTRAMOL_OVERLAP;

			/**
			*/
			static const char* BURIAL_DEPTH_SCALE;

		};


		struct Default
		{

			/**
			*/
			static const Size VERBOSITY;

			/**
			*/
			static const Size BASE_FUNCTION_TYPE;

			/** width of one HashGridBox in Angstroem */
			static double HASHGRID_RESOLUTION;

			/** the size of the HashGridBox in units of boxes per axis */
			static int HASHGRID_SIZE;

			static double NONBONDED_CUTOFF;

			/** allows scaling down the contribution of the conformational energy of the ligand to the overall score */
			static double CONFORMATION_SCALE;

			/** if set to true, all ligand nonbonded pairs are used to asses the conformational energy of the ligand. This way much time can be saved (for small+medium sized ligands) because the pairlist of the nonbonded comformation component will only be calculated _once_ for each ligand candidate. */
			static bool ALL_LIG_NONB_PAIRS;

			/** if set to true, StaticLigandFragments are calculated for each ligand candidate and the score for all nonbonded pairs _within_ one StaticLigandFragment is calculated ony _once_ for each ligand candidate. This way time can be saved if the static fragments of the ligands are not modified. */
			static bool USE_STATIC_LIG_FRAGMENTS;

			/** determines whether an overlap between two Hydrogen atoms should be considered a sterical clash */
			static bool IGNORE_H_CLASHES;

			/**  the allowed intermolecular overlap in Angstroem for two atoms. */
			static double ALLOWED_INTERMOL_OVERLAP;

			/**  the allowed intramolecular overlap in Angstroem for two atoms. */
			static double ALLOWED_INTRAMOL_OVERLAP;

			static int BURIAL_DEPTH_SCALE;

		};

		///
		ScoringFunction()
			;

		///
		ScoringFunction(const ScoringFunction& sf)
			;

		///
		ScoringFunction(AtomContainer& receptor, AtomContainer& ligand)
			;

		///
		ScoringFunction(AtomContainer& receptor, AtomContainer& ligand,
				const Options& options)
			;

		ScoringFunction(AtomContainer& receptor,
				AtomContainer& ligand, Options& options)
			;

		/** constructor for use without reference ligand
			@param hashgrid_origin the origin of the HashGrid that will hold receptor atoms as potential interaction partners for ligand atoms */
		ScoringFunction(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options);

		///
		virtual ~ScoringFunction()
			;

		///
		virtual void clear()
			;

		///
		ScoringFunction& operator = (const ScoringFunction& sf)
			;

		///
		bool setup()
			;

		///
		bool setup(AtomContainer& receptor, AtomContainer& ligand)
			;

		///
		bool setup(AtomContainer& receptor, AtomContainer& ligand, const Options& options)
			;

		///
		virtual bool specificSetup()
			throw(Exception::TooManyErrors);

		///
		void setMaximumNumberOfErrors(Size nr)
			;

		///
		Size getMaximumNumberOfErrors() const
			;

		///
		void setFirstMolecule(AtomContainer& molecule1)
			;

		///
		AtomContainer* getFirstMolecule() const
			;

		///
		void setSecondMolecule(AtomContainer& molecule2)
			;

		///
		AtomContainer* getSecondMolecule() const
			;

		/** Set the receptor of the complex (which is just the same as
				molecule1_).
		*/
		void setReceptor(AtomContainer& receptor)
			;

		/** Return the receptor of the complex (which is just the same as
				molecule1_).
		*/
		AtomContainer* getReceptor() const
			;

		/** Set the ligand of the complex (which is just the same as
				molecule2_).
		*/
		void setLigand(AtomContainer& ligand)
			;

		/** Return the ligand of the complex (which is just the same as
				molecule2_).
		*/
		AtomContainer* getLigand() const
			;

		///
		void setIntercept(double intercept)
			;

		///
		double getIntercept() const
			;

		/// Set the instance of BaseFunction for the scoring function.
		void setBaseFunction(ScoringBaseFunction& base_function)
			;

		/** Return a pointer to the instance of BaseFunction used by this
				scoring function.
		*/
		ScoringBaseFunction* getBaseFunction() const
			;

		///
		void insertComponent(ScoringComponent* component, float coefficient = 1.0f)
			;

		/** Remove a component from the component list of this scoring
				function and delete it from memory. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no component removed.
		*/
		void removeComponent(const ScoringComponent* component)
			;

		/** Remove a component from the component list of this scoring
				function and delete it from memory. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no component removed. Only the first occurrence of a name
				in the component list will be removed.
		*/
		void removeComponent(const String& name)
			;

		/** Set the coefficient of a component. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no coefficient set.
		*/
		void setCoefficient(const ScoringComponent* component, float coefficient)
			;

		/** Set the coefficient of a named component. If there was no matching
				component, do nothing. Note that the user will get no feedback if
				there was no coefficient set. Only the first occurrence of a name
				in the component list will be changed.
		*/
		void setCoefficient(const String& name, float coefficient)
			;

		/** Get the coefficient of a component. If there was no matching
				component, return false, ow return true.
		*/
		bool getCoefficient(const ScoringComponent* component,
				float& coefficient) const
			;

		/** Get the coefficient of a component. If there was no matching
				component, return false, ow return true.
		*/
		bool getCoefficient(const String& name, float& coefficient) const
			;

		/// Return a pointer to a component in the components vector.
		ScoringComponent* getComponent(const String& name) const
			;

		/// Return a pointer to a component in the components vector.
		ScoringComponent* getComponent(const Size index) const
			;

		///
		double calculateScore()
			;

		///
		const HashSet<const Atom*>& getUnassignedAtoms() const
			;

		///
		HashSet<const Atom*>& getUnassignedAtoms()
			;

		///
		static HashGrid3<Atom*>* initializeHashGrid(AtomContainer* sys, Vector3& center, double& resolution, int hashgrid_size)
			;

		/// Returns the standard deviation of the experimentally determined binding free energy of the used training data set
		double getExpEnergyStddev()
			;

		void enableStoreInteractionsOnlyForPhContraints()
			;

		/// converts the given seconds (e.g. result of BALL::Timer::getClockTime()) into the appropriate unit and output a String containing value and unit
		String convertTime(double seconds)
			;

		/// update all nonbonded pair-lists
		virtual void update()
			;

		/// calculates the score for the current ligand pose
		virtual double updateScore()
			;

		/** prints an explanation of the score as obtained by the last call of updateScore() to stdout
			@param detail if set to true, information about the inidividual score-contributions is printed as well as the overview */
		virtual void printResult(bool detail = 0)
			;

		void getScoreContributions(vector<double>& score_contributions, vector<String>& names)
			;

		/// stores the default options of this ScoringFunction in the given Option object
		static void getDefaultOptions(Options& options)
			;

		/// Returns a string containing the equation of this ScoringFunction (as sum of over all ScoringComponents)
		String getEquation()
			;

		/** Determines whether interactions calculated by this ScoringFunction should be saved to each ligand atom a in a->interactions.
			By default storing of interactions is disabled for new ScoringFunctions.  */
		void enableStoreInteractions(bool b = true)
			;

		/// Return the name of this ScoringFunction
		String getName()
			;

		int getBurialDepthScale()
			;

		String valueToString(double value)
			;

		/// Sets the coefficients of all ScoringComponents to 1 and unsets all transformations. This is necessay if obtained scores are to be used for an optimization via regression.
		void unsetTrainingParameters()
			;

		/// Set mean and standard deviation of the experimentally determined binding free energy of the used training data set
		void setNormalizationParameters(double stddev, double mean)
			;

		const HashGrid3<Atom*>* getHashGrid()
			;

		/// returns the geometrical center of the current ligand candidate
		const Vector3& getLigandCenter() const
			;

		double getLigandRadius() const
			;

		/** Tells whether interactions calculated by this ScoringFunction should be saved to each ligand atom a in a->interactions.
			By default storing of interactions is disabled for new ScoringFunctions. */
		bool storeInteractionsEnabled()
			;

		/// Returns the average number of receptor atoms neighboring each ligand atom as determined by the last call of update()
		virtual int getNoNeighboringReceptorAtoms()
			;

		/// return the number of atoms of the current ligand
		int getNoLigandAtoms()
			;

		Options getOptions()
			;

		/// creates StaticLigandFragments for the current ligand candidate.
		/// After this function has been called once for the current ligand candidate, conformation energy is only calculated anew between
		/// inter-fragment atom-pairs of this ligand candidate. This way, the speed of energy/score computation can be increased
		void createStaticLigandFragments()
			;

		const vector<Bond*>* getRotatableLigandBonds() const
			;

		const vector<StaticLigandFragment*>* getStaticLigandFragments() const
			;

		bool hasFlexibleResidues()
			;

		void setFlexibleResidues(const std::set<Residue*>& flexible_residues)
			;

		/** Assign the given rotamer to the specified residue.\n
			After applying the new rotamer, it is checked whether it overlaps with any other receptor residues. If this is the case, the previously used rotamer will re-applied and zero will be returned. Else, one will be returned by this function. \n
			The hash_grid_ of this ScoringFunction that keeps track of all receptor atoms will be automatically updated. */
		bool assignRotamer(Residue* residue, ResidueRotamerSet* rotamer_set, const Rotamer* rotamer)
			;

		/** Resets flexible residues to the conformations observed in the input pdb-file.\n
		This does only have any effect if optimization of flexible residues has been used before the call of this function. */
		void resetFlexibleResidues();

		/** Count the number of receptor atoms that have a distance <= distance_threshold to the specified atom
			@param onePerCell if set to true, only one receptor atom within each hashgrid cell will be taken into account, i.e. the number of neighboring nonempty cells is counted. */
		Size countNeighboringReceptorAtoms(const Atom* atom, double distance_threshold, bool onePerCell = 0, int* number_of_overlaps = 0) const
			;

		void resetResiduePositions(Residue* residue, list<Vector3>& old_positions)
			;

		// return the previously calculated score
		double getScore()
			;

		const map<Atom*, int>* getAtomFragmentMap()
			;

		/** list holding all ReferenceAreas of the ScoringFunction. \n
			ReferenceAreas can be added/deleted/modified directly by the user and will be used during all future calls of updateScore() */
		list<Constraint*> constraints;


		protected:

		/* @name Protected structs */
		struct Result
		{
			/** score for the conformation of the current ligand */
			double ligand_conformation;

			/** score for interaction between ligand and receptor */
			double interaction_score;

			/** one penalty value for each ReferenceArea */
			vector<double> refArea_penalties;

			/** final score value of the current pose, as determined by the last call of updateScore() */
			//double score;
		};

		/** set the name of this ScoringFunction */
		void setName(String name);

		/** initializes variables to take default values */
		//void setDefaults();

		void updateComponent(int id, AtomPairList*);

		/** calculates the geometrical center of the given structure
		@param no_ligand_atoms if specified, the number of atoms of the current ligand candidate is saved here */
		Vector3 calculateGeometricalCenter(AtomContainer* s, int* no_ligand_atoms = NULL);

		/** calculates the radius of a given structure, i.e. the maximal distance from its geometrical center */
		double calculateMoleculeRadius(AtomContainer* sys, Vector3& center);

		void setupFlexibleResidues_();

		/** use the current ligand as reference ligand. Thus, the average number of neighboring target atoms per reference ligand atom is calculated and can later be compared to the value for a ligand candidate, so that its depth of burial is thereby estimated. */
		virtual void setupReferenceLigand();

		/** Check how many of the atom pairs in the given AtomPairVector clash with each other. */
		int checkForAtomOverlaps(const AtomPairVector* pair_vector);

		/** creates a vector of pairs of ligand atoms and receptor atoms (or atoms of additional systems, e.g. water molecules). \n
		The structure to which the interactions of the current ligand are to be search is represented by the given HashGrid. \n
		The vector of nonbonded atom pairs created by this function is to be used to update the ScoringComponents
		@param rec_lig determines whether interactions between ligand and receptor are sought. If this is the case, the checks that make sure that two atoms chosen as an atompair are no neighbors and are not identical can be skipped. This way, some time can be saved.
		@param check_fragment if set to true it is is checked whether two atoms are not part of the same StaticLigandFragment
		@param intra_fragment if set to true (and check_fragment == 1), nonbonded interactions between atoms of the _same_ fragment are searched instead. \n
		This is used to calculated the energy of all StaticLigandFragments once for each ligand candidate.
		@param overlaps  the number of atom overlaps will be added to this value  */
		AtomPairVector* createNonbondedPairVector(HashGrid3<Atom*>* hashgrid, int& overlaps, bool rec_lig, bool check_fragments = 0, bool intra_fragment = 0);

		/** creates nonbonded pairs of ligand atoms. \n
		If StaticLigandFragments are used, only atoms that are part of different StaticLigandFragments are searched. This way, time can be saved during updating the ForceFields, since the score/energy _within_ each StaticLigandFragments will not change
		@param intra_fragment if set to true (and check_fragment == 1), nonbonded interactions between atoms of the _same_ fragment are searched instead. \n
		This is used to calculated the energy of all StaticLigandFragments once for each ligand candidate.
		@param overlaps the number of atom overlaps will be added to this value */
		AtomPairVector* createLigandNonbondedPairVector(bool intra_fragment, int& overlaps);

		bool hasPharmacophoreConstraints_();

		/** clears the MolecularInteractions saved for each ligand- and receptor-atom */
		void clearStoredInteractions_();

		/** calculates penalty-scores for all defined Contraints and returns the sum. */
		double calculateConstraintsScore();

		/** Enabled the ScoringComponents of the given types and disables all other ScoringComponents. Components that evaluate the intramolecular energy of the ligand will be disabled. */
		void enableInteractionComponents_(const list<String>& type_names);

		virtual double getES() = 0;

		/** Enables all ScoringComponents */
		void enableAllComponents_();

		/** fetches the StaticLigandFragments starting with a1. \n
			This function is used by createStaticLigandFragments(), so that is does not need to be called directly
			@param already_processed_atoms a temporary hashmap that will store already processed atoms */
		void fetchStaticLigandFragment(Atom* a1, int index);

		/** Returns the number of covalent bonds of the given atom
			@param threshold if specified, it is only checked whether there are at least so many covalent bonds. */
		int countCovalentBonds(const Atom* atom, int threshold = -1);

		/** calculates _all_ nonbonded pairs of ligand atoms, i.e. _without_ using a cutoff. This way much time can be saved (for small+medium sized ligands) because the pairlist of the nonbonded comformation component will not be changed by future calls of update() for this ligand */
		virtual void createAllLigandNonBondedPairs();

		/** calculates the energy within the StaticLigandFragments only!\n
			This needs to be done only once for each ligand candidate */
		virtual double calculateStaticLigandFragmentEnergy();

		/** Determines whether the given bond is a peptide bond and thereby not rotatable */
		bool isPeptideBond(const Bond* bond) const;

		/** ScoringFunction options */
		Options options_;

		/*_ The second molecule of the complex, being the ligand in
				receptor/ligand complexes
			*/
		AtomContainer* receptor_;

		/*_ The first molecule of the complex, being the receptor in
				receptor/ligand complexes
			*/
		AtomContainer* ligand_;

		//_ The name of the scoring function
		String name_;

		//_ The overall score of the scoring function
		double score_;

		//_ The intercept necessary for calculating the score
		double intercept_;

		/*_ The base funcion for scoring simple terms
		*/
		ScoringBaseFunction* base_function_;

		/*_ A list of components comprising the scoring function, along with
				the coefficients for each term necessary for calculating the final
				score.
		*/
		std::vector< std::pair<ScoringComponent*, float> > components_;

		//_ Atoms, for which the setup of the force field fails
		HashSet<const Atom*> unassigned_atoms_;

		//_ Max number of unassigned atoms
		Size max_number_of_errors_;

		//_ Actual number of countde errors
		Size number_of_errors_;

		/** the radius of the current ligand, i.e. the maximal distance from its geometrical center */
		double ligand_radius_;

		/** the number of atoms of the current ligand candidate. Is set by each call of calculateGeometricalCenter() */
		int ligand_atoms_;

		/** the geometrical center of the current ligand */
		Vector3 ligand_center_;

		/** HashGrid used to find potential interaction partners for a ligand atom */
		HashGrid3<Atom*>* hashgrid_;

		HashGrid3<Atom*>* all_residues_hashgrid_;
		HashGrid3<Atom*>* static_residues_hashgrid_;
		HashGrid3<Atom*>* flexible_residues_hashgrid_;

		/** the resolution that was specified by use of the Options object in the constructor */
		double resolution_;

		/** Maps each ligand atom to its StaticLigandFragments (if StaticLigandFragments have been calculated) */
		map<Atom*, int> atoms_to_fragments_;

		/** the average number of interactions per atom of a reference ligand within a small radius. This is used to estimate the depth of burial of the ligand (candidate) */
		int reference_neighbors_;

		/** the average number of target atoms within a distance of 'neighbor_cutoff' Angstroem of a ligand atom. \n
			the value is calculated during each call of createNonBondedPairList() */
		int neighboring_target_atoms_;

		/** the number of ligand atoms that were found to be lying outside of hash_grid_, as determined by the last call of createNonBondedPairList() */
		int misplaced_ligand_atoms_;

		/** the number of boxes around a ligand atom that are to be searched */
		int hashgrid_search_radius_;

		/** cutoff value for nonbonded energy */
		double nonbonded_cutoff_;

		/** squared cutoff value for nonbonded energy */
		double nonbonded_cutoff_2_;

		/** determines whether an overlap between two Hydrogen atoms should be considered a sterical clash */
		bool ignore_h_clashes_;

		/** the number of overlapping receptor-ligand atom pairs as determined by the last call of update() */
		int overlaps_;

		/** the number of overlapping ligand atom pairs as determined by the last call of update() */
		int ligand_intramol_overlaps_;

		/**  the allowed overlap in Angstroem for two atoms of two different molecules (used since even in many crystal structures, some atoms overlap minimally). */
		double allowed_intermolecular_overlap_;

		/**  the allowed overlap in Angstroem for two atoms of the same molecule (used since even in many crystal structures, some atoms overlap minimally). */
		double allowed_intramolecular_overlap_;

		/** the number of target atoms within this squared distance of ligand atoms are used to approximate the depth of burial of the ligand */
		double neighbor_cutoff_2_;

		/** contains the static fragments of the current ligand candidate.\n
		The conformation of each ligand is not changed during docking; only the position of fragments in relation to each other is modified. \n
		Thus, if static_ligand_fragments_ is not empty, createNonBondedPairList() will automatically search only for inter-fragment atom-pairs and ligand-receptor pairs, but not for intra-fragment pairs. */
		vector<StaticLigandFragment*> static_ligand_fragments_;

		vector<ScoringComponent*> scoring_components_;

		/** Determines whether interactions calculated by this ScoringFunction should be saved to each ligand atom a in a->interactions.\n
			By default this is disabled for new ScoringFunctions. */
		bool store_interactions_;

		bool store_interactions_phC_only_;

		/** all nonbonded pairs of the current ligand. Calculated without cutoff by createAllLigandNonBondedPairs() */
		AtomPairVector* all_ligand_nonbonded_;

		/** allows scaling down the contribution of the conformational energy of the ligand to the overall score */
		double conformation_scale_;

		/** see Default::ALL_LIG_NONB_PAIRS */
		bool use_all_lig_nonb_;

		/** see Default::USE_STATIC_LIG_FRAGMENTS */
		bool use_static_lig_fragments_;

		int burial_depth_scale_;

		/** the standard deviation of the experimentally determined binding free energy of the used training data set */
		double exp_energy_stddev_;

		/** the mean of the experimentally determined binding free energy of the used training data set */
		double exp_energy_mean_;

		set<Residue*> flexible_residues_;

		/** saves the final and all intermediate results of the last call of updateScore() */
		Result result_;

		/** the energy of the StaticLigandFragments. Is calculated once for each ligand candidate by createStaticLigandFragments() */
		double static_ligand_energy_;

		/** Rotatable bonds of the current ligand molecule. They are calculated by the function createStaticLigandFragments() */
		vector<Bond*> rotatable_ligand_bonds_;

		/** The original positions of all atoms of all flexible residues, i.e. as they appear in the input receptor object */
		list<list<Vector3> > flexres_org_positions_;

	};

} // namespace BALL

#endif // BALL_SCORING_COMMON_SCORINGFUNCTION_H
