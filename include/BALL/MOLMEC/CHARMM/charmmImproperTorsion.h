// $Id: charmmImproperTorsion.h,v 1.5 2000/08/30 19:58:16 oliver Exp $
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

namespace BALL 
{

	/**	Charmm improper torsion component.
			The CHARMM force field employs a harmonic out-of-plane potential for improper
			torsions.	An imporper torsion is defined for four atoms {\it A}, {\it B}, {\it C},
			and {\it D} where {\it A} is the central atom {\it B}, {\it C}, and {\it D} are bound to.\\
			
			The out-of-plane-angle \TEX{\phi} is defined as the angle $\phi$ between 
			the planes defined by {\it ABC} and {\it BCD}. 
			The improper torsion energy $E_{imp}$ is then defined as
			\begin{equation*}
				E_{imp} = k (\theta - \theta_0)^2
			\end{equation}
			where $k$ and $\theta_0$ are constants read from the parameter section and depend on
			the atom types of {\it A}, {\it B}, {\it C}, and {\it D}.
			\\
			{\bf Definition:}\URL{BALL/MOLMEC/CHARMM/charmmImproperTorsion.h}
			\\
	*/
	class CharmmImproperTorsion 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	*/
		//@{

		BALL_CREATE_DEEP(CharmmImproperTorsion)

		/**	Default constructor.
		*/
		CharmmImproperTorsion();

		/**	constructor.
		*/
		CharmmImproperTorsion(ForceField& force_field);

		/**	Copy constructor
		*/
		CharmmImproperTorsion(const CharmmImproperTorsion& charmm_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~CharmmImproperTorsion();
		//@}

		/**	@name	Setup Methods	*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup();

		//@}

		/**	@name	Accessors	*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double updateEnergy();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces();

		//@}


		private:

		/*_	@name	Private Attributes	*/
		//@{

		/*_	array with the torsions
		*/
		vector<QuadraticImproperTorsion::Data>	impropers_;

		QuadraticImproperTorsion								improper_parameters_;

		ResidueTorsions													improper_atoms_;
		//@}
	 
	};

} // namespace BALL 

#endif // BALL_MOLMEC_CHARMM_CHARMMIMPROPERTORSION_H
