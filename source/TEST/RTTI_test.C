// $Id: RTTI_test.C,v 1.3 2000/01/13 22:49:09 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/protein.h>

///////////////////////////

START_TEST(RTTI, "$Id: RTTI_test.C,v 1.3 2000/01/13 22:49:09 oliver Exp $")

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

CHECK(RTTI::isInstanceOf<>())
Protein p;
Protein*			p_ptr(&p);
Molecule*			m_ptr(&p);
BaseFragment*	b_ptr(&p);
TEST_EQUAL(isInstanceOf<Molecule>(*p_ptr), false)
TEST_EQUAL(isInstanceOf<Molecule>(*m_ptr), false)
TEST_EQUAL(isInstanceOf<Molecule>(*b_ptr), false)
TEST_EQUAL(isInstanceOf<Protein>(*p_ptr), true)
TEST_EQUAL(isInstanceOf<Protein>(*m_ptr), true)
TEST_EQUAL(isInstanceOf<Protein>(*b_ptr), true)
TEST_EQUAL(isInstanceOf<BaseFragment>(*p_ptr), false)
TEST_EQUAL(isInstanceOf<BaseFragment>(*m_ptr), false)
TEST_EQUAL(isInstanceOf<BaseFragment>(*b_ptr), false)
TEST_EQUAL(isInstanceOf<Residue>(*p_ptr), false)
TEST_EQUAL(isInstanceOf<Residue>(*m_ptr), false)
TEST_EQUAL(isInstanceOf<Residue>(*b_ptr), false)
Molecule m;
m_ptr = &m;
b_ptr = &m;
TEST_EQUAL(isInstanceOf<Molecule>(*m_ptr), true)
TEST_EQUAL(isInstanceOf<Molecule>(*b_ptr), true)
TEST_EQUAL(isInstanceOf<Protein>(*m_ptr), false)
TEST_EQUAL(isInstanceOf<Protein>(*b_ptr), false)
TEST_EQUAL(isInstanceOf<BaseFragment>(*m_ptr), false)
TEST_EQUAL(isInstanceOf<BaseFragment>(*b_ptr), false)
TEST_EQUAL(isInstanceOf<Residue>(*m_ptr), false)
TEST_EQUAL(isInstanceOf<Residue>(*b_ptr), false)
BaseFragment b;
b_ptr = &b;
TEST_EQUAL(isInstanceOf<Molecule>(*b_ptr), false)
TEST_EQUAL(isInstanceOf<Protein>(*b_ptr), false)
TEST_EQUAL(isInstanceOf<BaseFragment>(*b_ptr), true)
TEST_EQUAL(isInstanceOf<Residue>(*b_ptr), false)
RESULT											

CHECK(getDefault<>())
Protein p;
p = getDefault<Protein>();
RESULT

CHECK(getNew<>())
Protein* p = (Protein*)getNew<Protein>();
TEST_NOT_EQUAL(p, 0)
RESULT

CHECK(getName<>())
// there is not much to check - each damned compiler 
// tries his own demangling!
TEST_EQUAL(getName<Protein>(), String("BALL::Protein"))
RESULT

CHECK(getStreamName<>())
// there is not much to check - each damned compiler 
// tries his own demangling!
TEST_EQUAL(getName<Protein>(), String("BALL::Protein"))
RESULT

CHECK(castTo<>())
//BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
