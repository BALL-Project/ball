// $Id: fresnoLipophilic.h,v 1.1.2.1 2002/02/14 17:02:48 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#ifndef BALL_MOLMEC_FRESNO_FRESNOLIPOPHILIC_H
#define BALL_MOLMEC_FRESNO_FRESNOLIPOPHILIC_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL
{

	/** Fresno lipophilic component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoLipophilic.h}
	*/
	class FresnoLipophilic
		:	public ForceFieldComponent
	{

		public:

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoLipophilic()
			throw();

		/**	Constructor.
		*/
		FresnoLipophilic(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoLipophilic(const FresnoLipophilic& fhb, bool clone_deep = true)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoLipophilic()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoLipophilic& operator = (const FresnoLipophilic& fhb)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoLipophilic& fhb) const
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
		::vector< pair<const Atom*, const Atom*> > possible_lipophilic_interactions_;

		/*_ This length will be added to the sum of the van-der-Waals radii for
				obtaining the lower bound of the scoring function.
		*/
		double add_to_radii_;

		/*_ The upper bound for the scoring function is obtained by adding a
				constant to the lower bound.
		*/
		double add_to_lower_bound_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNOLIPOPHILIC_H
