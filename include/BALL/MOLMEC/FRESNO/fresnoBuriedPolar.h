// $Id: fresnoBuriedPolar.h,v 1.1.2.3 2003/05/07 16:10:39 anker Exp $
// Molecular Mechanics: Fresno force field, lipophilic component

#ifndef BALL_MOLMEC_FRESNO_FRESNOBURIEDPOLAR_H
#define BALL_MOLMEC_FRESNO_FRESNOBURIEDPOLAR_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL
{

	/** Fresno lipophilic component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoBuriedPolar.h}
	*/
	class FresnoBuriedPolar
		:	public ForceFieldComponent
	{

		public:

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoBuriedPolar()
			throw();

		/**	Constructor.
		*/
		FresnoBuriedPolar(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoBuriedPolar(const FresnoBuriedPolar& fhb)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoBuriedPolar()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoBuriedPolar& operator = (const FresnoBuriedPolar& fhb)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoBuriedPolar& fhb) const
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
		::vector< pair<const Atom*, const Atom*> > possible_buried_polar_interactions_;

		/*_
		*/
		double factor_;

		/*_ This length will be added to the sum of the van-der-Waals radii for
				obtaining the lower bound of the scoring function.
		*/
		double r1_offset_;

		/*_ The upper bound for the scoring function is obtained by adding a
				constant to the lower bound.
		*/
		double r2_offset_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNOBURIEDPOLAR_H
