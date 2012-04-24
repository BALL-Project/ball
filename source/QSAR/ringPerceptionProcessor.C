// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.C,v 1.22.4.2 2007/04/03 13:29:39 bertsch Exp $
//

#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/KERNEL/forEach.h>

#include <limits>

#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
#undef  BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG

#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
	#include <iostream>
#endif

#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_MAX_RUNS 100

using namespace std;

namespace BALL
{
	const char* RingPerceptionProcessor::Option::ALGORITHM_NAME = "algorithm_name";
	const char* RingPerceptionProcessor::Default::ALGORITHM_NAME = "Balducci";

	RingPerceptionProcessor::RingPerceptionProcessor()
		:	UnaryProcessor<AtomContainer>()
	{
		setDefaultOptions();
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
		setDefaultOptions();
	}

	void RingPerceptionProcessor::setDefaultOptions()
	{
		options.setDefault(Option::ALGORITHM_NAME, Default::ALGORITHM_NAME);
	}

	Processor::Result RingPerceptionProcessor::operator () (AtomContainer& ac)
	{
		// call the calculate function
		vector<vector<Atom*> > sssr;
		calculateSSSR(sssr, ac);

		// set all atom and bonds which are not in a ring (for consistence)
		AtomIterator a_it;
		BALL_FOREACH_ATOM(ac, a_it)
		{
			if (!a_it->hasProperty("InRing"))
			{
				a_it->setProperty("InRing", false);
			}
		}
		
		Atom::BondIterator b_it;
		BALL_FOREACH_BOND(ac, a_it, b_it)
		{
			if (!b_it->hasProperty("InRing"))
			{
				b_it->setProperty("InRing", false);
			}
		}
		return Processor::BREAK;
	}

	Size RingPerceptionProcessor::calculateSSSR(vector<vector<Atom*> >& sssr_orig, AtomContainer& ac)
	{
		all_small_rings_.clear();

		String algorithm_name = options.get(Option::ALGORITHM_NAME);
		if (algorithm_name == "Balducci")
		{
			// build molecular graph
			Molecule* mol = static_cast<Molecule*>(&ac);
			SimpleMolecularGraph mol_graph(*mol);
		
			vector<Bond*> to_delete;
      for (SimpleMolecularGraph::EdgeIterator eit = mol_graph.beginEdge(); eit != mol_graph.endEdge(); ++eit)
      {
        Bond::Type bond_type = eit->getBond()->getType();
        if (bond_type == Bond::TYPE__HYDROGEN || bond_type == Bond::TYPE__DISULPHIDE_BRIDGE)
        {
          to_delete.push_back(eit->getBond());
        }
      }

      for (Size i = 0; i != to_delete.size(); ++i)
      {
      	mol_graph.deleteEdge(*to_delete[i]);
      }

			// detect the bccs
			vector<SimpleMolecularGraph*> bccs;
			findAllBCC(bccs, mol_graph);

			Size num_rings(0);

			// for each bcc that potentially contains rings do the Balducci/Pearlman ring perception
			for (vector<SimpleMolecularGraph*>::iterator it = bccs.begin(); it != bccs.end(); ++it)
			{
				if ((*it)->getNumberOfNodes() > 2 && (*it)->getNumberOfEdges() > 2)
				{
					num_rings += BalducciPearlmanAlgorithm_(sssr_orig, **it);
				}
			}

			// now delete the bccs
			for (vector<SimpleMolecularGraph*>::iterator it = bccs.begin(); it != bccs.end(); ++it)
			{
				delete *it;
			}

			return num_rings;
		}
		else
		{
			if (algorithm_name == "Figueras")
			{
				return FiguerasAlgorithm_(sssr_orig, ac);
			}
			else
			{
				Log.error() << "Unknown ring perception algorithm: " << algorithm_name << endl;
				return 0;
			}
		}

		return 0;
	}

	const vector<vector<Atom*> >& RingPerceptionProcessor::getAllSmallRings() const
	{
		return all_small_rings_;
	}
	
