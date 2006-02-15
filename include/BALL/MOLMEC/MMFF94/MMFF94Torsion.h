// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Torsion.h,v 1.1.2.5 2006/02/15 23:09:36 amoll Exp $
//

// Molecular Mechanics: MMFF94 force field, bond stretch component

#ifndef BALL_MOLMEC_MMFF94_MMFF94TORSION_H
#define BALL_MOLMEC_MMFF94_MMFF94TORSION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#	include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

namespace BALL 
{
	/**	MMFF94 bond stretch component
    	\ingroup  MMFF94
	*/
	class MMFF94Torsion 
		: public ForceFieldComponent
	{
		public:

		/// see MMFFTOR.PAR
		struct Torsion
		{
			Torsion();

			Index type;
			double angle;
			Atom::StaticAtomAttributes*	atom1;
			Atom::StaticAtomAttributes*	atom2;
			Atom::StaticAtomAttributes*	atom3;
			Atom::StaticAtomAttributes*	atom4;
			double energy;  // debug
			double v1, v2, v3;
		};

		/**	@name	Type Definitions	
		*/
		//@{

		//@}	

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(MMFF94Torsion)

		/**	Default constructor.
		*/
		MMFF94Torsion();

		/**	Constructor.
		*/
		MMFF94Torsion(ForceField& force_field);

		/**	Copy constructor
		*/
		MMFF94Torsion(const MMFF94Torsion& MMFF94_stretch);

		/**	Destructor.
		*/
		virtual ~MMFF94Torsion();

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

		///
		Position getTorsionType(const vector<Atom*>& atoms) const;
		
		///
		const vector<Torsion>& getTorsions() const { return torsions_;}

		//@}

		protected:

		double getU_(Position e);
		double getV_(Position e);
		bool calculateHeuristic_(const Atom& aj, const Atom& ak, double& v1, double& v2, double& v3);

		vector<Torsion> torsions_;
		MMFF94TorsionParameters parameters_;
	 
	};
} // namespace BALL 

#endif // BALL_MOLMEC_MMFF94_MMFF94TORSION_H
