// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.h,v 1.1.2.9 2006/02/22 23:09:37 amoll Exp $
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

#ifndef BALL_MOLMEC_MMFF94_PROCESSORS_H
# include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>
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

		struct NonBondedPairData
		{
			NonBondedPairData();

			double eij;
			double rij;
			double rij_7;
			double VDW_energy; // for debugging
			// for electrostatics:
			bool 	 is_1_4;		 
			double qi;
			double qj;
			double ES_energy;  // for debugging
		};


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

		///
		const ForceField::PairVector& getAtomPairs() const { return atom_pair_vector_;}

		///
		const vector<NonBondedPairData>& getNonBondedData() const { return non_bonded_data_;}

		///
		double getVDWEnergy() const { return vdw_energy_;}

		///
		double getESEnergy() const { return electrostatic_energy_;}

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
		ForceField::PairVector 								atom_pair_vector_;
		vector<NonBondedPairData> 						non_bonded_data_;
    MolmecSupport::PairListAlgorithmType  algorithm_type_;
		double 																cut_off_;
		MMFF94VDWParameters 									vdw_parameters_;
		MMFF94ESParameters 										es_parameters_;
		HashMap<Atom*, double>								charge_map_;
		MMFF94ChargeProcessor 								charge_processor_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94VDW_H
