// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sdGenerator.h,v 1.4.10.2 2007/04/03 13:29:41 bertsch Exp $
//
// Author:
//   Holger Franken
//


#ifndef BALL_STRUCTURE_SDGENERATOR_H
#define BALL_STRUCTURE_SDGENERATOR_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <BALL/common.h>

#include <vector>

namespace BALL
{
	class System;
	class Atom;

	/**
		\brief The main class, provides methods for analysis of the input.
	*/
  class BALL_EXPORT SDGenerator
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
      std::vector<std::vector<Atom*> > getSSSR(System& molecule_sys);


    public:
		
			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{	
				/**	show hydrogen atoms.
				*/
		 		static const char* SHOW_HYDROGENS;
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const bool SHOW_HYDROGENS;
			};
			
			//@}
			

     /**
       *
       * \brief Default-Constructor
       */
      SDGenerator(bool show_hydrogens = false);


      /**
       *
       * \brief Destructor
       */
      virtual ~SDGenerator();

			/** @name Public Attributes
			*/
			//@{
			/// options
			Options options;

			/** Resets the options to default values.
			*/
			void setDefaultOptions();
			//@}

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
      std::pair<Atom*, Atom*> getNeighbours(std::vector<Atom*>& ring, Atom*& atom);

      /**
      * \brief Puts the Atoms in each ring of the input ringsystem into the correct order
      * @param ringsystem
      * @return the ringsystem with sorted atoms
      */
      std::vector<std::vector<Atom*> > sequenceRings(std::vector<std::vector<Atom*> >& ringsystem);
      
		/**
     * \brief Properties, used to describe atoms and their status
     */
		enum Property 
		{
			IN_RING,
			PRE_CORE_CHAIN,
			CORE_CHAIN,
			FOUND,
			INVALID,
			DEPOSITED,
			FIRSTNEIGHBOUR,
			SEQUENCED,
			FXAS,
			EDGE,
			BUILT_IN_CHAIN,
			ASSEMBLED,
			SHIFTED,
			PRE_ASSEMBLED,
			ROTATED,
			ZIG,
			ZAG,
			STRAIGHT
		};

	};

} // namepspace BALL

#endif
