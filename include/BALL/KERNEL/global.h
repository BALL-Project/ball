// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: global.h,v 1.25 2005/10/23 12:02:19 oliver Exp $
//

#ifndef BALL_KERNEL_GLOBAL_H
#define BALL_KERNEL_GLOBAL_H

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

namespace BALL 
{
	/**	Bond cloning method.
			This template function implements the cloning of  \link Bond Bond \endlink s in
			AtomContainers.  As Bonds are not integrated in the Composite tree
			structure of kernel objects, a simple deep cloning of a composite
			only copies all composites down to atoms. Bonds are not included
			because they are not <b>  children </b> of the atoms but stored in a bond
			array.  However, cloning of any kernel objects should naturally clone
			bonds, too. The implementation of this {\em cloning-with-bonds} is
			divided in two parts: first, a deep (recursive) cloning of all
			composites is performed. Second, the root composite (which is always
			an AtomContainer) calls  \link cloneBonds cloneBonds \endlink  for the cloned system to
			copy the bonds.
			 \par
			The trouble with this implementation is that each clone method must
			have the possibility to call <b>cloneBonds</b>, but only the first
			clone method in the recursive call tree is allowed to call it. This
			is guaranteed by the use of a global static variable
			 \link clone_bonds clone_bonds \endlink .  The first clone method called sets clone_bonds to
			<b>false</b> thereby forbidding the use of cloneBonds to all
			subsequently called clone methods. Then, it calls cloneBonds and
			resets clone_bonds to <b>true</b>.
			 \par
			This method assumes that the second argument (the composite without
			bonds) is a deep copy of the first argument (the composite containing
			the atoms). If the tree structures of both composites are not
			isomorphous, bonds are created in an unpredictable way.
			 \par
			<b>Namespace:</b> BALL
			 \par
			
			 \par
			@param atom_container	the atom_container containing the bonds
			@param cloned a deep copy of <b>atom_container</b>
    	
			\ingroup  KernelMiscellaneous
	*/
	template <class AtomContainerType>
	void cloneBonds(const AtomContainerType& atom_container, AtomContainerType& cloned)
	{
		typedef HashMap<const Atom*, Atom*>	AtomMap;
		AtomMap atom_map;

		std::list<const Bond*> bond_list;

		// iterate over the two composite structures in parallel
		// caveat: if the two composite structures are not isomorphous, bonds
		// are created between unrelated atoms!
		Atom::BondConstIterator bond_iter;		
		AtomConstIterator atom_iter_a(atom_container.beginAtom());
		AtomIterator atom_iter_b(cloned.beginAtom());

		for (; +atom_iter_a && +atom_iter_b; ++atom_iter_a, ++atom_iter_b)
		{
			// create a hash map containing a 1:1 relation for the atom pointers
			// atom_map maps atom pointers of atom_container to atom pointers in cloned
			atom_map.insert(std::pair<const Atom*, Atom*>(&*atom_iter_a, &*atom_iter_b));

			// iterate over all bonds and store the bonds in a list
			// to get each bond exactly once, we check the first atom
			for (bond_iter = atom_iter_a->beginBond(); bond_iter != atom_iter_a->endBond(); ++bond_iter) 
			{
				if (bond_iter->getFirstAtom() == &(*atom_iter_a))
				{
					bond_list.push_back(&(*bond_iter));
				}
			}
		}

		// iterate over all bonds and create a bond in the cloned structure
		// if both atoms of the bond are contained in the cloned structure,
		// thus preventing the copying of bonds between atoms of atom_container
		// and atoms outside atom_container
		std::list<const Bond*>::iterator list_iter = bond_list.begin();
		for ( ; list_iter != bond_list.end(); ++list_iter)
		{
			if (atom_map.has((*list_iter)->getFirstAtom()) && atom_map.has((*list_iter)->getSecondAtom()))
			{
				Atom* a1 = atom_map[(*list_iter)->getFirstAtom()];
				Atom* a2 = atom_map[(*list_iter)->getSecondAtom()];
				Bond*	tmp_bond = static_cast<Bond*>((*list_iter)->create(false, true));
				tmp_bond->createBond(*tmp_bond, *a1, *a2);
				*tmp_bond = **list_iter;
				tmp_bond->setFirstAtom(a1);
				tmp_bond->setSecondAtom(a2);
				tmp_bond->finalize();
			}
		}
	}

	/**	Global static variable needed for the cloning of kernel objects containing bonds.
			<b>Namespace:</b> BALL \par
			@see	cloneBonds
			\ingroup  KernelMiscellaneous
	*/
	extern bool clone_bonds;
} // namespace BALL  

#endif // BALL_KERNEL_GLOBAL_H
