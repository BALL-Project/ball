// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.C,v 1.6 2004/10/29 15:18:00 amoll Exp $
//

#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/COMMON/limits.h>

#include <vector>
#include <queue>

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

	Processor::Result RingPerceptionProcessor::operator () (AtomContainer& ac)
	{
		vector<vector<Atom*> > sssr;
		calculateSSSR(sssr, ac);
		return Processor::CONTINUE;
	}

	Size RingPerceptionProcessor::calculateSSSR(vector<vector<Atom*> >& sssr_orig, AtomContainer& ac)
	{
		// the algorithm runs on a copy, bc bonds and maybe atoms are destroyed!
		AtomContainer ac_copy(ac);

		// mapping is needed because this algorithms works on a copy
		HashMap<Atom*, Atom*> copy_to_orig;
		AtomIterator orig = ac.beginAtom();
		AtomIterator copy = ac_copy.beginAtom();
		for (; +orig; ++orig, ++copy)
		{
			copy_to_orig[&*copy] = &*orig;
		}
		
		// herein are all nodes (atoms)
		HashSet<Atom*> full_set; 			
		AtomIterator atom_it = ac_copy.beginAtom();
		for (; +atom_it; ++atom_it)
		{
			full_set.insert(&*atom_it);
		}

		HashSet<Atom*> trim_set; 			// herein are all the zero edge nodes
		vector<HashSet<Atom*> > SSSR; // stores the rings 

		while (full_set.size() != trim_set.size())
		{
			Size min_deg = Limits<int>::max();
			Atom* init = 0;

			// all atoms with 2 bonds
			HashSet<Atom*> nodes_n2_set;

			// iterate over all atoms in AtomContainer ac
			HashSet<Atom*>::Iterator it = full_set.begin();
			for (; +it ;++it)
			{
				Size nr_bonds = (**it).countBonds();

				//add all nodes with degree 0 to trim_set
				if (nr_bonds == 0)
				{
					trim_set.insert(*it);
					continue;
				}

				// find beginning node init in full_set-trim_set with minimal degree
				if (nr_bonds < min_deg)
				{
					min_deg = nr_bonds;
					init = *it;
				}

				// add n2-nodes from full_set - trim_set to nodes_n2
				if (nr_bonds == 2)
				{
					nodes_n2_set.insert(*it);
				}
			}

			if (min_deg == 1)
			{
				init->destroyBond(*(init->beginBond()->getPartner(*init)));
				trim_set.insert(init);
				continue;
			}

			HashSet<Atom*> ring_set;
			if (min_deg == 2)
			{
				for (it = nodes_n2_set.begin(); +it; ++it) 
				{
					Size ring_size = getRing_(*it, ring_set);
					if (ring_size > 2)
					{
						// add ring_set to SSSR
						SSSR.push_back(ring_set);
					}
				}

				// for every chain of n2 nodes, isolate one and delete the chain it belongs
				HashSet<Atom*>::Iterator it2 = nodes_n2_set.begin();
				for (; +it2; ++it2)
				{
					Atom& a = **it2;
					if (a.countBonds() != 0)
					{
						a.destroyBond(* a.beginBond()->getPartner(a));
					}
				}

				continue;
			}

			if (min_deg >= 3 && getRing_(init, ring_set) != 0)
			{
				// add ring_set ro sssr
				SSSR.push_back(ring_set);
				checkEdges_(ring_set);
			}
		}
	
		// now put the computed rings in the referenced structure
		for (vector<HashSet<Atom*> >::iterator i=SSSR.begin(); i != SSSR.end(); ++i)
		{
			vector<Atom*> ring;
			HashSet<Atom*>::Iterator j=i->begin();
			for (; j != i->end(); ++j)
			{
				ring.push_back(copy_to_orig[*j]);
			}
			sort(ring.begin(), ring.end());
			sssr_orig.push_back(ring);
		}
	
		// erase the copies (erasing here should be faster than searching by every input)
		sort(sssr_orig.begin(), sssr_orig.end());
		vector<vector<Atom*> >::iterator vhs_it = unique(sssr_orig.begin(),sssr_orig.end());
		sssr_orig.erase(vhs_it, sssr_orig.end());

		// set "InRing" properties of the bond and atoms
    for (Size i=0; i < sssr_orig.size(); ++i)
    {
			// set "InRing" as property for all ring atoms
			for (Size j=0; j < sssr_orig[i].size(); ++j)
			{
				sssr_orig[i][j]->setProperty("InRing", true);
			
				// set "InRing" as property for all ring bonds
				for (Size k=0; k < sssr_orig[i].size(); ++k)
				{
					if (sssr_orig[i][k]->isBoundTo(*sssr_orig[i][j]))
					{
						sssr_orig[i][k]->getBond(*sssr_orig[i][j])->setProperty("InRing", true);
					}
				}
			}	
		}
		
		// the atoms that are not participating in a ring
   	for (AtomIterator i = ac.beginAtom(); +i; ++i)
		{
		  if (!i->hasProperty("InRing"))
		  {
				i->setProperty("InRing", false);
			}
		}

		// the bonds that are not participating in a ring
		AtomIterator a_it = ac.beginAtom();
		Atom::BondIterator b_it = a_it->beginBond();
		BALL_FOREACH_BOND (ac, a_it, b_it)
		{
			if (!b_it->hasProperty("InRing"))
			{
				b_it->setProperty("InRing", false);
			}
		}
		
		return sssr_orig.size();
	}


	Size RingPerceptionProcessor::getRing_(Atom* n, HashSet<Atom*>& ring_set)
	{
		if (n == 0) return 0;
		deque<std::pair<Atom*, Atom*> > the_q; // double ended queue with node and its ancestor
		the_q.push_back(std::make_pair(n,n));
		
		HashSet<Atom*> tmp;
		tmp.insert(n);

		HashMap<Atom*, HashSet<Atom*> > paths;
		paths.insert(std::make_pair(n, tmp));

		while (!the_q.empty())
		{
			pair<Atom*, Atom*> atom_anc = the_q.front();
			Atom* atom 		 = atom_anc.first;
			Atom* ancestor = atom_anc.second;
			the_q.pop_front();

			Atom::BondIterator bond_it;
			for (bond_it=atom->beginBond(); +bond_it; ++bond_it)
			{
				Atom* bound_atom =  bond_it->getPartner(*atom);
				if (bound_atom == 0 ||
						bound_atom == ancestor)
				{
					continue;
				}

				if (!paths.has(bound_atom))
				{
					HashSet<Atom*> path_atom = paths[atom];
					path_atom.insert(bound_atom);
					paths[bound_atom] = path_atom;
				}
				else 
				{
					const HashSet<Atom*>& path1 = paths[bound_atom];
					const HashSet<Atom*>& path2 = paths[atom];
					HashSet<Atom*> merge(path1 + path2);

					if (path1.size() + path2.size() - merge.size() == 1)
					{
						ring_set = merge;
						return merge.size();
					}
				}
				the_q.push_back(std::make_pair(bound_atom, atom));
			}
		}
		return 0;
	}



	void RingPerceptionProcessor::checkEdges_(HashSet<Atom*>& ring_set)
	{
		Size min_ring_size = Limits<int>::max();

		// atom which is part of the smallest ring
		Atom* min_atom = 0; 
		
		HashSet<Atom*>::Iterator iter = ring_set.begin();
		for (; +iter; ++iter)
		{
			Atom::BondIterator bond_it=(*iter)->beginBond();
			for (; +bond_it; ++bond_it)
			{
				Atom* atom = bond_it->getPartner(**iter);
				if (atom == 0) continue;

				HashSet<Atom*> dummy;
				Size ring_size = getRing_(atom, dummy);
							
				if (min_ring_size > ring_size)
				{
					min_ring_size = ring_size;
					min_atom = *iter;
				}
			}
		}
		
		// Finally the atom which produces the smallest ring is deleted.
		if (min_atom != 0) min_atom->destroy();
	}

} // namespace BALL
