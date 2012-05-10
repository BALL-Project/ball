/* rescoring1D.C
 *
 * Copyright (C) 2011 Marcel Schumann
 *
 * This program free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

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


		void Rescoring1D::generateScoreContributions_(Molecule* mol, vector<vector<double> >* matrix, vector<double>* v)
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
