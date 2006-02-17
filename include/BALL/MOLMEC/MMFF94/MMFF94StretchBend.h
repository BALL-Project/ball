// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94StretchBend.h,v 1.1.2.5 2006/02/17 02:05:44 amoll Exp $
//

#ifndef BALL_MOLMEC_MMFF94_MMFF94STRETCHBEND_H
#define BALL_MOLMEC_MMFF94_MMFF94STRETCHBEND_H

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#	include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

namespace BALL 
{
	/**	MMFF94 bond stretch component
    	\ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94StretchBend 
		: public ForceFieldComponent
	{
		public:

		struct BALL_EXPORT StretchBend
		{
			double kba_ijk;
			double kba_kji;
			Atom::StaticAtomAttributes*	atom1;
			Atom::StaticAtomAttributes*	atom2;
			Atom::StaticAtomAttributes*	atom3;
			// debug:
			const double* delta_r_ij;
			const double* delta_r_kj;
			const double* delta_theta;
			Index sbtijk; 
			double energy;
		};

		BALL_CREATE(MMFF94StretchBend)

		/** @name	Constructors and Destructors	
		*/
		//@{ 

		/**	Default constructor.
		*/
		MMFF94StretchBend();

		/**	Constructor.
		*/
		MMFF94StretchBend(ForceField& force_field);

		/**	Copy constructor
		*/
		MMFF94StretchBend(const MMFF94StretchBend& to_copy);

		/**	Destructor.
		*/
		virtual ~MMFF94StretchBend();

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
		const vector<StretchBend>& getStretchBends() const { return stretch_bends_;}
		//@}

		///
		Index calculateSBTIJK(Position angle_type, 
													bool bond_type1,
													bool bond_type2);
		private:

		void errorOccured_(const String& string, 
											 const Atom& a1, const Atom& a2, const Atom& a3);

		vector<StretchBend> stretch_bends_;
 		MMFF94StretchBendParameters parameters_;
	 
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94STRETCHBEND_H
