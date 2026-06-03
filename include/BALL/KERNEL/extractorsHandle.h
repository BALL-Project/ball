// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 H3b.1 (D-H3.2/D-H3.8): handle-based extractor helpers.
//
// Additive analog of include/BALL/KERNEL/extractors.h. The v0 extractors
// (AtomList atoms(...), BondList bonds(...)) return std::list<Atom*>/<Bond*>
// and stay until H4 (no break here). These helpers return std::vector<
// AtomHandle> / std::vector<BondHandle> — the H3 migration target. Bonds are
// dedup-keyed on bond StableId (D-H3.8), the durable identity policy.
//

#ifndef BALL_KERNEL_EXTRACTORSHANDLE_H
#define BALL_KERNEL_EXTRACTORSHANDLE_H

#ifndef BALL_KERNEL_ATOMHANDLE_H
# include <BALL/KERNEL/atomHandle.h>
#endif

#ifndef BALL_KERNEL_BONDHANDLE_H
# include <BALL/KERNEL/bondHandle.h>
#endif

#ifndef BALL_KERNEL_STRUCTUREQUERY_H
# include <BALL/KERNEL/structureQuery.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>   // v0 -> handle bridge
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
# include <BALL/KERNEL/expression.h>      // H3b.2: predicate-filtered atomHandles
#endif

#include <vector>
#include <unordered_set>

namespace BALL
{
	/** Bridge a v0 `AtomContainer` to its container-row `ContainerHandleBase`
			via the public `getContainerRowStore_` / `getContainerRow_` accessors
			(the H2a/b binding). Returns a null handle if the container has not
			been adopted into a store.
	*/
	inline ContainerHandleBase asContainerHandle(const AtomContainer& c)
	{
		// getContainerRowStore_ / getContainerRow_ are non-const virtual overrides
		// in v0; the binding itself does not mutate the container.
		AtomContainer& nc = const_cast<AtomContainer&>(c);
		MoleculeStore* store = nc.getContainerRowStore_();
		std::uint32_t row = nc.getContainerRow_();
		if (store == nullptr || row == 0) return ContainerHandleBase();
		return ContainerHandleBase(*store, row);
	}

	/** Handle analog of `extractors.h::atoms(const AtomContainer&)` — returns
			the atoms of `fragment`'s subtree as `AtomHandle`s in v0-`Composite`-
			preorder (verified parity, H2c). Returns an empty vector if the
			container has not been bound to a store. (The v0 `AtomList`-returning
			extractor remains in extractors.h through H3-H4.)
	*/
	inline std::vector<AtomHandle> atomHandles(const AtomContainer& fragment)
	{
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return std::vector<AtomHandle>();
		return StructureQuery::atoms(root);
	}

	/** Predicate-filtered handle analog of
			`extractors.h::atoms(const AtomContainer&, const String&)`: returns the
			subtree's `AtomHandle`s matching the BALL `Expression`. Bridges through
			the v0 `Atom` via `getAtom()` (`Expression::operator()(const Atom&)`),
			so atoms with no v0 backing are skipped. Empty expression matches all
			atoms (same behavior as the v0 extractor).
	*/
	inline std::vector<AtomHandle> atomHandles(const AtomContainer& fragment,
	                                           const String& expression)
	{
		std::vector<AtomHandle> all = atomHandles(fragment);
		if (expression == "" || all.empty()) return all;
		std::vector<AtomHandle> out;
		Expression match(expression);
		for (Size i = 0; i < all.size(); ++i)
		{
			Atom* a = all[i].getAtom();
			if (a != 0 && match(*a)) out.push_back(all[i]);
		}
		return out;
	}

	/** Generic predicate-filtered handle extraction. `predicate` is any callable
			invocable as `bool predicate(const AtomHandle&)` — visited in v0-
			`Composite`-preorder. Useful for adapting `UnaryPredicate<AtomHandle>`
			subclasses or lambdas; lets H3b consumers migrate predicate-using code
			off `dynamic_cast`/`Atom*` while the v0 extractor stays.
	*/
	template <typename Predicate>
	inline std::vector<AtomHandle> atomHandlesIf(const AtomContainer& fragment,
	                                             Predicate&& predicate)
	{
		std::vector<AtomHandle> all = atomHandles(fragment);
		std::vector<AtomHandle> out;
		for (Size i = 0; i < all.size(); ++i)
			if (predicate(all[i])) out.push_back(all[i]);
		return out;
	}

	// --- v2.2 H3b.3: typed container-handle extractors over a v0 AtomContainer
	// root. Analogs of extractors.h's residues / chains / secondaryStructures
	// (which use dynamic_cast over the AtomContainer children). Delegate to
	// StructureQuery::fragmentsByRole on the bridged root — role-based, so
	// they survive HCP-2d's SS-as-annotation change and the H4 ContainerKind
	// shrink unchanged.
	inline std::vector<FragmentHandle> residueHandles(const AtomContainer& fragment)
	{
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return std::vector<FragmentHandle>();
		return StructureQuery::fragmentsByRole(root, FragmentRole::RESIDUE);
	}

	inline std::vector<FragmentHandle> chainHandles(const AtomContainer& fragment)
	{
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return std::vector<FragmentHandle>();
		return StructureQuery::fragmentsByRole(root, FragmentRole::CHAIN);
	}

	inline std::vector<FragmentHandle> secondaryStructureHandles(const AtomContainer& fragment)
	{
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return std::vector<FragmentHandle>();
		return StructureQuery::fragmentsByRole(root, FragmentRole::SECONDARY_STRUCTURE);
	}

	/** Handle analog of `extractors.h::bonds(const AtomContainer&, bool)` —
			returns each UNIQUE bond of the subtree's atoms as `BondHandle`s,
			dedup-keyed on `bond StableId` (the D-H3.8 durable identity, NOT raw
			Bond* pointers). When `selected_only` is true, only bonds whose BOTH
			endpoints are selected are returned.
	*/
	inline std::vector<BondHandle> bondHandles(const AtomContainer& fragment,
	                                           bool selected_only = false)
	{
		std::vector<BondHandle> out;
		std::vector<AtomHandle> atoms = atomHandles(fragment);
		if (atoms.empty()) return out;
		std::unordered_set<MoleculeStore::StableId> seen;

		for (Size i = 0; i < atoms.size(); ++i)
		{
			const AtomHandle& a = atoms[i];
			if (selected_only && !a.isSelected()) continue;
			Size n = a.countBonds();
			for (Size j = 0; j < n; ++j)
			{
				BondHandle b = a.getBond(j);
				if (!b) continue;
				if (selected_only)
				{
					AtomHandle f = b.getFirstAtom();
					AtomHandle s = b.getSecondAtom();
					if (!f || !f.isSelected()) continue;
					if (!s || !s.isSelected()) continue;
				}
				if (seen.insert(b.getStableId()).second)
					out.push_back(b);
			}
		}
		return out;
	}

} // namespace BALL

#endif // BALL_KERNEL_EXTRACTORSHANDLE_H
