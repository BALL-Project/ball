// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94Stretch.h,v 1.1.2.9 2005/03/24 16:17:38 amoll Exp $ 
//

// Molecular Mechanics: MMFF94 force field, bond stretch component

#ifndef BALL_MOLMEC_MMFF94_MMFF94STRETCH_H
#define BALL_MOLMEC_MMFF94_MMFF94STRETCH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#	include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

#ifndef BALL_KERNEL_STANDARDPREDICATES_H
 # include <BALL/KERNEL/standardPredicates.h>
#endif

namespace BALL 
{
	/**	MMFF94 bond stretch component
    	\ingroup  MMFF94
	*/
	class MMFF94Stretch 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{
		
		///
		struct Stretch
		{
			Atom* atom1;
			Atom* atom2;
			float kb;
			float r0;
			bool  sbmb; // debuging
			int reason; // debuging
		};

		BALL_CREATE(MMFF94Stretch)

		/**	Default constructor.
		*/
		MMFF94Stretch();

		/**	Constructor.
		*/
		MMFF94Stretch(ForceField& force_field);

		/**	Copy constructor
		*/
		MMFF94Stretch(const MMFF94Stretch& MMFF94_stretch);

		/**	Destructor.
		*/
		virtual ~MMFF94Stretch();

		//@}
		/**	@name Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup()
			throw(Exception::TooManyErrors);

		//@}
		/**	@name Accessors		
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual double updateEnergy();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces();

		/**	Update the bond list.
		*/
		virtual void update()
			throw(Exception::TooManyErrors);

		///
		MMFF94BondStretchParameters getParameters() { return parameters_;}

		const vector<Stretch>& getStretches() { return stretch_;}

		//@} 

		private:

		bool isInOneAromaticRing_(const Bond& bond);
		
		/*_	@name	Private Attributes	
		*/
		//_@{
		MMFF94BondStretchParameters parameters_;
		
		vector<Stretch> stretch_;

		vector< HashSet<Atom*> > rings_;

		Sp2HybridizedPredicate isSp2_;
		SpHybridizedPredicate  isSp_;
		//_@}
		
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94STRETCH_H
