// $Id: Gradient_test.C,v 1.2 2000/03/26 12:34:07 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/KERNEL/system.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
///////////////////////////

START_TEST(Gradient, "$Id: Gradient_test.C,v 1.2 2000/03/26 12:34:07 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Gradient* g_ptr;
CHECK(Gradient::Gradient())
	g_ptr = new Gradient();
	TEST_NOT_EQUAL(g_ptr, 0)
RESULT

CHECK(Gradient::isValid() const )
  TEST_EQUAL(g_ptr->isValid(), false)
RESULT

CHECK(Gradient::~Gradient())
  delete g_ptr;
RESULT

CHECK(Gradient::Gradient(const AtomVector& atoms))
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	System s;
	Molecule m;
	s.insert(m);
	m.insert(*a1);
	m.insert(*a2);
	
	a1->setForce(Vector3(1.0e-10, 1.0e-10, 1.0e-10));
	a1->setForce(Vector3(-1.0e-10, -1.0e-10, -1.0e-10));
		
	AtomVector atom_vector(s);
	Gradient grad(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(3.0) * Constants::NA * 1e-23)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms,  sqrt(3.0) * Constants::NA * 1e-23 / sqrt(6.0))
	TEST_EQUAL(grad.isValid(), true)
RESULT


CHECK(Gradient::Gradient(const Gradient& gradient, bool deep = true))
  //BAUSTELLE
RESULT


CHECK(Gradient::Gradient& operator = (const Gradient& rhs))
  //BAUSTELLE
RESULT


CHECK(Gradient::Gradient& operator = (const AtomVector& rhs))
  //BAUSTELLE
RESULT


CHECK(Gradient::set(const Gradient& gradient))
  //BAUSTELLE
RESULT


CHECK(Gradient::set(const AtomVector& atoms))
  //BAUSTELLE
RESULT


CHECK(Gradient::negate())
  //BAUSTELLE
RESULT


CHECK(Gradient::normalize())
  //BAUSTELLE
RESULT


CHECK(Gradient::invalidate())
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
