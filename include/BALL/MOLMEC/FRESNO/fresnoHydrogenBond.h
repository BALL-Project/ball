// $Id: fresnoHydrogenBond.h,v 1.1.2.1 2002/02/14 17:02:48 anker Exp $
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
		FresnoHydrogenBond(const FresnoHydrogenBond& fhb, bool clone_deep = true)
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

		/*_
		*/
		::vector< pair<const Atom*, const Atom*> > possible_hydrogen_bonds_;

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
