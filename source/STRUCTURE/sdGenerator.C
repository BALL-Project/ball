// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sdGenerator.C,v 1.2.2.1 2006/06/26 18:38:52 oliver Exp $
//
// Author:
//   Holger Franken
//

#include <BALL/STRUCTURE/sdGenerator.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>

#ifdef BALL_DEBUG_SDGENERATOR
# define DEBUG(a) Log.info() << a << endl;
#else
# define DEBUG(a) 
#endif

namespace BALL
{

	SDGenerator::SDGenerator()
	{}


	SDGenerator::~SDGenerator()
	{}

	void SDGenerator::checkAtoms(System& molecule_sys)
	{

		//      remove all Hydrogen-Atoms from the System
		bool no_H = 1;

		if (no_H)
		{
			Selector s("element(H)");
			molecule_sys.apply(s);

			DEBUG(s.getNumberOfSelectedAtoms() << " atoms selected")

			molecule_sys.removeSelected();
			DEBUG("all H's removed")
		}


		AtomIterator atom_it;


		//      distinguish between ring-atoms and core-chain-atoms
		for (atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); ++atom_it)
		{
			//      finding the ring-atoms


			RingFinder is_cyclic;

			if (is_cyclic(*atom_it))
			{
				atom_it -> setProperty(SDGenerator::IN_RING);
			}

			//      declare atoms as core-chain if they fulfil the following conditions:
			//              -       acylic
			//              -       have at least two neighbours, at least one of which is acyclic
			//              - do not have an adjacent triple bond, or two adjacent double-bonds

			else
			{
				Size acyclic_neighbours = 0;
				Size num_triple_bonds = 0;
				Size num_double_bonds = 0;

				//              -       have at least two neighbours
				if (atom_it -> Atom::countBonds() > 1)
				{
					for (Atom::BondIterator bond_it = atom_it -> beginBond(); bond_it != atom_it -> endBond(); ++bond_it)
					{
						if (bond_it -> getOrder() == 2)
							num_double_bonds++;

						else if (bond_it -> getOrder() == 3)
							num_triple_bonds++;

						//      -       at least one of the neighbours must be acyclic
						RingFinder is_cyclic2;
						if (bond_it -> getFirstAtom() == &*atom_it)
						{
							if (!(is_cyclic2(*(bond_it -> getSecondAtom()))))
							{
								acyclic_neighbours++;
							}
						}
						else if (!(is_cyclic2(*(bond_it -> getFirstAtom()))))
						{
							acyclic_neighbours++;
						}

					}

					// - check for all of the conditions and decide whether the atom might be core-chain or not
					if ((acyclic_neighbours > 0) && ((num_double_bonds < 2) ||( num_double_bonds == 0)))
					{
						atom_it -> setProperty(SDGenerator::PRE_CORE_CHAIN);
					}
				}
			}
		}


