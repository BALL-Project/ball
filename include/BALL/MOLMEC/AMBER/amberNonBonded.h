// $Id: amberNonBonded.h,v 1.11 2000/07/25 21:14:17 oliver Exp $
// Molecular Mechanics: Amber force field, bond stretch component

#ifndef BALL_MOLMEC_AMBER_NONBONDED_H
#define BALL_MOLMEC_AMBER_NONBONDED_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_LENNARDJONES_H
#	include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_POTENTIAL1210_H
#	include <BALL/MOLMEC/PARAMETER/potential1210.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SUPPORT_H
#	include <BALL/MOLMEC/COMMON/support.h>
#endif

namespace BALL 
{

	/**	Amber NonBonded (VdW + Electrostatic) component
	*/
	class AmberNonBonded 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(AmberNonBonded)

		/**	Default constructor.
		*/
		AmberNonBonded();

		/**	Constructor.
		*/
		AmberNonBonded(ForceField& force_field);

		/**	Copy constructor
		*/
		AmberNonBonded(const AmberNonBonded& amber_non_bonded, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberNonBonded();

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

		/**	Update the pair list.
				This method is called by the force field whenever
				\Ref{ForceField::update} is called. It is used
				to recalculate the nonbonded pair list.
		*/
		virtual void update();

		/**	Return the electrostatic energy.
		*/
		virtual double getElectrostaticEnergy() const;

		/**	Return the Van-der-Waals energy.
		*/
		virtual double getVdwEnergy() const;

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
			(const vector< pair<Atom*, Atom*> >& atom_vector,
			 const LennardJones& lennard_jones,
			 const Potential1210& hydrogen_bond);


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

		//@}


		private:

		/*_	@name	Private Attributes	*/
		//_@{

		/**	Vector array with all atom pairs whose distance is smaller than cut_off
		*/
		vector<LennardJones::Data>	non_bonded_;

    /** Vector of flags deciding whether the pair forms a hydrogen bond or a
				standard VdW interaction.
    */
    vector<bool>  is_hydrogen_bond_;
 
		/**	Number of 1-4 interactions in the vector non_bonded
		*/
		Size	number_of_1_4_;	

		/**	Cutoff distance for non-bonded interactions
		*/
		double	cut_off_;

		/**	Cutoff distance for vdw interactions
		*/
		double	cut_off_vdw_;

		/**	Cuton distance for vdw interactions
		*/
		double	cut_on_vdw_;

		/**	Cutoff distance for electrostatic interactions
		*/
		double	cut_off_electrostatic_;

		/**	Cuton distance for electrostatic interactions
		*/
		double	cut_on_electrostatic_;

		/**	Inverse cube of the difference of squares of cuton and cutoff for vdW.
				This value is required for the switching function
		*/
		double inverse_distance_off_on_vdw_3_;
		
		/**	Inverse cube of the difference of squares of cuton and cutoff for eletrostatic.
				This value is required for the switching function
		*/
		double inverse_distance_off_on_electrostatic_3_;
		
		/**	Scaling factor for vdw_1_4_interactions
		*/
		double	scaling_vdw_1_4_;

		/**	Scaling factor for electrostatic_1_4_interactions
		*/
		double	scaling_electrostatic_1_4_;

		/*_     Flag for using constant or distance dependent dielectric constant.
            True = distance dependent
    */
    bool    use_dist_depend_dielectric_; 

    /** The most efficient algorithm to calculate the non-bonded atom pairs.
        {\tt BRUTE\_FORCE}: brute force: all against all\\
        {\tt HASH\_GRID}: box grid
    */
    MolmecSupport::PairListAlgorithmType  algorithm_type_;
 		
		LennardJones	van_der_waals_;

		Potential1210 hydrogen_bond_;

		//_@}

	};

} // namespace BALL

#endif // BALL_MOLMEC_AMBER_AMBERVDW_H
