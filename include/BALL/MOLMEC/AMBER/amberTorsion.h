// $Id: amberTorsion.h,v 1.5 2000/02/06 19:45:01 oliver Exp $
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_AMBER_AMBERTORSION_H
#define BALL_MOLMEC_AMBER_AMBERTORSION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_COSINETORSION_H
#	include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

namespace BALL 
{

	/**	Amber bond stretch component
	*/
	class AmberTorsion 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Type Definitions	*/
		//@{

		struct SingleAmberTorsion 
		{
			Atom*	atom1;
			Atom*	atom2;
			Atom*	atom3;
			Atom*	atom4;

			float		V;
			unsigned char	f;
			float		phase;

			SingleAmberTorsion()
				:	atom1(0),
					atom2(0),
					atom3(0),
					atom4(0),
					V(0),
					f(0),
					phase(0)
			{
			}
				

			SingleAmberTorsion(FFPSCosineTorsion::SingleTorsion& t)
			{
				atom1 = t.atom1;
				atom2 = t.atom2;
				atom3 = t.atom3;
				atom4 = t.atom4;

				V = t.values.V / t.values.n;
				f = (unsigned char)t.values.f;
				phase = ((2.0 * BALL::Constants::PI)/360.0) * t.values.phase;

			}
		};
		//@}
			
		/**	@name	Constructors and Destructors	*/
		//@{

		BALL_CREATE(AmberTorsion)

		/**	Default constructor.
		*/
		AmberTorsion();

		/**	constructor.
		*/
		AmberTorsion(ForceField* force_field);

		/**	Copy constructor
		*/
		AmberTorsion(const AmberTorsion& amber_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberTorsion();
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
		virtual float updateEnergy();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces();

		//@}


		private:

		/*_	@name	Private Attributes	*/
		//@{

		/*_	array with the torsions
		*/
		vector<SingleAmberTorsion> 	torsion_;

		FFPSCosineTorsion		torsion_parameters_;
		
		FFPSCosineTorsion		improper_parameters_;

		FFParameterSection	impropers_;
		//@}
	 
	};

} // namespace BALL 

#endif // BALL_MOLMEC_AMBER_AMBERTORSION_H
