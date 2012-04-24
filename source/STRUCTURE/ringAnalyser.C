// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/ringAnalyser.h>

#include <BALL/COMMON/exception.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

//#define BALL_DEBUG_RINGANALYSER

#ifdef BALL_DEBUG_RINGANALYSER
# define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a)
#endif

using namespace std;

namespace BALL
{

	RingAnalyser::Ring::Ring()
		: PropertyManager(),
		  atoms(),
			type(NONE),
			shared_bonds()	
	{
	}

	RingAnalyser::Ring::Ring(std::vector<Atom*> const& atoms_)
		: PropertyManager(),
		  atoms(atoms_),
		  type(NONE),
			shared_bonds()
	{
	}

	Position RingAnalyser::Ring::predecessor(Position i) const
	{
		if (i >= atoms.size())
			throw (Exception::IndexOverflow(__FILE__, __LINE__, i, atoms.size()));

		return ((i+atoms.size()-1) % atoms.size());
	}

	Position RingAnalyser::Ring::successor(Position i) const
	{
		if (i >= atoms.size())
			throw (Exception::IndexOverflow(__FILE__, __LINE__, i, atoms.size()));

		return ((i+1) % atoms.size());
	}

	RingAnalyser::RingAnalyser()
		: rings_(),
			ring_systems_()
	{}


	RingAnalyser::~RingAnalyser()
	{}

	void RingAnalyser::clear()
	{
		rings_.clear();
		ring_systems_.clear();
		peeling_order_.clear();
		rings_per_bond_.clear();
	}

	bool RingAnalyser::start()
	{
		clear();

		return true;
	}

	Processor::Result RingAnalyser::operator () (AtomContainer& ac)
	{
		// compute the smallest set of smallest rings
		RingPerceptionProcessor rpp;
		std::vector<std::vector<Atom*> > sssr;
		rpp.calculateSSSR(sssr, ac);

		rings_.resize(sssr.size());
		// and convert it into our own data structure
		for (Position i=0; i<sssr.size(); ++i)
		{
			rings_[i].atoms = sssr[i];
		}

		// sequence the rings in the sssr to prepare them for analysis and construction
		for (Position i=0; i<rings_.size(); ++i)
			sequenceRing(rings_[i].atoms);

		// cluster the rings into ring systems joined by at least one atom
		clusterRings_();

		// find bonds shared between rings
		findSharedBonds_();

		// compute the ring types
		peelRings_();

		return Processor::BREAK;
	}

	bool RingAnalyser::finish()
	{
		return true;
	}

	Size RingAnalyser::getNumberOfRingSystems()
	{
		return ring_systems_.size();
	}

	std::vector<RingAnalyser::Ring> RingAnalyser::getRingSystem(Position i)
	{
		if (i >= ring_systems_.size())
		{
			throw (Exception::IndexOverflow(__FILE__, __LINE__, i, ring_systems_.size()));
		}

		std::vector<Position> ring_system = ring_systems_[i];

		std::vector<Ring> result;
		for (Position j=0; j<ring_system.size(); ++j)
			result.push_back(rings_[ring_system[j]]);

		return result;
	}

	std::vector<Position> RingAnalyser::getPeelingOrder(Position i)
	{
		if (i >= peeling_order_.size())
		{
			throw (Exception::IndexOverflow(__FILE__, __LINE__, i, peeling_order_.size()));
		}

		return peeling_order_[i];
	}

	// check wether the ring appears in the Ring-Template-Database
	bool RingAnalyser::assignRTD_(std::list<Position>& /* unassigned_rings */)
	{
   /// ???? TODO: add ring template database
		return false;
	}

	// check, whether an atom is part of a certain ring
	bool RingAnalyser::isInRing(const Atom* atom, vector<Atom*> const& ring) const
	{
		for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
		{
			if (ring[i] == atom)
			{
				return true;
			}
		}

		return false;
	}

	bool RingAnalyser::isInRingSystem(const Atom* atom, Index i) const
	{
		std::vector<Position> const& ring_system = ring_systems_[i];

		for (Position j=0; j<ring_system.size(); ++j)
		{
			if (isInRing(atom, rings_[ring_system[j]].atoms))
				return true;
		}

		return false;
	}

