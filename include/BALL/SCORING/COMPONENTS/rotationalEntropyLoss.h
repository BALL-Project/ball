// $Id: rotationalEntropyLoss.h,v 1.2 2006/05/21 18:15:28 anker Exp $
// Molecular Mechanics: SLICK force field, rotational entropy loss

#ifndef BALL_MOLMEC_SLICK_SLICKROTATION_H
#define BALL_MOLMEC_SLICK_SLICKROTATION_H

#include <stack>

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/DATATYPE/hashGrid.h>

namespace BALL
{

	/** SLICK rotational entropy loss contrbution
			{\bf Definition:} \URL{BALL/SCORING/COMPONENTS/rotationalEntropyLoss.h}
	*/
	class RotationalEntropyLoss
		:	public ScoringComponent
	{

		public:

		enum AlgorithmType
		{
			///
			ALGORITHM__GUESS = 0,
			///
			ALGORITHM__DATABASE = 1
		};

		enum CalculationMethod
		{
			/// The original Chemscore term.
			CALCULATION__ORIGINAL = 1,

			/// The term developed by Böhm (N_rot)
			CALCULATION__BOEHM = 2,

			/// The number of glycosidic bonds
			CALCULATION__GLYCOSIDIC_BONDS = 3,

			/// The number of frozen glycosidic bonds
			CALCULATION__FROZEN_GLYCOSIDIC_BONDS = 4,

			/// The contribution of glycosidic bonds to the original term
			CALCULATION__GLYCOSIDIC_CONTRIBUTION = 5
		};

		/**	Option names
		*/
		struct Option
		{
			/**
			*/
			static const char* ROT_BIND_OFFSET;

			/**
			*/
			static const char* ROT_GRID_SPACING;

			/**
			*/
			static const char* ROT_ALGORITHM;

			/**
			*/
			static const char* ROT_METHOD;

			/**
			*/
			static const char* VERBOSITY;

		};

		/** Default values for SLICK options.
		*/
		struct Default
		{

			/**
			*/
			static const float ROT_BIND_OFFSET;

			/**
			*/
			static const float ROT_GRID_SPACING;

			/**
			*/
			static const Size ROT_ALGORITHM;

			/**
			*/
			static const Size ROT_METHOD;

			/**
			*/
			static const Size VERBOSITY;

		};

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		RotationalEntropyLoss()
			throw();

		/**	Constructor.
		*/
		RotationalEntropyLoss(ScoringFunction& sf)
			throw();

		/**	Copy constructor
		*/
		RotationalEntropyLoss(const RotationalEntropyLoss& fr)
			throw();

		/**	Destructor.
		*/
		virtual ~RotationalEntropyLoss()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const RotationalEntropyLoss& operator = (const RotationalEntropyLoss& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const RotationalEntropyLoss& fr) const
			throw();

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw();

		//@}
		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double calculateScore()
			throw();

		//@}

		private:

		/*_ All possible rotatable bonds.
		*/
		std::vector<const Bond*> rotatable_bonds_;

		/*_ All glycosidic bonds.
		*/
		HashSet<const Bond*> glycosidic_bonds_;

		/*_ The number of rotatable bonds (i. e. rotatable_bonds_.size()).
		*/
		Size N_rot_;

		/*_ A vector of bool indicating which of rotatable bond was frozen upon
		 * binding.
		*/
		std::vector<bool> is_frozen_;

		/* A flag defining which algorithm should be used for finding
		 * rotatable bonds
		*/
		Size algorithm_type_;

		/* The percentages of nonlipophilc heavy atoms on each side of the bond
		 * (named P(r) in the paper).
		*/
		std::vector< std::pair<double, double> > heavy_atom_fractions_;

		/*_ This hash grid contains all receptor atoms. We need it for obtaining
		 * those ligand atoms that are close to the receptor.
		*/
		HashGrid3<const Atom*>* grid_;

		/*_ The spacing of the grid_.
		*/
		float grid_spacing_;

		/*_
		 */
		Molecule* receptor_;

		/*_
		 */
		Molecule* ligand_;

		/*_
		 */
		float bind_distance_offset_;

		/*_
		*/
		Size calculation_method_;

		/*_ The fresno atom types that are stored in the fresno force field
		*/
		HashMap<const Atom*, Size> fresno_types_;

		/*_ A simple DFS algorithm for identifying rings (i. e. cycles) of the
				molecule.
		*/
		void cycleDFS_(const Atom* atom,
				HashSet<const Atom*>& visited,
				HashSet<const Bond*>& tree,
				std::stack<const Bond*>& possible_cycle_bonds,
				HashSet<const Bond*>& cycle_bonds,
				int& cycle_count)
			throw();


		/*_ A simple DFS implementation for counting heavy (as defined by
		 * Rognan et al.) atoms.
		*/
		void heavyAtomsDFS_(const Atom* atom, const Bond* bond,
				HashSet<const Atom*>& visited,
				int& heavy_atom_count, int& nonlip_heavy_atom_count)
			throw();
		
		/*_ Find out which bonds are still frozen
		 */
		void updateFrozenBonds_()
			throw();
		
		/*_
		 */
		bool frozenBondsDFS_(const Atom* atom,
				HashSet<const Atom*>& visited)
			throw();
	};

} // namespace BALL

#endif // BALL_MOLMEC_SLICK_SLICKROTATION_H