		// - core-chain-atoms must have at least one neighbour, that is a core-chain-atom, too
		for (atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); ++atom_it)
		{
			for (Atom::BondIterator bond_it = atom_it -> beginBond(); bond_it != atom_it -> endBond(); ++bond_it)
			{
				if (atom_it -> hasProperty(SDGenerator::PRE_CORE_CHAIN))
				{
					if (bond_it -> getFirstAtom() == &*atom_it)
					{
						if (bond_it -> getSecondAtom() -> hasProperty(SDGenerator::PRE_CORE_CHAIN))
						{
							atom_it -> setProperty(SDGenerator::CORE_CHAIN);
							atom_it -> setProperty(SDGenerator::FXAS);                           //      Property, later used in chain analysis
							continue;
						}
					}

					else if (bond_it -> getFirstAtom() -> hasProperty(SDGenerator::PRE_CORE_CHAIN))
					{
						atom_it -> setProperty(SDGenerator::CORE_CHAIN);
						atom_it -> setProperty(SDGenerator::FXAS);                           //      Property, later used in chain analysis
					}
				}
			}

		}
		DEBUG("\t-*-[checkAtoms]:\tRing-atoms and core-chain-atoms found." << endl)
	}


	//      find an atom's neighbours inside a certain ring

	pair<Atom*, Atom*> SDGenerator::getNeighbours(vector<Atom*>& ring, Atom*& atom)
	{
		//      a ring must contain at least 3 Atoms
		if (ring.size() < 3)
		{
			throw Exception::InvalidRange(__FILE__, __LINE__, ring.size());
		}

		//      clear Properties, that have possibly been set in a previuos call
		for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
		{
			ring[i] -> clearProperty(SDGenerator::FIRSTNEIGHBOUR);
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

	//      using  Figueras algorithm to calculate the smallest set of smallest rings
	vector <vector<Atom*> > SDGenerator::getSSSR(System& molecule_sys)
	{

		vector<vector<Atom*> > sssr;

		DEBUG("\t-*-[getSSSR]:\t\t"  << "searching SSSR.")

		//      call the implementation of  Figueras algorithm
		RingPerceptionProcessor getRings;

		getRings.RingPerceptionProcessor::calculateSSSR(sssr, molecule_sys);

		DEBUG("\t-*-[getSSSR]:\t\t" << sssr.size() << " Rings found." << endl)

		return sssr;
	}


	vector<vector<Atom*> > SDGenerator::sequenceRings(vector<vector<Atom*> >& ringsystem)
	{

		//      sequencing the rings in the sssr to prepare them for anaylis and construction
		vector<vector<Atom*> >  sequenced_rings;

		for (vector<vector<Atom*> >::size_type j = 0; j != ringsystem.size(); j++)
		{
			//      create a new vector for each ring to hold the sequenced atoms
			vector<Atom*> seq_ring;

			//      create the start-atom
			Atom* a1;

			//      clear Properties, that have possibly been set in a previuos call
			for (vector<Atom*>::size_type k = 0; k != ringsystem[j].size(); k++)
			{
				ringsystem[j][k] ->  clearProperty(SDGenerator::SEQUENCED);
			}

			//      set the first atom in the unsequenced ring as start-atom of the new sequenced one
			a1 = ringsystem[j][0];
			seq_ring.push_back(a1);
			a1 -> setProperty(SDGenerator::SEQUENCED);

			//      find the start-atom's neighbours in the ring
			SDGenerator sdg;
			pair<Atom*, Atom*> a1_neighbours = sdg.getNeighbours(ringsystem[j], a1);

			Atom* next_a = new Atom;   //      the next atom to be considered
			Atom* curr_a = new Atom;   //      the atom that is currently beeing considered

			//      continue with the first neighbour

			curr_a = a1_neighbours.first;

			while(1)
			{
				//      get the current atom's neighbours Size he ring

				pair<Atom*, Atom*> neighbours = sdg.getNeighbours(ringsystem[j], curr_a);

				//      if one of the neighbours was the start-atom and the other neighbour has already been sequenced, the complete ring has been sequenced
				if ((neighbours.first == a1) ||(neighbours.second == a1))
				{

					if (neighbours.first == a1)
					{

						if (neighbours.second -> hasProperty(SDGenerator::SEQUENCED))
						{
							seq_ring.push_back(curr_a);

							break;
						}
					}

					else if (neighbours.second == a1)
					{

						if (neighbours.first -> hasProperty(SDGenerator::SEQUENCED))
						{
							seq_ring.push_back(curr_a);

							break;
						}
					}
				}

				// otherwise find the neighbour that has not been sequenced, yet
				if ((!(neighbours.first -> hasProperty(SDGenerator::SEQUENCED))) && (neighbours.second -> hasProperty(SDGenerator::SEQUENCED)))
				{
					next_a = neighbours.first;
				}
				else if ((!(neighbours.second -> hasProperty(SDGenerator::SEQUENCED))) && (neighbours.first -> hasProperty(SDGenerator::SEQUENCED)))
				{
					next_a = neighbours.second;
				}

				//      append the current atom to the sequenced ring
				seq_ring.push_back(curr_a);

				//      declare it as sequenced
				curr_a -> setProperty(SDGenerator::SEQUENCED);

				// proceed to the next atom
				curr_a = next_a;

			}

			//      append the completely sequenced ring to the set of sequenced rings
			sequenced_rings.push_back(seq_ring);

		}
		DEBUG("\t-*-[sequenceRings]:\tRings have been sequenced." << endl)
		return sequenced_rings;
	}


	void SDGenerator::generateSD(System& molecule_sys)
	{

		//      distinguish between core-chain-atoms, ring-atoms and others
		checkAtoms(molecule_sys);

		//      get the "smallest set of smallest rings" (SSSR)
		vector<vector<Atom*> > rings = getSSSR(molecule_sys);

		//      sequence the atoms in each ring
		vector<vector<Atom*> > seq_rings = sequenceRings(rings);


		//      cluster the rings Sizeo connected ringsystems
		RingClusterer riclu;
		vector<vector<vector<Atom*> > > ringsystems = riclu.clusterRings(seq_rings);


		//      analyse the way the rings are connected and construct each ringsystem in the suitable way
		vector<RingAnalyser::RingInfo> analysed_rings;

		for (vector<vector<vector<Atom*> > >::size_type i = 0; i != ringsystems.size(); i++)
		{
			cerr << endl << "\tRingsystem No.: " << i+1 << endl << endl;

			//      analyse the way the rings are connected
			RingAnalyser ra;
			analysed_rings = ra.analyseRings(ringsystems[i]);

			//      construct each ringsystem in the suitable way
			RSConstructor rsc;
			rsc.constructRS(analysed_rings, i);

			for (vector<vector<Atom*> >::size_type j = 0; j != ringsystems[i].size(); j++)
			{
				for (vector<Atom*>::size_type k = 0; k != ringsystems[i][j].size(); k++)
				{
					ringsystems[i][j][k] -> setProperty(SDGenerator::DEPOSITED);
					ringsystems[i][j][k] -> setProperty(SDGenerator::PRE_ASSEMBLED);

				}
			}

		}


		//      find all chains and sort them by their length
		ChainBuilder cb;
		vector<vector<Atom*> > chains = cb.buildChains(molecule_sys);



		//      assemble the Structure Diagram from the prepared Fragments
		MoleculeAssembler ma;
		ma.assembleMolecule(molecule_sys, ringsystems, chains);

		DEBUG("Structure Diagram has been generated.")
	}

} // namespace BALL
