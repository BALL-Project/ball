// $Id: amberTorsion.h,v 1.2 1999/09/21 11:06:54 oliver Exp $
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

		/**	@name	Enums	*/
		//@{

		/**	Error codes of the AmberTorsion class
		*/
		enum ErrorCode 
		{
			ERROR__UNKNOWN = 0,

			NUMBER_OF_ERRORS
		};

		//@}


		/**	@name	Type Definitions	*/
		//@{

		struct SingleAmberTorsion 
		{
			Atom*	atom1;
			Atom*	atom2;
			Atom*	atom3;
			Atom*	atom4;

			float						V;
			unsigned char		f;
			unsigned char		phase;

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

				//if (Maths::isZero(t.values.phase))
				if (t.values.phase < 90.0)
				{
					phase = (unsigned char)0;
				} else {
					phase = (unsigned char)1;
				}
			}
		};
		//@}
			
		/**	@name	Constructors and Destructors	*/
		//@{

		BALL_CREATE(AmberTorsion)

		/**	Default constructor.
		*/
		AmberTorsion(void);

		/**	constructor.
		*/
		AmberTorsion(ForceField* force_field);

		/**	Copy constructor
		*/
		AmberTorsion(const AmberTorsion& amber_stretch, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberTorsion(void);
		//@}

		/**	@name	Setup Methods	*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup(void);

		//@}

		/**	@name	Accessors	*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual float updateEnergy(void);

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces(void);

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
