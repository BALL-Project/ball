// $Id: fresnoHydrogenBond.h,v 1.1.2.4 2003/05/07 16:10:39 anker Exp $
// Molecular Mechanics: Fresno force field, hydrogen bond component

#ifndef BALL_MOLMEC_FRESNO_FRESNOHYDROGENBOND_H
#define BALL_MOLMEC_FRESNO_FRESNOHYDROGENBOND_H

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

namespace BALL
{

	/** Fresno Hydrogen Bond component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoHydrogenBond.h}
	*/
	class FresnoHydrogenBond
		:	public ForceFieldComponent
	{

		public:

		BALL_CREATE(FresnoHydrogenBond)

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoHydrogenBond()
			throw();

		/**	Constructor.
		*/
		FresnoHydrogenBond(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoHydrogenBond(const FresnoHydrogenBond& fhb)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoHydrogenBond()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoHydrogenBond& operator = (const FresnoHydrogenBond& fhb)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoHydrogenBond& fhb) const
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

		/*_ This vector contains all possible hydrogen bonds. It is created
		 * during the setup process. Pairs are always of the form (hydrogen,
		 * acceptor). The donor can be found easily by following the only bond
		 * of the hydrogen.
		*/
		::vector< pair<const Atom*, const Atom*> > possible_hydrogen_bonds_;

		/*_ A hash map containing all hydrogens and bools indicating whether
		 * this hydrigen was already used for scoring a hydrogen bond. At the
		 * moment we use the first hydrogen bond we find and hope that this
		 * will indeed be the only one contributing to the energy score. This
		 * HashMap will only be used by updateEnergy() but we build it in
		 * setup().
		*/
		HashMap<const Atom*, bool> already_used_;

		/*_
		*/
		double factor_;

		/*_
		*/
		double h_bond_distance_lower_;

		/*_
		*/
		double h_bond_distance_upper_;

		/*_
		*/
		double h_bond_angle_lower_;

		/*_
		*/
		double h_bond_angle_upper_;

		/*_ The ideal hbond length.
		*/
		double ideal_hbond_length_;

		/*_ The ideal hbond angle.
		*/
		double ideal_hbond_angle_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNOHYDROGENBOND_H
