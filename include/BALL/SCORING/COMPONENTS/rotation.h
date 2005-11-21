// $Id: rotation.h,v 1.1 2005/11/21 19:27:06 anker Exp $
// Molecular Mechanics: SLICK force field, rotational entropy loss

#ifndef BALL_MOLMEC_SLICK_SLICKROTATION_H
#define BALL_MOLMEC_SLICK_SLICKROTATION_H

#include <stack>

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/DATATYPE/hashGrid.h>

namespace BALL
{

	/** SLICK rotational entropy loss contrbution
			{\bf Definition:} \URL{BALL/MOLMEC/SLICK/slickRotation.h}
	*/
	class SlickRotation
		:	public ForceFieldComponent
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

			/// 
		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		SlickRotation()
			throw();

		/**	Constructor.
		*/
		SlickRotation(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		SlickRotation(const SlickRotation& fr)
			throw();

		/**	Destructor.
		*/
		virtual ~SlickRotation()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const SlickRotation& operator = (const SlickRotation& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const SlickRotation& fr) const
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
		virtual double updateEnergy()
			throw();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces()
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
		double factor_;

		/*_
		 */
		float bind_distance_offset_;

		/*_
		*/
		Size calculation_method_;

		/*_ The fresno atom types that are stored in the fresno force field
		*/
		const HashMap<const Atom*, short>* fresno_types_;

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
