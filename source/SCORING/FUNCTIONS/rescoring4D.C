#include <BALL/SCORING/FUNCTIONS/rescoring4D.h>
#include <BALL/SCORING/COMPONENTS/aromaticRingStacking.h>
#include <BALL/SCORING/COMPONENTS/polarity.h>
#include <BALL/KERNEL/molecularInteractions.h>


using namespace BALL;


//namespace BALL
//{
	//namespace Docking
	//{

		Rescoring4D::Rescoring4D(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf)
		: Rescoring3D(receptor, reference_ligand, options, free_energy_label, sf)
		{
			name_ = "Rescoring4D";
			use_calibration_ = true;
			reg3D_grid_names_.clear();
			reg3D_grid_names_.push_back("rescore4D ES");
			reg3D_grid_names_.push_back("rescore4D vdW");
			reg3D_grid_names_.push_back("rescore4D fragSolv");
			reg3D_grid_names_.push_back("rescore4D HB");
		}

		// obtain score-contributions for a single atom
		void Rescoring4D::generateAtomScoreContributions_(const Atom* atom, vector<double>& contributions)
		{
			if (!atom)
			{
				contributions.resize(4, 0);
				return;
			}
			if (contributions.size() == 0) contributions.resize(4, 0);
			if (atom->interactions)
			{
				contributions[0] = atom->interactions->getInteractionEnergy("ES");
				contributions[1] = atom->interactions->getInteractionEnergy("vdW");
				contributions[2] = atom->interactions->getInteractionEnergy("fragSolv");
				contributions[3] = atom->interactions->getInteractionEnergy("HB");
			}
		}
	//}
//}
