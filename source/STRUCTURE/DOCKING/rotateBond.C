#include "rotateBond.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/COMMON/constants.h>

#include <BALL/FORMAT/HINFile.h>

namespace BALL
{
  RotateBond::RotateBond()
    :hinge_(0),
     nail_(0),
     dihedral_lite_(0),
     dihedral_heavy_(0)
  {
  }
  
  RotateBond::RotateBond(const RotateBond& rb)
    :hinge_(rb.hinge_),
     nail_(rb.nail_),
     dihedral_lite_(rb.dihedral_lite_),
     dihedral_heavy_(rb.dihedral_heavy_),
     rotate_atoms_(rb.rotate_atoms_)
  {
  }

  RotateBond::RotateBond(const Bond& b)
    :dihedral_lite_(0),
     dihedral_heavy_(0)      
  {
    
    Atom* at1 = const_cast<Atom*>(b.getFirstAtom());
    Atom* at2 = const_cast<Atom*>(b.getSecondAtom());
    
    setup(at1,at2,0,0);
  }
  
  RotateBond::RotateBond(Atom* at1, Atom* at2)
    :dihedral_lite_(0),
     dihedral_heavy_(0)   
  {
    setup(at1,at2,0,0);
  }
  
  RotateBond::RotateBond(const Bond& b, Atom* at3, Atom* at4)
  {
    Atom* at1 = const_cast<Atom*>(b.getFirstAtom());
    Atom* at2 = const_cast<Atom*>(b.getSecondAtom());   
    
    setup(at1,at2,at3,at4);
  }
  
  RotateBond::RotateBond(Atom* axis1, Atom* axis2, Atom* dihedral1, Atom* dihedral2)
  {
    setup(axis1,axis2,dihedral1,dihedral2);
  }
    
  RotateBond::~RotateBond()
  {
  }
      
  void RotateBond::rotate(const Angle& a,bool restorePosition)
  {
    // checks for validity 
    if (!nail_ || !hinge_)
      {
	Log.error() << "error: RotateBond not valid" << std::endl;
	exit(-1);
      }
 
    // move molecule, that the rotation axis alligns with the x-axis 
    allign();

    Matrix4x4 rotation_matrix; 
    
    rotation_matrix.setRotationX(a);

    TransformationProcessor rotation(rotation_matrix);

    HashSet<Atom*>::Iterator it = rotate_atoms_.begin();

    // rotate the whole lighter part of the molecule
    for (;it != rotate_atoms_.end();it++)
      (*it)->apply(rotation);

    // move back molecule
    if (restorePosition)
      restore();
  }

  void RotateBond::collectAtoms(Atom* probe, Atom* block, 
				Atom* start,
				HashSet<Atom*>& hs)
  {
    hs.insert(probe);
    
    /** iterate over all bonds of atom at_a
     */
    for (AtomBondConstIterator it = probe->beginBond(); it != probe->endBond(); it++)
      {
	/** get other atom of bond
	 */
	Atom* partner = it->getPartner(*probe);
	
	if (partner == block)
	  if (probe == start)
	    continue;
	  else
	    {
	      Log.error() << "error: rotation axis is part of a ring" << std::endl;
	      exit(-1);
	    }
	
	/** test if partner has not already been discoverd or is not at_b
	 */
	if (!hs.has(partner))
	  collectAtoms(partner,block,start,hs);
      }
  }
  
  void RotateBond::setup(Atom* at1, Atom* at2, Atom* at3, Atom* at4)
  {
    // take arbitrary choice for hinge_ and nail_
    hinge_ = at1;
    nail_ = at2;

    collectAtoms(hinge_,nail_,hinge_,rotate_atoms_);
    
    // if rotate_atoms_ contains more than half of the molecule, swap hinge_ and nail_
    if (rotate_atoms_.getSize() > hinge_->getMolecule()->countAtoms()/2)
      {
	nail_ = at1;
	hinge_ = at2;
	
	rotate_atoms_.clear();
	
	collectAtoms(hinge_,nail_,hinge_,rotate_atoms_);
      }  
  
    // if dihedral atoms are defined
    if (at3 != 0 && at4 != 0)
      if (at1->isBoundTo(*nail_))
	{
	  dihedral_heavy_ = at3;
	  dihedral_lite_ = at4;
	}
      else
	{
	  dihedral_heavy_ = at4;
	  dihedral_lite_ = at3;
	}  
  }

  void RotateBond::allign()
  {
    StructureMapper structure_mapper;
  
    // map hinge_ to the origin, nail_ on the negativ x-axis and, if definded, dihedral_heavy_ in the hyperplane, definded by x- and z-axis  
    translation_matrix_ = structure_mapper.matchPoints(hinge_->getPosition(),nail_->getPosition(),
						       dihedral_heavy_ != 0 ? dihedral_heavy_->getPosition() : nail_->getPosition(),
						       Vector3(0.0,0.0,0.0),Vector3(-1.0,0.0,0.0),Vector3(-1.0,0.0,1.0));
    
    TransformationProcessor translation(translation_matrix_);
    
    nail_->getMolecule()->apply(translation);
  }
  
  void RotateBond::restore()
  {
    translation_matrix_.invert();
    
    TransformationProcessor restore(translation_matrix_);
    
    nail_->getMolecule()->apply(restore);
  }

  Angle RotateBond::getDihedral()
  {
    allign();
    
    // calculate dihedral angle(dihedral_heavy_'s position vector points in direction (0,0,1))
    Angle angle = Vector3(0.0,dihedral_lite_->getPosition().y,dihedral_lite_->getPosition().z).getAngle(Vector3(0.0,0.0,1.0));

    // to allow unambiguous definition 
    if (dihedral_lite_->getPosition().y > 0)
      angle = Angle(2*Constants::PI) - angle;
    
    restore();
    
    return angle;
  }

  void RotateBond::setDihedral(const Angle& target)
  {
    Angle status = getDihedral();

    status -= target;
    
    rotate(-status);
  }


  bool RotateBond::operator<(const RotateBond& rb)
  {
    if (rotate_atoms_.size() != rb.rotate_atoms_.size())
      return rotate_atoms_.size() < rb.rotate_atoms_.size();
    
    Molecule* mol = nail_->getMolecule();
    Molecule* mol_rb = rb.nail_->getMolecule();

    for (uint x = 0; x < mol->countAtoms() && x < mol_rb->countAtoms(); x++)
      {
	if (mol->getAtom(x) == nail_ && mol_rb->getAtom(x) == rb.nail_)
	  break;

	if (mol->getAtom(x) == nail_)
	  return true;

	if (mol_rb->getAtom(x) == rb.nail_)
	  return false;
      }

    for (uint x = 0; x < mol->countAtoms() && x < mol_rb->countAtoms(); x++)
      {
	if (mol->getAtom(x) == nail_)
	  return true;
	
	if (mol_rb->getAtom(x) == rb.nail_)
	  return false;
      }
  
    return true;
  }
    

}