	void RingAnalyser::sequenceRing(std::vector<Atom*>& ring)
	{
		// the atom we start with stays in its place
		Atom* start_atom = ring[0];

		// put all other atoms into a list
		std::list<Atom*> ring_atoms;
		for (Position k = 1; k < ring.size(); k++)
		{
			ring_atoms.push_back(ring[k]);
		}

		Size  last_index = 0;
		Atom* last_atom = start_atom;
		while(!ring_atoms.empty())
		{
			for (std::list<Atom*>::iterator ring_it  = ring_atoms.begin(); 
			                                ring_it != ring_atoms.end(); ++ring_it)
			{
				if (last_atom->isBoundTo(**ring_it))
				{
					ring[++last_index] = *ring_it;
					last_atom = *ring_it;
					ring_atoms.erase(ring_it);
					break;
				}
			}
		}

		DEBUG("\t-*-[sequenceRings]:\tRing has been sequenced." << endl)
	}

	// clustering rings into connected ringsystems
	void RingAnalyser::clusterRings_()
	{
		// compute all ring memberships
		HashMap<Atom*, std::set<Position> > rings_per_atom;

		for (Position i=0; i<rings_.size(); ++i)
		{
			std::vector<Atom*>& current_ring = rings_[i].atoms;

			for (Position j=0; j<current_ring.size(); ++j)
			{
				rings_per_atom[current_ring[j]].insert(i);
			}
		}

		// start at the beginning of the list of sets and compare it with all later sets
		for (HashMap<Atom*, std::set<Position> >::iterator first_set_it  = rings_per_atom.begin(); 
		                                                   first_set_it != rings_per_atom.end(); ++first_set_it)
		{
			HashMap<Atom*, std::set<Position> >::iterator second_set_it = first_set_it;
			++second_set_it;
			for (; second_set_it != rings_per_atom.end(); ++second_set_it)
			{
				// do the sets have a non-empty intersection?
				bool intersection_found = false;
				for(std::set<Position>::iterator member_it  = first_set_it->second.begin(); 
				                                 member_it != first_set_it->second.end(); ++member_it)
				{
					if (find(second_set_it->second.begin(), second_set_it->second.end(), *member_it) != second_set_it->second.end())
					{
						intersection_found = true;
						break;
					}
				}

				if (intersection_found)
				{
					std::set<Position> new_set;

					set_union(first_set_it->second.begin(), first_set_it->second.end(), 
							      second_set_it->second.begin(), second_set_it->second.end(), 
										std::inserter(new_set, new_set.begin()));

					first_set_it->second.clear();
					second_set_it->second = new_set;

					break;
				}
			}
		}

		for (HashMap<Atom*, std::set<Position> >::iterator merged_set_it  = rings_per_atom.begin(); 
				                                               merged_set_it != rings_per_atom.end(); ++merged_set_it)
		{
			if (merged_set_it->second.size() != 0)
			{
				ring_systems_.push_back(std::vector<Position>());

				for (std::set<Position>::iterator it  = merged_set_it->second.begin(); 
				                               it != merged_set_it->second.end(); ++it)
				{
					ring_systems_.back().push_back(*it);
				}
			}
		}

		DEBUG("\t-*-[RingAnalyser]:\t" << ring_systems_.size() << " ringsystems found. ")
	}

	// Find the bonds shared between rings in the sssr
	// Precondition: the rings must be sorted such that a_i, a_i+1 share a bond (and a_size-1, a_0)
	void RingAnalyser::findSharedBonds_()
	{
		rings_per_bond_.clear();

		for (Position current_system_index = 0; 
		              current_system_index < ring_systems_.size(); ++current_system_index)
		{
			std::vector<Position>& current_system = ring_systems_[current_system_index];


			for (Position current_ring_index = 0; current_ring_index < current_system.size(); ++current_ring_index)
			{
				std::vector<Atom*>& current_ring = rings_[current_system[current_ring_index]].atoms;

				for (Position current_atom=0; current_atom < current_ring.size() - 1; ++current_atom)
				{
					Atom const*  first_atom = current_ring[current_atom  ];
					Atom const* second_atom = current_ring[current_atom+1];

					Bond const* bond = first_atom->getBond(*second_atom);

					if (!rings_per_bond_.has(bond))
					{
						rings_per_bond_[bond] = std::deque<Position>();
					}

					rings_per_bond_[bond].push_back(current_system[current_ring_index]);
				}

				Atom const*  first_atom = current_ring.back();
				Atom const* second_atom = current_ring[0];

				Bond const* bond = first_atom->getBond(*second_atom);

				if (!rings_per_bond_.has(bond))
				{
					rings_per_bond_[bond] = std::deque<Position>();
				}

				rings_per_bond_[bond].push_back(current_system[current_ring_index]);
			}

			for (HashMap<Bond const*, std::deque<Position> >::iterator b_it  = rings_per_bond_.begin(); 
					b_it != rings_per_bond_.end(); 
					++b_it)
			{
				if (b_it->second.size() > 1)
				{
					for (std::deque<Position>::iterator ring_it  = b_it->second.begin(); 
							                                ring_it != b_it->second.end(); ++ring_it)
					{
						rings_[*ring_it].shared_bonds.insert(b_it->first);
					}
				}
			}
		}
	}

