// $Id: SnapShot_test.C,v 1.3 2001/12/30 13:29:02 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MOLMEC/COMMON/snapShot.h>

///////////////////////////

START_TEST(SnapShot, "$Id: SnapShot_test.C,v 1.3 2001/12/30 13:29:02 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

// tests for class SnapShot::

CHECK(SnapShot::SnapShot() throw())
  //?????
RESULT


CHECK(SnapShot::SnapShot(const SnapShot& snapshot) throw())
  //?????
RESULT


CHECK(SnapShot::SnapShot(const Size no_of_atoms) throw())
  //?????
RESULT


CHECK(SnapShot::~SnapShot() throw())
  //?????
RESULT


CHECK(SnapShot::SnapShot& operator = (const SnapShot& snapshot) throw())
  //?????
RESULT


CHECK(SnapShot::clear() throw())
  //?????
RESULT


CHECK(SnapShot::bool operator == (const SnapShot& snapshot) const  throw())
  //?????
RESULT


CHECK(SnapShot::isValid() const  throw())
  //?????
RESULT


CHECK(SnapShot::setIndex(Size index) throw())
  //?????
RESULT


CHECK(SnapShot::getIndex() const  throw())
  //?????
RESULT


CHECK(SnapShot::setNumberOfAtoms(Size number_of_atoms) throw())
  //?????
RESULT


CHECK(SnapShot::getNumberOfAtoms() const  throw())
  //?????
RESULT


CHECK(SnapShot::getDataLength() const  throw())
  //?????
RESULT


CHECK(SnapShot::getTotalLength() const  throw())
  //?????
RESULT


CHECK(SnapShot::getPotentialEnergy() const  throw())
  //?????
RESULT


CHECK(SnapShot::getKineticEnergy() const  throw())
  //?????
RESULT


CHECK(SnapShot::setAtomPositions(const std::vector<Vector3>& atom_postions) throw())
  //?????
RESULT


CHECK(SnapShot::getAtomPositions() const  throw())
  //?????
RESULT


CHECK(SnapShot::setAtomVelocitites(const std::vector<Vector3>& atom_velocities) throw())
  //?????
RESULT


CHECK(SnapShot::getAtomVelocities() const  throw())
  //?????
RESULT


CHECK(SnapShot::setAtomForces(const std::vector<Vector3>& atom_forces) throw())
  //?????
RESULT


CHECK(SnapShot::getAtomForces() const  throw())
  //?????
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
