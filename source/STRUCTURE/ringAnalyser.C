// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/ringAnalyser.h>

#include <BALL/CONCEPT/property.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/KERNEL/forEach.h>

#include <set>
#include <deque>

#define BALL_DEBUG_RINGANALYSER

#ifdef BALL_DEBUG_RINGANALYSER
# define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a)
#endif

using namespace std;

namespace BALL
{

	RingAnalyser::RingAnalyser()
	{}


	RingAnalyser::~RingAnalyser()
	{}

	// check wether the ring appears in the Ring-Template-Database
	bool RingAnalyser::checkRTD(vector<Atom*>& /* ring */)
	{
   /// ???? TODO: add ring template database
		return false;
	}

	// check, whether an atom is part of a certain ring
	bool RingAnalyser::isInRing(const Atom* atom, vector<Atom*>& ring)
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

	// clustering rings into connected ringsystems
	vector<vector<vector<Atom*> > > RingAnalyser::clusterRings(vector<vector<Atom*> >& rings)
	{
		// compute all ring memberships
		HashMap<Atom*, std::set<Index> > rings_per_atom;

		for (Position i=0; i<rings.size(); ++i)
		{
			for (Position j=0; j<rings[i].size(); ++j)
			{
				rings_per_atom[rings[i][j]].insert(i);
			}
		}

		// start at the beginning of the list of sets and compare it with all later sets
		for (HashMap<Atom*, std::set<Index> >::iterator first_set_it = rings_per_atom.begin(); 
		                                                first_set_it != rings_per_atom.end(); ++first_set_it)
		{

			HashMap<Atom*, std::set<Index> >::iterator second_set_it = first_set_it;
			++second_set_it;
			for (; second_set_it != rings_per_atom.end(); ++second_set_it)
			{
				// do the sets have a non-empty intersection?
				bool intersection_found = false;
				for(std::set<Index>::iterator member_it = first_set_it->second.begin(); 
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
					std::set<Index> new_set;

					set_union(first_set_it->second.begin(), first_set_it->second.end(), 
							      second_set_it->second.begin(), second_set_it->second.end(), 
										std::inserter(new_set, new_set.begin()));

					first_set_it->second.clear();
					second_set_it->second = new_set;

					break;
				}
			}
		}

		// create the result vector
		std::vector<std::vector<std::vector<Atom* > > > ringsystems;

		for (HashMap<Atom*, std::set<Index> >::iterator merged_set_it = rings_per_atom.begin(); 
				                                            merged_set_it != rings_per_atom.end(); ++merged_set_it)
		{
			if (merged_set_it->second.size() != 0)
			{
				ringsystems.push_back(std::vector<std::vector<Atom* > >());

				for (std::set<Index>::iterator it = merged_set_it->second.begin(); it != merged_set_it->second.end(); ++it)
				{
					ringsystems.back().push_back(rings[*it]);
				}
			}
		}

		DEBUG("\t-*-[RingAnalyser]:\t" << ringsystems.size() << " ringsystems found. ")

		return ringsystems;
	}

	// Count the number of bonds shared between rings in a given ring system
	// Precondition: the rings must be sorted such that a_i, a_i+1 share a bond (and a_size-1, a_0)
	std::vector<Size> RingAnalyser::countSharedBonds(std::vector<std::vector<Atom*> >& ring_system)
	{
		HashMap<Bond*, std::deque<Index> > ring_per_bond;

		for (Position current_ring = 0; current_ring < ring_system.size(); ++current_ring)
		{
			for (Position current_atom=0; current_atom < ring_system[current_ring].size() - 1; ++current_atom)
			{
				ring_per_bond[ring_system[current_ring][current_atom]->getBond(*(ring_system[current_ring][current_atom+1]))].push_back(current_ring);
			}

			ring_per_bond[ring_system[current_ring].back()->getBond(*(ring_system[current_ring][0]))].push_back(current_ring);
		}

		std::vector<Size> result(ring_system.size(), 0);

		for (HashMap<Bond*, std::deque<Index> >::iterator b_it=ring_per_bond.begin(); b_it!=ring_per_bond.end(); ++b_it)
		{
			if (b_it->second.size() > 1)
			{
				for (std::deque<Index>::iterator ring_it = b_it->second.begin(); ring_it != b_it->second.end(); ++b_it)
				{
					++result[*ring_it];
				}
			}
		}

		return result;
	}

