// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/atomBijection.h>
#include <BALL/KERNEL/atomHandle.h>      // v2.2 H3d.B: D-H3.8 opt-in
// v2.2 (PR-removal phase 6): formerly visible via the removed persistenceManager.h include chain
#include <BALL/DATATYPE/stringHashMap.h>

#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/extractors.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/DATATYPE/hashGrid.h>

using namespace std;

namespace BALL
{

	AtomBijection::AtomBijection(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
		: pairs_()           // v2.2 H3d-DR FLAW 6: extracted member, no more base init
	{
		assignByName(A, B, limit_to_selection);
	}

	/* Calculate the root mean squared deviation */
	double AtomBijection::calculateRMSD() const
	{
		double sum_of_squares = 0.0;

		if (!empty())
		{
			// v2.2 H3d.B: resolve sids via atomA/atomB; skip stale entries.
			Size live = 0;
			for (Size i = 0; i < size(); ++i)
			{
				Atom* a = atomA(i);
				Atom* b = atomB(i);
				if (a == nullptr || b == nullptr) continue;
				++live;
				const Vector3& r(a->getPosition());
				sum_of_squares += r.getSquareDistance(b->getPosition());
			}

			// calculate mean square deviation
			if (live > 0)
				sum_of_squares = sqrt(sum_of_squares / (double)live);
		}

		// return RMSD
		return sum_of_squares;
	}

	Size AtomBijection::assignByName(AtomContainer& A, AtomContainer& B, bool limit_to_selection)
	{
		// Clear old bijection.
		clear();

		// v2.2 H3d.B (D-H3.8): name -> StableId index instead of Atom*
		// keys. Resolution back to Atom* happens at push time via the
		// store's reverse map.
		StringHashMap<MoleculeStore::StableId> A_names;
		MoleculeStore* store_A = nullptr;
		for (AtomIterator ai = A.beginAtom(); +ai; ++ai)
		{
			MoleculeStore* s = ai->getStore();
			if (s == nullptr) continue;
			if (store_A == nullptr) store_A = s;
			else if (s != store_A) continue;   // single-store discipline
			A_names.insert(std::pair<String, MoleculeStore::StableId>(
				ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID),
				s->stable_id(ai->getStoreIndex())));
		}

		// Iterate over all atoms of B and try to find an
		// atom in A identical names.
		auto resolve_A = [&](MoleculeStore::StableId sid) -> Atom*
		{
			if (store_A == nullptr) return nullptr;
			MoleculeStore::Index idx = store_A->atom_idx_by_stable_id(sid);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
			return store_A->back_ptr(idx);
		};
		for (AtomIterator ai = B.beginAtom(); +ai; ++ai)
		{
			const String full = ai->getFullName(Atom::ADD_VARIANT_EXTENSIONS_AND_ID);
			if (A_names.has(full))
			{
				Atom* a_atom = resolve_A(A_names[full]);
				if (a_atom == nullptr) { A_names.erase(full); continue; }
				if (   !limit_to_selection
						|| (ai->isSelected() || a_atom->isSelected()))
				{
					push_back(AtomPair(a_atom, &*ai));
				}
				A_names.erase(full);
			}
		}

		// Check whether we could map anything.
		// If not, try to map by atom name alone.
		if (size() == 0)
		{
			// v2.2 H3d.B: Stage 2 with sid-keyed StringHashMap.
			A_names.clear();
			store_A = nullptr;
			for (AtomIterator ai = A.beginAtom(); +ai; ++ai)
			{
				MoleculeStore* s = ai->getStore();
				if (s == nullptr) continue;
				if (store_A == nullptr) store_A = s;
				else if (s != store_A) continue;
				A_names.insert(std::pair<String, MoleculeStore::StableId>(
					ai->getName(), s->stable_id(ai->getStoreIndex())));
			}
			clear();
			for (AtomIterator ai = B.beginAtom(); +ai; ++ai)
			{
				if (A_names.has(ai->getName()))
				{
					Atom* a_atom = resolve_A(A_names[ai->getName()]);
					if (a_atom == nullptr) { A_names.erase(ai->getName()); continue; }
					if (   !limit_to_selection
							|| (ai->isSelected() || a_atom->isSelected()))
					{
						push_back(AtomPair(a_atom, &*ai));
					}
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
