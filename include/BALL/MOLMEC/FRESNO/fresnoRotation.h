// $Id: fresnoRotation.h,v 1.1.2.1 2002/02/14 17:02:48 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#ifndef BALL_MOLMEC_FRESNO_FRESNOROTATION_H
#define BALL_MOLMEC_FRESNO_FRESNOROTATION_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/DATATYPE/hashGrid.h>

namespace BALL
{

	/** Fresno lipophilic component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoRotation.h}
	*/
	class FresnoRotation
		:	public ForceFieldComponent
	{

		public:

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoRotation()
			throw();

		/**	Constructor.
		*/
		FresnoRotation(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoRotation(const FresnoRotation& fr, bool clone_deep = true)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoRotation()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoRotation& operator = (const FresnoRotation& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoRotation& fr) const
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
		::vector<const Bond*> rotatable_bonds_;

		/*_ The number of rotatable bonds (i. e. rotatable_bonds_.size()).
		*/
		Size N_rot_;

		/*_ A vector iof bool indicating which of rotatable bond was frozen upon
		 * binding.
		*/
		::vector<bool> frozen_bonds_;

		/* The percentages of nonlipophilc heavy atoms on each side of the bond
		 * (named P(r) in the paper).
		*/
		::vector< ::pair<double, double> > heavy_atom_fractions_;

		/*_ This hash grid contains all receptor atoms. We need it for obtaining
		 * those ligand atoms that are close to the receptor.
		*/
		HashGrid3<const Atom*>* receptor_grid_;

		/*_ The spacing of the receptor_grid_.
		*/
		float receptor_grid_spacing_;

		/*_ The fresno atom types that are stored in the fresno force field
		*/
		const HashMap<const Atom*, short>* fresno_types_;

		/*_
		*/
		bool ringDFS_(const Atom& atom, const Atom& first_atom,
				const Size limit, HashSet<const Bond*>& visited) const
			throw();

		/*_
		*/
		void findDFS_(const Atom* atom, const Bond* bond, 
				HashSet<const Atom*>& visited, double& min_distance) const
			throw();

		/*_
		*/
		void countDFS_(const Atom* atom, const Bond* bond, 
				HashSet<const Atom*>& visited, Size& heavy_count) const
			throw();

		/*_
		*/
		Size countHeavyAtoms_(const Atom* atom, const Bond* bond) const
			throw();
	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNOROTATION_H
