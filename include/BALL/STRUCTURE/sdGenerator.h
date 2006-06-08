// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sdGenerator.h,v 1.2 2006/06/08 07:30:25 oliver Exp $
//
// Author:
//   Holger Franken
//


#ifndef BALL_STRUCTURE_SDGENERATOR_H
#define BALL_STRUCTURE_SDGENERATOR_H


#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/STRUCTURE/ringClusterer.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/rsConstructor.h>
#include <BALL/STRUCTURE/chainBuilder.h>
#include <BALL/STRUCTURE/moleculeAssembler.h>


namespace BALL
{


        /**
                * \brief The main class, provides methods for analysis of the input.
                *
                */
        class SDGenerator
        {

        private:

                /**
                * \brief Distinguishes between ring-atoms and core-chain-atoms, removes all H-Atoms from the System
                * @param molecule_sys
                */
                void checkAtoms(System& molecule_sys);

                /**
                * \brief Determines the smallest set of smallest Rings of the input System
                * @param molecule_sys
                * @return the smallest set of smallest Rings
                */
                vector <vector<Atom*> > getSSSR(System& molecule_sys);


        public:
                /**
                 *
                 * \brief Default-Constructor
                 */
                SDGenerator();

                /**
                 *
                 * \brief Destructor
                 */
                ~SDGenerator();

                /**
                * \brief Generates a structure Diagram from the input System
                * @param molecule_sys
                */
                void generateSD(System& molecule_sys);

                /**
                * \brief finds an atom's neighbours inside a certain ring
                * \param ring   the ring that shall be searched for the neighbours
                * \param atom   the atom, whos neighbours shall be found
                * @return atom's neighbours inside of ring
                */
                pair<Atom*, Atom*> getNeighbours(vector<Atom*>& ring, Atom*& atom);

                /**
                * \brief Puts the Atoms in each ring of the input ringsystem into the correct order
                * @param ringsystem
                * @return the ringsystem with sorted atoms
                */
                vector<vector<Atom*> > sequenceRings(vector<vector<Atom*> >& ringsystem);
                
								/**
                * \brief Properties, used to describe atoms and their status
                */
								enum Property 
								{
												in_ring,
												pre_core_chain,
												core_chain,
												found,
												invalid,
												deposited,
												firstneighbour,
												sequenced,
 												FxAS,
												edge,
												built_in_chain,
												assembled,
												shifted,
												pre_assembled,
												rotated,
												zig,
												zag,
												straight
								};

        };

} // namepspace BALL

#endif
