// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.C,v 1.3 2004/09/02 13:12:51 amoll Exp $
//

#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/COMMON/limits.h>

#include <iostream>
using std::endl;
#include <vector>
using std::vector;
#include <utility>
#include <queue>
using std::priority_queue;
using std::deque;
using std::pair;

namespace BALL
{
	RingPerceptionProcessor::RingPerceptionProcessor()
		:	UnaryProcessor<AtomContainer>()
	{
	}

	RingPerceptionProcessor::RingPerceptionProcessor(const RingPerceptionProcessor& rp)
		:	UnaryProcessor<AtomContainer>(rp)
	{
	}

	RingPerceptionProcessor& RingPerceptionProcessor::operator = (const RingPerceptionProcessor& /* rp */)
	{
		return *this;
	}

	RingPerceptionProcessor::~RingPerceptionProcessor()
	{
	}

	Processor::Result RingPerceptionProcessor::operator () (AtomContainer& AtomContainer)
	{
		vector<vector<Atom*> > sssr;
		calculateSSSR(sssr, AtomContainer);
		return Processor::CONTINUE;
	}

	Size RingPerceptionProcessor::calculateSSSR(vector<vector<Atom*> >& sssr_orig, AtomContainer& ac)
	{
		//cerr << "RingPerceptionProcessr::calculateSSSR(...)";
		AtomContainer AtomContainer;
		AtomContainer = ac; // the algorithm runs on a copy, bc bonds and maybe atoms are destroyed!

		// mapping is needed because this algorithms works on a copy
		HashMap<Atom*, Atom*> copy_to_orig;
		AtomIterator orig = ac.beginAtom();
		AtomIterator copy = AtomContainer.beginAtom();
		for (;orig!=ac.endAtom();++orig,++copy)
		{
			copy_to_orig.insert(std::make_pair(&(*copy),&(*orig)));
		}
		
		HashSet<Atom*> full_set; // herein are all nodes (atoms)
		HashSet<Atom*> trim_set; // herein are all the zero edge nodes
		vector<HashSet<Atom*> > SSSR; // stores the rings 
		
		AtomIterator atom_it = AtomContainer.beginAtom();
		for (;atom_it!=AtomContainer.endAtom();++atom_it)
		{
			full_set.insert(&(*atom_it));
		}

		while (full_set.size() != trim_set.size())
		{
			HashSet<Atom*>::Iterator it = full_set.begin();
			unsigned int min_deg = Limits<int>::max();
			Atom * init;
			HashSet<Atom*> nodes_n2_set;
			for (;it!=full_set.end();++it)
			{
				//add all nodes with degree 0 to trim_set
				if ((*it)->countBonds() == 0)
				{
					trim_set.insert(*it);
				}
				else 
				{
					// find beginning node init in full_set-trim_set with minimal degree
					if ((*it)->countBonds() < min_deg)
					{
						min_deg = (*it)->countBonds();
						init = *it;
					}
					// add n2-nodes from full_set - trim_set to nodes_n2
					if ((*it)->countBonds() == 2)
					{
						nodes_n2_set.insert(*it);
					}
				}
			}

			if (min_deg == 1)
			{
				init->destroyBond(*(init->beginBond()->getPartner(*init)));
				trim_set.insert(init);
			}
			else 
			{
				HashSet<Atom*> ring_set;
				if (min_deg == 2)
				{
					for (it=nodes_n2_set.begin();it!=nodes_n2_set.end();++it) 
					{
						Size ring_size = getRing_(*it, ring_set);
						if (ring_size > 2)
						{
							// add ring_set to SSSR
							SSSR.push_back(ring_set);
						}
					}
					// for every chain of n2 nodes, isolate one and delete the chain it belongs
					HashSet<Atom*>::Iterator it2;
					for (it2=nodes_n2_set.begin();it2!=nodes_n2_set.end();++it2)
					{
						if ((*it2)->countBonds() != 0)
						{
							if (!(*it2)->destroyBond(*((*it2)->beginBond()->getPartner(**it2))))
							{
								// should really not occur!
							}
						}
					}
				}
				else 
				{
					if (min_deg >= 3)
					{
						Size ring_size = getRing_(init, ring_set);
						if (ring_size > 0)
						{
							// add ring_set ro sssr
							SSSR.push_back(ring_set);
							checkEdges_(ring_set, AtomContainer);
						}
					}
				}
			}
		}
	
		// now put the computet rings in the referenced structure
		for (vector<HashSet<Atom*> >::iterator i=SSSR.begin();i!=SSSR.end();++i)
		{
			vector<Atom*> ring;
			for (HashSet<Atom*>::Iterator j=i->begin();j!=i->end();++j)
			{
				ring.push_back(copy_to_orig[*j]);
			}
			sort(ring.begin(),ring.end());
			sssr_orig.push_back(ring);
		}
	
		// erase the copies (erasing here should be faster than searching by every input
		sort(sssr_orig.begin(),sssr_orig.end());
		vector<vector<Atom*> >::iterator vhs_it = unique(sssr_orig.begin(),sssr_orig.end());
		sssr_orig.erase(vhs_it,sssr_orig.end());

		// set "InRing" properties of the bond and atoms;
    // save the atoms as "InRing" as atom property
    for (Size i=0;i!=sssr_orig.size();++i)
    {
			for (Size j=0;j!=sssr_orig[i].size();++j)
			{
				sssr_orig[i][j]->setProperty("InRing", true);
			}
			for (Size j=0;j!=sssr_orig[i].size();++j)
			{
				for (Size k=0;k!=sssr_orig[i].size();++k)
				{
					if (sssr_orig[i][k]->isBoundTo(*sssr_orig[i][j]))
					{
						sssr_orig[i][k]->getBond(*sssr_orig[i][j])->setProperty("InRing", true);
					}
				}
			}	
		}
		
		// the atoms that are not participating a ring
   	for (AtomIterator i=AtomContainer.beginAtom();i!=AtomContainer.endAtom();++i)
		{
		  if (!i->hasProperty("InRing"))
		  {
				i->setProperty("InRing", false);
			}
		}

		AtomIterator a_it = AtomContainer.beginAtom();
		Atom::BondIterator b_it = a_it->beginBond();
		BALL_FOREACH_BOND (AtomContainer, a_it, b_it)
		{
			if (!b_it->hasProperty("InRing"))
			{
				b_it->setProperty("InRing", false);
			}
		}

		//cerr << "..ended!" << endl;
		
		return sssr_orig.size();
	}



