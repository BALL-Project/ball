// $Id: amberNonBonded.h,v 1.1 1999/08/26 07:53:21 oliver Exp $
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

namespace BALL 
{

	/**	Amber NonBonded (VdW + Electrostatic) component
	*/
	class AmberNonBonded 
		: public ForceFieldComponent
	{
		public:

		/**	@name	Enums	*/
		//@{

		/**	Error codes of the AmberNonBonded class
		*/
		enum ErrorCode 
		{
			ERROR__UNKNOWN = 0,

			NUMBER_OF_ERRORS
		};

		//@}

		/**	@name	Constructors and Destructors	
		*/
		//@{

		BALL_CREATE(AmberNonBonded)

		/**	Default constructor.
		*/
		AmberNonBonded(void);

		/**	Constructor.
		*/
		AmberNonBonded(ForceField* force_field);

		/**	Copy constructor
		*/
		AmberNonBonded(const AmberNonBonded& amber_non_bonded, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~AmberNonBonded(void);

		//@}
		

		/**	@name	Setup Methods	
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup(void);

		//@}


		/**	@name	Accessors	
		*/
		//@{

		/**	Calculates and returns the component's energy.
		*/
		virtual float updateEnergy(void);

		/**	Calculates and returns the vdw energy.
		*/
		virtual float updateVdWEnergy(void);

		/**	Calculates and returns the electrostatic energy.
		*/
		virtual float updateElectrostaticEnergy(void);

		/**	Calculates and returns the component's forces.
		*/
		virtual void updateForces(void);

		/**	Calculates and returns the vdw forces.
		*/
		virtual void updateVdWForces(void) const;

		/**	Calculates and returns the electrostatic forces.
		*/
		virtual void updateElectrostaticForces(void) const;

		/**	Return the electrostatic energy.
		*/
		virtual float getElectrostaticEnergy() const;

		/**	Return the Van-der-Waals energy.
		*/
		virtual float getVdwEnergy() const;

		//@}

		/**	@name Neighbourhood and Parameter calculations
		*/
		//@{

		/**	Computes the most efficient way to calculate the non-bonded atom pairs
		*/
		virtual int	determineMethodOfAtomPairGeneration(void);

		/**	Build a vector of non-bonded atom pairs with the vdw parameters
		*/
		virtual void buildVectorOfNonBondedAtomPairs
			(const vector< pair<Atom*, Atom*> >& atom_vector,
			 const FFPSLennardJones& lennard_jones,
			 const FFPSPotential1210& hydrogen_bond);


		//@}

		protected:

		/**	@name	Protected Attributes	*/
		//@{

		/**	Value of the electrostatic energy
		*/
		float	electrostatic_energy_;

		/**	Value of the vdw energy
		*/
		float	vdw_energy_;

		//@}


		private:

		/*_	@name	Private Attributes	*/
		//_@{

		/**	Vector array with all atom pairs whose distance is smaller than cut_off
		*/
		vector<FFPSLennardJones::LennardJones>	non_bonded_;

    /** Vector of flags deciding whether the pair forms a hydrogen bond or a
				standard VdW interaction.
    */
    vector<bool>  is_hydrogen_bond_;
 
		/**	Number of 1-4 interactions in the vector non_bonded
		*/
		Size	number_of_1_4_;	

		/**	Cut_off distance for non-bonded interactions
		*/
		float	cut_off_;

		/**	Cut_off distance for vdw interactions
		*/
		float	cut_off_vdw_;

		/**	Cut_off distance for electrostatic interactions
		*/
		float	cut_off_electrostatic_;

		/**	Scaling factor for vdw_1_4_interactions
		*/
		float	scaling_vdw_1_4_;

		/**	Scaling factor for electrostatic_1_4_interactions
		*/
		float	scaling_electrostatic_1_4_;

		/**	A number that characterizes the most efficient way to calculate the non-bonded atom pairs
			0: brute force: all against all
			1: box grid
		*/
		int	algorithm_type_;

		//_@}

	};

} // namespace BALL

#endif // BALL_MOLMEC_AMBER_AMBERVDW_H
