// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94NonBonded.h,v 1.1.8.4 2007/05/16 20:34:22 amoll Exp $
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
			double vdw_energy; // for debugging
			// for electrostatics:
			bool 	 is_1_4;		 
			double qi;
			double qj;
			double es_energy;  // for debugging
		};

		/// flag to enable ES
		#define MMFF94_ES_ENABLED "enable ES"
		
		/// flag to enable VDW
		#define MMFF94_VDW_ENABLED "enable VDW"

		BALL_CREATE(MMFF94NonBonded)

		///	Default constructor.
		MMFF94NonBonded()
			;

		///	Constructor.
		MMFF94NonBonded(ForceField& force_field)
			;

		///	Copy constructor
		MMFF94NonBonded(const MMFF94NonBonded& MMFF94_non_bonded)
			;

		///	Destructor.
		virtual ~MMFF94NonBonded()
			;
		
		/// Assignment operator
		const MMFF94NonBonded& operator = (const MMFF94NonBonded& anb)
			;

		/// Clear method
		virtual void clear()
			;

		/// Equality operator
		bool operator == (const MMFF94NonBonded& anb)
			;

		///	Setup method.
		virtual bool setup()
			throw(Exception::TooManyErrors);

		///	Calculates and returns the component's energy.
		virtual double updateEnergy()
			;

		///	Calculates and returns the component's forces.
		virtual void updateForces()
			;

		/**	Update the pair list.
				This method is called by the force field whenever
				 \link ForceField::update ForceField::update \endlink  is called. It is used
				to recalculate the nonbonded pair list.
		*/
		virtual void update()
			throw(Exception::TooManyErrors);

		///	Computes the most efficient way to calculate the non-bonded atom pairs
		virtual MolmecSupport::PairListAlgorithmType
			determineMethodOfAtomPairGeneration()
			;

		///
		const ForceField::PairVector& getAtomPairs() const { return atom_pair_vector_;}

		///
		const vector<NonBondedPairData>& getNonBondedData() const { return non_bonded_data_;}

		///
		double getVDWEnergy() const;

		///
		double getESEnergy() const;

		protected:

		//_	Value of the electrostatic energy
		double	es_energy_;

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
		double 																vdw_cut_on_, vdw_cut_off_;
		double 																es_cut_on_, es_cut_off_;
		MMFF94VDWParameters 									vdw_parameters_;
		// dielectric constant
		double 																dc_; 
		// dielectric model exponent
		double 																n_;
		bool 																	es_enabled_;
		bool 																	vdw_enabled_;
		bool 																	enable_es_switch_;
		bool 																	enable_vdw_switch_;
		double 	es_d_on2_, es_d_off2_, 
						es_d_on_, es_d_off_,
						es_ac_, es_bc_, 
						es_cc_, es_dc_, es_denom_, es_con_, es_cover3_, es_dover5_,
						es_eadd_, es_eaddr_, es_const_, es_constr_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94VDW_H
