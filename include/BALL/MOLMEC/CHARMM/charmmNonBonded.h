// $Id: charmmNonBonded.h,v 1.6 2001/02/18 20:50:38 amoll Exp $
// Molecular Mechanics: Charmm force field, bond stretch component

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

	/**	Charmm NonBonded (VdW + Electrostatic) component
	*/
	class CharmmNonBonded 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE_DEEP(CharmmNonBonded)

		/**	Default constructor.
		*/
		CharmmNonBonded();

		/**	Constructor.
		*/
		CharmmNonBonded(ForceField& force_field);

		/**	Copy constructor
		*/
		CharmmNonBonded(const CharmmNonBonded& charmm_non_bonded, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~CharmmNonBonded();

		//@}
		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
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

		/**	Return the electrostatic energy.
		*/
		virtual double getElectrostaticEnergy() const;

		/**	Return the Van-der-Waals energy.
		*/
		virtual double getVdwEnergy() const;

		/** Return the solvation energy.
		*/
		virtual double getSolvationEnergy() const;


		//@}

		/**	@name Neighbourhood and Parameter calculations
		*/
		//@{

		/**	Computes the most efficient way to calculate the non-bonded atom pairs
		*/
		virtual MolmecSupport::PairListAlgorithmType	
			determineMethodOfAtomPairGeneration();

		/**	Build a vector of non-bonded atom pairs with the vdw parameters
		*/
		virtual void buildVectorOfNonBondedAtomPairs
			(const vector< pair<Atom*, Atom*> >& atom_vector);

		//@}

		protected:

		/**	@name	Protected Attributes	*/
		//@{

		/**	Value of the electrostatic energy
		*/
		double	electrostatic_energy_;

		/**	Value of the vdw energy
		*/
		double	vdw_energy_;

		/** Value of the solvation energy
		*/
		double solvation_energy_;

		//@}

		private:

		/*_	@name	Private Attributes	
		*/
		//_@{

		/**	Vector array with all atom pairs whose distance is smaller than cut_off
		*/
		vector<LennardJones::Data>	non_bonded_;

		/**	Vector with parameters for the solvation component
		*/
		vector<CharmmEEF1::Values> solvation_values_;
 
		/**	Number of 1-4 interactions in the vector non_bonded
		*/
		Size	number_of_1_4_;	

		/**	Cutoff distance for non-bonded interactions
		*/
		float	cut_off_;

		/**	Cutoff distance for vdw interactions
		*/
		float	cut_off_vdw_;

		/**	Start of the switch function for the vdw interactions
		*/
		float	cut_on_vdw_;

		/**	Cutoff distance for electrostatic interactions
		*/
		float	cut_off_electrostatic_;

		/**	Start of the switch function for the vdw interactions
		*/
		float	cut_on_electrostatic_;

		/**	Cutoff distance for solvation contribution (EEF1)
		*/
		float	cut_off_solvation_;

		/**	Inverse cube of the difference of cutoff and cuton for vdW
		*/
		float inverse_difference_off_on_vdw_3_;

		/**	Inverse cube of the difference of cutoff and cuton for solvation
		*/
		float inverse_difference_off_on_solvation_3_;

		/**	Inverse cube of the difference of cutoff and cuton for electrostatic
		*/
		float inverse_difference_off_on_electrostatic_3_;

		/**	Start of the switch function for the solvation contribution (EEF1)
		*/
		float	cut_on_solvation_;

		/**	Scaling factor for vdw_1_4_interactions
		*/
		float	scaling_vdw_1_4_;

		/**	Scaling factor for electrostatic_1_4_interactions
		*/
		float	scaling_electrostatic_1_4_;

		/*_ Flag for using constant or distance dependent dielectric constant 
				True = distance dependent
		*/
		bool    use_dist_depend_dielectric_; 

		/**	The most efficient algorithm to calculate the non-bonded atom pairs.
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
