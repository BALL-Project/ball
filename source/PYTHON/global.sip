# 1 "global.h"
namespace BALL {
 template <class AtomContainerType>
 void cloneBonds(const AtomContainerType& atom_container, AtomContainerType& cloned)
 {
  AtomIterator atom_iter_a;
  AtomIterator atom_iter_b;
  Atom::BondIterator bond_iter;
  typedef HashMap<const Atom*, Atom*> AtomMap;
  list<Bond*> bond_list;
  AtomMap   atom_map;
  for (atom_iter_a = atom_container.beginAtom(), atom_iter_b = cloned.beginAtom();
     !atom_iter_a.isEnd(); ++atom_iter_a, ++atom_iter_b)
  {
   atom_map.insert(pair<const Atom*, Atom*>(&(*atom_iter_a), &(*atom_iter_b)));
   for (bond_iter = (*atom_iter_a).beginBond(); +bond_iter; ++bond_iter) 
   {
    if (*(*bond_iter).getFirstAtom() == *atom_iter_a)
    {
     bond_list.push_back(&(*bond_iter));
    }
   }
  }
  list<Bond*>::iterator list_iter = bond_list.begin();
  for ( ; list_iter != bond_list.end(); ++list_iter)
  {
   if (atom_map.has((*list_iter)->getFirstAtom()) && atom_map.has((*list_iter)->getSecondAtom()))
   {
    Atom* a1 = atom_map[(*list_iter)->getFirstAtom()];
    Atom* a2 = atom_map[(*list_iter)->getSecondAtom()];
    Bond* tmp_bond = new Bond;
    tmp_bond->createBond(*tmp_bond, *a1, *a2);
    tmp_bond->set(**list_iter);
    tmp_bond->setFirstAtom(a1);
    tmp_bond->setSecondAtom(a2);
    tmp_bond->finalize();
   }
  }
 }
 extern bool clone_bonds;
}  
