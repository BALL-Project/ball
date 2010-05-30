// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// Molecular Mechanics: Charmm force field, non-bonded component
// (van der Waals, electrostatics, EEF1 solvation)
//

#ifndef BALL_MOLMEC_CHARMM_NONBONDED_H
#define BALL_MOLMEC_CHARMM_NONBONDED_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_LENNARDJONES_H
#	include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_POTENTIAL1210_H
#	include <BALL/MOLMEC/PARAMETER/potential1210.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
#	include <BALL/MOLMEC/PARAMETER/charmmEEF1.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SUPPORT_H
#	include <BALL/MOLMEC/COMMON/support.h>
#endif

namespace BALL 
{
	/**	Charmm NonBonded component.
		This force field component implements the non-bonded interactions
		in the CHARMM force field: van-der-Waals interaction, electrostatics,
		and solvation (in EEF1 only).
			
    	\ingroup  CHARMM
	*/
	class BALL_EXPORT CharmmNonBonded 
		: public ForceFieldComponent
	{
		public:

		/// flag to enable NonBonded
		#define CHARMM_NB_ENABLED "enable NB"

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(CharmmNonBonded)

		/**	Default constructor.
		*/
		CharmmNonBonded()
			;

		/**	Constructor.
		*/
		CharmmNonBonded(ForceField& force_field)
			;

		/**	Copy constructor
		*/
		CharmmNonBonded(const CharmmNonBonded& charmm_non_bonded)
			;

		/**	Destructor.
		*/
		virtual ~CharmmNonBonded()
			;

		//@}
		/** @name Assignment
		*/
		//@{
		
		/** Assignment operator
		*/
		const CharmmNonBonded& operator = 
			(const CharmmNonBonded& charmm_non_bonded)
			;

		/** Clear method
		*/
		virtual void clear()
			;

		//@}
		/** @name Predicates
		*/
		//@{

		/** Equality operator
		*/
		bool operator == (const CharmmNonBonded& charmm_non_bonded)
			;

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
			;

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces()
			;

		/**	Return the electrostatic energy.
		*/
		virtual double getElectrostaticEnergy() const
			;

		/**	Return the Van-der-Waals energy.
		*/
		virtual double getVdwEnergy() const
			;

		/** Return the solvation energy.
		*/
		virtual double getSolvationEnergy() const
			;

		//@}
		/**	@name Neighbourhood and Parameter calculations
		*/
		//@{

		/**	Computes the most efficient way to calculate the non-bonded atom pairs
		*/
		virtual MolmecSupport::PairListAlgorithmType	
			determineMethodOfAtomPairGeneration()
			;

		/**	Build a vector of non-bonded atom pairs with the vdw parameters
		*/
		virtual void buildVectorOfNonBondedAtomPairs
			(const std::vector<std::pair<Atom*, Atom*> >& atom_vector)
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

		/*_ Value of the solvation energy
		*/
		double solvation_energy_;

		//_@}
		/*_	@name	Private Attributes	
		*/
		//_@{

		private:

		/*_	Vector array with all atom pairs whose distance is smaller than cut_off
		*/
		vector<LennardJones::Data>	non_bonded_;
		
		/*_ A helper array for buildVectorOfNonBondedAtomPairs(). This is
				declared here to save the ctor within the method.
		*/
		vector<bool> is_torsion_;

		/*_	Number of 1-4 interactions in the vector non_bonded
		*/
		Size	number_of_1_4_;	

		/*_	Cutoff distance for non-bonded interactions
		*/
		float	cut_off_;

		/*_	Cutoff distance for vdw interactions
		*/
		float	cut_off_vdw_;

		/*_	Start of the switch function for the vdw interactions
		*/
		float	cut_on_vdw_;

		/*_	Cutoff distance for electrostatic interactions
		*/
		float	cut_off_electrostatic_;

		/*_	Start of the switch function for the vdw interactions
		*/
		float	cut_on_electrostatic_;

		/*_	Cutoff distance for solvation contribution (EEF1)
		*/
		float	cut_off_solvation_;

		/*_	Start of the switch function for the solvation contribution (EEF1)
		*/
		float	cut_on_solvation_;

		/*_	Inverse cube of the difference of cutoff and cuton for vdW
		*/
		float inverse_difference_off_on_vdw_3_;

		/*_	Inverse cube of the difference of cutoff and cuton for solvation
		*/
		float inverse_difference_off_on_solvation_3_;

		/*_	Inverse cube of the difference of cutoff and cuton for electrostatic
		*/
		float inverse_difference_off_on_electrostatic_3_;

		/*_	Scaling factor for vdw_1_4_interactions
		*/
		float	scaling_vdw_1_4_;

		/*_	Scaling factor for electrostatic_1_4_interactions
		*/
		float	scaling_electrostatic_1_4_;

		/*_ Flag for using constant or distance dependent dielectric constant 
				True = distance dependent
		*/
		bool    use_dist_depend_dielectric_; 

		/*_	The most efficient algorithm to calculate the non-bonded atom pairs.
				{\tt BRUTE\_FORCE}: brute force: all against all\\
				{\tt HASH\_GRID}: box grid
		*/
		MolmecSupport::PairListAlgorithmType	algorithm_type_;
		
		LennardJones								van_der_waals_parameters_;

		LennardJones								van_der_waals_parameters_14_;

		CharmmEEF1									solvation_parameters_;

		vector<CharmmEEF1::Values>	solvation_;

		bool use_solvation_component_;

		//_@}

	};
} // namespace BALL

#endif // BALL_MOLMEC_CHARMM_CHARMMVDW_H
