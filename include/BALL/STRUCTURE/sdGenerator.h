// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
	 * Structure Diagram Generation.
	 *	
	 * This class provides methods for analysis of the input.
	 */
  class BALL_EXPORT SDGenerator
  {
    public:
		
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
			
			/** @name Constructors and Destructors.
			 */
			//@{

			/**
			 * Default-Constructor
			 */
			SDGenerator(bool show_hydrogens = false);

			/**
			 * Destructor
			 */
			virtual ~SDGenerator();
			//@}

			/** @name Public Attributes
			*/
			//@{
			/// options
			Options options;
			//@}

			/** @name Accessors
			 */
			//@{
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
      std::pair<Atom*, Atom*> getNeighbours(std::vector<Atom*>& ring, Atom* atom);

  	protected:

      /**
      * \brief Puts the Atoms in each ring of the input ringsystem into the correct order
      * @param ringsystem
      * @return the ringsystem with sorted atoms
      */
      void sequenceRings_(std::vector<std::vector<Atom*> >& ringsystem, std::vector<std::vector<Atom*> >& sequenced_rings);
      
      /**
      * \brief Distinguishes between ring-atoms and core-chain-atoms, removes all H-Atoms from the System
      * @param molecule_sys
      */
      void prepare_(System& molecule_sys);

			/**
			 *  The smallest set of smallest rings.
			 */
			std::vector<std::vector<Atom*> > sssr_;

	};

} // namepspace BALL

#endif
