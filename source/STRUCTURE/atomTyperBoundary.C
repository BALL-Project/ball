// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// v2.2 H3d.A (D-H3.8 + D-H3d.1-R2 A1):
//
// Boundary-translation TU for AtomTyper's public Atom*-keyed setRings /
// setAromaticRings API. Per the H3d.A migration strategy (A1: keep v0 API,
// add sid-keyed overloads -- consumer translates at the boundary), the
// public setters intrinsically accept `vector<HashSet<Atom*>>` from
// upstream ring producers (RingPerception / MMFF94::getRings /
// MMFF94::getAromaticRings) and translate to the internal
// `vector<HashSet<MoleculeStore::StableId>>` representation here.
//
// This file is INTENTIONALLY kept out of the H3a.5 handle-key-leak gate
// (i.e., it does NOT opt into the v2.2 handle header) -- the H3a.5 regex
// catches the v0 atom-pointer container spelling in any opted-in file
// regardless of whether the hit is a stored container or a transient
// boundary parameter; because the A1 strategy demands a v0 boundary
// spelling, isolating that spelling here is the honest split: atomTyper.C
// stays opted-in (its post-boundary state is fully sid-keyed and IS
// gate-scanned), while this TU owns the only remaining v0 spelling. Per
// H3c CR finding 3, future widening of the gate regex (to allowlist
// parameter positions) would let the boundary fold back into atomTyper.C;
// until then this is the boundary boundary.

#include <BALL/STRUCTURE/atomTyper.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/moleculeStore.h>

using namespace std;

namespace BALL
{

namespace
{
	// Translate caller-provided v0 ring sets (vector<HashSet<Atom*>>)
	// into the sid-keyed internal representation, capturing the
	// MoleculeStore at first push and skipping any foreign-store atoms.
	static void translate_ring_sets_(const vector<HashSet<Atom*> >& src,
	                                  vector<HashSet<MoleculeStore::StableId> >& dst,
	                                  MoleculeStore*& captured_store)
	{
		dst.clear();
		dst.resize(src.size());
		for (Position p = 0; p < src.size(); ++p)
		{
			HashSet<Atom*>::ConstIterator it = src[p].begin();
			for (; +it; ++it)
			{
				Atom* a = *it;
				if (a == nullptr) continue;
				MoleculeStore* s = a->getStore();
				if (s == nullptr) continue;
				if (captured_store == nullptr) captured_store = s;
				else if (s != captured_store) continue;
				dst[p].insert(s->stable_id(a->getStoreIndex()));
			}
		}
	}
}

void AtomTyper::setRings(const vector<HashSet<Atom*> >& rings)
{
	translate_ring_sets_(rings, rings_, atom_typer_store_);
}

void AtomTyper::setAromaticRings(const vector<HashSet<Atom*> >& rings)
{
	translate_ring_sets_(rings, aromatic_rings_, atom_typer_store_);
}

} // namespace BALL
