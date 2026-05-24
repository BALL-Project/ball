// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 HCP-2b.1: BALL::StructureQuery free-function namespace.
//
// The polymer-structure questions the v0 typed containers answered via member
// functions + typed iterators (Protein::residues()/chains()/getResidueByID/
// getNTerminal/getCTerminal, Chain::getNTerminal/getCTerminal, ...) re-expressed
// as free functions over the HCP-2a ROLE-AWARE handles (MoleculeHandle /
// FragmentHandle) + the HCP-1a role columns. Canonical-depth-honouring: a query
// finds RESIDUE-role fragments whether they sit directly under a chain or under a
// chain -> SecondaryStructure layer, because it filters by ROLE over the whole
// subtree -- so it survives the HCP-2d SS-as-annotation change and the H4
// ContainerKind shrink unchanged (V22-HCP2-DESIGN.md, HCP-2b, D-2b.1..D-2b.5).
//
// READ-ONLY (header-only): walks via ContainerHandleBase::countChildren() +
// getChildContainer(i); no store mutation, no new store state. Entry point is a
// MoleculeHandle (there is no SYSTEM container row); the caller obtains it from
// the v0 Molecule's container row during dual existence.
//
// ENCAPSULATION (D66a): includes ONLY the public containerHandle.h (+ <vector>),
// never <BALL/KERNEL/_moleculeStoreInternal.h>.
//
// SCOPE: the deprecated role-filtered iterator alias for the ~40-file v0
// ResidueIterator/ChainIterator consumer surface is HCP-2b.2 (D-HC5), not here.
//

#ifndef BALL_KERNEL_STRUCTUREQUERY_H
#define BALL_KERNEL_STRUCTUREQUERY_H

#ifndef BALL_KERNEL_CONTAINERHANDLE_H
# include <BALL/KERNEL/containerHandle.h>
#endif

#include <vector>

namespace BALL
{
	/** Free-function structure queries over the role-aware container handles
			(HCP-2b.1). All functions are READ-only and header-only; single-result
			queries return a null FragmentHandle (`operator bool` false) when absent
			(the D61 null-handle idiom).

			\ingroup KernelContainers
	*/
	namespace StructureQuery
	{
		namespace detail
		{
			/** Visit every fragment-level container in `node`'s subtree in PREORDER
					(parent before children). Atoms are leaves and are skipped. `visit`
					is called once per fragment container with a bound FragmentHandle.
			*/
			template <typename Visitor>
			inline void visitFragments_(const ContainerHandleBase& node, Visitor&& visit)
			{
				std::size_t n = node.countChildren();
				for (std::size_t i = 0; i < n; ++i)
				{
					ContainerHandleBase c = node.getChildContainer(i);
					if (!c) continue;                       // atom or out-of-range -> skip
					FragmentHandle f = c.as<FragmentHandle>();
					if (f) visit(f);                         // preorder: parent first
					visitFragments_(c, visit);               // then its subtree
				}
			}
		} // namespace detail

		/** All fragments of role `role` in `root`'s subtree, in preorder. This is
				the building block; the role-specific queries below are sugar over it.
		*/
		inline std::vector<FragmentHandle> fragmentsByRole(const ContainerHandleBase& root,
		                                                    FragmentRole role)
		{
			std::vector<FragmentHandle> out;
			detail::visitFragments_(root, [&](const FragmentHandle& f)
			{
				if (f.getFragmentRole() == role) out.push_back(f);
			});
			return out;
		}

		/// Direct/indirect CHAIN-role fragments of a molecule (v0 Protein::chains()).
		inline std::vector<FragmentHandle> chains(const MoleculeHandle& m)
		{ return fragmentsByRole(m, FragmentRole::CHAIN); }

		/// All RESIDUE-role fragments in a molecule's subtree (v0 Protein::residues()).
		inline std::vector<FragmentHandle> residues(const MoleculeHandle& m)
		{ return fragmentsByRole(m, FragmentRole::RESIDUE); }

		/// All RESIDUE-role fragments in a chain's / SS's subtree (v0 Chain::residues()).
		inline std::vector<FragmentHandle> residues(const FragmentHandle& f)
		{ return fragmentsByRole(f, FragmentRole::RESIDUE); }

