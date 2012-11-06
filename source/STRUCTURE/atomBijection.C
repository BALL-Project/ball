// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/atomBijection.h>

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/extractors.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/DATATYPE/hashGrid.h>

using namespace std;

namespace BALL
{

	AtomBijection::AtomBijection(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
		: std::vector<std::pair<Atom*, Atom*> >()
	{
		assignByName(A, B, limit_to_selection);
	}

	/* Calculate the root mean squared deviation */
	double AtomBijection::calculateRMSD() const
	{
		double sum_of_squares = 0.0;

		if (!empty())
		{
			for (Size i = 0; i < size(); ++i)
			{
				Vector3& r(operator [] (i).first->getPosition());
				sum_of_squares += r.getSquareDistance(operator [] (i).second->getPosition());
			}

			// calculate mean square deviation
			sum_of_squares = sqrt(sum_of_squares / (double)size());
		}

		// return RMSD
		return sum_of_squares;
	}

	Size AtomBijection::assignByName(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
	{
		// Clear old bijection.
		clear();

		// Remember the names of A and their atom pointers.
		StringHashMap<Atom*> A_names;
		for (AtomIterator ai = A.beginAtom(); +ai; ++ai)
		{
			A_names.insert(std::pair<String, Atom*>(ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID), &*ai));
		}

		// Iterate over all atoms of B and try to find an 
		// atom in A identical names.
		for (AtomIterator ai = B.beginAtom(); +ai; ++ai)
		{
			if (A_names.has(ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID)))
			{
				if (   !limit_to_selection
						|| (ai->isSelected() || A_names[ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID)]->isSelected()))
				{
					// We found two matching atoms. Remember them.
					push_back(AtomPair(A_names[ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID)], &*ai));
				}
				// Throw away the hash map entry in order to avoid
				// 1:n mappings.
				A_names.erase(ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID));
			}
		}

		// Check whether we could map anything. 
		// If not, try to map by atom name alone.
		if (size() == 0)
		{
			// Next stage: try to map by atom name only.
			A_names.clear();
			for (AtomIterator ai = A.beginAtom(); +ai; ++ai)
			{
				A_names.insert(std::pair<String, Atom*>(ai->getName(), &*ai));
			}
			clear();
			for (AtomIterator ai = B.beginAtom(); +ai; ++ai)
			{
				if (A_names.has(ai->getName()))
				{
					if (   !limit_to_selection
							|| (ai->isSelected() || A_names[ai->getName()]->isSelected()))
					{
						// We found two matching atoms. Remember them.
						push_back(std::pair<Atom*, Atom*>(A_names[ai->getName()], &*ai));
					}
					// Throw away the hash map entry in order to avoid
					// 1:n mappings.
					A_names.erase(ai->getName());
				}
			}
		}

		return size();
	}

	Size AtomBijection::assignTrivial(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
	{
		// Delete old bijection.
		clear();

		// Map in order -- first atom of A onto
		// first atom of B and so on.
		AtomIterator ai(A.beginAtom());
		AtomIterator bi(B.beginAtom());
		for (; +ai && +bi; ++ai, ++bi)
		{
			if (   !limit_to_selection
					|| (ai->isSelected() || bi->isSelected()))
			{
				push_back(std::pair<Atom*, Atom*>(&*ai, &*bi));
			}
		}

		return size();
	}

	Size AtomBijection::assignCAlphaAtoms(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
	{
		// Delete old bijection.
		clear();

		// Extract all residues in A and B
		ResidueList rla(residues(A));
		ResidueList rlb(residues(B));

		// Walk over the residues in parallel
		ResidueList::iterator ita(rla.begin());
		ResidueList::iterator itb(rlb.begin());
		for (; ita != rla.end() && itb != rlb.end(); ++ita, ++itb)
		{
			// If the two residues do have an atom named CA, push back the pair.
			Atom* caa = (*ita)->getAtom("CA");
			Atom* cab = (*itb)->getAtom("CA");
			if (    (caa != 0) && (cab != 0)
					&&  (   !limit_to_selection
					     || (caa->isSelected() || cab->isSelected())))
			{
				push_back(AtomPair(caa, cab));
			}
		}
		//
		return size();
	}

	Size AtomBijection::assignBackboneAtoms(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
	{
		// Delete old bijection.
		clear();

		// Extract all residues in A and B
		ResidueList rla(residues(A));
		ResidueList rlb(residues(B));

		// Walk over the residues in parallel
		ResidueList::iterator ita(rla.begin());
		ResidueList::iterator itb(rlb.begin());
		for (; ita != rla.end() && itb != rlb.end(); ++ita, ++itb)
		{
			// If the two residues do have backbone atoms (CA, C, N, O, H)
			// map then onto each other.
			Atom* a = (*ita)->getAtom("CA");
			Atom* b = (*itb)->getAtom("CA");
			if (    (a != 0) && (b != 0)
				  &&  (   !limit_to_selection
					     || (a->isSelected() || b->isSelected())))
			{
				push_back(AtomPair(a, b));
			}
			a = (*ita)->getAtom("C");
			b = (*itb)->getAtom("C");
			if (    (a != 0) && (b != 0)
				  &&  (   !limit_to_selection
					     || (a->isSelected() || b->isSelected())))
			{
				push_back(AtomPair(a, b));
			}
			a = (*ita)->getAtom("N");
			b = (*itb)->getAtom("N");
			if (    (a != 0) && (b != 0)
					&&  (   !limit_to_selection
					     || (a->isSelected() || b->isSelected())))
			{
				push_back(AtomPair(a, b));
			}
			a = (*ita)->getAtom("O");
			b = (*itb)->getAtom("O");
			if (    (a != 0) && (b != 0)
			    &&  (   !limit_to_selection
					     || (a->isSelected() || b->isSelected())))
			{
				push_back(AtomPair(a, b));
			}
			a = (*ita)->getAtom("H");
			b = (*itb)->getAtom("H");
			if (    (a != 0) && (b != 0)
					&&  (   !limit_to_selection
					     || (a->isSelected() || b->isSelected())))
			{
				push_back(AtomPair(a, b));
			}
		}
		//
		return size();
	}

	Size AtomBijection::assignAtomsByProperty(AtomContainer& A, AtomContainer& B)
	{
		// Delete old bijection.
		clear();
		// Map in order -- first atom of A onto
		// first atom of B and so on.
		AtomIterator ai(A.beginAtom());
		AtomIterator bi(B.beginAtom());
		for (; +ai && +bi; ++ai, ++bi)
		{
			if ( ai->hasProperty("ATOMBIJECTION_RMSD_SELECTION") || bi->hasProperty("ATOMBIJECTION_RMSD_SELECTION"))
			{
				push_back(std::pair<Atom*, Atom*>(&*ai, &*bi));
			}
		}
		return size();
	}



} // namespace BALL
