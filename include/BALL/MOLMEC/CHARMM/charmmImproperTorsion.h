// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// Molecular Mechanics: Charmm force field, bond stretch component

#ifndef BALL_MOLMEC_CHARMM_CHARMMIMPROPERTORSION_H
#define BALL_MOLMEC_CHARMM_CHARMMIMPROPERTORSION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H
#	include <BALL/MOLMEC/PARAMETER/quadraticImproperTorsion.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_RESIDUETORSIONS_H
#	include <BALL/MOLMEC/PARAMETER/residueTorsions.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif

namespace BALL 
{
	/**	Charmm improper torsion component.
			The CHARMM force field employs a harmonic out-of-plane potential for improper
			torsions.	An imporper torsion is defined for four atoms <i>  A </i>, <i>  B </i>, <i>  C </i>,
			and <i>  D </i> where <i>  A </i> is the central atom <i>  B </i>, <i>  C </i>, and <i>  D </i> are bound to. \par
			The out-of-plane-angle <b>  phi </b> is defined as the angle between the planes defined 
			by <i>  ABC </i> and <i>  BCD </i>. 
			The improper torsion energy $E_{imp}$ is then defined as
			$ E_{imp} = k (\theta - \theta_0)^2 $
			where $k$ and $\theta_0$ are constants read from the parameter section and depend on
			the atom types of <i>  A </i>, <i>  B </i>, <i>  C </i>, and <i>  D </i>.	 \par
			
    	\ingroup  CHARMM
	*/
	class BALL_EXPORT CharmmImproperTorsion 
		: public ForceFieldComponent
	{
		public:

		/// flag to enable improper torsions
		#define CHARMM_IMPROPER_TORSIONS_ENABLED "enable IT"

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(CharmmImproperTorsion)

		/**	Default constructor.
		*/
		CharmmImproperTorsion();

		/**	Constructor.
		*/
		CharmmImproperTorsion(ForceField& force_field);

		/**	Copy constructor
		*/
		CharmmImproperTorsion(const CharmmImproperTorsion& charmm_stretch);

		/**	Destructor.
		*/
		virtual ~CharmmImproperTorsion();

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw(Exception::TooManyErrors);

		//@}
		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double updateEnergy();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces();

		//@}

		private:

		/*_	@name	Private Attributes
		*/
		//_@{

		/*_	array with the torsions
		*/
		vector<QuadraticImproperTorsion::Data>	impropers_;

		QuadraticImproperTorsion								improper_parameters_;

		ResidueTorsions													improper_atoms_;

		//_@}
	 
	};
} // namespace BALL 

#endif // BALL_MOLMEC_CHARMM_CHARMMIMPROPERTORSION_H
