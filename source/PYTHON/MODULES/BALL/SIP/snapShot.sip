// $Id: snapShot.sip,v 1.4 2001/05/07 00:58:11 oliver Exp $

class SnapShot
{
%HeaderCode
	#include <BALL/MOLMEC/COMMON/snapShot.h>
	using namespace BALL;
%End
 public:
 SnapShot()
   throw();
 SnapShot(const SnapShot& /snapshot/)
   throw();
 ~SnapShot();
 // const SnapShot& operator = (const SnapShot& /snapshot/)
 //  throw();
 virtual void clear()
   throw();
 // bool operator == (const SnapShot& /snapshot/) const
 //   throw();
    bool isValid() const
   throw();
  void setIndex(Size /index/)
   throw();
  Size getIndex() const
   throw();
  void setNumberOfAtoms(Size /number_of_atoms/)
   throw();
  Size getNumberOfAtoms() const
   throw();
  void setPotentialEnergy(DoubleReal /potential_energy/) 
   throw();
  DoubleReal getPotentialEnergy() const
   throw();
  void setKineticEnergy(DoubleReal /kinetic_energy/)
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
  void takeSnapShot(const System& /system/)
   throw(OutOfMemory);
  void applySnapShot(System& /system/) const
   throw();
  void getAtomPositions(const System& /system/)
   throw(OutOfMemory);
  void setAtomPositions(System& /system/) const
   throw();
  void getAtomVelocities(const System& /system/)
   throw(OutOfMemory);
  void setAtomVelocitites(System& /system/) const
   throw();
  void getAtomForces(const System& /system/)
   throw(OutOfMemory);
  void setAtomForces(System& /system/) const
   throw();
};
