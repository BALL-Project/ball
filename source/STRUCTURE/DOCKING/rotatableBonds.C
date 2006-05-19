// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: rotatableBonds.C,v 1.1.2.1 2006/05/19 14:18:31 leonhardt Exp $

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/KERNEL/system.h>
#include <BALL/COMMON/constants.h>

#include <iostream>

#include "rotateBonds.h"
#include "rotableb.h"

#include "algorithm"


using namespace std;


namespace BALL
{
	RotateBonds::RotateBonds(BALL::Molecule& mol)
	{
		System sys;
		sys.insert(mol);
		build(sys);
		sys.remove(mol);
		
	}
	
	
	
	void RotateBonds::build(BALL::System& sys)
	{
		
		sideChainFlag = false;
		
		mol_ = sys.getMolecule(0);
		
		/** store rotable bonds in r_bonds
		 */
		std::vector<const Bond*> r_bonds;
		find_rotation(sys,r_bonds);
		
		const BALL::Atom *r_a,*r_b;
		
		/** resize vector according to r_bonds
		 */
		hinges_.resize(r_bonds.size());
		
		/** needed to calculate hinges
		 */
		HashSet<const Atom*> visited;
		
		/** iterate over all rotable bonds
		 */
		for (uint r = 0; r < r_bonds.size(); r++)
			{
				/** get atoms belonging to rotabale bond 
				 */
				r_a = r_bonds[r]->getFirstAtom();
				r_b = r_bonds[r]->getSecondAtom();
				hinges_[r].nail = r_a;
				hinges_[r].angle = r_b;
				
				/** get atoms that must be rotated with the rotable bond 
				 */
				getAtoms(hinges_[r].angle,hinges_[r].nail,hinges_[r].vec,visited);
				visited.destroy();
				
				vector<const Atom*> aux;
				
				getAtoms(hinges_[r].nail,hinges_[r].angle,aux,visited);
				visited.destroy();
				
				if (hinges_[r].vec.size() > aux.size())
					{
						hinges_[r].nail = r_b;
						hinges_[r].angle = r_a;					
						
						hinges_[r].vec = aux;
					}
			}
		
		std::sort(hinges_.begin(),hinges_.end(), HingeCompare());
		std::sort(hinges_.begin(),hinges_.end(), HingeCompare2());
	}
	
	
	RotateBonds::RotateBonds(const RotateBonds& rb):hinges_(rb.hinges_), mol_(rb.mol_)
	{
	}
	
	
	RotateBonds& RotateBonds::operator=(const RotateBonds& rb)
	{
		if (this == &rb)
			return *this;
		
		hinges_ = rb.hinges_;
		
  mol_ = rb.mol_;
	
  return *this;
	}
	
	
	
	RotateBonds::~RotateBonds()
	{
	}  
	
	void RotateBonds::rotate(std::vector<double> values)
	{
		
		/** i	
		 */
		for (uint x = 0; x < hinges_.size(); x++)
			{
				
				r_matrix_c.setRotationX(BALL::Angle((2*values[x] * PI-PI)));
				
				
				
				BALL::TransformationProcessor rotation_c(r_matrix_c);     
				
				/** now rotable bond lies on the x-axis, rotate all atoms defined in hinges_[x]  
				 */
				for (uint z = 0; z != hinges_[x].vec.size(); z++)
					const_cast<Atom*>(hinges_[x].vec[z])->apply(rotation_c);
			}
	}
	
	RotateBonds::RotateBonds():mol_(0)
	{
	}
	
	void RotateBonds::getAtoms(const BALL::Atom* at_a,  const BALL::Atom* at_b, 
														 std::vector<const BALL::Atom*>& vec, 
														 BALL::HashSet<const BALL::Atom*>& visited)
	{
		
		BALL::AtomBondConstIterator it = at_a->beginBond();
		
		visited.insert(at_a);
		vec.push_back(at_a);
  
		/** iterate over all bonds of atom at_a
		 */
		for (; it != at_a->endBond(); it++)
			{
				/** get other atom of bond
				 */
				const BALL::Atom* partner = it->getPartner(*at_a);
				
				/** test if partner has not already been discoverd or is not at_b
				 */
				if (visited.has(partner) == false && at_a != at_b)
					getAtoms(partner,at_b, vec, visited);
			}
	}
	
	
	int RotateBonds::numberBonds()
	{
		return hinges_.size();
	}
}



