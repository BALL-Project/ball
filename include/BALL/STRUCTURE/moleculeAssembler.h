// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_MOLECULEASSEMBLER_H
#define BALL_STRUCTURE_MOLECULEASSEMBLER_H

#include <BALL/STRUCTURE/sdGenerator.h>

namespace BALL
{

        /**
                * \brief class, providing methods for the assembly of a structure diagram
                *
                */
        class BALL_EXPORT MoleculeAssembler
        {

        public:

                /**
                * \brief Default-Constructor
                */
                MoleculeAssembler();

                /**
                * \brief Destructor
                */
                ~MoleculeAssembler();

                /**
                * \brief        assembles the structure diagram from previously prepared fragments
                * @param molecule_sys System of the input molecule
                * @param ringsystems the prefabricated ringsystems
                * @param chains the chains of the molecule
                */
                void assembleMolecule(System& molecule_sys, vector<vector<vector<Atom*> > >& ringsystems, vector<vector<Atom*> >& chains);
								
								/**
                * \brief        comparator for sorting the ringsystems by their size
								* @param x ringsystem
								* @param y ringsystem
                */
								static bool compareRingsystems(const vector<vector<Atom*> >& x, const vector<vector<Atom*> >& y);

        private:
                list<Atom*> aq_;

                /**
                * \brief adds a prefabricated ringsystem to the structure diagram
                * @param start_atom the already positioned neighbour-atom of the ringsystem
                * @param assemble_atom the atom, that belongs to the ringsystem and is a neighbour of start_atom
                * @param ringsystems all prefabricated ringsystems of the molecule
                */
                void assembleRS(Atom*& start_atom, Atom*& assemble_atom, vector<vector<vector<Atom*> > >& ringsystems);


                /**
                * \brief adds a chain to the structure diagram
                * @param start_atom the already positioned neighbour-atom of the chain
                * @param assemble_atom the atom, that belongs to the chain and is a neighbour of start_atom
                * @param chains all chains of the molecule
                */
                void assembleChain(Atom*& start_atom, Atom*& assemble_atom, vector<vector<Atom*> >& chains);

                /**
                * \brief adds single atoms to the structure diagram
                * @param start_atom the already positioned neighbour-atom of the single atoms
                * @param molecule_sys System of the input molecule
                */
                void assembleSubstituents(Atom*& start_atom, System& molecule_sys);

                /**
                * \brief determines the free space, which is left around an atom to assemble further substituents
                * @param atom
                */
                void updateCFS(Atom*& atom);

        };

} // namespace BALL

#endif // BALL_STRUCTURE_MOLECULEASSEMBLER_H

