// $Id: GlobalKernel_test.C,v 1.1 2001/06/26 20:32:00 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/global.h>
#include <BALL/KERNEL/chain.h>
///////////////////////////

START_TEST(class_name, "$Id: GlobalKernel_test.C,v 1.1 2001/06/26 20:32:00 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK("cloneBonds")
	Chain c1, c2;
	Residue r11, r21, r12, r22;
	c1.insert(r11);
	c1.insert(r12);
	c2.insert(r21);
	c2.insert(r22);
	PDBAtom a111, a112, a121, a122, a211, a212, a221, a222;
	r11.insert(a111);
	r11.insert(a112);
	r12.insert(a121);
	r12.insert(a122);
	r21.insert(a211);
	r21.insert(a212);
	r22.insert(a221);
	r22.insert(a222);

	a111.createBond(a112);
	a111.getBond(0)->setName("1.");
	a111.getBond(0)->setOrder(Bond::ORDER__QUADRUPLE);
	a111.getBond(0)->setType(Bond::TYPE__DISULPHIDE_BRIDGE);
	
	a111.createBond(a121);
	a111.getBond(1)->setName("2.");
	a111.getBond(1)->setOrder(Bond::ORDER__AROMATIC);
	a111.getBond(1)->setType(Bond::TYPE__PEPTIDE);

	a112.createBond(a121);
	a112.getBond(1)->setName("3.");
	a112.getBond(1)->setOrder(Bond::ORDER__TRIPLE);
	a112.getBond(1)->setType(Bond::TYPE__HYDROGEN);

	
	cloneBonds(c1, c2);
	TEST_EQUAL(a211.countBonds(), 2)
	
	TEST_EQUAL(a211.isBoundTo(a212), true)
	TEST_EQUAL(a211.getBond(0)->getName(), "1.")
	TEST_EQUAL(a211.getBond(0)->getOrder(), Bond::ORDER__QUADRUPLE)
	TEST_EQUAL(a211.getBond(0)->getType(), Bond::TYPE__DISULPHIDE_BRIDGE)

	TEST_EQUAL(a211.isBoundTo(a221), true)
	TEST_EQUAL(a211.getBond(1)->getName(), "2.")
	TEST_EQUAL(a211.getBond(1)->getOrder(), Bond::ORDER__AROMATIC)
	TEST_EQUAL(a211.getBond(1)->getType(), Bond::TYPE__PEPTIDE)

	TEST_EQUAL(a212.isBoundTo(a221), true)
	TEST_EQUAL(a212.getBond(1)->getName(), "3.")
	TEST_EQUAL(a212.getBond(1)->getOrder(), Bond::ORDER__TRIPLE)
	TEST_EQUAL(a212.getBond(1)->getType(), Bond::TYPE__HYDROGEN)
	
	TEST_EQUAL(a222.countBonds(), 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
