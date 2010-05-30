// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/chainBuilder.h>

#ifdef BALL_DEBUG_CHAINBUILDER
#define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a) 
#endif

namespace BALL
{

	ChainBuilder::ChainBuilder()
	{}


	ChainBuilder::~ChainBuilder()
	{}

	vector<vector<Size> > ChainBuilder::calcAdjMatrix(vector<Atom*>& core_chain_atoms)
	{
		//      build a matrix, holding a "1" if two atoms are bound to each other and "0" if they are not

		DEBUG("\t-*-[ChainHandler]:\tcalcAdjMatrix started." << endl)

		int nodes = core_chain_atoms.size() -1;

		if (nodes < 0)
		{
			nodes = 0;
		}

		vector<vector<Size> > adj_matrix;

		DEBUG("nodes = " << nodes)

		int x(0);
		int y(0);

		for (x = 0; x <= nodes; x++)
		{
			adj_matrix.push_back(vector<Size>(nodes + 1, 0));
		}


		for (x = 0; x <= nodes; x++)
		{
			for (y = 0; y <= nodes; y++)
			{
				if (core_chain_atoms[x] -> isBoundTo(*(core_chain_atoms[y])))
				{
					adj_matrix[x][y] = 1;
				}
			}
		}

		for (x = 0; x <= nodes; x++)
		{
			adj_matrix[x][x] = 1;
		}

		DEBUG("\t-*-[ChainHandler]:\tcalcAdjMatrix done." << endl)

		return adj_matrix;
	}



	void ChainBuilder::visit_c_areas(Size& k, vector<vector<Size> >& adj_matrix, vector<int>& val, Size& nodes, Size& id, vector<Atom*>& core_chain_atoms, vector<Atom*>& chain_area)
	{
		//      recursive visit-procedure used in the depth-first-search for chain-areas
		Size t(0);
		val[k] = ++id;

		for (t = 0; t <= nodes; t++)
		{
			if (adj_matrix[k][t] != 0)
			{
				if (val[t] == 0)
				{
					visit_c_areas(t, adj_matrix, val, nodes, id,  core_chain_atoms, chain_area);
				}
			}
		}
		chain_area.push_back(core_chain_atoms[k]);
	}


	void ChainBuilder::findChainAreas(Size& nodes, vector<vector<Size> >& adj_matrix, 
																		vector<Atom*>& core_chain_atoms, vector<vector<Atom*> >& chain_areas)
	{
		//      depth-first-search for the chain-areas
		vector<int> val(nodes + 1, 0);        //      vector to hold the order, in which the atoms were visited
		Size id(0);
		Size k(0);

		for (k = 0; k <= nodes; k++)
		{
			if (val[k] == 0)
			{
				vector<Atom*> chain_area;
				//      the recursive visit-function finds one connected chain-area each time it is called
				visit_c_areas(k, adj_matrix, val, nodes, id, core_chain_atoms, chain_area);
				chain_areas.push_back(chain_area);
			}
		}
	}

	void ChainBuilder::findEdges(vector<Atom*>& chain_area)
	{
		//      find atoms that have only one adjacent core-chain-atom and therefore must be an edge of the chain-area
		for (vector<Atom*>::size_type i = 0; i < chain_area.size(); i++)
		{
			Size adjacent_core_chain_atoms = 0;

			for (Atom::BondIterator bond_it = chain_area[i] -> beginBond(); bond_it != chain_area[i] -> endBond(); bond_it++)
			{
				if (bond_it -> getPartner(*chain_area[i]) -> hasProperty(SDGenerator::CORE_CHAIN))
				{
					adjacent_core_chain_atoms ++;
				}
			}

			if (adjacent_core_chain_atoms < 2)
			{
				chain_area[i] -> setProperty(SDGenerator::EDGE);
			}
		}
	}


