// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_GENETICDOCK_ROTATE_BOND_H
#define BALL_DOCKING_GENETICDOCK_ROTATE_BOND_H

#include <BALL/DATATYPE/hashSet.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/matrix44.h>

// This class changes the rotation angle(radians) around a defined axis, furthermore it is possible to select
// 2 additional atoms to calculate and change the dihedral angle. The rotation axis must not be contained in a
// ring system. If this condition holds true, the molecule is divided by the rotation axis. All atoms of the
// lighter part rotate while the larger part of the molecule remains unchanged.


namespace BALL
{
  class Atom;
  class Bond;

  class BALL_EXPORT RotateBond
    {
    public:

      /** standard constructor
       */
      RotateBond();

      /** copy constructor
       */
      RotateBond(const RotateBond&);

      /** constructor using bond to define axis
       */
      RotateBond(const Bond&);

      /** define axis by atoms
       */
      RotateBond(Atom*, Atom*);

      /** define axis and 2 atoms for dihedral angle
       */
      RotateBond(const Bond&, Atom*, Atom*);

      /** define 2 axis and 2 dihedral atoms
       */
      RotateBond(Atom*, Atom*, Atom*, Atom*);

      /** destructor
       */
      ~RotateBond();

      /**  change dihedral angle
       */
      void rotate(const Angle&, bool restorePosition = true);

      /** set dihedral angle to new value
       */
      void setDihedral(const Angle&);

      /** get current dihedral angle
       */
      Angle getDihedral();

      /** smaller operator
       */
      bool operator<(const RotateBond& rb);

    private:

      /** first atom of the rotation axis
       */
      Atom* hinge_;

      /** second atom of the rotation axis
       */
      Atom* nail_;

      /** defines dihedral angle
       */
      Atom* dihedral_lite_;

      /** defines dihedral angle
       */
      Atom* dihedral_heavy_;

      /** these atoms rotate
       */
      HashSet<Atom*> rotate_atoms_;

      /** collects atoms
       */
      void collectAtoms(Atom*,
			Atom*,
			Atom*,
			HashSet<Atom*>&);

      /** used by constructors
       */
      void setup(Atom*, Atom*, Atom*, Atom*);
    };
}


#endif /* BALL_DOCKING_GENETICDOCK_ROTATE_BOND_H */