	Size RingPerceptionProcessor::getRing_(Atom* n, HashSet<Atom*>& ring_set)
	{
		deque<std::pair<Atom*, Atom*> > the_q; // double ended queue with node and its ancestor
		the_q.push_back(std::make_pair(n,n));
		
		Atom::BondIterator bond_it;
		HashMap<Atom*, HashSet<Atom*> > paths;
		HashSet<Atom*> tmp;
		tmp.insert(n);
		paths.insert(std::make_pair(n, tmp));

		while (!the_q.empty())
		{
			pair<Atom*, Atom*> atom_anc = the_q.front();
			Atom* atom = atom_anc.first;
			Atom* ancestor = atom_anc.second;
			the_q.pop_front();
			for (bond_it=atom->beginBond();bond_it!=atom->endBond();++bond_it)
			{
				Atom* bound_atom =  bond_it->getPartner(*atom);
				if (bound_atom != ancestor)
				{
					if (!paths.has(bound_atom))
					{
						HashSet<Atom*> path_atom = paths[atom];
						path_atom.insert(bound_atom);
						paths.insert(std::make_pair(bound_atom, path_atom));
					}
					else 
					{
						HashSet<Atom*> merge, atom1, atom2;
						atom1 = paths[bound_atom];
						atom2 = paths[atom];
						merge.set(atom1);

						// workaround, don't know how to merge to hashsets, the sets are small, 
						// doesn't touch the performance that much
						HashSet<Atom*>::Iterator set_it;
						for (set_it=atom2.begin();set_it!=atom2.end();++set_it)
						{
							if (!merge.has(*set_it))
							{
								merge.insert(*set_it);
							}
						}

						if (atom1.size()+atom2.size()-merge.size() == 1)
						{
							ring_set = merge;
							return merge.size();
						}
					}
					the_q.push_back(std::make_pair(bound_atom, atom));
				}
			}
		}
		return 0;
	}



	void RingPerceptionProcessor::checkEdges_(HashSet<Atom*>& ring_set, AtomContainer& ac)
	{
		// Every node with degree 3 (which is a memeber of ring_set) is deleted testwise.
		// Finally the node which produces the smalles ring is deleted.

		HashSet<Atom*>::Iterator iter=ring_set.begin();
		Atom * min_atom = *iter; // set as default to avoid compiler warning
		Size min_ring_size = Limits<int>::max();
		
		for (;iter!=ring_set.end();++iter)
		{
			if ((*iter)->countBonds() == 3)
			{
				AtomContainer AtomContainer = ac;
				
				// mapping to switch between copy and orig AtomContainer
				HashMap<Atom*, Atom*> copy_to_orig;
				HashMap<Atom*, Atom*> orig_to_copy;
				AtomIterator copy = AtomContainer.beginAtom();
				AtomIterator orig = ac.beginAtom();
				for (;orig!=ac.endAtom();++copy, ++orig)
				{
					copy_to_orig.insert(std::make_pair(&(*copy),&(*orig)));
					orig_to_copy.insert(std::make_pair(&(*orig),&(*copy)));
				}
				
				orig_to_copy[*iter]->destroy();
				
				Atom::BondIterator bond_it=(*iter)->beginBond();
				for (;bond_it!=(*iter)->endBond();++bond_it)
				{
					Atom* atom = bond_it->getPartner(**iter);
					HashSet<Atom*> rs;
					Size size = getRing_(atom, rs);
								
					if (min_ring_size > size)
					{
						min_ring_size = size;
						min_atom = *iter;
					}
				}
			}
		}
		// now delete min_atom
		min_atom->destroy();
	}

} // namespace BALL
