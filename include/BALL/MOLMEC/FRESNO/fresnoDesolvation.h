// $Id: fresnoDesolvation.h,v 1.1.2.1 2002/02/14 17:02:48 anker Exp $
// Molecular Mechanics: Fresno force field, hydrogen bond component

#ifndef BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H
#define BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H

#include <BALL/SOLVATION/poissonBoltzmann.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL
{

	/** Fresno Hydrogen Bond component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoDesolvation.h}
	*/
	class FresnoDesolvation
		:	public ForceFieldComponent
	{

		public:

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoDesolvation()
			throw();

		/**	Constructor.
		*/
		FresnoDesolvation(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoDesolvation(const FresnoDesolvation& fhb, bool clone_deep = true)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoDesolvation()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoDesolvation& operator = (const FresnoDesolvation& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoDesolvation& fr) const
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

		/*_ The Poisson-Boltzmann equation solver needed for calculating the
		 * reaction field energy.
		*/
		FDPB fdpb_;

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNODESOLVATION_H
