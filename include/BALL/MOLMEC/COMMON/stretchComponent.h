// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_COMMON_STRETCHCOMPONENT_H
#define BALL_MOLMEC_COMMON_STRETCHCOMPONENT_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
# include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
# include <BALL/MOLMEC/PARAMETER/quadraticBondStretch.h>
#endif

namespace BALL
{
	class ForceField;

	/**
	 * A ForceField component implementing the harmonic potential used
	 * by Amber and CHARMM
	 * \ingroup MOLMEC
	 */
	class BALL_EXPORT StretchComponent : public ForceFieldComponent
	{
		public:
			BALL_CREATE(StretchComponent)

			/**
			 * Default Constructor
			 */
			StretchComponent();

			/**
			 * Detailed Constructor
			 */
			StretchComponent(ForceField& force_field);

			/**	Destructor.
			*/
			virtual ~StretchComponent();

			//@}

			//@}
			/**	@name Accessors		
			*/
			//@{

			/**
			 * @copydoc ForceFieldComponent::updateEnergy()
			 */
			virtual double updateEnergy();

			/**
			 * @copydoc ForceFieldComponent::updateForces()
			 */
			virtual void updateForces();

			//@} 

		protected:
			/*_	@name	Private Attributes	
			*/
			//_@{

			/*_
			*/
			std::vector<QuadraticBondStretch::Data> stretch_;

			/*_	The stretch parameters section
			*/
			QuadraticBondStretch  stretch_parameters_;

			//_@}
	};
} // namespace BALL

#endif // BALL_MOLMEC_COMMON_STRETCHCOMPONENT_H