	Size RingPerceptionProcessor::FiguerasAlgorithm_(vector<vector<Atom*> >& sssr_orig, AtomContainer& ac)
	{
		// the algorithm runs on a copy, bc bonds and maybe atoms are destroyed!
		AtomContainer ac_copy(ac);

		// mapping is needed because this algorithms works on a copy
		HashMap<Atom*, Atom*> copy_to_orig_;
		AtomIterator orig = ac.beginAtom();
		AtomIterator copy = ac_copy.beginAtom();
		for (; +orig; ++orig, ++copy)
		{
			copy_to_orig_[&*copy] = &*orig;
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
			Size min_deg = std::numeric_limits<int>::max();
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
						Atom::BondIterator b_it = a.beginBond();
						a.destroyBond(* b_it->getPartner(a));
					}
				}

				continue;
			}

			if (min_deg > 2 && getRing_(init, ring_set) != 0)
			{
				// add ring_set ro sssr
				SSSR.push_back(ring_set);
				checkEdges_(ring_set, ac_copy);
			}
		}
	
		// now put the computed rings in the referenced structure
		for (vector<HashSet<Atom*> >::iterator i = SSSR.begin(); i != SSSR.end(); ++i)
		{
			vector<Atom*> ring;
			HashSet<Atom*>::Iterator j = i->begin();
			for (; j != i->end(); ++j)
			{
				ring.push_back(copy_to_orig_[*j]);
			}
			sort(ring.begin(), ring.end());
			sssr_orig.push_back(ring);
		}
	
		// erase the copies (erasing here should be faster than searching by every input)
		sort(sssr_orig.begin(), sssr_orig.end());
		vector<vector<Atom*> >::iterator vhs_it = unique(sssr_orig.begin(), sssr_orig.end());
		sssr_orig.erase(vhs_it, sssr_orig.end());

		// set "InRing" properties of the bond and atoms
    for (Size i = 0; i < sssr_orig.size(); ++i)
    {
			// set "InRing" as property for all ring atoms
			for (Size j = 0; j < sssr_orig[i].size(); ++j)
			{
				sssr_orig[i][j]->setProperty("InRing", true);
			
				// set "InRing" as property for all ring bonds
				for (Size k = 0; k < sssr_orig[i].size(); ++k)
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
		the_q.push_back(std::make_pair(n, n));
		
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
				Atom* bound_atom = bond_it->getPartner(*atom);
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


	void RingPerceptionProcessor::checkEdges_(HashSet<Atom*>& ring_set, AtomContainer& ac)
	{
		// from Figueras' paper:
		// "checkEdges(n, ringSet)... selects an optimum edge for
		// elimination in structures without N2 nodes. Each edge in
		// ringSet is selected in turn for trial, and the edge is restored
		// after its trial is finished. The larger of the two rings that
		// includes the end nodes of the trial is noted. Final
		// selection for edge eliminiation is that edge that is incident
		// on the smallest of the collection of these largest rings. After
		// an edge is removed, new N2 nodes will appear to serve for as
		// foci for further ring perception."

		// The last sentence is definitely incorrect, as there are 
		// (sub)structures which have only atoms which are bonded 
		// four times!

		typedef HashSet<Atom*>::Iterator hs_it;
		HashSet<Bond*> visited_bonds;
		vector<Size> largest_rings;
		vector<std::pair<Atom*, Atom*> > incident_edges;

		hs_it it = ring_set.begin();
		hs_it it2 = ring_set.begin();
		
		for (; +it; ++it)
		{
			// bond to examine
			Atom * partner = 0;
			for (it2 = ring_set.begin(); +it2; ++it2)
			{
				if ((*it)->isBoundTo(**it2))
				{
					Bond* bnd = (*it)->getBond(**it2);
					if (!visited_bonds.has(bnd))
					{
						visited_bonds.insert(bnd);
						partner = *it2;
						break;
					}
				}
			}

			// delete bond on a copy, mapping needed
			AtomContainer ac_copy(ac);
	    HashMap<Atom*, Atom*> orig_to_copy;
	    AtomIterator orig = ac.beginAtom();
	    AtomIterator copy = ac_copy.beginAtom();
			
	    for (; +orig; ++orig, ++copy)
	    {
				orig_to_copy[&*orig] = &*copy;
			}

			Atom * atom1 = orig_to_copy[*it];
			Atom * atom2 = orig_to_copy[partner];	
			atom1->destroyBond(*atom2);

			// notice largest ring which contains 
			// the end atom of the bond
			HashSet<Atom*> ring_set1, ring_set2;
			Size ring_size1 = getRing_(atom1, ring_set1);
			Size ring_size2 = getRing_(atom2, ring_set2);
			
			if (ring_size1 > ring_size2)
			{
				largest_rings.push_back(ring_set1.size());
				incident_edges.push_back(std::make_pair(*it, partner));
			}
			else
			{
				largest_rings.push_back(ring_set2.size());
				incident_edges.push_back(std::make_pair(*it, partner));
			}
		}

		// find smallest of the biggest rings
		vector<Size>::iterator it1=largest_rings.begin();
		Size iter(0), smallest_ring_it(0), smallest_ring(0);

		for (; it1 != largest_rings.end(); ++it1, ++iter)
		{
			if (*it1 < smallest_ring)
			{
				smallest_ring = *it1;
				smallest_ring_it = iter;
			}
		}

		// destroy the selected bond
		incident_edges[smallest_ring_it].first->destroyBond(*incident_edges[smallest_ring_it].second);
		
	}


	// bcc code

	Size RingPerceptionProcessor::findAllBCC(vector<SimpleMolecularGraph*>& bccs, SimpleMolecularGraph& graph)
	{
		// delete all content from the strcutures
		visited_.clear();
		Size dfbi(0);
		HashMap<NodeItem<Index, Index>*, Size> DFBIndex;
		P_.clear();
		parents_.clear();
		BCC_ = stack<EdgeItem<Index, Index>* >();
		visited_bonds_.clear();

		// for each node in the graph apply the recursive function
		for (SimpleMolecularGraph::NodeIterator ait = graph.beginNode(); ait != graph.endNode(); ++ait)
		{
			NodeItem<Index, Index>* v = &*ait;
			if (!visited_.has(v))
			{
				DFSBCC_(bccs, dfbi, DFBIndex, v);
			}
		}

		return bccs.size();
	}

	void RingPerceptionProcessor::DFSBCC_(vector<SimpleMolecularGraph*>& bccs, Size dfbi, 
																				HashMap<NodeItem<Index, Index>*, Size> DFBIndex, 
																				NodeItem<Index, Index>* v)
	{
		visited_.insert(v);
		dfbi++;
		DFBIndex[v] = dfbi;
		P_[v] = dfbi;
	
		for (NodeItem<Index, Index>::Iterator bit = v->begin(); bit != v->end(); ++bit)
		{
			if (!visited_bonds_.has(*bit))
			{
				BCC_.push(*bit);
				visited_bonds_.insert(*bit);

				#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
				cerr << String('\t', dfbi) << "pushed: " << *bit << " (stack size: " << BCC_.size() << endl;
				#endif

				NodeItem<Index, Index>* v_prime = 0;
				if (&(*bit)->getSource() == v)
				{				
					v_prime = &(*bit)->getTarget();
				}
				else
				{
					v_prime = &(*bit)->getSource();
				}

				if (!visited_.has(v_prime))
				{
					parents_[v_prime] = v;
					
					#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
					cerr << String('\t', dfbi) << "Calling DFSBCC_" << endl;
					#endif
					
					DFSBCC_(bccs, dfbi, DFBIndex, v_prime);
					#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
					cerr << String('\t', dfbi) << "returned DFSBCC_ " << P_[v_prime] << " " << DFBIndex[v] << endl;
					#endif 
					
					if (P_[v_prime] >= DFBIndex[v])
					{
						// we found a bcc
						#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
						cerr << String('\t', dfbi) << "> found bcc (stack size: " << BCC_.size() << ")" << endl;
						#endif

						// pop all the bcc member bonds from the stack
						HashSet<Atom* > ac;
						HashSet<Bond* > add_edges;
						while (BCC_.top() != *bit)
						{
							EdgeItem<Index, Index>* bond = BCC_.top();
							add_edges.insert(bond->getBond());
							ac.insert(bond->getSource().getAtom());
							ac.insert(bond->getTarget().getAtom());
							BCC_.pop();
						}
						
						EdgeItem<Index, Index>* bond = BCC_.top();
						ac.insert(bond->getSource().getAtom());
						ac.insert(bond->getTarget().getAtom());
						add_edges.insert(bond->getBond());
						BCC_.pop();
						
						// now all items are collected, lets build the new graph						
						// first adding the nodes!!!
						SimpleMolecularGraph* new_graph = new SimpleMolecularGraph;
						for (HashSet<Atom*>::Iterator it = ac.begin(); it != ac.end(); ++it)
						{
							new_graph->newNode(**it);
						}

						// second add the edges
						for (HashSet<Bond*>::Iterator it = add_edges.begin(); +it; ++it)
						{
							new_graph->newEdge(**it);
						}

						bccs.push_back(new_graph);							
					}
					#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
					cerr << String('\t', dfbi) << "setting P[v] to min(" << P_[v] << 
						", " << P_[v_prime] << "); v_prime not visited before" << endl;
					#endif

					P_[v] = min(P_[v], P_[v_prime]);
				}
				else
				{
					if (v_prime != parents_[v])
					{
						#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
						cerr << String('\t', dfbi) << " setting P[v] to min(" << P_[v] << 
							", " << DFBIndex[v_prime] << "); v_prime visited before && v_prime != Father[v]" << endl;
						#endif
						P_[v] = min(P_[v], DFBIndex[v_prime]);
					}
				}
			}
			#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
			else
			{
				cerr << String('\t', dfbi) << "bond already visited" << endl;
			}
			#endif
		}	
	}


	// Balducci, Pearlman algorithm
	HashMap<RingPerceptionProcessor::TNode_*, NodeItem<Index, Index> *> RingPerceptionProcessor::tnode_to_atom_;
	HashMap<NodeItem<Index, Index>* , RingPerceptionProcessor::TNode_*> RingPerceptionProcessor::atom_to_tnode_;
	HashMap<EdgeItem<Index, Index> *, Size> RingPerceptionProcessor::bond_to_index_;
	HashMap<Size, EdgeItem<Index, Index> *> RingPerceptionProcessor::index_to_bond_;
	vector<BitVector> RingPerceptionProcessor::rings_;
	vector<BitVector> RingPerceptionProcessor::matrix_;
	vector<BitVector> RingPerceptionProcessor::forwarded_rings_;
	vector<BitVector> RingPerceptionProcessor::tested_beers_;
	vector<vector<Atom*> > RingPerceptionProcessor::all_small_rings_;
	vector<BitVector> RingPerceptionProcessor::all_small_beers_;

	void RingPerceptionProcessor::TNode_::recieve()
	{
#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
		cerr << "size of recieve buffer is " << recieve_buffer.size() << endl;
		for (vector<PathMessage_>::iterator it1 = recieve_buffer.begin(); it1 != recieve_buffer.end(); ++it1)
		{
			cerr << it1->beep << " (" << it1->beep.countValue(true) << ")" << endl;
		}
#endif

		vector<BitVector> do_not_forward;

		// build the A array
		HashMap<EdgeItem<Index, Index>*, HashMap<TNode_*, vector<PathMessage_> > > array_A;
		for (vector<PathMessage_>::iterator it = recieve_buffer.begin(); it != recieve_buffer.end(); ++it)
		{
			array_A[it->efirst][it->nfirst].push_back(*it);
		}

		// merge the messages
		for (HashMap<EdgeItem<Index, Index>*, HashMap<TNode_*, vector<PathMessage_> > >::Iterator it1 = array_A.begin(); it1 != array_A.end(); ++it1)
		{
			for (HashMap<TNode_*, vector<PathMessage_> >::Iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
			{
				if (it2->second.size() > 1)
				{
					vector<PathMessage_> new_message;
					new_message.push_back(it2->second[0]);
					
					for (Size i = 1; i != it2->second.size(); ++i)
					{
						do_not_forward.push_back(it2->second[i].beep);
					}
					it2->second = new_message;
				}
			}
		}

		HashMap<TNode_*, vector<PathMessage_> > array_B;

		// handle inverse-edge collisions
		for (HashMap<EdgeItem<Index, Index>*, HashMap<TNode_*, vector<PathMessage_> > >::Iterator it1 = array_A.begin(); it1 != array_A.end(); ++it1)
		{
			for (HashMap<TNode_*, vector<PathMessage_> >::Iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
			{
				HashMap<TNode_*, vector<PathMessage_> >::Iterator it3 = it2;
				for(++it3; it3 != it1->second.end(); ++it3)
				{
					BitVector beer = it2->second[0].beep | it3->second[0].beep;
					
					do_not_forward.push_back(it3->second[0].beep);
					do_not_forward.push_back(it2->second[0].beep);
					forwarded_rings_.push_back(beer);
				}
				array_B[it2->first].push_back(it2->second[0]);
			}
		}

		// handle collisions
		for (HashMap<TNode_*, vector<PathMessage_> >::Iterator it1 = array_B.begin(); it1 != array_B.end(); ++it1)
		{
			for (vector<PathMessage_>::iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
			{
				for (vector<PathMessage_>::iterator it3 = it2 + 1; it3 != it1->second.end(); ++it3)
				{
					BitVector beer = it2->beep | it3->beep;
					do_not_forward.push_back(it2->beep);
					do_not_forward.push_back(it3->beep);
					forwarded_rings_.push_back(beer);
				}
			}
		}

		// store all non-collided path messages in the send buffer
		for (vector<PathMessage_>::iterator it1 = recieve_buffer.begin(); it1 != recieve_buffer.end(); ++it1)
		{
			bool has(false);
			for (vector<BitVector>::iterator it2 = do_not_forward.begin(); it2 != do_not_forward.end(); ++it2)
			{
				if (it1->beep == *it2)
				{
					has = true;
					break;
				}
			}
			if (!has)
			{
				send_buffer.push_back(*it1);
			}
		}

		// delete all messages in the recieve buffer
		recieve_buffer.clear();
	}

	void RingPerceptionProcessor::TNode_::send()
	{
#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
		cerr << "size of send buffer is " << send_buffer.size() << endl;
#endif
		for (Size i = 0; i != send_buffer.size(); ++i)
		{
			PathMessage_ pm = send_buffer[i];
			NodeItem<Index, Index>* a = tnode_to_atom_[this];
			for (NodeItem<Index, Index>::Iterator bit = a->begin(); bit != a->end(); ++bit)
			{
				TNode_* node = 0;
				// determine which node
				if (&(*bit)->getSource() == a)
				{
					node = atom_to_tnode_[&(*bit)->getTarget()];
				}
				else
				{
					node = atom_to_tnode_[&(*bit)->getSource()];
				}
				if (node != pm.nlast)
				{
					// build new message
					PathMessage_ new_pm;
					new_pm.nfirst = pm.nfirst;
					new_pm.efirst = pm.efirst;
					new_pm.beep = pm.beep;
					new_pm.push(*bit, this);
					// append it to the recieve buffer of the target node
					node->recieve_buffer.push_back(new_pm);
				}
			}
		}
		
		// delete the messages in the send buffer
		send_buffer.clear();
	}

	void RingPerceptionProcessor::PathMessage_::push(EdgeItem<Index, Index>* bond, TNode_* node)
	{
		// set the bit, and the node the message arives from
		beep.setBit(bond_to_index_[bond]);
		nlast = node;
	}

	void RingPerceptionProcessor::BalducciPearlmanRingSelector_(BitVector beer)
	{
#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
		cerr << "new beer: " << beer << " (" << beer.countValue(true) << ")" << endl;
		cerr << "matrix before: " << matrix_.size() << endl;
		for (vector<BitVector>::const_iterator it = matrix_.begin(); it != matrix_.end(); ++it)
		{
			cerr << *it << " (" << it->countValue(true) << ")" << endl;
		}
#endif
		// linear independency tests
		if (rings_.empty())
		{
			rings_.push_back(beer);
			matrix_.push_back(beer);
			return;
		}

		// 1. perform gaussian elimination
		//tmp_matrix.push_back(beer);
		BitVector new_beer = beer;
		Size hi_bit(0);
		Size r_begin(0);
		for (Size i = 0; i != new_beer.getSize(); ++i)
		{
			if (new_beer[i])
			{
				for (Size r = r_begin; r < matrix_.size(); ++r)
				{
					for (Size c = 0; c != matrix_[r].getSize(); ++c)
					{
						if (matrix_[r][c])
						{
							hi_bit = c;
							break;
						}
					}
					if (i == hi_bit)
					{
						r_begin = r + 1;
						new_beer ^= matrix_[r];
						break;
					}
				}
			}
		}

		if (new_beer.countValue(true) == 0)
		{
			return;
		}
		
		// if linearly independent add to the matrix
		// sort matrix w.r.t highest bit (= maintain echelon format)
		Size beer_index = 0;
		while(!new_beer[beer_index]) {
			++beer_index;
		}

		Size cur_col = 0;
		std::vector<BitVector>::iterator it = matrix_.begin();
		for(; it != matrix_.end(); ++it)
		{
			while(!(*it)[cur_col]) {
				++cur_col;
			}

			if(cur_col > beer_index) {
				break;
			}
		}

		matrix_.insert(it, new_beer);

#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG	
		cerr << "sorted matrix: " << matrix_.size() << endl;
		for (vector<BitVector>::const_iterator it = matrix_.begin(); it != matrix_.end(); ++it)
		{
			cerr << *it << " (" << it->countValue(true) << ")" << endl;
		}
#endif
	
/*
 		// this code fragment is not necessary because the gauss from is
		// garanteed from the above elimination (if successful)
		// 2. maintain gauss form
		hi_bit = 0;
		// for each column
		for (Size c = 0; c != new_matrix[0].getSize(); ++c)
		{
			// for each row
			for (Size r = c + 1; r < new_matrix.size(); ++r)
			{
				// find highest bit
				for (Size j = 0; j != new_matrix[r].getSize(); ++j)
				{
					if (new_matrix[r][j])
					{
						hi_bit = j;
						break;
					}
				}
				if (hi_bit == c)
				{
					new_matrix[r] = new_matrix[c] ^ new_matrix[r];
				}
			}			
		}
*/

		rings_.push_back(beer);
	}

	Size RingPerceptionProcessor::BalducciPearlmanAlgorithm_(vector<vector<Atom*> >& sssr, SimpleMolecularGraph& graph)
	{
		Size num_atoms = graph.getNumberOfNodes();
		Size num_bonds = graph.getNumberOfEdges();
		
		// clear old data from the static variables
		bond_to_index_.clear();
		index_to_bond_.clear();
		atom_to_tnode_.clear();
		tnode_to_atom_.clear();
		rings_.clear();
		matrix_.clear();
		forwarded_rings_.clear();
		tested_beers_.clear();
		all_small_beers_.clear();
		
		// 1. init the flow-network

		// do the node to tnode mapping
		for (SimpleMolecularGraph::NodeIterator ait = graph.beginNode(); ait != graph.endNode(); ++ait)
		{
			TNode_* node = new TNode_();
			atom_to_tnode_[&*ait] = node;
			tnode_to_atom_[node] = &*ait;
		}

		// do the bond to index mapping for the bitvector
		Size bond_num(0);
		for (SimpleMolecularGraph::EdgeIterator bit = graph.beginEdge(); bit != graph.endEdge(); ++bit)
		{
			bond_to_index_[&*bit] = bond_num;
			index_to_bond_[bond_num++] = &*bit;
		}

		// fill in the messages
		for (SimpleMolecularGraph::NodeIterator ait = graph.beginNode(); ait != graph.endNode(); ++ait)
		{
			for (NodeItem<Index, Index>::Iterator bit = ait->begin(); bit != ait->end(); ++bit)
			{
				PathMessage_ pm;
				BitVector beep(num_bonds);
				beep.fill(false);
				// set the bit for the first (outgoing) edge
				beep.toggleBit(bond_to_index_[*bit]);
				pm.beep = beep;
				TNode_* tnode = 0;
				// determine which node to set
				if ((*bit)->getSource() == *ait)
				{
					tnode = atom_to_tnode_[&(*bit)->getTarget()];
				}
				else
				{
					tnode = atom_to_tnode_[&(*bit)->getSource()];
				}
				pm.nfirst = tnode;
				pm.nlast = tnode;
				pm.efirst = *bit;
				// append the message to the send_buff this node
				atom_to_tnode_[&*ait]->send_buffer.push_back(pm);
			}
		}
	
		// calculate how many rings we must find
		Size num_rings = num_bonds - num_atoms + 1;

		// the nodes are forced to talk until they get enough rings
		Size count(1);
		while (rings_.size() < num_rings)
		{
			count++;

#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
			cerr << count << ". round (" << rings_.size() << " of " << num_rings << ")" << endl;
#endif
			
			// calling all sends 
			for (SimpleMolecularGraph::NodeIterator ait = graph.beginNode(); ait != graph.endNode(); ++ait)
			{
				atom_to_tnode_[&*ait]->send();
			}
			// calling all recieves
			for (SimpleMolecularGraph::NodeIterator ait = graph.beginNode(); ait != graph.endNode(); ++ait)
			{
				atom_to_tnode_[&*ait]->recieve();
			}

			// now invoke the BalducciPearlmanRingSelector_ which selects the correct rings of size > 2 * count -2
			// first process rings of size 2 * count - 1 (odd sized rings of this phase)
			vector<BitVector> even_sized;
			for (vector<BitVector>::iterator it = forwarded_rings_.begin(); it != forwarded_rings_.end(); ++it)
			{
				if (it->countValue(true) == 2 * count - 1)
				{
					if (find(tested_beers_.begin(), tested_beers_.end(), *it) == tested_beers_.end())
					{
						tested_beers_.push_back(*it);
						BalducciPearlmanRingSelector_(*it);
						if (it->countValue(true) == 3 || it->countValue(true) == 5)
						{
							all_small_beers_.push_back(*it);
						}
					}
				}
				else
				{
					even_sized.push_back(*it);
				}
			}

			// now process the even-sized rings
			for (vector<BitVector>::const_iterator it = even_sized.begin(); it != even_sized.end(); ++it)
			{
				if (find(tested_beers_.begin(), tested_beers_.end(), *it) == tested_beers_.end())
				{
					tested_beers_.push_back(*it);
					BalducciPearlmanRingSelector_(*it);
					if (it->countValue(true) == 4 || it->countValue(true) == 6)
					{
						all_small_beers_.push_back(*it);
					}
				}
			}
			// clean up for next round
			forwarded_rings_.clear();
			
			// this is just in the case there is s.th. going wrong, to avoid an endless loop 
			if (count > BALL_QSAR_RINGPERCEPTIONPROCESSOR_MAX_RUNS)
			{
				Log.error() << "RingPerceptionProcessor: something went wrong or the largest ring ist greater than " << BALL_QSAR_RINGPERCEPTIONPROCESSOR_MAX_RUNS << "." << endl;
				break;
			}
		}

		// now set the named property InRing to true, for the ring bonds
		for (Size i = 0; i != rings_.size(); ++i)
		{
			HashSet<Atom*> in_ring;
			vector<Atom*> ring;
			for (Size j = 0; j != rings_[i].getSize(); ++j)
			{
				if (rings_[i][j])
				{
					Bond* b = index_to_bond_[j]->getBond();
					b->setProperty("InRing", true);
					Atom* a = b->getPartner(*b->getFirstAtom());
					a->setProperty("InRing", true);
					if (!in_ring.has(a))
					{
						in_ring.insert(a);
						ring.push_back(a);
					}
					a = b->getPartner(*b->getSecondAtom());
					a->setProperty("InRing", true);
					if (!in_ring.has(a))
					{
						in_ring.insert(a);
						ring.push_back(a);
					}
				}
			}
			sssr.push_back(ring);
		}

		// now handle small membered rings
		for (Size i = 0; i != all_small_beers_.size(); ++i)
		{
			HashSet<Atom*> in_ring;
			vector<Atom*> ring;
			for (Size j = 0; j != all_small_beers_[i].getSize(); ++j)
			{
				if (all_small_beers_[i][j])
				{
					Bond* b = index_to_bond_[j]->getBond();
					Atom* a = b->getPartner(*b->getFirstAtom());
					if (!in_ring.has(a))
					{
						in_ring.insert(a);
						ring.push_back(a);
					}

					a = b->getPartner(*b->getSecondAtom());
					if (!in_ring.has(a))
					{
						in_ring.insert(a);
						ring.push_back(a);
					}
				}
			}
			all_small_rings_.push_back(ring);
		}
		
		// delete TNodes
		for (HashMap<NodeItem<Index, Index>* , TNode_*>::Iterator it = atom_to_tnode_.begin(); 
				 it != atom_to_tnode_.end(); ++it)
		{
			delete it->second;
		}
		return rings_.size();
	}

} // namespace BALL