	// check wether a ring is connected in FUSED or BRIDGED type to the ringsystem or neither of them
	RingAnalyser::RingType RingAnalyser::checkFusedOrBridged(vector<Atom*>& ring, vector<RingInfo>& ringset, Size num_shared_bonds, bool& first_bridged_to_fused)
	{
		// before each call of this procedure, the Property "found" must be cleared from all bonds in the current ringsystem
		for (Position i = 0; i < ringset.size(); i++)
		{
			for (Position j = 0; j < ringset[i].ring.size(); j++)
			{
				Atom::BondIterator bond_it;
				BALL_FOREACH_ATOM_BOND(*(ringset[i].ring[j]), bond_it)
				{
					bond_it->clearProperty(SDGenerator::FOUND);
				}
			}
		}

		// count the number of bonds that the current ring shares with rest of the ringsystem

		// iterating over all rings in the ringsystem
		vector<Bond*> shared_bonds_vec;

		Size shared_bonds = 0;
		for (vector<RingInfo>::size_type k = 0; k != ringset.size(); k++)
		{
			if (ringset[k].ring != ring)
			{
				//      iterating over all atoms of the current ring
				for (vector<Atom*>::size_type j = 0; j != ringset[k].ring.size(); j++)
				{
					//      iterating over all bonds of the current atom
					for (Atom::BondIterator bond_it2 = ringset[k].ring[j] -> beginBond(); bond_it2 != ringset[k].ring[j] -> endBond(); bond_it2++)
					{
						//      if the current bond is part of the ring
						RingAnalyser ra;
						if (ra.isInRing(bond_it2 -> getFirstAtom(), ringset[k].ring) && ra.isInRing(bond_it2 -> getSecondAtom(), ringset[k].ring))
						{
							//      iterating over all atoms in the ring, that is to be checked
							for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
							{
								//      iterating over all bonds of each atom in the ring that is to be checked
								for (Atom::BondIterator bond_it = ring[i] -> beginBond(); bond_it != ring[i] -> endBond(); bond_it++)
								{
									//      if this bond is part of the ring that is to be checked, too
									if (ra.isInRing(bond_it -> getFirstAtom(), ring) && ra.isInRing(bond_it -> getSecondAtom(), ring) )
									{
										//      if the two bonds are the same and neither of them has been found before
										if ((*bond_it == *bond_it2)  && (!(bond_it -> hasProperty(SDGenerator::FOUND))) && (!(bond_it2 -> hasProperty(SDGenerator::FOUND))))
										{
											//      declare them as found
											bond_it -> setProperty(SDGenerator::FOUND);
											bond_it2 -> setProperty(SDGenerator::FOUND);

											//      increment the number of shared bonds
											shared_bonds += 1;
											shared_bonds_vec.push_back(&*bond_it);
										}
									}
								}
							}
						}
					}
				}
			}
		}

		//      if more than one shared bond is found, check, whether the bonds are neighbours
		//      if they are, its a bridged ring, otherwise the ring is fused to more than one other ring
		bool neighboring_bonds = false;
		if (shared_bonds_vec.size() > 1)
		{
			for (Size i = 0; i != shared_bonds_vec.size(); i++)
			{
				for (Size j = 0; j != shared_bonds_vec.size(); j++)
				{
					if (i != j)
					{
						if ((shared_bonds_vec[i] -> getFirstAtom() == shared_bonds_vec[j] -> getFirstAtom()) ||
								shared_bonds_vec[i] -> getFirstAtom() == shared_bonds_vec[j] -> getSecondAtom() ||
								shared_bonds_vec[i] -> getSecondAtom() == shared_bonds_vec[j] -> getFirstAtom() ||
								shared_bonds_vec[i] -> getSecondAtom() == shared_bonds_vec[j] -> getSecondAtom()   )
						{
							neighboring_bonds = true;
						}
					}
				}
			}
		}

		if (shared_bonds > 1)
		{
			if (ringset.size() > 2 && !first_bridged_to_fused)
			{
				first_bridged_to_fused = true;
				return FUSED;               //      for construction reasons, the first bridged-ring must be assigned FUSED
			}
			else if (!neighboring_bonds)
			{
				return FUSED;       // if the ring shares more than 1 non-neighboring bond, it is assigned FUSED
			}
			else
			{
				return BRIDGED;               // if the ring shares more than 1 neighboring bond, it is assigned BRIDGED
			}
		}

		if (shared_bonds == 1)
		{
			return FUSED;       // if the ring shares exactly 1 bond with the ringsystem, it is assigned FUSED
		}
		return NONE;       //      otherwise it is assigned neither BRIDGED nore FUSED
	}


