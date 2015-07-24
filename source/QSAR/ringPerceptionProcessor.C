// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringPerceptionProcessor.C,v 1.22.4.2 2007/04/03 13:29:39 bertsch Exp $
//

#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>

#include <limits>

#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
#include <iostream>
#endif

#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
#undef  BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG

#define BALL_QSAR_RINGPERCEPTIONPROCESSOR_MAX_RUNS 1000

using namespace std;

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
	// delete TNodes if still existing (e.g.: when an exception was thrown)
	for (HashMap<NodeItem<Index, Index>* , TNode_*>::Iterator it = atom_to_tnode_.begin();
			 it != atom_to_tnode_.end(); ++it)
	{
		delete it->second;
	}
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
	// do not start the algorithm, if no rings do exist in the given molecule:
	if( ((long)ac.countBonds() - (long)ac.countAtoms() + 1) < 1)
		return 0;

	all_small_rings_.clear();

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

	Size num_rings(0);

	num_rings = BalducciPearlmanAlgorithm_(sssr_orig, mol_graph);

	return num_rings;
}

const vector<vector<Atom*> >& RingPerceptionProcessor::getAllSmallRings() const
{
	return all_small_rings_;
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
				// make sure the pathes have only one shared edge:
				if( haveSingleIntersection(it2->second[0].beep, it3->second[0].beep) )
				{
					BitVector beer = it2->second[0].beep | it3->second[0].beep;
					forwarded_rings_.push_back(beer);

					do_not_forward.push_back(it3->second[0].beep);
					do_not_forward.push_back(it2->second[0].beep);
				}
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
				// make sure that the pathes do not contain identical edges:
				if(haveZeroIntersection(it2->beep, it3->beep))
				{
					BitVector beer = it2->beep | it3->beep;
					forwarded_rings_.push_back(beer);

#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
					cout<<"found normal collision:"<<endl;
					cout<<"BEEP1: "<< RingPerceptionProcessor::SMILESfromBit(it2->beep)<<endl;
					cout<<it2->beep<< " ("<<it2->beep.countValue(true)<<")"<<endl;
					cout<<"BEEP2: "<< RingPerceptionProcessor::SMILESfromBit(it3->beep)<<endl;
					cout<<it3->beep<< " ("<<it3->beep.countValue(true)<<")"<<endl;
					cout<<"RESULT: "<< RingPerceptionProcessor::SMILESfromBit(beer)<<endl;
					cout<<beer<< " ("<<beer.countValue(true)<<")"<<endl;
#endif

					do_not_forward.push_back(it2->beep);
					do_not_forward.push_back(it3->beep);
				}
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

// Return true if 'node' is not yet contained within the current edges of 'beep'
bool RingPerceptionProcessor::TNode_::nodeIsNew(BitVector& beep, NodeItem<Index, Index>* node)
{
	for(unsigned short i = 0; i < beep.getSize(); ++i)
	{
		if ( beep.getBit(i) )
		{
			EdgeItem<Index, Index>* bnd = index_to_bond_[i];
			if(&bnd->getSource() == node || &bnd->getTarget() == node)
				return false;
		}
	}
	return true;
}

// return true if 'beep1' and 'beep2' have only different edges (no edge is
// contained in both pathes)
bool RingPerceptionProcessor::TNode_::haveZeroIntersection(BitVector& beep1, BitVector& beep2)
{
	for(unsigned int i = 0; i < beep1.getSize(); i++)
	{
		if( beep1.getBit(i) && beep2.getBit(i) )
			return false;
	}
	return true;
}

// return true if only one identical edge is found in both beeps
bool RingPerceptionProcessor::TNode_::haveSingleIntersection(BitVector& beep1, BitVector& beep2)
{
	bool is_found = false;
	for(unsigned int i = 0; i < beep1.getSize(); i++)
	{
		if( beep1.getBit(i) && beep2.getBit(i) )
		{
			if( is_found )
				return false;
			else
				is_found = true;
		}
	}
	return is_found;
}

void RingPerceptionProcessor::TNode_::send()
{
#ifdef BALL_QSAR_RINGPERCEPTIONPROCESSOR_DEBUG
	cerr << "size of send buffer is " << send_buffer.size() << endl;
#endif
	for (Size i = 0; i != send_buffer.size(); ++i)
	{
		PathMessage_& pm = send_buffer[i];
		NodeItem<Index, Index>* a = tnode_to_atom_[this];
		for (NodeItem<Index, Index>::Iterator bit = a->begin(); bit != a->end(); ++bit)
		{
			TNode_* node = 0;
			// determine which node
			if ( (*bit)->getSource() == *a)
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

				if( !new_pm.beep[ bond_to_index_[*bit] ] && nodeIsNew(new_pm.beep, tnode_to_atom_[node]) ) // do not send if this edge was already set
				{
					new_pm.push(*bit, this);
					// append it to the recieve buffer of the target node
					node->recieve_buffer.push_back(new_pm);
				}
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
	cerr <<endl<<RingPerceptionProcessor::SMILESfromBit(beer)<<endl;
	cerr << "new beer: " <<endl<< beer << " (" << beer.countValue(true) << ")" << endl;
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

	// theoretically we should use 'new_beer.countValue(true) == 0', but 2
	// remaining 'new' edges are not enough to form a new ring, thus a BEER
	// having 2 new ones should also not occur or be allowed:
	if (new_beer.countValue(true) < 3)
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
	all_small_rings_.clear();

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
	while (rings_.size() != num_rings)
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
			throw Exception::RingProcessorException("ringPerceptionProcessor.C", 551);
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
	atom_to_tnode_.clear();

	return rings_.size();
}

} // namespace BALL
