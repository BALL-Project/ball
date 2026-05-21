// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL v2.2 HCP-1 (hierarchy collapse): PUBLIC role-taxonomy types.
//
// The hierarchy collapse (V22-H1bPRIME-DESIGN.md, D-HC1) replaces the eight
// molecular C++ subclasses with FOUR node kinds -- System / Molecule /
// Fragment / Atom -- plus a typed ROLE that carries the former subclass'
// identity. ContainerKind shrinks to {MOLECULE, FRAGMENT}; these role enums
// are the orthogonal identity dimension stored in the container row payload.
//
// This header is PUBLIC + dependency-free (only <cstdint>) so the value-handle
// layer (containerHandle.h) and the MoleculeStore scalar role accessors can
// share one definition without leaking the private internal header (D66a CI
// gate). Mirrors the structure of containerKind.h.
//
// DUAL EXISTENCE: through HCP-1/HCP-2 these roles are ADDED alongside the
// existing ContainerKind tag (the v0 typed tree stays the source of truth, the
// store table is the verified mirror, D60). ContainerKind shrinks + the typed
// handles collapse to role-aware Molecule/Fragment handles at HCP-2; the v0
// classes are deleted at H4.

#ifndef BALL_KERNEL_CONTAINERROLE_H
#define BALL_KERNEL_CONTAINERROLE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <cstdint>

namespace BALL
{
	/** Role of a `Molecule` node (the former Molecule/Protein/NucleicAcid
			distinction + solvent/ligand/ion classification). `Molecule` stays a
			distinct node kind (NOT a Fragment role) -- it preserves System-child
			molecule iteration, orphan construction, import grouping, and solvent
			classification (HC1-3 / collapse §2a).

			\ingroup KernelContainers
	*/
	enum class MoleculeRole : std::uint8_t
	{
		UNKNOWN        = 0,
		PROTEIN        = 1,   ///< former Protein
		NUCLEIC_ACID   = 2,   ///< former NucleicAcid
		SMALL_MOLECULE = 3,
		POLYMER        = 4,
		LIGAND         = 5,
		SOLVENT        = 6,
		WATER          = 7,
		ION            = 8
	};

	/** Role of a `Fragment` node (the recursive sub-container). Covers the
			former Chain/Residue/Nucleotide/SecondaryStructure plus generic
			fragmentation (domains, functional groups, arbitrary fragments).

			Canonical PDB depth (collapse §2a): System -> Molecule(PROTEIN/NA) ->
			Fragment(CHAIN) -> Fragment(RESIDUE) -> Atom. SECONDARY_STRUCTURE is a
			NON-owning annotation Fragment belonging to its chain (D-HC4); it
			references its start/end residues, it does not own them.

			\ingroup KernelContainers
	*/
	enum class FragmentRole : std::uint8_t
	{
		UNKNOWN             = 0,
		CHAIN               = 1,   ///< former Chain
		RESIDUE             = 2,   ///< former Residue / Nucleotide (see ResidueKind)
		SECONDARY_STRUCTURE = 3,   ///< former SecondaryStructure (annotation, D-HC4)
		DOMAIN              = 4,
		FUNCTIONAL_GROUP    = 5,
		ARBITRARY           = 6    ///< former generic Fragment
	};

	/** Kind of a `Fragment` whose role is RESIDUE. Absorbs the former
			`Residue::Property` IDENTITY bits (AMINO_ACID / WATER / NON_STANDARD)
			into one typed field. The remaining Residue::Property bits
			(HAS_SSBOND, CYCLIC) stay sparse container properties; N/C-terminal is
			DERIVED (first/last polymer residue of the chain), not stored
			(D-HC1 / H1bP-5).

			\ingroup KernelContainers
	*/
	enum class ResidueKind : std::uint8_t
	{
		UNKNOWN     = 0,
		AMINO_ACID  = 1,
		NUCLEOTIDE  = 2,
		WATER       = 3,
		LIGAND      = 4,
		ION         = 5,
		NONSTANDARD = 6
	};

	/** Kind of a `Fragment` whose role is SECONDARY_STRUCTURE. Maps 1:1 from
			the former `SecondaryStructure::Type` (HELIX/COIL/STRAND/TURN/UNKNOWN);
			helix-class / sheet-id / strand-number / sense ride as SS payload
			(D-HC4).

			\ingroup KernelContainers
	*/
	enum class SSKind : std::uint8_t
	{
		UNKNOWN = 0,
		HELIX   = 1,
		STRAND  = 2,
		TURN    = 3,
		COIL    = 4
	};

} // namespace BALL

#endif // BALL_KERNEL_CONTAINERROLE_H
