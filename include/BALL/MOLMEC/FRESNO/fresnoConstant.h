// $Id: fresnoConstant.h,v 1.1.2.1 2002/03/15 14:47:51 anker Exp $
// Molecular Mechanics: Fresno force field, additive constant

#ifndef BALL_MOLMEC_FRESNO_FRESNOCONSTANT_H
#define BALL_MOLMEC_FRESNO_FRESNOCONSTANT_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>

namespace BALL
{

	/** Fresno Hydrogen Bond component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoConstant.h}
	*/
	class FresnoConstant
		:	public ForceFieldComponent
	{

		public:

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		FresnoConstant()
			throw();

		/**	Constructor.
		*/
		FresnoConstant(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		FresnoConstant(const FresnoConstant& fhb, bool clone_deep = true)
			throw();

		/**	Destructor.
		*/
		virtual ~FresnoConstant()
			throw();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/** Assignment.
		*/
		const FresnoConstant& operator = (const FresnoConstant& fr)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Predicates.
		*/
		//@{

		bool operator == (const FresnoConstant& fr) const
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

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNOCONSTANT_H
