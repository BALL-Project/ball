// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/moleculeAssembler.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/KERNEL/forEach.h>

//#define BALL_DEBUG_SDGENERATOR

#ifdef BALL_DEBUG_SDGENERATOR
# define DEBUG(a) Log.info() << a << endl;
#else
# define DEBUG(a) 
#endif

namespace BALL
{
	const char* SDGenerator::Option::SHOW_HYDROGENS       = "sd_generator_show_hydrogens";
	const char* SDGenerator::Option::STANDARD_BOND_LENGTH = "sd_generator_standard_bond_length";

	const bool  SDGenerator::Default::SHOW_HYDROGENS       = true;
	const float SDGenerator::Default::STANDARD_BOND_LENGTH = 2.0f;
 
	SDGenerator::SDGenerator(bool show_hydrogens)
	{
		setDefaultOptions();
		options[SDGenerator::Option::SHOW_HYDROGENS] = show_hydrogens;
	}

	SDGenerator::~SDGenerator()
	{
	}

	void SDGenerator::prepare_(System& molecule_sys)
	{
		// check if all hydrogen atoms should be deleted from the System
		bool show_H = options.getBool(Option::SHOW_HYDROGENS);

		if (!show_H)
		{
			Selector s("element(H)");
			molecule_sys.apply(s);

			DEBUG(s.getNumberOfSelectedAtoms() << " atoms selected")

			molecule_sys.removeSelected();
			DEBUG("all H's removed")
		}

		// compute the smallest set of smallest rings and analyze them
		// NOTE: the RingAnalyser sets the "InRing" property for us
		molecule_sys.apply(ring_analyser_);

		AtomIterator at_it;

		// distinguish between ring-atoms and core-chain-atoms
		BALL_FOREACH_ATOM(molecule_sys, at_it)
		{
			//  declare atoms as core-chain if they fulfil the following conditions:
			//   - acylic
			//   - have at least two neighbours, at least one of which is acyclic
			//   - do not have an adjacent triple bond, or two adjacent double-bonds
			if (!at_it->getProperty("InRing").getBool())
			{
				Size acyclic_neighbours = 0;
				Size num_triple_bonds = 0;
				Size num_double_bonds = 0;

				// - have at least two neighbours
				if (at_it->countBonds() > 1)
				{
					Atom::BondIterator bond_it;
					BALL_FOREACH_ATOM_BOND(*at_it, bond_it)
					{
						if (bond_it->getOrder() == 2)
							num_double_bonds++;

						else if (bond_it->getOrder() == 3)
							num_triple_bonds++;

						// - at least one of the neighbours must be acyclic
						Atom* partner = bond_it->getPartner(*at_it);

						if (!(partner->getProperty("InRing").getBool()))
						{
							++acyclic_neighbours;
						}
					}

					// - check for all of the conditions and decide whether the atom might be core-chain or not
					if ((acyclic_neighbours > 0) && ((num_double_bonds < 2) || (num_triple_bonds == 0)))
					{
						at_it->setProperty(SDGenerator::PRE_CORE_CHAIN);
					}
				}
			}
		}

		// - core-chain-atoms must have at least one neighbour that is a core-chain-atom, too
		BALL_FOREACH_ATOM(molecule_sys, at_it)
		{
			if (at_it->hasProperty(SDGenerator::PRE_CORE_CHAIN))
			{
				Atom::BondIterator bond_it;
				BALL_FOREACH_ATOM_BOND(*at_it, bond_it)
				{
					Atom* partner = bond_it->getPartner(*at_it);

					if (partner->hasProperty(SDGenerator::PRE_CORE_CHAIN))
					{
						at_it->setProperty(SDGenerator::CORE_CHAIN);
						at_it->setProperty(SDGenerator::FXAS);                           //      Property, later used in chain analysis
						break;
					}
				}
			}

		}
		DEBUG("\t-*-[checkAtoms]:\tRing-atoms and core-chain-atoms found." << endl)
	}

