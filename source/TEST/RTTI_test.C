// $Id: RTTI_test.C,v 1.2 2000/01/03 15:30:00 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/protein.h>

///////////////////////////

START_TEST(RTTI, "$Id: RTTI_test.C,v 1.2 2000/01/03 15:30:00 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::RTTI;
CHECK(RTTI::isKindOf<>())
Protein p;
Protein*			p_ptr(&p);
Molecule*			m_ptr(&p);
BaseFragment*	b_ptr(&p);
TEST_EQUAL(isKindOf<Molecule>(*p_ptr), true)
TEST_EQUAL(isKindOf<Molecule>(*m_ptr), true)
TEST_EQUAL(isKindOf<Molecule>(*b_ptr), true)
TEST_EQUAL(isKindOf<Protein>(*p_ptr), true)
TEST_EQUAL(isKindOf<Protein>(*m_ptr), true)
TEST_EQUAL(isKindOf<Protein>(*b_ptr), true)
TEST_EQUAL(isKindOf<BaseFragment>(*p_ptr), true)
TEST_EQUAL(isKindOf<BaseFragment>(*m_ptr), true)
TEST_EQUAL(isKindOf<BaseFragment>(*b_ptr), true)
TEST_EQUAL(isKindOf<Residue>(*p_ptr), false)
TEST_EQUAL(isKindOf<Residue>(*m_ptr), false)
TEST_EQUAL(isKindOf<Residue>(*b_ptr), false)
Molecule m;
m_ptr = &m;
b_ptr = &m;
TEST_EQUAL(isKindOf<Molecule>(*m_ptr), true)
TEST_EQUAL(isKindOf<Molecule>(*b_ptr), true)
TEST_EQUAL(isKindOf<Protein>(*m_ptr), false)
TEST_EQUAL(isKindOf<Protein>(*b_ptr), false)
TEST_EQUAL(isKindOf<BaseFragment>(*m_ptr), true)
TEST_EQUAL(isKindOf<BaseFragment>(*b_ptr), true)
TEST_EQUAL(isKindOf<Residue>(*m_ptr), false)
TEST_EQUAL(isKindOf<Residue>(*b_ptr), false)
BaseFragment b;
b_ptr = &b;
TEST_EQUAL(isKindOf<Molecule>(*b_ptr), false)
TEST_EQUAL(isKindOf<Protein>(*b_ptr), false)
TEST_EQUAL(isKindOf<BaseFragment>(*b_ptr), true)
TEST_EQUAL(isKindOf<Residue>(*b_ptr), false)
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
