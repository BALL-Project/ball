// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Bend.h,v 1.1.2.8 2006/02/02 23:53:00 amoll Exp $
//

// Molecular Mechanics: MMFF94 force field, bond stretch component

#ifndef BALL_MOLMEC_MMFF94_MMFF94BEND_H
#define BALL_MOLMEC_MMFF94_MMFF94BEND_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#	include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

namespace BALL 
{
	/**	MMFF94 bond stretch component
    	\ingroup  MMFF94
	*/
	class MMFF94Bend 
		: public ForceFieldComponent
	{
		public:

		/// see MMFFANG.PAR
		struct Bend
		{
			double theta0;
			double delta_theta;
			double ka;
			Atom::StaticAtomAttributes*	atom1;
			Atom::StaticAtomAttributes*	atom2;
			Atom::StaticAtomAttributes*	atom3;
			bool is_linear;
			Position ATIJK;
			double energy;  // debug
		};

		BALL_CREATE(MMFF94Bend)

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		MMFF94Bend();

		/**	Constructor.
		*/
		MMFF94Bend(ForceField& force_field);

		/**	Copy constructor
		*/
		MMFF94Bend(const MMFF94Bend& to_copy);

		/**	Destructor.
		*/
		virtual ~MMFF94Bend();

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
		Position getBendType(const Bond& bond1, const Bond& bond2,
										 		 Atom& atom1, Atom& atom2, Atom& atom3) const;
		
		///
		const vector<Bend>& getBends() const { return bends_;}

		///
		double calculateEmpericalReferenceAngle(Atom& atom1, Atom& atom2, Atom& atom3) const;
		
		///
		double calculateEmpericalForceConstant(Atom& atom1, Atom& atom2, Atom& atom3, double angle_0) const;
		//@}

		private:

		static double z_[];
		static double c_[];
		static Position atom_nr_[];

		vector<Bend> bends_;
		MMFF94BendParameters parameters_;
	 
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94BEND_H
