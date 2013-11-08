// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/rescoring1D.h>
#include <BALL/KERNEL/molecularInteractions.h>


namespace BALL
{

		Rescoring1D::Rescoring1D(AtomContainer& receptor, AtomContainer& reference_ligand, Options& options, String free_energy_label, ScoringFunction* sf)
			: Rescoring(receptor, reference_ligand, options, free_energy_label, sf)
		{
			name_ = "Rescoring1D";
			use_calibration_ = 1;
			setup_();
		}

		void Rescoring1D::setup_()
		{
			scoring_function_->enableStoreInteractions(1);

			Protein* protein = dynamic_cast<Protein*>(scoring_function_->getReceptor());
			if (!protein)
			{
				System* sys = dynamic_cast<System*>(scoring_function_->getReceptor());
				if (sys)
				{
					protein = sys->getProtein(0);
				}
			}
			if (protein)
			{
				protein_ = protein;
			}
			else
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Rescoring1D setup error", "No Protein found in receptor object");
			}
		}


		void Rescoring1D::generateScoreContributions_(vector<vector<double> >* matrix, vector<double>* v)
		{
			Size column_no = 0;

			for (ResidueConstIterator it = protein_->beginResidue(); +it; it++, column_no++)
			{
				double score = 0;
				for (AtomConstIterator it2 = it->beginAtom(); +it2; it2++)
				{
					if (it2->interactions) score += it2->interactions->getInteractionEnergy();
				}

				if (matrix)
				{
					(*matrix)[column_no].push_back(score);
				}
				else if (v)
				{
					v->push_back(score);
				}
			}
		}
}
