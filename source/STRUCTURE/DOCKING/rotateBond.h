#ifndef _ROTATE_BOND_
#define _ROTATE_BOND_

#include <BALL/DATATYPE/hashSet.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/matrix44.h>

// This class changes the rotation angle(radians) around a defined axis, furthermore it is possible to select 2 additional atoms to 
// calculate and change the dihedral angle. The rotation axis must not be contained in a ring system. If this condition holds true,
// the molecule is divided by the rotation axis. All atoms of the lighter part rotate while the larger part of the molecule remains unchanged.      


namespace BALL
{
  class Atom;
  class Bond;
  
  class RotateBond
    {
    public:
     
      // constructors
      RotateBond();

      RotateBond(const RotateBond&);
      
      // define axis directly
      RotateBond(const Bond&);
      
      // define axis by atoms
      RotateBond(Atom*,Atom*);
      
      // define axis and 2 atoms for dihedral angle
      RotateBond(const Bond&,Atom*,Atom*);
      
      // define 2 axis and 2 dihedral atoms
      RotateBond(Atom*,Atom*,Atom*,Atom*);

      ~RotateBond();

      //  change rotation angle
      void rotate(const Angle&, bool restorePosition = true);

      // set dihedral angle to new value
      void setDihedral(const Angle&);

      // get current dihedral angle
      Angle getDihedral();

      bool operator<(const RotateBond& rb);

    private:

      // to rotate, this atom is moved to the origin
      Atom* hinge_;
      
      // is placed on the negative x-axis 
      Atom* nail_;

      // defines the dihedral angle
      Atom* dihedral_lite_;

      // is moved into the hyperplane, spanned by x and z-axis
      Atom* dihedral_heavy_;

      // these atoms rotate 
      HashSet<Atom*> rotate_atoms_;
    
      // used by allign and restore
      Matrix4x4 translation_matrix_;
      
      // collects atoms
      void collectAtoms(Atom*, 
			Atom*, 
			Atom*,
			HashSet<Atom*>&);
      
      // used by constructors
      void setup(Atom*,Atom*,Atom*,Atom*);
      
      // moves molecule to the correct position
      void allign();
      
      // restores position of the heavier part of the molecule
      void restore();

    };
}


#endif
