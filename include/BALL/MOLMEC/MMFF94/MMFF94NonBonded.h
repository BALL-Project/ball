// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.h,v 1.1.2.3 2006/02/17 02:05:43 amoll Exp $
//

// Molecular Mechanics: MMFF94 force field, non-bonded component

#ifndef BALL_MOLMEC_MMFF94_NONBONDED_H
#define BALL_MOLMEC_MMFF94_NONBONDED_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SUPPORT_H
#	include <BALL/MOLMEC/COMMON/support.h>
#endif

namespace BALL 
{
	/**	MMFF94 NonBonded (VdW + Electrostatic) component
			
    	\ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94NonBonded 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(MMFF94NonBonded)

		/**	Default constructor.
		*/
		MMFF94NonBonded()
			throw();

		/**	Constructor.
		*/
		MMFF94NonBonded(ForceField& force_field)
			throw();

		/**	Copy constructor
		*/
		MMFF94NonBonded(const MMFF94NonBonded& MMFF94_non_bonded)
			throw();

		/**	Destructor.
		*/
		virtual ~MMFF94NonBonded()
			throw();
		//@}

		/** Assignment
		*/
		//@{
		
		/** Assignment operator
		*/
		const MMFF94NonBonded& operator = (const MMFF94NonBonded& anb)
			throw();

		/** Clear method
		*/
		virtual void clear()
			throw();

		//@}
		/** Predicates
		*/
		//@{
			
		/** Equality operator
		*/
		bool operator == (const MMFF94NonBonded& anb)
			throw();

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
		virtual double updateEnergy()
			throw();

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces()
			throw();

		/**	Update the pair list.
				This method is called by the force field whenever
				 \link ForceField::update ForceField::update \endlink  is called. It is used
				to recalculate the nonbonded pair list.
		*/
		virtual void update()
			throw(Exception::TooManyErrors);

		/**	Return the electrostatic energy.
		*/
		virtual double getElectrostaticEnergy() const
			throw();

		/**	Return the Van-der-Waals energy.
		*/
		virtual double getVdwEnergy() const
			throw();

		//@}
		/**	@name Neighbourhood and Parameter calculations
		*/
		//@{

		/**	Computes the most efficient way to calculate the non-bonded atom pairs
		*/
		virtual MolmecSupport::PairListAlgorithmType
			determineMethodOfAtomPairGeneration()
			throw();

		/**	Build a vector of non-bonded atom pairs with the vdw parameters
		*/
		virtual void buildVectorOfNonBondedAtomPairs(const std::vector<std::pair<Atom*, Atom*> >& atom_vector)
			throw(Exception::TooManyErrors);

		//@}

		protected:

		/*_	@name	Protected Attributes	
		*/
		//_@{

		/*_	Value of the electrostatic energy
		*/
		double	electrostatic_energy_;

		/*_	Value of the vdw energy
		*/
		double	vdw_energy_;

		//_@}

		private:

		/*_	@name	Private Attributes	
		*/
		//_@{

    /*_ The most efficient algorithm to calculate the non-bonded atom pairs.
        {\tt BRUTE\_FORCE}: brute force: all against all\\
        {\tt HASH\_GRID}: box grid
    */
    MolmecSupport::PairListAlgorithmType  algorithm_type_;
		double cut_off_;
		vector<Atom*> non_bonded_;

		//_@}

	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94VDW_H