	void ChainBuilder::visit_chains(Size& k,  vector<vector<Size> >& adj_matrix, vector<int>& val, Size& id, vector<Atom*>& chain_area, Size& end, bool& breaker, Size& t, vector<Atom*>& prev_nodes)
	{
		list<Size> queue;

		//queue.enQueue(k);     //      put the starting atom Sizeo the queue
		queue.push_back(k);

		//while ((!queue.isEmpty()) && (!(breaker)))
		while ((!queue.empty()) && (!(breaker)))
		{
			//k = queue.deQueue();  //      get the next atom from the queue
			k = *queue.begin();
			queue.erase(queue.begin());

			val[k] = ++id;

			//      enqueue every atom that is bound to the current one and has not been seen yet, and set its previous atom to the current one
			for (t = 0; t != adj_matrix.size(); t++)
			{
				if ((adj_matrix[k][t] == 1) && (k != t))
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

	// get the index of an Atom-poSizeer in a certain vector<Atom*>
	Size ChainBuilder::getIndex(Atom*& atom, vector<Atom*>& container)
	{
		for (Size i = 0; i != container.size(); i++)
		{
			if (container[i] == atom)
			{
				return i;
			}
		}

		DEBUG("!!! getIndex failed !!! ")
		return INT_MAX; 
	}


	vector<Atom*> ChainBuilder::findPath(Atom*& first_edge, Atom*& second_edge,vector<Atom*>& chain_area, vector<vector<Size> >& adj_matrix)
	{
		//breadth first search with traceback to find the path between every two edges in chain-area
		vector<Atom*> path;

		vector<int> val;                                                        //      vector to hold the order, in which the atoms were visited
		vector<Atom*> prev_nodes;                       //      vector to hold the previously visited atom for each atom
		Size id = 0;

		Size t(0); //      starting position for the traceback after the end atom has been found

		//      get the indices of the two edges between which the path shall be found
		Size start = getIndex(first_edge, chain_area);
		Size end = getIndex(second_edge, chain_area);

		//      initialize the vectors val and prev_nodes
		for (Size i = 0; i != chain_area.size(); i++)
		{
			val.push_back(INT_MAX);           
			prev_nodes.push_back((Atom*)0);        
		}


		Size k = start;

		//      flag to indicate when the end is found and finish the search
		bool breaker = 0;

		while (!breaker)
		{
			if (val[k] == INT_MAX)
			{
				visit_chains(k, adj_matrix, val, id, chain_area, end, breaker, t, prev_nodes);
			}
		}

		//      traceback from the end of the path along the previously found atoms to the beginning of the path
		Atom* traceback = chain_area[t];

		while (traceback != chain_area[start])
		{
			path.push_back(traceback);

			traceback = prev_nodes[getIndex(traceback, chain_area)];
		}
		path.push_back(chain_area[start]);

		return path;
	}

  // comparator, used to sort the chains by their length
	
	bool ChainBuilder::compareChains(const vector<Atom*>& x, const vector<Atom*>& y)
	{
		return x.size() > y.size();
	}

	vector<vector<Atom*> > ChainBuilder::findChains(vector<Atom*>& chain_area, vector<vector<Size> >& adj_matrix)
	{
		DEBUG("\t-*-[buildChains]:\t findChains called.")

		vector<vector<Atom*> > chains;

		//      for every two edges of the chain-area, find the path beween them
		for (vector<Atom*>::size_type i = 0; i != chain_area.size(); i++)
		{
			for (vector<Atom*>::size_type j = 0; j != chain_area.size(); j++)
			{
				if (i != j)
				{

					if (chain_area[i] -> hasProperty(SDGenerator::EDGE)  && chain_area[j] -> hasProperty(SDGenerator::EDGE))
					{
						vector<Atom*> chain = findPath(chain_area[i], chain_area[j], chain_area, adj_matrix);
						chains.push_back(chain);
					}

				}
			}
		}
		//      sort the found chains by their length
		
		// 		ChainBuilder cb;
		sort(chains.begin(), chains.end(), ChainBuilder::compareChains);

		vector<vector<Atom*> > new_chains;

		Size max_adj_rings = 0;
		Size keep_i = 0;
		for (Size i = 0; i != chains.size(); i++)
		{
			Size adj_rings = 0;
			if (chains[i].size() == chains[0].size())
			{

				for (Atom::BondIterator bond_it = chains[i][0] -> beginBond(); bond_it != chains[i][0] -> endBond(); ++bond_it)
				{
					if (bond_it -> getPartner(*chains[i][0]) -> hasProperty(SDGenerator::IN_RING))
					{
						adj_rings++;
					}
				}

				// if there is more than one chain of maximum length, chose the one with the most adjacent rings as the main chain
				for (Atom::BondIterator bond_it = chains[i][chains[i].size() -1] -> beginBond(); bond_it != chains[i][chains[i].size() -1] -> endBond(); ++bond_it)
				{
					if (bond_it -> getPartner(*chains[i][chains[i].size() -1]) -> hasProperty(SDGenerator::IN_RING))
					{
						adj_rings++;
					}
				}
				if (adj_rings > max_adj_rings)
				{
					max_adj_rings = adj_rings;
					keep_i = i;
				}
			}
		}

		// arrange all the other chains around the the main chain

		for (Size i = 0; i != chains[keep_i].size(); i++)
		{
			chains[keep_i][i] -> setProperty(SDGenerator::BUILT_IN_CHAIN);
		}

		new_chains.push_back(chains[keep_i]);

		bool pushbacked = 0;
		bool increment_i = 0;
		Size j = 0;
		for (Size i = 1; i != chains.size();)
		{
			pushbacked = 0;
			increment_i = 0;

			vector<Atom*> new_chain;

			while (!(increment_i))
			{
				if (!(chains[i][j] -> hasProperty(SDGenerator::BUILT_IN_CHAIN)))
				{
					chains[i][j] -> setProperty(SDGenerator::BUILT_IN_CHAIN);
					new_chain.push_back(chains[i][j]);
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

				if (j == chains[i].size())
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

		return new_chains;

	}


	vector<vector<Atom*> > ChainBuilder::buildChains(System& molecule_sys)
	{
		//      declare return-vector
		vector<vector<Atom*> > all_chains;

		//      build a vector holding all core-chain-atoms
		vector<Atom*> core_chain_atoms;

		vector<Size> visited;

		AtomIterator atom_it;

		for (atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); ++atom_it)
		{
			if (atom_it -> hasProperty(SDGenerator::CORE_CHAIN))
			{
				core_chain_atoms.push_back(&(*atom_it));
			}

		}

		//      calculate an adjacence-matrix for the core-chain-atoms

		if (core_chain_atoms.size() > 0)
		{
			vector<vector<Size> > adj_matrix = calcAdjMatrix(core_chain_atoms);

			Size nodes = core_chain_atoms.size() - 1;

			vector<vector<Atom*> > chain_areas;

			//      depth-first-search for areas, consisting of chains
			findChainAreas(nodes, adj_matrix, core_chain_atoms, chain_areas);

			DEBUG("\t-*-[buildChains]:\t" << chain_areas.size() << " chain areas found." << endl)

			for (vector<vector<Atom*> >::size_type i = 0; i < chain_areas.size(); i++)
			{
				//      find all ending core chain atoms in a chain-area
				findEdges(chain_areas[i]);

				//      calculate the adjacency matrix for the chain-area
				adj_matrix = calcAdjMatrix(chain_areas[i]);

				//      breadth-first search for all chains in the chain-area
				vector<vector<Atom*> > chains = findChains(chain_areas[i], adj_matrix);

				for (Size i = 0; i < chains.size(); i++)
				{
					all_chains.push_back(chains[i]);
				}

			}
			// ChainBuilder cb;
			sort(all_chains.begin(), all_chains.end(), ChainBuilder::compareChains);
		}

		return all_chains;

	}

} // namespace BALL


