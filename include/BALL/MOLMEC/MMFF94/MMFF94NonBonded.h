// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.h,v 1.1.2.5 2006/02/19 00:37:04 amoll Exp $
//

#ifndef BALL_MOLMEC_MMFF94_NONBONDED_H
#define BALL_MOLMEC_MMFF94_NONBONDED_H

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SUPPORT_H
#	include <BALL/MOLMEC/COMMON/support.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#	include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
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

			/*
		struct VDWPair
		{
			Atom::StaticAtomAttributes* atom1;
			Atom::StaticAtomAttributes* atom2;
			*/


		BALL_CREATE(MMFF94NonBonded)

		///	Default constructor.
		MMFF94NonBonded()
			throw();

		///	Constructor.
		MMFF94NonBonded(ForceField& force_field)
			throw();

		///	Copy constructor
		MMFF94NonBonded(const MMFF94NonBonded& MMFF94_non_bonded)
			throw();

		///	Destructor.
		virtual ~MMFF94NonBonded()
			throw();
		
		/// Assignment operator
		const MMFF94NonBonded& operator = (const MMFF94NonBonded& anb)
			throw();

		/// Clear method
		virtual void clear()
			throw();

		/// Equality operator
		bool operator == (const MMFF94NonBonded& anb)
			throw();

		///	Setup method.
		virtual bool setup()
			throw(Exception::TooManyErrors);

		///	Calculates and returns the component's energy.
		virtual double updateEnergy()
			throw();

		///	Calculates and returns the component's forces.
		virtual void updateForces()
			throw();

		/**	Update the pair list.
				This method is called by the force field whenever
				 \link ForceField::update ForceField::update \endlink  is called. It is used
				to recalculate the nonbonded pair list.
		*/
		virtual void update()
			throw(Exception::TooManyErrors);

		///	Return the electrostatic energy.
		virtual double getElectrostaticEnergy() const
			throw();

		///	Return the Van-der-Waals energy.
		virtual double getVdwEnergy() const
			throw();

		///	Computes the most efficient way to calculate the non-bonded atom pairs
		virtual MolmecSupport::PairListAlgorithmType
			determineMethodOfAtomPairGeneration()
			throw();

		protected:

		//_	Value of the electrostatic energy
		double	electrostatic_energy_;

		//_	Value of the vdw energy
		double	vdw_energy_;

		private:

    /*_ The most efficient algorithm to calculate the non-bonded atom pairs.
        {\tt BRUTE\_FORCE}: brute force: all against all\\
        {\tt HASH\_GRID}: box grid
    */
		ForceField::PairVector atom_pair_vector_;
		vector<double> eijs_;
		vector<double> rijs_;
		vector<double> rijs_7_;
    MolmecSupport::PairListAlgorithmType  algorithm_type_;
		double cut_off_;
		vector<Atom*> non_bonded_;
		MMFF94VDWParameters parameters_;

		//_@}

	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94VDW_H