	bool RingAnalyser::isCentralRing_(std::list<Position>::iterator ring, std::list<Position>& unassigned_rings)
	{
		// find an arbitrary other ring
		std::list<Position>::iterator next_ring = unassigned_rings.begin();

		while (next_ring == ring) ++next_ring;

		if (next_ring == unassigned_rings.end())
		{
			// this should never happen!
			Log.warn() << "isCentralRing_ failed!" << std::endl;
			return false;
		}

		// look for neighbours
		HashMap<Atom*, std::list<Position> > atom_to_rings;
		for (std::list<Position>::iterator r_it = unassigned_rings.begin(); r_it != unassigned_rings.end(); ++r_it)
		{
			if (*r_it != *ring)
			{
				Ring& current_ring = rings_[*r_it];

				for (Position i=0; i<current_ring.atoms.size(); ++i)
				{
					Atom* atom = current_ring.atoms[i];

					if (!atom_to_rings.has(atom))
						atom_to_rings[atom] = std::list<Position>();

					atom_to_rings[atom].push_back(*r_it);
				}
			}
		}

		std::set<Position> component;
		std::set<Position> to_test;
		HashSet<Position>  done;

		to_test.insert(*next_ring);
		while (!to_test.empty())
		{
			Position next = *(to_test.begin());
			to_test.erase(to_test.begin());

			done.insert(next);
			component.insert(next);

			Ring& next_ring = rings_[next];
			for (Position i=0; i<next_ring.atoms.size(); ++i)
			{
				std::list<Position>& neighbours = atom_to_rings[next_ring.atoms[i]];

				for (std::list<Position>::iterator n_it = neighbours.begin(); n_it != neighbours.end(); ++n_it)
				{
					if (*n_it != next && !done.has(*n_it))
						to_test.insert(*n_it);
				}
			}
		}

		return (component.size() < unassigned_rings.size() - 1);
	}