	void SDGenerator::clear()
	{
		ring_analyser_.clear();
		chains_.clear();
	}

	void SDGenerator::generateSD(System& molecule_sys)
	{
		// distinguish between core-chain-atoms, ring-atoms, and others
		prepare_(molecule_sys);

		for (Size i = 0; i < ring_analyser_.getNumberOfRingSystems(); i++)
		{
			// construct each ringsystem in the suitable way
			constructRingSystem_(i);
			// TODO: ?????? ??? ?????? ??? ???????
		std::vector<RingAnalyser::Ring> current_system = ring_analyser_.getRingSystem(i);
			for (Position j = 0; j < current_system.size(); j++)
			{
		// test
				for (Position k = 0; k < current_system[j].atoms.size(); k++)
				{
					current_system[j].atoms[k]->setProperty(SDGenerator::DEPOSITED);
					current_system[j].atoms[k]->setProperty(SDGenerator::PRE_ASSEMBLED);
				}
			}
		}

		treatChains_(molecule_sys);

		// assemble the Structure Diagram from the prepared Fragments
		MoleculeAssembler ma;
// TODO: remove the MoleculeAssembler! This is just a hack...
		std::vector<std::vector<std::vector<Atom*> > > ringsystems;
		for (Position i=0; i<ring_analyser_.getNumberOfRingSystems(); ++i)
		{
			ringsystems.push_back(std::vector<std::vector<Atom*> >());

			std::vector<RingAnalyser::Ring> current_system = ring_analyser_.getRingSystem(i);

			for (Position j = 0; j < current_system.size(); j++)
			{
				ringsystems.back().push_back(current_system[j].atoms);
			}
		}

		ma.assembleMolecule(molecule_sys, ringsystems, chains_);

		// be nice and clear up old stuff
		clear();

		DEBUG("Structure Diagram has been generated.")
	}

	void SDGenerator::buildRegularPolygon_(RingAnalyser::Ring& ring, Position first_anchor_index, bool clockwise)
	{
		Position second_anchor_index = (first_anchor_index + 1) % ring.atoms.size();

		Vector3& anchor_start = ring.atoms[first_anchor_index]->getPosition();
		Vector3& anchor_end   = ring.atoms[second_anchor_index]->getPosition();

		Vector3 anchor = anchor_end - anchor_start;

		float phi = 2.*M_PI / ring.atoms.size();

		float l = anchor.getLength();
		float r = l/(2.*sin(phi/2.));
		float h = l/(2.*tan(phi/2.));

		Vector3 orth_anchor(anchor.y, -anchor.x, 0.);
		orth_anchor.normalize();

		if (!clockwise)
		{
			orth_anchor *= -1.f;
			phi *= -1.;
		}

		Vector3 center          = anchor_start + anchor*0.5 + orth_anchor*h;
		Vector3 center_to_start = anchor_start - center;

		float phi_0 = ((center_to_start.x > 0) ? +1. : -1.) * acos(std::min(center_to_start.y/r, 1.f));

		Position current_atom_index = (second_anchor_index + 1) % ring.atoms.size();

		for (Position i=2; i<ring.atoms.size(); ++i)
		{
			Vector3 new_position = center + Vector3(r*sin(phi_0 + i*phi), r*cos(phi_0 + i*phi), 0);
			ring.atoms[current_atom_index]->setPosition(new_position);
			ring.atoms[current_atom_index]->setProperty(SDGenerator::DEPOSITED);
			current_atom_index = (current_atom_index + 1) % ring.atoms.size();
		}
	}

