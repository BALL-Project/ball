// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Extractors_test.C,v 1.1.4.1 2006/02/14 15:03:05 amoll Exp $
//
// Author:
//   Oliver Kohlbacher
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/extractors.h>

///////////////////////////

START_TEST(Extractors, "$Id: Extractors_test.C,v 1.1.4.1 2006/02/14 15:03:05 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

											
CHECK(AtomList atoms(const AtomContainer& fragment, const String& expression))
	// ????
RESULT

CHECK(AtomList atoms(const AtomContainer& fragment))
	// ????
RESULT

CHECK(AtomList atoms(const AtomList& atoms, const String& expression))
	// ????
RESULT

CHECK(PDBAtomList PDBAtoms(const AtomContainer& fragment, const String& expression))
	// ????
RESULT

CHECK(PDBAtomList PDBAtoms(const AtomContainer& fragment))
	// ????
RESULT

CHECK(BondList bonds(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(BondList bonds(const Atom& atom))
	// ????
RESULT

CHECK(AtomContainerList atomContainers(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(ResidueList residues(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(FragmentList fragments(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(MoleculeList molecules(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(ProteinList proteins(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(SecondaryStructureList secondaryStructures(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(ChainList chains(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(NucleicAcidList nucleicAcids(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT

CHECK(NucleotideList nucleotides(const AtomContainer& fragment, bool selected_only = false))
	// ????
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