	bool RingAnalyser::peelNextRing_(std::list<Position>& unassigned_rings, bool peel_bridged)
	{
		// if there is only one ring left, it is of type CORE and we are done...
		if (unassigned_rings.size() == 1)
		{
			rings_[*(unassigned_rings.begin())].type = CORE;
			peeling_order_.back().push_back(*(unassigned_rings.begin()));
			unassigned_rings.clear();

			return true;
		}

		// otherwise, check whether the remaining rings can be found in the template database
		if (assignRTD_(unassigned_rings))
			return true;

		// ok, we have to look a little deeper...
		Size min_bond_num = 1000;
		std::list<Position>::iterator min_ring_it = unassigned_rings.end();

		Size min_partner_num = 1000;
		std::list<Position>::iterator min_bridge_it = unassigned_rings.end();

		// examine every unassigned ring
		for (std::list<Position>::iterator current_ring_it  = unassigned_rings.begin();
		                                   current_ring_it != unassigned_rings.end(); ++current_ring_it)
		{
			Position ring_index = *current_ring_it;

			std::set<Bond const*> shared_bonds = rings_[ring_index].shared_bonds;
			if (shared_bonds.empty())
			{
				// since we know our ring system has more than one ring (otherwise, we would
				// already have assigned CORE), this has to be a SPIRO ring.
				// 
				// we can obviously not hope to find a ring with less bonds to the ring system,
				// so we can happily peel it away
				rings_[ring_index].type = SPIRO;
				peeling_order_.back().push_back(ring_index);
				unassigned_rings.erase(current_ring_it);
				return true;
			}

			// find bonds shared with unassigned neighbours
			std::set<Bond const*> shared_unassigned_bonds;
			Size num_unassigned_neighbours = 0;

			for (std::set<Bond const*>::iterator b_it = shared_bonds.begin(); b_it != shared_bonds.end(); ++b_it)
			{
				std::deque<Position>& neighbours = rings_per_bond_[*b_it];

				bool still_shared = false;
				for (std::deque<Position>::iterator n_it = neighbours.begin(); n_it != neighbours.end(); ++n_it)
				{
					if (rings_[*n_it].type == NONE)
					{
						still_shared = true;
						++num_unassigned_neighbours;
					}
				}

				if (still_shared)
					shared_unassigned_bonds.insert(*b_it);
			}

			// does r bridge other unassigned rings?

			// if more than one shared bond is found, check, whether the bonds are neighbours
			// if they are, its a bridged ring, otherwise the ring is fused to more than one other ring
			bool neighboring_bonds = false;
			if (shared_unassigned_bonds.size() > 1) // a single bond can't bridge...
			{
				std::set<Bond const*>::iterator first_bond = shared_unassigned_bonds.begin(); 
				for (; first_bond != shared_unassigned_bonds.end() && !neighboring_bonds; ++first_bond)
				{
					std::set<Bond const*>::iterator second_bond = first_bond;
					++second_bond;

					for (; second_bond != shared_unassigned_bonds.end(); ++second_bond)
					{
						Atom const* a_11 = (*first_bond)->getFirstAtom();
						Atom const* a_12 = (*first_bond)->getSecondAtom();

						Atom const* a_21 = (*second_bond)->getFirstAtom();
						Atom const* a_22 = (*second_bond)->getSecondAtom();

						if ( (a_11 == a_21) || (a_11 == a_22) || (a_12 == a_21) || (a_12 == a_22) )
						{
							neighboring_bonds = true;
							break;
						}
					}
				}
			}
				
			if (isCentralRing_(current_ring_it, unassigned_rings)) 
				continue; // ignore central rings 
				
			if (neighboring_bonds) {

				// if we allow peeling of bridged rings, we may need this one later
				if (num_unassigned_neighbours < min_partner_num)
				{
					min_partner_num = num_unassigned_neighbours;
					min_bridge_it = current_ring_it;
				}

				continue;
			}

			// if we arrived here, let's see if this ring is a better candidate than all previous ones
			if (shared_unassigned_bonds.size() < min_bond_num)
			{
				min_bond_num = shared_unassigned_bonds.size();
				min_ring_it = current_ring_it;
			}
		}

		// did we find a suitable candidate?
		if (min_ring_it != unassigned_rings.end())
		{
			// peel it away
			rings_[*min_ring_it].type = FUSED;
			peeling_order_.back().push_back(*min_ring_it);
			unassigned_rings.erase(min_ring_it);

			return true;
		}

		// or a bridged one?
		if (peel_bridged && min_bridge_it != unassigned_rings.end())
		{
			// peel it away
			rings_[*min_bridge_it].type = BRIDGED;
			peeling_order_.back().push_back(*min_bridge_it);
			unassigned_rings.erase(min_bridge_it);

			return true;
		}

		// nope? what a pity.
		return false;
	}

	void RingAnalyser::peelRings_()
	{
		// iterate over all ring systems
		for (Position current_system_index = 0; current_system_index < ring_systems_.size(); ++current_system_index)
		{
			peeling_order_.push_back(std::vector<Position>());

			std::vector<Position>& current_system = ring_systems_[current_system_index];

			// Initialize all rings
			for (Position current_ring_index = 0; current_ring_index < current_system.size(); ++current_ring_index)
			{
				Ring& current_ring = rings_[current_system[current_ring_index]];
				current_ring.type = NONE;
			}

			// Now try the peeling
			std::list<Position> unassigned_rings;
			std::copy(current_system.begin(), current_system.end(), std::back_inserter(unassigned_rings));

			// we run the assignment twice, once without the license to ki... ummh peel bridges,
			// and once with
			for (Position current_run = 0; current_run < 2; ++current_run)
			{
				while (!unassigned_rings.empty())
				{
					// try to find a peelable fused ring
					while (peelNextRing_(unassigned_rings, current_run == 1))
						continue; // if it worked, try the next  

					// if we did not find another ring to peel, we have to exit
					break;
				}
			}

			// no ring should be left behind...
			if (!unassigned_rings.empty())
			{
				Log.warn() << "Warning: RingAnalyser found an incomplete ring type assignment!" << std::endl;
			}

			// when we arrive here, the peeling order for this ring system contains global indices (into the rings_ vector)
			// but our clients will want indices into the current ring system => convert them now
			
			HashMap<Position, Position> global_to_local;
			for (Position i = 0; i < current_system.size(); ++i)
			{
				global_to_local[current_system[i]] = i;
			}

			std::vector<Position>& current_peeling_order = peeling_order_.back();
			for (Position i=0; i<current_peeling_order.size(); ++i)
			{
				current_peeling_order[i] = global_to_local[current_peeling_order[i]];
			}
		}
	}

} // namespace BALL
