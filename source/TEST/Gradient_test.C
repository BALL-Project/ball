// $Id: Gradient_test.C,v 1.3 2001/07/07 02:51:42 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/KERNEL/system.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
///////////////////////////

START_TEST(Gradient, "$Id: Gradient_test.C,v 1.3 2001/07/07 02:51:42 oliver Exp $")

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

Atom* a1 = new Atom;
Atom* a2 = new Atom;
System s;
Molecule m;
s.insert(m);
m.insert(*a1);
m.insert(*a2);

a1->setForce(Vector3(1.0e-10, 1.0e-10, 1.0e-10));
a2->setForce(Vector3(-1.0e-10, -1.0e-10, -1.0e-10));
	
AtomVector atom_vector(s);
const double factor = Constants::NA * 1e-23;


CHECK(Gradient::size())
	Gradient grad;
	TEST_EQUAL(grad.size(), 0)
RESULT

CHECK(Gradient::Gradient(const AtomVector& atoms))
	Gradient grad(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)
RESULT


CHECK(Gradient::Gradient(const Gradient& gradient))
	Gradient grad(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms,  factor)
	TEST_EQUAL(grad.isValid(), true)

	Gradient grad_copy(grad);

	TEST_EQUAL(grad_copy.size(), 2)
	TEST_REAL_EQUAL(grad_copy.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad_copy.inv_norm, 1.0 / grad_copy.norm)
	TEST_REAL_EQUAL(grad_copy.rms,  factor)
	TEST_EQUAL(grad_copy.isValid(), true)	
RESULT


CHECK(Gradient::Gradient& operator = (const Gradient& rhs))
	Gradient grad(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)

	Gradient grad_copy;
	grad_copy = grad;

	TEST_EQUAL(grad_copy.size(), 2)
	TEST_REAL_EQUAL(grad_copy.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad_copy.inv_norm, 1.0 / grad_copy.norm)
	TEST_REAL_EQUAL(grad_copy.rms, factor)
	TEST_EQUAL(grad_copy.isValid(), true)	
RESULT


CHECK(Gradient::Gradient& operator = (const AtomVector& rhs))
	Gradient grad;
	grad = atom_vector;

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)
RESULT


CHECK(Gradient::set(const Gradient& gradient))
	Gradient grad(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)

	Gradient grad_copy;
	grad_copy.set(grad);

	TEST_EQUAL(grad_copy.size(), 2)
	TEST_REAL_EQUAL(grad_copy.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad_copy.inv_norm, 1.0 / grad_copy.norm)
	TEST_REAL_EQUAL(grad_copy.rms, factor)
	TEST_EQUAL(grad_copy.isValid(), true)	
RESULT


CHECK(Gradient::set(const AtomVector& atoms))
	Gradient grad;
	grad.set(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)
RESULT

CHECK(Gradient::operator [] (int) const)
	Gradient grad(atom_vector);
	Vector3 force = grad[0]  - (atom_vector[0]->getForce() * Constants::NA / -1.0e13);
	TEST_EQUAL(force.getLength(), 0.0)
	force = grad[1]  - (atom_vector[1]->getForce() * Constants::NA / -1.0e13);
	TEST_EQUAL(force.getLength(), 0.0)
RESULT

CHECK(Gradient::negate())
	Gradient grad;
	grad.set(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)
	Vector3 force = atom_vector[0]->getForce() * Constants::NA / -1.0e13 - grad[0];
	TEST_REAL_EQUAL(force.getLength(), 0.0)
	force = atom_vector[1]->getForce() * Constants::NA / -1.0e13 - grad[1];
	TEST_REAL_EQUAL(force.getLength(), 0.0)

	grad.negate();

	force = atom_vector[0]->getForce() * Constants::NA / -1.0e13 + grad[0];
	TEST_REAL_EQUAL(force.getLength(), 0.0)
	force = atom_vector[1]->getForce() * Constants::NA / -1.0e13 + grad[1];
	TEST_REAL_EQUAL(force.getLength(), 0.0)
	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)
RESULT


CHECK(Gradient::normalize())
	Gradient grad;
	grad.set(atom_vector);

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, sqrt(6.0) * factor)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0 / grad.norm)
	TEST_REAL_EQUAL(grad.rms, factor)
	TEST_EQUAL(grad.isValid(), true)

	grad.normalize();

	TEST_EQUAL(grad.size(), 2)
	TEST_REAL_EQUAL(grad.norm, 1.0)
	TEST_REAL_EQUAL(grad.inv_norm, 1.0)
	TEST_EQUAL(grad.isValid(), true)
RESULT


CHECK(Gradient::invalidate())
	Gradient grad(atom_vector);
	TEST_EQUAL(grad.isValid(), true)
	grad.invalidate();
	TEST_EQUAL(grad.isValid(), false)
RESULT

CHECK(Gradient::operator * (const Gradient& rhs) const)
	Gradient g1(atom_vector);
	Gradient g2(atom_vector);
	g2.negate();
	
	PRECISION(1e-3)
	TEST_REAL_EQUAL(g1 * g2, -6.0 * factor * factor)

	PRECISION(1e-5)
	g1.normalize();
	g2.normalize();
	TEST_REAL_EQUAL(g1 * g2, -1.0)
	TEST_REAL_EQUAL(g2 * g1, -1.0)
RESULT

CHECK(begin()/end())
	Gradient grad(atom_vector);
	Gradient::Iterator it;
	it = grad.begin();
	TEST_EQUAL(*it, grad[0])
	it++;
	TEST_EQUAL(*it, grad[1])
	it++;
	bool comparison = (it == grad.end());
	TEST_EQUAL(comparison, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
