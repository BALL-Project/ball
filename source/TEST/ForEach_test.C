// $Id: ForEach_test.C,v 1.1 2001/06/28 19:14:07 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PDBAtom.h>
///////////////////////////

START_TEST(class_name, "$Id: ForEach_test.C,v 1.1 2001/06/28 19:14:07 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BALL_FOREACH_MOLECULE)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_FRAGMENT)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_ATOMCONTAINER)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_ATOM)
	// BAUSTELLE
RESULT

CHECK(BALL_FOREACH_ATOM_BOND)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_ATOM_PAIR)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_BOND)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_INTERBOND)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_INTRABOND)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_PROTEIN)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_CHAIN)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_SECONDARYSTRUCTURE)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_RESIDUE)
	// BAUSTELLE
RESULT


CHECK(BALL_FOREACH_PDBATOM)
	// BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
