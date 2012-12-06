// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/SEQUENCE/homologyModeller.h>
#include <BALL/SEQUENCE/alignment.h>

#include <BALL/STRUCTURE/residueRotamerSet.h>
#include <BALL/STRUCTURE/rotamerLibrary.h>

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/residue.h>

#include <BALL/MATHS/matrix44.h>


namespace BALL
{

	HomologyModeller::HomologyModeller()
	{   }

	HomologyModeller::~HomologyModeller()
	{   }

	bool HomologyModeller::createSimpleModel(Alignment& alignment, Protein& query)
	{
		if (alignment.empty())
		{
			return false;
		}

		if (!alignment.isAligned())
		{
			return false;
		}

		// find out which aligned sequence corresponds to the query protein
		bool found_query = false;
		int  query_alignment_idx = 0;

		// iterate through every sequence in the alignment
		for (unsigned int al_row = 0; al_row < alignment.rows(); al_row++)
		{
			if ( (alignment.getSequence(al_row).getOrigin()) == &query)
			{
				found_query = true;
				query_alignment_idx =  al_row;
			}
		}

		if (!found_query)
		{
			Log.info() << "Protein could not be found in the alignment." << std::endl;
			return false;
		}

		// we currently simply take the first structure as template
		// TODO!!
		int  template_alignment_idx = query_alignment_idx;
		for (unsigned int al_row = 0; al_row < alignment.rows(); al_row++)
		{
			if (alignment.getSequence(al_row).getOrigin() != &query)
			{
				template_alignment_idx = al_row;
			}
		}

		if (template_alignment_idx == query_alignment_idx)
		{
			Log.info() << "No template could be found in the alignment." << std::endl;
			return false;
		}

		// build the model
		bool unmapped_atoms     = false;
		bool structure_has_gaps = false;

		//iterate through every character in the current sequence
		for (unsigned int al_char = 0; al_char < alignment.cols(); al_char++)
		{
			Residue* current_query    = alignment.getSeqChar(query_alignment_idx, al_char).getResidue();
			Residue* current_template = alignment.getSeqChar(template_alignment_idx, al_char).getResidue();

			if (current_query && current_template)
			{
				// find all matching atoms
				for (AtomIterator at_it_q = current_query->beginAtom(); +at_it_q; at_it_q++)
				{
					bool found_atom = false;
					for (AtomIterator at_it_t = current_template->beginAtom(); +at_it_t; at_it_t++)
					{
						if (at_it_q->getName() == at_it_t->getName())
						{
							at_it_q->setPosition(at_it_t->getPosition());
							found_atom = true;
						}
					}
					if (!found_atom)
					{
						unmapped_atoms = true;
						at_it_q->getResidue()->setProperty("HOMOLOGY_MODELLER_SKIPPED", true);
					}
				}
			}
			else
			{
				structure_has_gaps = true;
			}
		}

		if (structure_has_gaps)
		{
			Log.warn() << " There might be structural gaps in the query." << std::endl;
		}
		if (unmapped_atoms)
		{
			Log.warn() << " Some atoms in the query protein could not be matched to the target and thus have no reasonable position." << std::endl;
		}

		return true;
	}

	bool HomologyModeller:: fixSkippedAtomsByRotamerLibrary(Protein& query, String property_name)
	{
		// use the rotamer library to identify best side-chain placement
		RotamerLibrary rotamer_library;

		bool use_property_limitation = (property_name != "");

		for (ResidueIterator res_it = query.beginResidue(); +res_it; ++res_it)
		{
			if (use_property_limitation && res_it->hasProperty(property_name))
			{
				ResidueRotamerSet* res_set = rotamer_library.getRotamerSet(*res_it);

				// Helper variables for finding the closest rotamer
				unsigned int i_min = 0;
				float i_min_val = std::numeric_limits<float>::infinity();
				const Rotamer r = res_set->getRotamer(*res_it);

				unsigned int i = 0;
				for(ResidueRotamerSet::iterator it = res_set->begin(); it != res_set->end(); ++it, ++i)
				{
					// Determine the closest residue
					float dist = fabs(r.chi1 - it->chi1) + fabs(r.chi2 - it->chi2)
										 + fabs(r.chi3 - it->chi3) + fabs(r.chi4 - it->chi4);

					if (dist < i_min_val)
					{
						i_min = i;
						i_min_val = dist;
					}
				}
				// apply
				res_set->setRotamer(*res_it, res_set->getRotamer(i_min));
			}
		}
		return true;
	}


}//namespace BALL
