// $Id: SnapShot_test.C,v 1.2 2001/07/15 17:32:41 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MOLMEC/COMMON/snapShot.h>

///////////////////////////

START_TEST(SnapShot, "$Id: SnapShot_test.C,v 1.2 2001/07/15 17:32:41 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

// tests for class SnapShot::

CHECK(SnapShot::SnapShot() throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::SnapShot(const SnapShot& snapshot) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::SnapShot(const Size no_of_atoms) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::~SnapShot() throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::SnapShot& operator = (const SnapShot& snapshot) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::clear() throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::bool operator == (const SnapShot& snapshot) const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::isValid() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::setIndex(Size index) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getIndex() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::setNumberOfAtoms(Size number_of_atoms) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getNumberOfAtoms() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getDataLength() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getTotalLength() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getPotentialEnergy() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getKineticEnergy() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::setAtomPositions(const std::vector<Vector3>& atom_postions) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getAtomPositions() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::setAtomVelocitites(const std::vector<Vector3>& atom_velocities) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getAtomVelocities() const  throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::setAtomForces(const std::vector<Vector3>& atom_forces) throw())
  //BAUSTELLE
RESULT


CHECK(SnapShot::getAtomForces() const  throw())
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
