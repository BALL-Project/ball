// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: generalizedBorn.h,v 1.2 2004/10/30 08:27:38 anker Exp $
//

#ifndef BALL_SOLVATION_GENERALIZED_BORN_H
#define BALL_SOLVATION_GENERALIZED_BORN_H

#include <ostream>

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_HASHGRID_H
#include <BALL/DATATYPE/hashGrid.h>
#endif

namespace BALL
{

	/**

	Generalized Born model for calculating the electrostatic part of the
	solvation free energy (taken from [1]).

	[1] Lee at al., J. Chem. Phys. 116(24):10606-10614, 2002
	"Novel generalized Born methods"

	*/
	class GeneralizedBorn
	{

		public:

			GeneralizedBorn();
			GeneralizedBorn(const AtomContainer& atom_container);

			bool setup(const AtomContainer& atom_container);
			bool setup();
			void update();

			void setSolventDC(float solvent_dc);
			void setSoluteDC(float solute_dc);

			float calculateEnergy();

			void dump(std::ostream& os = std::cout);

		private:

			bool setupLookupGrid_();
			bool updateLookupGrid_();
			bool setupIntegrationPoints_();
			bool setupIntegrationWeights_();

			void calculateBornRadii_();
			float calculateAtomicVolumeContribution_(const Vector3& r);
			float integrate_(const Atom* atom);

			// Constants that describe the physical properties
			float k_;
			float k_prime_;
			float eps_solvent_;
			float eps_solute_;

			const AtomContainer* atom_container_;
			HashMap<const Atom*, float> born_radii_;
			HashGrid3<const Atom*> lookup_grid_;
			// The helper grid is used for sppeding up the update of the lookup_grid_
			HashGrid3<const Atom*> helper_grid_;
			vector<float> shell_radii_;
			vector<Vector3> integration_points_;
			vector<float> integration_weights_CFA_;
			vector<float> integration_weights_CT_;
			HashMap<const Atom*, float> nearest_radius_rec_;

			// These parameters control the approximation
			float beta_;
			float lambda_;
			float gamma_0_;
			float P_;
			float alpha_0_;

			// The following members are used by the lookup grid code only
			float epsilon_;
			float grid_spacing_;
			float r_buffer_;
			float C_;
			float D_;

			// Verbosity
			Size verbosity_;


	};
}

#endif // BALL_SOLVATION_GENERALIZED_BORN_H
