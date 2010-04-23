// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/rsConstructor.h>
#include <BALL/STRUCTURE/chainBuilder.h>
#include <BALL/STRUCTURE/moleculeAssembler.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/KERNEL/forEach.h>

#define BALL_DEBUG_SDGENERATOR

#ifdef BALL_DEBUG_SDGENERATOR
# define DEBUG(a) Log.info() << a << endl;
#else
# define DEBUG(a) 
#endif

namespace BALL
{
	const char* SDGenerator::Option::SHOW_HYDROGENS = "sd_generator_show_hydrogens";
	const bool  SDGenerator::Default::SHOW_HYDROGENS = true;
 
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

		// compute the smallest set of smallest rings
		// NOTE: the RingPerceptionProcessor sets the "InRing" property for us
		RingPerceptionProcessor rrp;
		rrp.calculateSSSR(sssr_, molecule_sys);

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

	// find an atom's neighbours inside a certain ring
	pair<Atom*, Atom*> SDGenerator::getNeighbours(vector<Atom*>& ring, Atom* atom)
	{
		// a ring must contain at least 3 Atoms
		if (ring.size() < 3)
		{
			throw Exception::InvalidRange(__FILE__, __LINE__, ring.size());
		}

		// clear properties that have possibly been set in a previuos call
		for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
		{
			ring[i]->clearProperty(SDGenerator::FIRSTNEIGHBOUR);
		}

		//      declare the pair of neighbours that is to be returned
		pair<Atom*, Atom*> neighbours;

		bool find_second = false;       //      has the first neighbour been found yet?
		for (Atom::BondIterator bond_it = atom -> beginBond(); bond_it != atom -> endBond(); bond_it++)
		{
			RingAnalyser ra;
			if (find_second == false)        //      if the first neighbour has not been found, yet
			{
				//      neighbours must be bound to the checked atom and be part of the same ring
				if ((ra.isInRing(bond_it -> getPartner(*atom), ring)))
				{
					neighbours.first = bond_it -> getPartner(*atom);
					bond_it -> getPartner(*atom) -> setProperty(SDGenerator::FIRSTNEIGHBOUR);
					find_second = true;
				}
			}
			else    //      if the first neighbour has already been found
			{
				//      neighbours must be bound to the checked atom and be part of the same ring
				if (ra.isInRing(bond_it -> getPartner(*atom), ring)  && !(bond_it -> getPartner(*atom) -> hasProperty(SDGenerator::FIRSTNEIGHBOUR)))
				{
					neighbours.second = bond_it -> getPartner(*atom);
				}
			}
		}
		return neighbours;
	}

	void SDGenerator::sequenceRings_(std::vector<std::vector<Atom*> >& ringsystem, std::vector<std::vector<Atom*> >& sequenced_rings)
	{
		// make room for the reordered rings
		sequenced_rings.resize(ringsystem.size());

		// sequencing the rings in the sssr to prepare them for analysis and construction
		for (Position j = 0; j < ringsystem.size(); j++)
		{
			// create a new vector for each ring to hold the sequenced atoms
			std::vector<Atom*>& seq_ring = sequenced_rings[j];

			// the atom we start with
			Atom* start_atom = ringsystem[j][0];
			seq_ring.push_back(start_atom);

			// put all other atoms into a list
			std::list<Atom*> ring_atoms;
			for (Position k = 1; k < ringsystem[j].size(); k++)
			{
				ring_atoms.push_back(ringsystem[j][k]);
			}

			Atom* last_atom = start_atom;
			while(!ring_atoms.empty())
			{
				for (std::list<Atom*>::iterator ring_it = ring_atoms.begin(); ring_it != ring_atoms.end(); ++ring_it)
				{
					if (last_atom->isBoundTo(**ring_it))
					{
						seq_ring.push_back(*ring_it);
						last_atom = *ring_it;
						ring_atoms.erase(ring_it);
						break;
					}
				}
			}
		}
		DEBUG("\t-*-[sequenceRings]:\tRings have been sequenced." << endl)
	}

	void SDGenerator::generateSD(System& molecule_sys)
	{
		// distinguish between core-chain-atoms, ring-atoms, and others
		prepare_(molecule_sys);

		// sequence the atoms in each ring
		std::vector<vector<Atom*> > seq_rings;
		sequenceRings_(sssr_, seq_rings);

		// cluster the rings into connected ringsystems
		RingAnalyser ra;
		std::vector<std::vector<std::vector<Atom*> > > ringsystems = ra.clusterRings(seq_rings);

		// analyse the way the rings are connected and construct each ringsystem in the suitable way
		std::vector<RingAnalyser::RingInfo> analysed_rings;

		for (Size i = 0; i < ringsystems.size(); i++)
		{
			// analyse the way the rings are connected
			analysed_rings = ra.analyseRings(ringsystems[i]);

			// construct each ringsystem in the suitable way
			RSConstructor rsc;
			rsc.constructRS(analysed_rings, i);

			for (Position j = 0; j < ringsystems[i].size(); j++)
			{
				for (vector<Atom*>::size_type k = 0; k != ringsystems[i][j].size(); k++)
				{
					ringsystems[i][j][k]->setProperty(SDGenerator::DEPOSITED);
					ringsystems[i][j][k]->setProperty(SDGenerator::PRE_ASSEMBLED);
				}
			}
		}

		// find all chains and sort them by their length
		ChainBuilder cb;
		vector<vector<Atom*> > chains = cb.buildChains(molecule_sys);

		// assemble the Structure Diagram from the prepared Fragments
		MoleculeAssembler ma;
		ma.assembleMolecule(molecule_sys, ringsystems, chains);

		DEBUG("Structure Diagram has been generated.")
	}
	
	void SDGenerator::setDefaultOptions()
	{		
	 	options.setDefaultBool(SDGenerator::Option::SHOW_HYDROGENS,
	 												 SDGenerator::Default::SHOW_HYDROGENS);
	}
} // namespace BALL