	//      check wether the ring is connected to ring ringsystem in SPIRO way
	RingAnalyser::RingType RingAnalyser::checkSpiro(vector<Atom*>& ring, vector<RingInfo>& ringset)
	{
		//      count the number of atoms the ring shares with the rest of the ringsystem

		//      iterate over every atom in the ring
		for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
		{
			//      iterate over every ring in the ringsystem
			for (vector<RingInfo>::size_type j = 0; j != ringset.size(); j++)
			{
				Size shared_atoms = 0;
				if (ring != ringset[j].ring)    //      don't compare the ring with itself
				{
					//      iterate over every atom
					for (vector<Atom*>::size_type k = 0; k != ringset[j].ring.size(); k++)
					{
						//      if an atom appears in the ring that is to be checked and in another ring of the ringsystem
						if (ring[i] == ringset[j].ring[k])
						{
							shared_atoms++; //      increment the number o shared atoms
						}
					}
				}
				if (shared_atoms == 1)
				{
					return SPIRO;       //      if the ring shares exactly      1 atom with the rest of the ringsystem, it is assigned SPIRO
				}
			}
		}
		return NONE;
	}

	std::vector<RingAnalyser::RingInfo> RingAnalyser::analyseRings(std::vector<std::vector<Atom*> >& ringsystem)
	{
		// [Procedure A]: Assignment Engine
		std::vector<RingInfo> ringset(ringsystem.size());
		bool first_bridged_to_fused = 0;

		// build a vector that contains all rings of the current ringsystem and an assignment that 
		// indicates the way in which the ring is conected to the rest of the system
		for (Position i = 0; i < ringsystem.size(); i++)
		{
			RingInfo record;
			record.ring = ringsystem[i];
			record.assignment = NONE;    // "0" means that that the ring has not been given any assignment yet
			ringset[i] = record;
		}

		std::vector<Size> num_shared_bonds = countSharedBonds(ringsystem);

		Size unassigned_rings = ringset.size();

		for (Position i = 0; i < ringset.size(); i++)
		{
			// 1. If only one ring is left, assign it CORE. Then exit.
			if (unassigned_rings == 1)
			{
				ringset[i].assignment = CORE; //  ... If only one ring is left, assign it CORE. Then exit
				break;
			}
			else
			{
				// 2. If the remainig rings constitute a RS (RingSystem) present in the RTD (RingTemplateDatabase),
				//    get their coordinates, assign the ring type TEMPLATE and exit
				if (checkRTD(ringset[i].ring))
				{
					ringset[i].assignment = TEMPLATE; // "1" means "Template"
					--unassigned_rings;
				}
				else // 3. If there is a fused, bridged, or spiro ring as determined by checkFusedOrBridged or checkSpiro:
				{
					// a) Assign it type FUSED, BRIDGED, or SPIRO and remove it from further consideration
					if (ringset[i].assignment == NONE)
					{
						ringset[i].assignment = checkFusedOrBridged(ringset[i].ring, ringset, num_shared_bonds[i], first_bridged_to_fused); // "2" means FUSED and "3" means BRIDGED
						if (ringset[i].assignment == NONE)
						{
							ringset[i].assignment = checkSpiro(ringset[i].ring, ringset); // "4" means SPIRO
						}
					}

					if (ringset[i].assignment != NONE)
						--unassigned_rings;
				}

				//  b) If using an RTD or only one ring remains, go to step 1 of this procedure
				//  c) Go to beginning of step 3 of this procedure

			}
		}
		return ringset;
	}

} // namespace BALL