	void SDGenerator::buildOpenPolygon_(RingAnalyser::Ring& ring, Position first_anchor_index, Position second_anchor_index)
	{
		const size_t N = ring.atoms.size();
		double alpha = 2 * M_PI / N;

		double bond_length = options.getReal(Option::STANDARD_BOND_LENGTH);

		double l = (bond_length / 2) / cos(alpha / 2.);
		Vector3 d_vect = ring.atoms[second_anchor_index]->getPosition()
		               - ring.atoms[first_anchor_index]->getPosition();

		const double D = d_vect.getLength();

		double hh = l*l - D*D/4.;

		while (hh < 0)
		{
			bond_length *= 1.2;
		  l = (bond_length / 2) / cos((alpha) / 2.);
		  hh = l*l - D*D/4.;
		}
		const double h = sqrt(hh);

		Vector3 d_ortho(d_vect.y, -d_vect.x, 0);

		// find out in which direction we should attach the new ring
		// (it should be the direction pointing away from the center of
		// gravity of all previously deposited atoms)

		int pos = 1;
		Vector3 cog_deposited;
		for (Position i = first_anchor_index  % N; i != (second_anchor_index + 1) % N; i = (i + 1) % N, ++pos)
		{
			cog_deposited += ring.atoms[i]->getPosition();	
		}
		cog_deposited /= pos;

		Vector3 mid_anchor = ring.atoms[first_anchor_index]->getPosition() + d_vect/2;

		if ((cog_deposited - mid_anchor) * d_ortho > 0)
		{
			d_ortho *= -1.0;
			alpha *= -1.0;
		}

		Vector3 center = ring.atoms[first_anchor_index]->getPosition() + d_vect/2 + d_ortho*(h/D);
		Vector3 center_to_start = ring.atoms[second_anchor_index]->getPosition() - center;

		float phi_0 = ((center_to_start.x > 0) ? +1. : -1.) * acos(std::min(center_to_start.y/l, 1.));

		pos = 1;
		for (int i = (second_anchor_index + 1) % N; i != first_anchor_index; i = (i + 1) % N, ++pos)
		{
			Vector3 new_position(center.x + l*sin(phi_0 + pos*alpha), center.y + l*cos(phi_0 +pos*alpha), 0);
			ring.atoms[i]->setPosition(new_position);
			ring.atoms[i]->setProperty(SDGenerator::DEPOSITED);
		}

	}

	void SDGenerator::constructRingSystem_(Position current_ring_system_index)
	{
		std::vector<RingAnalyser::Ring> current_system = ring_analyser_.getRingSystem(current_ring_system_index);
		std::vector<Position> peeling_order = ring_analyser_.getPeelingOrder(current_ring_system_index);

		// undo the peeling backwards
		for (std::vector<Position>::reverse_iterator ring_it = peeling_order.rbegin(); ring_it != peeling_order.rend(); ++ring_it)
		{
			RingAnalyser::Ring& current_ring = current_system[*ring_it];

			switch (current_ring.type)
			{
				case RingAnalyser::CORE:	
					DEBUG("Attaching Core");
					attachCore_(*ring_it, current_system, (float)current_ring_system_index);
					break;
				case RingAnalyser::TEMPLATE:
					DEBUG("Attaching Template");
					attachTemplate_(*ring_it, current_system);
					break;
				case RingAnalyser::SPIRO:
					DEBUG("Attaching Spiro");
					attachSpiro_(*ring_it, current_system);
					break;
				case RingAnalyser::FUSED:
					DEBUG("Attaching Fused");
					attachFused_(*ring_it, current_system);
					break;
				case RingAnalyser::BRIDGED:
					DEBUG("Attaching Bridged");
					attachBridged_(*ring_it, current_system);
					break;
				default:
					Log.error() << "SDGenerator::constructRingSystem_(): Cannot attach unknown ring type" << std::endl;
					break;
			}
		}
	}

