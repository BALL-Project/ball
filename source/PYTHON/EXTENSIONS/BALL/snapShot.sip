// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class SnapShot
{
%TypeHeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
 public:
 SnapShot()
   throw();
 SnapShot(const SnapShot&)
   throw();
 ~SnapShot() throw();
 // const SnapShot& operator = (const SnapShot&)
 //  throw();
	virtual void clear() throw();
	bool operator == (const SnapShot&) const throw();
  bool isValid() const throw();
  void setIndex(Size) throw();
  Size getIndex() const throw();
  void setNumberOfAtoms(Size) throw();
  Size getNumberOfAtoms() const
   throw();
  void setPotentialEnergy(DoubleReal) 
   throw();
  DoubleReal getPotentialEnergy() const
   throw();
  void setKineticEnergy(DoubleReal)
   throw();
  DoubleReal getKineticEnergy() const
   throw();
  //void setAtomPositions(const ::std::vector<Vector3>& atom_postions)
  // throw();
  // const ::std::vector<Vector3>& getAtomPositions() const
  // throw();
  // void setAtomVelocities(const ::std::vector<Vector3>& atom_velocities)
  //  throw();
  // const ::std::vector<Vector3>& getAtomVelocities() const
  //  throw();
  // void setAtomForces(const ::std::vector<Vector3>& atom_forces)
  //  throw();
  // const ::std::vector<Vector3>& getAtomForces() const
  //  throw();
  void takeSnapShot(const System&)
   throw(OutOfMemory);
  void applySnapShot(System&) const
   throw();
  void getAtomPositions(const System&)
   throw(OutOfMemory);
  void setAtomPositions(System&) const
   throw();
  void getAtomVelocities(const System&)
   throw(OutOfMemory);
  void setAtomVelocitites(System&) const
   throw();
  void getAtomForces(const System&)
   throw(OutOfMemory);
  void setAtomForces(System&) const
   throw();
};
