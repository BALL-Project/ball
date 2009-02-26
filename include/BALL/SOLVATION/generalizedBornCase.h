// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: generalizedBornCase.h,v 1.1 2005/11/21 19:38:38 anker Exp $
//

#ifndef BALL_SOLVATION_GENERALIZED_BORN_H
#define BALL_SOLVATION_GENERALIZED_BORN_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#include <map>
#include <set>

namespace BALL 
{
	/** Compute the electrostatics of a molecule in solution.

	    This implementation bases on

			[1] "Modification of the Generalized Born Model Suitable for 
			Macromolecules"
			Onufriev, Bashford, Case
			J. Phys. Chem. B 104:3712-3720, 2000

			This paper uses techniques from

			[2] "Semianalytical Treatment of Solvation for Molecular
			Mechanics and Dynamics"
			Still, Tempczyk, Hawley, Hendrickson
			J. Am. Chem. Soc. 112(16):6127-6129, 1990

			[3] "Application of a pairwise generalized Born model to
			proteins and nucleic acids: inclusion of salt eff¤ects "
			Srinivasan, Trevathan, Beroza, Case
			Theor. Chem. Acc. 101(6):426-434, 1999

	*/
	class GeneralizedBornModel
	{
		public:

		struct Option
		{
			///
			static const String VERBOSITY;

			///
			static const String LAMBDA;

			///
			static const String DELTA;

			///
			static const String KAPPA;

			///
			static const String RHO_0;

			///
			static const String DC_SOLVENT;

			///
			static const String DC_SOLUTE;

			///
			static const String FILENAME;
		};

		struct Default
		{
			///
			static const Size VERBOSITY;

			///
			static const float LAMBDA;

			///
			static const float DELTA;

			///
			static const float KAPPA;

			///
			static const float RHO_0;

			///
			static const float DC_SOLVENT;

			///
			static const float DC_SOLUTE;

			///
			static const String FILENAME;
		};

		///
		GeneralizedBornModel()
			throw(Exception::FileNotFound);

		///
		~GeneralizedBornModel();

		///
		void clear();

		/// We need an additional optioned version of that piece of code
		bool setup(const AtomContainer& ac)
			throw(Exception::FileNotFound);

		/// ??? This should be done through Options!
		void setScalingFactorFile(const String& filename);

		///
		void setScalingFactors(const StringHashMap<float>& scaling_factors);

		///
		const StringHashMap<float>& getScalingFactors() const;

		/// ??? Should be done by options
		void setSolventDC(float solvent_dc);

		/// ??? Should be done by options
		void setSoluteDC(float solute_dc);

		/// Compute the total energy of the system
		float calculateEnergy();

		/// Compoute only the solvation energy (meaning the transfer energy
		/// which is a kind of a reaction field energy)
		float calculateSolvationEnergy();

		/// Calculate the total potential created by every other atom of the
		/// system for every atom and return a hashmap conteining the potential
		/// values.
		void calculatePotential(HashMap<const Atom*, float>& p_hash) const;

		/// Calculate the total potential created by every other atom of the
		/// system at the position of atom_i
		float calculatePotential(const Atom& atom_i) const;

		///
		bool readScalingFactors(const String& inifile_name)
			throw(Exception::FileNotFound);


		private:

		/* Parameters for the generalized Born model
		*/
		class GBParameters : public ParameterSection
		{
			public:

			GBParameters();

			~GBParameters();

			bool extractSection(Parameters& parameters, const String& section_name);

			const StringHashMap<float>& getScalingFactors() const;

			float getSoluteDC() const;

			float getSolventDC() const;

			float getKappa() const;

			private:

			/*_
			*/
			StringHashMap<float> scaling_factors_;

			/*_
			*/
			float dc_solute_;

			/*_
			*/
			float dc_solvent_;

			/*_
			*/
			float lambda_;

			/*_
			*/
			float delta_;
			
			/*_
			*/
			float kappa_;
			
			/*_
			*/
			float rho_0_;

		};

		/** The atom container representing the molecule
		*/
		AtomContainer* ac_;

		/** The atom list
		*/
		std::vector<const Atom*> atoms_;

		/** The Born radii of all atoms
		*/
		std::vector<float> born_radii_;

		/** The pairs of atoms we have to look at. This list only inlcudes
				*different* atoms. Not suitable for self-energy calculations,
		*/
		std::vector< std::pair<Size, Size> > pair_list_;

		/** Distances between atoms of the pair list.
		*/
		std::vector< float > pair_distances_;
		std::vector< float > pair_charges_;
		std::vector< std::vector< float > > squared_distances_;

		/** Dielectric constants of the solute and the surrounding solvent
		*/
		float dc_solute_;
		float dc_solvent_;

		/** Empirical scaling factor for the estimation of effective Born
		    radii. See [3] for details.
		*/
		StringHashMap<float> scaling_factors_;
		std::vector<float> scaling_factor_;

		/**
		*/
		String scaling_factor_filename_;

		/** Scaling factor of integral part of effective Born radii calculation
				rectifying the underestimation of the radii caused by the vdW
				approximation (instead of real SAS calculations).
		*/
		float lambda_;

		/** Downshift of the effictve Born radii for rectifying the energy
		    increase caused by the lambda factor. More or less magic (see
				[1] for details) but allows to use the scaling_factors_ without
				reparamterizing them.
		*/
		float delta_;

		/** Debye-Hueckel parameter describing salt effects (ionic strength)
		    in units of 1/Angstrom.
		*/
		float kappa_;

		/** Radius offset (see [2] for details).
		*/
		float rho_0_;

		/**
		 */
		float to_kJ_mol_;

		/** Calculate the Born radii of all atoms
		*/
		void calculateBornRadii_();
		
		/** Calculate the energetic cost of creating a charge distribution in
				a uniform dielectric
		*/
		float calculateCreationEnergy_() const;

		/** Calculate the energetic cost of transfering a solute into a
				dielectric medium.
		*/
		float calculateTransferEnergy_() const;

		/** A smooth function describing an approximation of the solute volume
				and several Born-dependent effects (including Coulomb integrals).
				There are several techniques in usage. We implement the on
				described in [1].
		*/
		float f_GB_(Size i, Size j) const;

		/** An approximation of the coulomb integral
		*/
		float coulombIntegralApproximation_(float distance, float scaled_radius_j,
				float radius_i) const;



	};

 
} // namespace BALL

#endif // BALL_SOLVATION_GENERALIZED_BORN_H
