// $Id: global.h,v 1.7 2001/02/23 01:00:43 amoll Exp $

#ifndef BALL_KERNEL_GLOBAL_H
#define BALL_KERNEL_GLOBAL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

namespace BALL 
{

	/**	Bond cloning method.
			This template function implements the cloning of \Ref{Bond}s in AtomContainers.
			As \Ref{Bond}s are not integrated in the Composite tree structure of kernel objects,
			a simple deep cloning of a composite only copies all composites down to atoms. Bonds are not
			included because they are not {\em children} of the atoms but stored in a bond array.
			However, cloning of any kernel objects should naturally clone bonds, too. The implementation of
			this {\em cloning-with-bonds} is divided in two parts: first, a deep (recursive) cloning of
			all composites is performed. Second, the root composite (which is always an AtomContainer,
      as it is either a System or derived from AtomContainer) calls \Ref{cloneBonds} for the
			cloned system to copy the bonds.\\
      The trouble with this implementation is that each clone method must have the possibility to
      call {\bf cloneBonds}, but only the first clone method in the recursive call tree is allowed
      to call it. This is guaranteed by the use of a global static variable \Ref{clone_bonds}.
			The first clone method called sets \Ref{clone_bonds} to {\bf false} thereby forbidding the use of
      \Ref{cloneBonds} to all subsequently called clone methods. Then, it calls \Ref{cloneBonds} and 
			resets \Ref{clone_bonds} to {\bf true}.\\
			This method assumes that the second argument (the composite without bonds) is a deep
			copy of the first argument (the composite containing the atoms). If the tree structures of both
			composites are not isomorphous, bonds are created in an unpredictable way.\\
			{\bf Namespace:} BALL\\
			{\bf Definition:} \URL{BALL/KERNEL/global.h}
			\\
			@param atom_container	the atom_container containing the bonds
			@param cloned a deep copy of {\bf atom_container}
	*/
	template <class AtomContainerType>
	void cloneBonds(const AtomContainerType& atom_container, AtomContainerType& cloned)
	{
		AtomIterator atom_iter_a;
		AtomIterator atom_iter_b;
		Atom::BondIterator bond_iter;
		
		typedef HashMap<const Atom*, Atom*>	AtomMap;

		list<Bond*>	bond_list;
		AtomMap			atom_map;

		// iterate over the two composite structures in parallel
		// caveat: if the two composite structures are not isomorphous, bonds
		// are created between unrelated atoms!
		for (atom_iter_a = atom_container.beginAtom(), atom_iter_b = cloned.beginAtom();
				 !atom_iter_a.isEnd(); ++atom_iter_a, ++atom_iter_b)
		{
			// create a hash map containing a 1:1 relation for the atom pointers
			// atom_map maps atom pointers of atom_container to atom pointers in cloned
			atom_map.insert(pair<const Atom*, Atom*>(&(*atom_iter_a), &(*atom_iter_b)));

			// iterate over all bonds and store the bonds in a list
			// to get each bond exactly once, we check the first atom
			for (bond_iter = (*atom_iter_a).beginBond(); +bond_iter; ++bond_iter) 
			{
				if (*(*bond_iter).getFirstAtom() == *atom_iter_a)
				{
					bond_list.push_back(&(*bond_iter));
				}
			}
		}

		// iterate over all bonds and create a bond in the cloned structure
		// if both atoms of the bond are contained in the cloned structure,
		// thus preventing the copying of bonds between atoms of atom_container
		// and atoms outside atom_container
		list<Bond*>::iterator list_iter = bond_list.begin();
		for ( ; list_iter != bond_list.end(); ++list_iter)
		{
			if (atom_map.has((*list_iter)->getFirstAtom()) && atom_map.has((*list_iter)->getSecondAtom()))
			{
				Atom* a1 = atom_map[(*list_iter)->getFirstAtom()];
				Atom* a2 = atom_map[(*list_iter)->getSecondAtom()];
				Bond*	tmp_bond = new Bond;
				tmp_bond->createBond(*tmp_bond, *a1, *a2);
				(*tmp_bond) = (**list_iter);
				tmp_bond->setFirstAtom(a1);
				tmp_bond->setSecondAtom(a2);
				tmp_bond->finalize();
			}
		}
	}

	/**	Global static variable needed for the cloning of kernel objects containing bonds.
			@see	cloneBonds
			{\bf Namespace:} BALL\\
			{\bf Definition:} \URL{BALL/KERNEL/global.h}			
	*/
	extern bool clone_bonds;

} // namespace BALL  

#endif // BALL_KERNEL_GLOBAL_H