		/// All SECONDARY_STRUCTURE-role fragments in a molecule's subtree.
		inline std::vector<FragmentHandle> secondaryStructures(const MoleculeHandle& m)
		{ return fragmentsByRole(m, FragmentRole::SECONDARY_STRUCTURE); }

		/// All SECONDARY_STRUCTURE-role fragments in a chain's subtree.
		inline std::vector<FragmentHandle> secondaryStructures(const FragmentHandle& f)
		{ return fragmentsByRole(f, FragmentRole::SECONDARY_STRUCTURE); }

		/** Generic first RESIDUE-role fragment in `root`'s subtree (preorder), NO
				kind filter. Building block; use `nTerminal` for the v0 N-terminus.
		*/
		inline FragmentHandle firstResidue(const ContainerHandleBase& root)
		{
			FragmentHandle found;
			detail::visitFragments_(root, [&](const FragmentHandle& f)
			{
				if (!found && f.getFragmentRole() == FragmentRole::RESIDUE) found = f;
			});
			return found;
		}

		/** Generic last RESIDUE-role fragment in `root`'s subtree (preorder), NO
				kind filter. Building block; use `cTerminal` for the v0 C-terminus.
		*/
		inline FragmentHandle lastResidue(const ContainerHandleBase& root)
		{
			FragmentHandle found;
			detail::visitFragments_(root, [&](const FragmentHandle& f)
			{
				if (f.getFragmentRole() == FragmentRole::RESIDUE) found = f;   // keep last
			});
			return found;
		}

		/** Polymer N-terminus (v0 getNTerminal): the FIRST RESIDUE-role fragment in
				preorder that is an AMINO_ACID. v0 filters via isAminoAcid()
				(= hasProperty(PROPERTY__AMINO_ACID), materialised to
				ResidueKind::AMINO_ACID). Null handle if the subtree has no amino acid.
		*/
		inline FragmentHandle nTerminal(const ContainerHandleBase& root)
		{
			FragmentHandle found;
			detail::visitFragments_(root, [&](const FragmentHandle& f)
			{
				if (!found
				    && f.getFragmentRole() == FragmentRole::RESIDUE
				    && f.getResidueKind() == ResidueKind::AMINO_ACID) found = f;
			});
			return found;
		}

		/** Polymer C-terminus (v0 getCTerminal): the LAST RESIDUE-role fragment in
				preorder that is an AMINO_ACID. Null handle if none.
		*/
		inline FragmentHandle cTerminal(const ContainerHandleBase& root)
		{
			FragmentHandle found;
			detail::visitFragments_(root, [&](const FragmentHandle& f)
			{
				if (f.getFragmentRole() == FragmentRole::RESIDUE
				    && f.getResidueKind() == ResidueKind::AMINO_ACID) found = f;  // keep last
			});
			return found;
		}

		/** First RESIDUE-role fragment in `m`'s subtree whose `getID() == id`
				(preorder); null handle if none (v0 Protein::getResidueByID, first match).
		*/
		inline FragmentHandle residueByID(const MoleculeHandle& m, const String& id)
		{
			FragmentHandle found;
			detail::visitFragments_(m, [&](const FragmentHandle& f)
			{
				if (!found
				    && f.getFragmentRole() == FragmentRole::RESIDUE
				    && f.getID() == id) found = f;
			});
			return found;
		}

		/** The enclosing SECONDARY_STRUCTURE-role fragment of `residue` (walk
				parents); null handle if the residue is not under an SS layer. During
				dual existence SS is an owning layer (chain -> SS -> residue); at HCP-2d
				it becomes a non-owning annotation and this query is superseded by an
				annotation lookup -- callers are role-based so they survive that change.
		*/
		inline FragmentHandle secondaryStructureOf(const FragmentHandle& residue)
		{
			ContainerHandleBase p = residue.getParent();
			while (p)
			{
				FragmentHandle f = p.as<FragmentHandle>();
				if (f && f.getFragmentRole() == FragmentRole::SECONDARY_STRUCTURE) return f;
				p = p.getParent();
			}
			return FragmentHandle();
		}

	} // namespace StructureQuery

} // namespace BALL

#endif // BALL_KERNEL_STRUCTUREQUERY_H