	void SDGenerator::attachCore_(Size core_index, std::vector<RingAnalyser::Ring>& current_system, float x_start)
	{
		std::vector<Atom*>& ring = current_system[core_index].atoms;

		// set the standard bond-length
		double bond_length = options.getReal(Option::STANDARD_BOND_LENGTH);

		// prepare the first two atom positions
		if (ring.size() % 2)
		{ 
			// odd numbered ring
			ring[0]->setPosition(Vector3(0.f, x_start, 0.f));
			ring[1]->setPosition(Vector3(bond_length, x_start, 0.f));
		}
		else
		{
			// even numbered ring
			ring[0]->setPosition(Vector3(x_start, 0.f, 0.f));
			ring[1]->setPosition(Vector3(x_start, bond_length, 0.f));
		}

		ring[0]->setProperty(SDGenerator::DEPOSITED);
		ring[1]->setProperty(SDGenerator::DEPOSITED);

		buildRegularPolygon_(current_system[core_index], 0, true);
		current_system[core_index].setProperty(SDGenerator::DEPOSITED);

		DEBUG("\t-*-[RSConstructor]:\t (attachCore_):\t done." << endl)
	}

	// interface for a Ring Template Database 
	// if a RTD will be installed, this interface can be used to get information from it
	void SDGenerator::attachTemplate_(Position /* current_ring */, std::vector<RingAnalyser::Ring>& /*current_system*/)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachTemplate):\t started.")
	}

	// attach a fused-ring to a (partially) prefabricated ringsystem
	void SDGenerator::attachFused_(Position current_ring_index, std::vector<RingAnalyser::Ring>& current_system)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachFused_):\t\t started.")

		// the fused ring that is to be attached
		std::vector<Atom*>& ring = current_system[current_ring_index].atoms;

		// the bonds this ring shares with other rings
		std::set<Bond const*>& shared_bonds = current_system[current_ring_index].shared_bonds;

		// find the two atoms that are shared by the ring that is to be 
		// constructed with another ring of the ring system that has already
		// been deposited

		// compute all ring memberships
		HashMap<Atom const*, std::set<Position> > rings_per_atom;

		for (Position i=0; i<current_system.size(); ++i)
		{
			for (Position j=0; j<current_system[i].atoms.size(); ++j)
			{
				rings_per_atom[current_system[i].atoms[j]].insert(i);
			}
		}

		Bond const* shared_bond = NULL;
		Atom const* first_atom  = NULL;
		Atom const* second_atom = NULL;

		Position neighbouring_ring;

		for (std::set<Bond const*>::iterator bond_it  = shared_bonds.begin(); 
		                                     bond_it != shared_bonds.end(); ++bond_it)
		{
			first_atom  = (*bond_it)->getFirstAtom();
			second_atom	= (*bond_it)->getSecondAtom();

			std::set<Position>&  first_rings = rings_per_atom[first_atom];
			std::set<Position>& second_rings = rings_per_atom[second_atom];

			std::set<Position> intersection;

			std::set_intersection(first_rings.begin(),  first_rings.end(),
			                      second_rings.begin(), second_rings.end(),
														std::inserter(intersection, intersection.begin()));

			for (std::set<Position>::iterator int_it = intersection.begin(); int_it != intersection.end(); ++int_it)
			{
				if (current_system[*int_it].hasProperty(SDGenerator::DEPOSITED))
				{
					shared_bond = *bond_it;
					neighbouring_ring = *int_it;
					break;
				}
			}

			if (shared_bond)
				break;
		}

		// if we have found a suitable bond...
		if (shared_bond)
		{
			// now we need to know if both rings are sequenced such that they see the bond in the same
			// direction, i.e. that the first atom of the bond preceeds the second one in both rings, or
			// vice versa

			// find the bond in the old ring that will be taken for the correct direction of the bonds 
			// in the new ring, i.e. the bond between the second shared atom and the next atom in the old ring
			Position first_atom_index, second_atom_index;
			for (Position i=0; i<ring.size(); ++i)
			{
				if (ring[i] == first_atom)
					first_atom_index = i;
				if (ring[i] == second_atom)
					second_atom_index = i;
			}

			Index index_diff = second_atom_index - first_atom_index;

			bool forward;

			if (abs(index_diff) == 1)
			{
				forward = (index_diff > 0);
			}
			else
			{
				forward = (second_atom_index == 0);
			}

			if (!forward)
			{
				std::swap(first_atom_index, second_atom_index);
			}

			std::vector<Atom*>& ref_ring = current_system[neighbouring_ring].atoms;

			// compute the center of the deposited neighbor
			Position num_depos_atoms = 1;
			Vector3 deposited_cog;
			for (Position i=0; i<ref_ring.size(); ++i, ++num_depos_atoms)
			{
				if (ref_ring[i]->hasProperty(SDGenerator::DEPOSITED))
					deposited_cog += ref_ring[i]->getPosition();
			}
			deposited_cog /= num_depos_atoms;

			Vector3 anchor = ring[second_atom_index]->getPosition() - ring[first_atom_index ]->getPosition();

			Vector3 orth_anchor(anchor.y, -anchor.x, 0.);
			Vector3 center = ring[first_atom_index]->getPosition() + anchor*0.5;
			
			bool clockwise = true;
			if ((deposited_cog - center) * orth_anchor > 0)
				clockwise = false;

			buildRegularPolygon_(current_system[current_ring_index], first_atom_index, clockwise);

			current_system[current_ring_index].setProperty(SDGenerator::DEPOSITED);

			DEBUG("\t-*-[SDGenerator]:\t (attachFused):\t done.")
		}
		//      if not both of the shared atoms have been found, append the cuurent ring at the end of the ringsystem and construct it later
		else
		{
// TODO: do i have to append it again?
			DEBUG("\t-*-[SDGenerator]:\t (attachFused):\t aborted. Ring appended.")
		}
	}

	// attach a bridged ring to a (partially) prefabricated ringsystem
	void SDGenerator::attachSpiro_(Position current_ring_index, std::vector<RingAnalyser::Ring>& current_system)
	{
		// for now, we do something extremely simple: construct a core-ring and translate it
		// to the correct position.
		// TODO: take care of the overlaps this procedure *will* produce!
		RingAnalyser::Ring& current_ring = current_system[current_ring_index];

		Position i;
		for (i=0; i<current_ring.atoms.size(); ++i)
		{
			if (current_ring.atoms[i]->hasProperty(SDGenerator::DEPOSITED))
				break;
		}

		if (i == current_ring.atoms.size())
		{
			Log.error() << "SDGenerator::attachSpiro_(): Can only attach rings with one deposited atom!" << std::endl;
			return;
		}

		Vector3 correct_position = current_ring.atoms[i]->getPosition();

		attachCore_(current_ring_index, current_system, 0);

		Vector3 translation = correct_position - current_ring.atoms[i]->getPosition();

		for (Position i=0; i<current_ring.atoms.size(); ++i)
		{
			current_ring.atoms[i]->setPosition(current_ring.atoms[i]->getPosition() + translation);
		}

		current_ring.setProperty(SDGenerator::DEPOSITED);
	}

	// attach a spiro-type ring to a (partially) prefabricated ringsystem
	void SDGenerator::attachBridged_(Position current_ring_index, std::vector<RingAnalyser::Ring>& current_system)
	{
		// construct a BRIDGED ring by the open polygon - method
		DEBUG("\t-*-[SDGenerator]:\t (attachBridged):\t started.")

		if(current_system[current_ring_index].hasProperty(SDGenerator::DEPOSITED))
		{
			return;
		}

		RingAnalyser::Ring& ring  = current_system[current_ring_index];
		std::vector<Atom*>& atoms = ring.atoms;

		//Make sure the ring contains at least two deposited atoms
		int counter = 0;
		for(Position j = 0; j < atoms.size(); ++j)
		{
			if(atoms[j]->hasProperty(SDGenerator::DEPOSITED))
			{
				++counter;
			}
		}

		if(counter < 2)
		{
			// This is clearly not a bridged ring or the peeling went wrong
			Log.error() << "SDGenerator::attachBridged_(): Can't attach ring with only one placed atom!" << std::endl;

			return;
		}
		else if (counter == atoms.size())
		{
			ring.setProperty(SDGenerator::DEPOSITED);
			//Ring is already deposited and we should bail out
			return;
		}

		// ok, let's find the first bridge head
		Position first_anchor_point = 0; 
		Position second_anchor_point = 0;

		for (Position i=0; i<atoms.size(); ++i)
		{
			bool last_deposited = atoms[ring.predecessor(i)]->hasProperty(SDGenerator::DEPOSITED);
			bool   is_deposited = atoms[i]->hasProperty(SDGenerator::DEPOSITED);
			bool next_deposited = atoms[ring.successor(i)]->hasProperty(SDGenerator::DEPOSITED);

			if (is_deposited && !last_deposited)
			{
				first_anchor_point = i;
			}

			if (is_deposited && !next_deposited)
			{
				second_anchor_point = i;
			}
		}

		// if both bridge heads are next to each other, we can treat this like a fused ring
		if (counter == 2)
		{
			attachFused_(current_ring_index, current_system);
			return;
		}
		else
		{
			buildOpenPolygon_(ring, first_anchor_point, second_anchor_point);
		}

		ring.setProperty(SDGenerator::DEPOSITED);
	}

	void SDGenerator::computeAdjacencyMatrix_(std::vector<Atom*>& chain, std::vector<bool>& result)
	{
		Size nodes = chain.size();
		
		// yes, this matrix should be symmetric, really, or even sparse. but even for large
		// chains, this does not really matter
		result.resize(nodes*nodes, false);

		for (Position i=0; i<nodes; ++i)
		{
			for (Position j=0; j<nodes; ++j)
			{
				// the diagonal should be set to true also (for the later stages)
				if ((i == j) || (chain[i]->isBoundTo(*(chain[j]))))
					result[i+j*nodes] = true;
			}
		}
	}

	bool SDGenerator::compareChains_(const vector<Atom*>& x, const vector<Atom*>& y)
	{
		return x.size() > y.size();
	}

	void SDGenerator::visitChainAreas_(Size k, std::vector<bool>& adj_matrix, std::vector<int>& val, Size nodes, 
	                                   Size id, std::vector<Atom*>& core_chain_atoms, std::vector<Atom*>& chain_area)
	{
		// recursive visit-procedure used in the depth-first-search for chain-areas
		Size t = 0;
		val[k] = ++id;

		for (t = 0; t < nodes; ++t)
		{
			if (adj_matrix[k + t*core_chain_atoms.size()])
			{
				if (val[t] == 0)
				{
					visitChainAreas_(t, adj_matrix, val, nodes, id,  core_chain_atoms, chain_area);
				}
			}
		}
		chain_area.push_back(core_chain_atoms[k]);
	}

	void SDGenerator::visitChains_(Size& k, std::vector<bool>& adj_matrix, std::vector<int>& val, Size& id, 
	                               std::vector<Atom*>& chain_area, Size& end, bool& breaker, Size& t, 
																 std::vector<Atom*>& prev_nodes)
	{
		std::list<Size> queue;

		queue.push_back(k);

		while (!queue.empty() && !breaker)
		{
			k = *queue.begin();
			queue.erase(queue.begin());

			val[k] = ++id;

			// enqueue every atom that is bound to the current one and has not 
			// been seen yet, and set its previous atom to the current one
			for (t = 0; t < adj_matrix.size(); t++)
			{
				if (adj_matrix[k + t * chain_area.size()] && (k != t))
				{
					//      if the end atom is reached, stop the search and return
					if (chain_area[t] == chain_area[end])
					{
						prev_nodes[t] = chain_area[k];
						breaker = 1;
						break;
					}

					if (val[t] == INT_MAX) 
					{
						prev_nodes[t] = chain_area[k];
						queue.push_back(t);
						val[t] = -1;
					}
				}
			}
		}
	}

	std::vector<Atom*> SDGenerator::findPath_(Atom*& first_edge, Atom*& second_edge,
	                                          std::vector<Atom*>& chain_area, std::vector<bool>& adj_matrix)
	{
		// breadth first search with traceback to find the path between every two edges in chain-area
		std::vector<Atom*> path;

		// vector to hold the order in which the atoms were visited
		std::vector<int> val;

		// vector to hold the previously visited atom for each atom
		std::vector<Atom*> prev_nodes; 

		Size id = 0;

		// starting position for the traceback after the end atom has been found
		Size t = 0;

		// get the indices of the two edges between which the path shall be found
		Position start = 0, end = 0;
		for (Position i = 0; i < chain_area.size(); ++i)
		{
			if (chain_area[i] == first_edge)
				start = i;
			if (chain_area[i] == second_edge)
				end = i;
		}

		// initialize the vectors val and prev_nodes
		for (Size i = 0; i < chain_area.size(); i++)
		{
			val.push_back(INT_MAX);           
			prev_nodes.push_back((Atom*)0);        
		}

		Size k = start;

		// flag to indicate when the end is found and finish the search
		bool breaker = 0;

		while (!breaker)
		{
			if (val[k] == INT_MAX)
			{
				visitChains_(k, adj_matrix, val, id, chain_area, end, breaker, t, prev_nodes);
			}
		}

		// traceback from the end of the path along the previously found atoms to the beginning of the path
		Atom* traceback = chain_area[t];

		while (traceback != chain_area[start])
		{
			path.push_back(traceback);

			Position new_index = 0;
			for (Position i=0; i<chain_area.size(); ++i)
			{
				if (chain_area[i] == traceback)
					new_index = i;
			}
			traceback = prev_nodes[new_index];
		}

		path.push_back(chain_area[start]);

		return path;
	}



	void SDGenerator::treatChains_(AtomContainer& ac)
	{
		// a vector holding all core-chain atoms
		std::vector<Atom*> core_chain_atoms;
		std::vector<Size> visited;

		AtomIterator at_it;
		BALL_FOREACH_ATOM(ac, at_it)
		{
			if (at_it->hasProperty(SDGenerator::CORE_CHAIN))
				core_chain_atoms.push_back(&*at_it);
		}

		// calculate an adjacency matrix for the core-chain atoms
		if (core_chain_atoms.size() > 0)
		{
			std::vector<bool> adj_matrix;
			computeAdjacencyMatrix_(core_chain_atoms, adj_matrix);

			std::vector<std::vector<Atom*> > chain_areas;

			// depth-first-search for chain areas

			// vector to hold the order in which the atoms were visited
			std::vector<int> val(core_chain_atoms.size(), 0);
			
			Size id = 0, k = 0;

			for (k = 0; k < core_chain_atoms.size(); k++)
			{
				if (val[k] == 0)
				{
					std::vector<Atom*> chain_area;
					// the recursive visit-function finds one connected chain-area each time it is called
					visitChainAreas_(k, adj_matrix, val, core_chain_atoms.size(), id, core_chain_atoms, chain_area);
					chain_areas.push_back(chain_area);
				}
			}

			DEBUG("\t-*-[buildChains]:\t" << chain_areas.size() << " chain areas found." << endl)

			for (Position i = 0; i < chain_areas.size(); ++i)
			{
				// find atoms that have only one adjacent core-chain atom (the edges of the chain area)
				std::vector<Atom*>& chain_area = chain_areas[i];

				std::deque<Atom*> edges;

				for (Position j = 0; j < chain_area.size(); j++)
				{
					Atom* current_atom = chain_area[j];

					Size adjacent_core_chain_atoms = 0;

					Atom::BondIterator bond_it;
					BALL_FOREACH_ATOM_BOND(*current_atom, bond_it)
					{
						if (bond_it->getPartner(*current_atom)->hasProperty(SDGenerator::CORE_CHAIN))
						{
							adjacent_core_chain_atoms++;
						}
					}

					if (adjacent_core_chain_atoms < 2)
					{
						edges.push_back(current_atom);
					}
				}

				// calculate the adjacency matrix for the chain-area
				computeAdjacencyMatrix_(chain_area, adj_matrix);

				// breadth-first search for all chains in the chain-area
				std::vector<std::vector<Atom*> > found_chains;
				// for every two edges of the chain-area, find the path beween them
				for (std::deque<Atom*>::iterator edge_i = edges.begin(); edge_i != edges.end(); ++edge_i)
				{
					for (std::deque<Atom*>::iterator edge_j = edge_i + 1; edge_j != edges.end(); ++edge_j)
					{
						std::vector<Atom*> chain = findPath_(*edge_i, *edge_j, chain_area, adj_matrix);
						found_chains.push_back(chain);
					}
				}

				// sort the found chains by their length
				std::sort(found_chains.begin(), found_chains.end(), SDGenerator::compareChains_);

				std::vector<std::vector<Atom*> > new_chains;

				Size max_adj_rings = 0;
				Size keep_i = 0;
				for (Size i = 0; i < found_chains.size(); i++)
				{
					Size adj_rings = 0;
					if (found_chains[i].size() == found_chains[0].size())
					{
						Atom* current_atom = found_chains[i][0];
						Atom::BondIterator bond_it;
						BALL_FOREACH_ATOM_BOND(*current_atom, bond_it)
						{
							if (bond_it->getPartner(*current_atom)->hasProperty(SDGenerator::IN_RING))
								adj_rings++;
						}

						current_atom = found_chains[i].back();
						BALL_FOREACH_ATOM_BOND(*current_atom, bond_it)
						{
							if (bond_it->getPartner(*current_atom)->hasProperty(SDGenerator::IN_RING))
								adj_rings++;
						}

						// if there is more than one chain of maximum length, chose the one 
						// with the most adjacent rings as the main chain
						if (adj_rings > max_adj_rings)
						{
							max_adj_rings = adj_rings;
							keep_i = i;
						}
					}
				}

				// arrange all the other chains around the the main chain
				for (Size i = 0; i < found_chains[keep_i].size(); i++)
				{
					found_chains[keep_i][i]->setProperty(SDGenerator::BUILT_IN_CHAIN);
				}

				new_chains.push_back(found_chains[keep_i]);

				// ??? TODO: what does this *DO* ???
				bool pushbacked = 0;
				bool increment_i = 0;
				Size j = 0;
				for (Size i = 1; i < found_chains.size(); ++i)
				{
					pushbacked = 0;
					increment_i = 0;

					std::vector<Atom*> new_chain;

					while (!(increment_i))
					{
						if (!(found_chains[i][j]->hasProperty(SDGenerator::BUILT_IN_CHAIN)))
						{
							found_chains[i][j]->setProperty(SDGenerator::BUILT_IN_CHAIN);
							new_chain.push_back(found_chains[i][j]);
						}
						else
						{
							if (new_chain.size() > 0)
							{
								new_chains.push_back(new_chain);
								pushbacked = 1;

								break;
							}
						}

						j++;

						if (j == found_chains[i].size())
						{
							j = 0;
							increment_i = 1;
						}
					}

					if (new_chain.size() > 0 && !pushbacked)
					{
						new_chains.push_back(new_chain);
					}

					if (increment_i)
					{
						i++;
					}
				}

				for (Position new_chain=0; new_chain<new_chains.size(); ++new_chain)
					chains_.push_back(new_chains[new_chain]);
			}

			std::sort(chains_.begin(), chains_.end(), SDGenerator::compareChains_);
		}
	}

	void SDGenerator::setDefaultOptions()
	{		
	 	options.setDefaultBool(Option::SHOW_HYDROGENS, 
		                       Default::SHOW_HYDROGENS);

	 	options.setDefaultReal(Option::STANDARD_BOND_LENGTH, 
		                        Default::STANDARD_BOND_LENGTH);
	}
} // namespace BALL
