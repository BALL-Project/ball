// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 H1b (D66a): PUBLIC container-handle API types.
//
// ContainerKind + ContainerChildRef are the public-facing types of the
// container value-handle layer (containerHandle.h) and the scalar
// container accessors on MoleculeStore. They are deliberately kept in a
// small public header so the public handle header never needs to include
// the private _moleculeStoreInternal.h (which the D31b CI grep gate
// forbids leaking into public/iterator headers).
//
// ContainerKind lives HERE (single definition). _moleculeStoreInternal.h
// includes this header rather than redefining the enum, so the internal
// ContainerTable / ContainerRow and the public handle layer share one
// ContainerKind type (no ODR conflict). The internal ChildRef
// (kind:u8 + idx, with CONTAINER/ATOM tags) stays private; the public
// ContainerChildRef below is the boundary-crossing equivalent that the
// MoleculeStore scalar accessor returns.
//

#ifndef BALL_KERNEL_CONTAINERKIND_H
#define BALL_KERNEL_CONTAINERKIND_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <cstdint>

namespace BALL
{
	/** Container kind tag for the v2.2 flat-SoA molecular hierarchy
			(D58). Atoms/Bonds are NOT containers — they have their own SoA
			columns + bond table — so this tag covers only the molecular
			container kinds (Molecule/Chain/Residue/...).

			\ingroup KernelContainers
	*/
	enum class ContainerKind : std::uint8_t
	{
		NONE                = 0,
		MOLECULE            = 1,
		PROTEIN             = 2,
		CHAIN               = 3,
		RESIDUE             = 4,
		SECONDARY_STRUCTURE = 5,
		NUCLEOTIDE          = 6,
		NUCLEIC_ACID        = 7,
		FRAGMENT            = 8,
	};

	/** Public child-reference value type (D66a). The boundary-crossing
			equivalent of the internal ChildRef: a container's ordered child
			is either a child atom (an atom store index) or a child container
			(a container-row index). `is_atom` discriminates which table
			`idx` indexes.

			Returned by `MoleculeStore::container_child_()` and by the
			container handles' `getChild()`. Keeps the private ChildRef out
			of the public handle header.

			\ingroup KernelContainers
	*/
	struct ContainerChildRef
	{
		bool          is_atom = false;
		std::uint32_t idx     = 0;

		ContainerChildRef() = default;
		ContainerChildRef(bool atom, std::uint32_t i) : is_atom(atom), idx(i) {}

		bool operator==(const ContainerChildRef& o) const
		{ return is_atom == o.is_atom && idx == o.idx; }
		bool operator!=(const ContainerChildRef& o) const { return !(*this == o); }
	};

} // namespace BALL

#endif // BALL_KERNEL_CONTAINERKIND_H
