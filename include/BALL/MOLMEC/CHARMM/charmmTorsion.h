// $Id: charmmTorsion.h,v 1.4 2000/06/30 05:53:21 oliver Exp $
// Molecular Mechanics: CHARMM force field, proper torsion component

#ifndef BALL_MOLMEC_CHARMM_CHARMMTORSION_H
#define BALL_MOLMEC_CHARMM_CHARMMTORSION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_COSINETORSION_H
#	include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_RESIDUETORSIONS_H
#	include <BALL/MOLMEC/PARAMETER/residueTorsions.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

namespace BALL 
{

	/**	CHARMM torsion component
	*/
	class CharmmTorsion 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Type Definitions	*/
		//@{

		struct SingleCharmmTorsion 
		{
			Atom*	atom1;
			Atom*	atom2;
			Atom*	atom3;
			Atom*	atom4;

			float		V;
			unsigned char	f;
			float		phase;

			SingleCharmmTorsion()
				:	atom1(0),
					atom2(0),
					atom3(0),
					atom4(0),
					V(0),
					f(0),
					phase(0)
			{
			}
				

			SingleCharmmTorsion(CosineTorsion::SingleData& t)
			{
				atom1 = t.atom1;
				atom2 = t.atom2;
				atom3 = t.atom3;
				atom4 = t.atom4;

				V = t.values.V / t.values.n;
				f = (unsigned char)t.values.f;

				// convert phase from degrees to radiant
				phase = (BALL::Constants::PI / 180.0) * t.values.phase;

			}
		};
		//@}
			
		/**	@name	Constructors and Destructors	*/
		//@{

		BALL_CREATE(CharmmTorsion)

		/**	Default constructor.
		*/
		CharmmTorsion();

		/**	constructor.
		*/
		CharmmTorsion(ForceField& force_field);

		/**	Copy constructor
		*/
		CharmmTorsion(const CharmmTorsion& charmm_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~CharmmTorsion();
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

		/*_	Vector containing the parameters for each torsion.
		*/
		vector<SingleCharmmTorsion> 	torsion_;

		/*_ Contents of the [Torsions] section of the parameter file.
		*/
		CosineTorsion									torsion_parameters_;		

		/*_ Contents of the [ResidueTorsions] section of the parameter file.
		*/
		ResidueTorsions								residue_torsions_;

		/*_	Set to true, if the torsions are read from the ResidueTorsions section.
		*/
		bool use_residue_torsion_list_;
		//@}
	 
	};

} // namespace BALL 

#endif // BALL_MOLMEC_CHARMM_CHARMMTORSION_H
