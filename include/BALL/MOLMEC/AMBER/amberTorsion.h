// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif

namespace BALL 
{
	/**	Amber bond stretch component
    	\ingroup  AMBER
	*/
	class BALL_EXPORT AmberTorsion 
		: public ForceFieldComponent
	{
		public:

		/// flag to enable Torsions
		#define AMBER_TORSIONS_ENABLED "enable Torsions"

		/**	@name	Type Definitions	
		*/
		//@{

		struct SingleAmberTorsion 
		{
			Atom* atom1;
			Atom* atom2;
			Atom* atom3;
			Atom* atom4;

			float					V;
			unsigned char	f;
			float					phase;

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
				

			SingleAmberTorsion(CosineTorsion::SingleData& t)
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

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(AmberTorsion)

		/**	Default constructor.
		*/
		AmberTorsion();

		/**	Constructor.
		*/
		AmberTorsion(ForceField& force_field);

		/**	Copy constructor
		*/
		AmberTorsion(const AmberTorsion& amber_stretch);

		/**	Destructor.
		*/
		virtual ~AmberTorsion();

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		 * @throws BALL::Exception::TooManyErrors
		*/
		virtual bool setup();

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
		vector<SingleAmberTorsion> 	torsion_;

		CosineTorsion			torsion_parameters_;
		
		CosineTorsion			improper_parameters_;

		ParameterSection	impropers_;

		//_@}
	 
	};
} // namespace BALL 

#endif // BALL_MOLMEC_AMBER_AMBERTORSION_H
