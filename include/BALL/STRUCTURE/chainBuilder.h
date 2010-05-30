// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


#ifndef BALL_STRUCTURE_CHAINBUILDER_H
#define BALL_STRUCTURE_CHAINBUILDER_H

#include <BALL/common.h>

#include <vector>

namespace BALL
{
	class Atom;
	class System;

        /**
                * \brief This class provides methods for analysis of the chains in a molecule
                *
                */
        class BALL_EXPORT ChainBuilder
        {
        public:

                /**
                 * \brief Default-Constructor
                 */
                ChainBuilder();

                /**
                 * \brief Destructor
                 */
                ~ChainBuilder();

                /**
                * \brief Determines the the main chain and side chains of a structure diagram
                * @param molecule_sys the input System
                * @return the chains sorted by their length
                */
                vector<vector<Atom*> > buildChains(System& molecule_sys);

								/**
                * \brief        comparator for sorting the chains by their length
								* @param x chain
								* @param y chain
                */
 								static bool compareChains(const vector<Atom*>& x, const vector<Atom*>& y);

        private:

                /**
                * \brief creates a matrix, holding a "1" if two atoms a connected and "0" if they are not
                * @param core_chain_atoms the core chain atoms of a molecule
                * @return the adjacence-matrix representing the molecule-graph
                */
                vector<vector<Size> > calcAdjMatrix(vector<Atom*>& core_chain_atoms);

                /**
                * \brief recursive visit-finction, used by findChainAreas()
                * @param k counter for the sequence-vector
                * @param adj_matrix adjacence-matrix representing the molecule-graph
                * @param val sequence, in which the atoms are visited
                * @param nodes number of nodes in the molecule-graph
                * @param id indicates, at which point in the sequence a certain atom has been visited
                * @param core_chain_atoms the core chain atoms of the molecule
                * @param chain_area the chain area that is to be filled with its atoms
                */
                void visit_c_areas(Size& k, vector<vector<Size> >& adj_matrix, vector<int>& val, Size& nodes, Size& id, vector<Atom*>& core_chain_atoms, vector<Atom*>& chain_area);

                /**
                * \brief performs a depth-first-search for chain-areas in a molecule
                * @param nodes number of nodes in the molecule-graph
                * @param adj_matrix adjacence-matrix representing the molecule-graph
                * @param core_chain_atoms the core chain atoms of the molecule
                * @param chain_areas the chain areas that are to be found
                */
                void findChainAreas(Size& nodes, vector<vector<Size> >& adj_matrix, vector<Atom*>& core_chain_atoms, vector<vector<Atom*> >& chain_areas);

                /**
                * \brief finds all edges in one chain area
                * @param chain_area
                */
                void findEdges(vector<Atom*>& chain_area);

                /**
                * \brief visit-function used by findPath()
                * @param k counter for the sequence-vector
                * @param adj_matrix adjacence-matrix representing the molecule-graph
                * @param val sequence, in which the atoms are visited
                * @param id indicates, at which point in the sequence a certain atom has been visited
                * @param chain_area the chain area whos chains are to be found
                * @param end Index of the second edge - atom in the chain-area-vector
                * @param breaker flag, indicating, whether athe second edge has been reached
                * @param t starting point for the traceback
                * @param prev_nodes traceback-vector
                */
                void visit_chains(Size& k,  vector<vector<Size> >& adj_matrix, vector<int>& val, Size& id, vector<Atom*>& chain_area, Size& end, bool& breaker, Size& t, vector<Atom*>& prev_nodes);

                /**
                * \brief breadth-first-search, finding the shortest path between two edges of a chain-area
                * @param first_edge the first of the two edges
                * @param second_edge the second of the two edges
                * @param chain_area the chain area that is to be searched
                * @param adj_matrix adjacence-matrix representing the molecule-graph
                * @return the shortest path between first_edge and second_edge
                */
                vector<Atom*> findPath(Atom*& first_edge, Atom*& second_edge,vector<Atom*>& chain_area, vector<vector<Size> >& adj_matrix);

                /**
                * \brief finds all relevant chains in one chain-area
                * @param chain_area the chain area that is to be searched
                * @param adj_matrix adjacence-matrix representing the molecule-graph
                * @return all relevant chains in chain_area
                */
                vector<vector<Atom*> > findChains(vector<Atom*>& chain_area, vector<vector<Size> >& adj_matrix);

                /**
                * \brief helper-function to determine an Atom's index in a certain vector<Atom*>
                * @param atom the Atom
                * @param container the vector<Atom*>
                * @return the Atom's index
                */
                Size getIndex(Atom*& atom, vector<Atom*>& container);

        };

} // namespace BALL

#endif // BALL_STRUCTURE_CHAINBUILDER_H
