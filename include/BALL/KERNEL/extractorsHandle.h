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

#ifndef BALL_KERNEL_EXPRESSIONPREDICATE_H
# include <BALL/KERNEL/expressionPredicate.h>  // H3b.5: ExpressionPredicate overload
#endif

#ifndef BALL_KERNEL_COMPILEDEXPRESSION_H
# include <BALL/KERNEL/compiledExpression.h>   // H3b.5: compiled-bitmap path
#endif

#ifndef BALL_KERNEL_MOLECULESTORE_H
# include <BALL/KERNEL/moleculeStore.h>        // H3b.5: store used by evaluate()
#endif

#include <vector>
#include <unordered_set>
#include <cstdint>
#include <type_traits>

namespace BALL
{
	/** Bridge a v0 `AtomContainer` to its container-row `ContainerHandleBase`
			via the public `getContainerRowStore_` / `getContainerRow_` accessors
			(the H2a/b binding). Returns a null handle if the container has not
			been adopted into a store.
	*/
	inline ContainerHandleBase asContainerHandle(const AtomContainer& c)
	{
		// H3b-CR NIT 5: the accessors are const-qualified (Composite.h:1640-1641
		// + AtomContainer.h:411-412) so no const_cast is needed; the binding is
		// genuinely a read-only query.
		MoleculeStore* store = c.getContainerRowStore_();
		std::uint32_t row = c.getContainerRow_();
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
	template <typename Predicate,
	          typename = typename std::enable_if<
	              std::is_invocable<Predicate&, const AtomHandle&>::value
	          >::type>
	inline std::vector<AtomHandle> atomHandlesIf(const AtomContainer& fragment,
	                                             Predicate&& predicate)
	{
		std::vector<AtomHandle> all = atomHandles(fragment);
		std::vector<AtomHandle> out;
		for (Size i = 0; i < all.size(); ++i)
			if (predicate(all[i])) out.push_back(all[i]);
		return out;
	}

	// --- v2.2 H3b.5: ExpressionPredicate + CompiledExpression integration.
	// Sugar for code that already uses the v0 predicate hierarchy (TruePred /
	// SelectedPred / ResiduePred / standardPredicates.h) — bridge through the
	// v0 Atom under each handle. Predicates that operate on Composite-shape
	// (e.g. ResiduePredicate) keep working because Atom retains its v0 parent
	// links until H4. Atoms with no v0 backing are skipped (consistent with
	// the H3b.2 Expression-filtered overload).

	/// ExpressionPredicate overload: yields handles whose v0 Atom matches `p`.
	inline std::vector<AtomHandle> atomHandlesIf(const AtomContainer& fragment,
	                                             const ExpressionPredicate& p)
	{
		std::vector<AtomHandle> all = atomHandles(fragment);
		std::vector<AtomHandle> out;
		for (Size i = 0; i < all.size(); ++i)
		{
			Atom* a = all[i].getAtom();
			if (a != 0 && p(*a)) out.push_back(all[i]);
		}
		return out;
	}

	/** CompiledExpression overload: amortizes parse over the whole subtree
			via the store-wide bitmap evaluator (one O(N_store) pass), then
			intersects with the subtree atoms. Much cheaper than `evaluate_one`
			per atom when N_subtree is large or the predicate tree is deep.
			Throws `Exception::InvalidArgument` if `compiled` was compiled
			against a different store, or if its compile generation is stale
			(see CompiledExpression::evaluate).
	*/
	inline std::vector<AtomHandle> atomHandlesBy(const AtomContainer& fragment,
	                                              const CompiledExpression& compiled)
	{
		std::vector<AtomHandle> out;
		std::vector<AtomHandle> all = atomHandles(fragment);
		if (all.empty()) return out;

		// H3b-CR FLAW 4: defensively enforce the single-store invariant. Today
		// asContainerHandle + atom traversal can only yield handles bound to the
		// fragment's container-row store, but turning a latent mirror corruption
		// into a loud failure (rather than silent bitmap-miss filtering) is
		// strictly safer in an additive migration.
		MoleculeStore* store = all[0].getStore();
		if (store == 0) return out;
		for (Size i = 1; i < all.size(); ++i)
		{
			if (all[i].getStore() != store)
			{
				throw Exception::InvalidArgument(__FILE__, __LINE__,
					"atomHandlesBy: subtree spans multiple stores -- "
					"CompiledExpression is store-specific (D-H3 invariant)");
			}
		}

		std::vector<std::uint8_t> bitmap;
		compiled.evaluate(*store, bitmap);

		for (Size i = 0; i < all.size(); ++i)
		{
			std::uint32_t idx = all[i].getStoreIndex();
			if (idx < bitmap.size() && bitmap[idx] != 0) out.push_back(all[i]);
		}
		return out;
	}

	// --- v2.2 H3b.3 (H3b-CR FLAW 1 fix): typed container-handle extractors
	// over a v0 AtomContainer root. Analogs of extractors.h's residues / chains
	// / secondaryStructures (which iterate `beginAtomContainer()` -- which
	// VISITS THE ROOT FIRST in v0 Composite traversal). To preserve parity we
	// must emit `root` itself if it matches, then the matching descendants.
	// This matches `for (it = c.beginAtomContainer(); +it; ++it)`'s behavior
	// for `c` itself.
	inline std::vector<FragmentHandle> residueHandles(const AtomContainer& fragment)
	{
		std::vector<FragmentHandle> out;
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return out;
		FragmentHandle root_f = root.as<FragmentHandle>();
		if (root_f && root_f.getFragmentRole() == FragmentRole::RESIDUE)
			out.push_back(root_f);
		std::vector<FragmentHandle> sub =
			StructureQuery::fragmentsByRole(root, FragmentRole::RESIDUE);
		for (Size i = 0; i < sub.size(); ++i) out.push_back(sub[i]);
		return out;
	}

	inline std::vector<FragmentHandle> chainHandles(const AtomContainer& fragment)
	{
		std::vector<FragmentHandle> out;
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return out;
		FragmentHandle root_f = root.as<FragmentHandle>();
		if (root_f && root_f.getFragmentRole() == FragmentRole::CHAIN)
			out.push_back(root_f);
		std::vector<FragmentHandle> sub =
			StructureQuery::fragmentsByRole(root, FragmentRole::CHAIN);
		for (Size i = 0; i < sub.size(); ++i) out.push_back(sub[i]);
		return out;
	}

	inline std::vector<FragmentHandle> secondaryStructureHandles(const AtomContainer& fragment)
	{
		std::vector<FragmentHandle> out;
		ContainerHandleBase root = asContainerHandle(fragment);
		if (!root) return out;
		FragmentHandle root_f = root.as<FragmentHandle>();
		if (root_f && root_f.getFragmentRole() == FragmentRole::SECONDARY_STRUCTURE)
			out.push_back(root_f);
		std::vector<FragmentHandle> sub =
			StructureQuery::fragmentsByRole(root, FragmentRole::SECONDARY_STRUCTURE);
		for (Size i = 0; i < sub.size(); ++i) out.push_back(sub[i]);
		return out;
	}

	// --- v2.2 H3b.4: ContainerKind-level extractors. Forward-stable analogs
	// of extractors.h's molecules / proteins / nucleicAcids / fragments. Since
	// HCP-2 collapses Protein + NucleicAcid into MOLECULE and Chain / Residue
	// / SecondaryStructure / Nucleotide into FRAGMENT, we group at the H4-
	// stable {MOLECULE, FRAGMENT} layer: `moleculeHandles` matches any
	// molecule-equivalent kind today (MOLECULE | PROTEIN | NUCLEIC_ACID), and
	// `fragmentHandles` matches any fragment-equivalent kind (FRAGMENT |
	// CHAIN | RESIDUE | SECONDARY_STRUCTURE | NUCLEOTIDE). Typed
	// `proteinHandles` / `nucleicAcidHandles` are intentionally NOT added --
	// they disappear at H4.
	//
	// **HCP-2 shrink follow-up (H3b-CR FLAW 2):** at the HCP-2 commit that
	// removes PROTEIN / NUCLEIC_ACID / CHAIN / RESIDUE / SECONDARY_STRUCTURE
	// / NUCLEOTIDE from `enum class ContainerKind`, the obsolete arms of
	// `isMoleculeKind_` / `isFragmentKind_` STOP COMPILING and must be
	// removed in the same commit (only `MOLECULE` / `FRAGMENT` survive).
	// This is intentional: a hard-fail at the shrink commit catches every
	// caller in one go. Logged in V22-API-BREAK-LEDGER.md.
	namespace detail
	{
		inline bool isMoleculeKind_(ContainerKind k)
		{
			return k == ContainerKind::MOLECULE
			    || k == ContainerKind::PROTEIN
			    || k == ContainerKind::NUCLEIC_ACID;
		}
		inline bool isFragmentKind_(ContainerKind k)
		{
			return k == ContainerKind::FRAGMENT
			    || k == ContainerKind::CHAIN
			    || k == ContainerKind::RESIDUE
			    || k == ContainerKind::SECONDARY_STRUCTURE
			    || k == ContainerKind::NUCLEOTIDE;
		}

		// Local recursive walker -- preorder, all kinds, no skipping. (We
		// don't reuse StructureQuery::containersByKind which is single-kind:
		// we want any-of-set membership in one pass.) NIT 7: visitor pattern
		// avoids per-node temporary-vector allocation.
		template <typename KindPred, typename Visit>
		inline void visitContainersIf_(const ContainerHandleBase& root,
		                                KindPred&& kp, Visit&& v)
		{
			std::size_t n = root.countChildren();
			for (std::size_t i = 0; i < n; ++i)
			{
				ContainerHandleBase c = root.getChildContainer(i);
				if (!c) continue;
				if (kp(c.getKind())) v(c);
				visitContainersIf_(c, kp, v);
			}
		}
	} // namespace detail

	inline std::vector<MoleculeHandle> moleculeHandles(const AtomContainer& root_c)
	{
		std::vector<MoleculeHandle> out;
		ContainerHandleBase root = asContainerHandle(root_c);
		if (!root) return out;
		// FLAW 1 fix: include root if it itself is molecule-kind (v0 parity).
		if (detail::isMoleculeKind_(root.getKind()))
		{
			MoleculeHandle root_m = root.as<MoleculeHandle>();
			if (root_m) out.push_back(root_m);
		}
		detail::visitContainersIf_(root, detail::isMoleculeKind_,
			[&](const ContainerHandleBase& c)
			{
				MoleculeHandle m = c.as<MoleculeHandle>();
				if (m) out.push_back(m);
			});
		return out;
	}

	// FRAGMENT-kind containers in `root_c`'s subtree -- the role-agnostic
	// counterpart of residueHandles / chainHandles / secondaryStructureHandles.
	// Analog of v0 extractors.h::fragments(c).
	inline std::vector<FragmentHandle> fragmentHandles(const AtomContainer& root_c)
	{
		std::vector<FragmentHandle> out;
		ContainerHandleBase root = asContainerHandle(root_c);
		if (!root) return out;
		// FLAW 1 fix: include root if it itself is fragment-kind.
		if (detail::isFragmentKind_(root.getKind()))
		{
			FragmentHandle root_f = root.as<FragmentHandle>();
			if (root_f) out.push_back(root_f);
		}
		detail::visitContainersIf_(root, detail::isFragmentKind_,
			[&](const ContainerHandleBase& c)
			{
				FragmentHandle f = c.as<FragmentHandle>();
				if (f) out.push_back(f);
			});
		return out;
	}

	/** Handle analog of `extractors.h::bonds(const AtomContainer&, bool)` --
			returns each UNIQUE bond of the subtree's atoms as `BondHandle`s,
			dedup-keyed on `bond StableId` (the D-H3.8 durable identity, NOT raw
			Bond* pointers). When `selected_only` is true, matches v0 semantics
			(extractors.C:108-130): iterate the SELECTED atoms only and accept
			ALL their bonds -- so a bond participates iff AT LEAST ONE endpoint
			is in the selected-atom set. (The H3b.1 first cut required BOTH
			endpoints selected; H3b-CR FLAW 3 restored v0 parity.)
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
				if (seen.insert(b.getStableId()).second)
					out.push_back(b);
			}
		}
		return out;
	}

} // namespace BALL

#endif // BALL_KERNEL_EXTRACTORSHANDLE_H
