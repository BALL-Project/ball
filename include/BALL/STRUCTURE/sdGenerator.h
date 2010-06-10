// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_SDGENERATOR_H
#define BALL_STRUCTURE_SDGENERATOR_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_STRUCTURE_RINGANALYSER_H
# include <BALL/STRUCTURE/ringAnalyser.h>
#endif

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
				STRAIGHT,
				NEGATIVE_ANGLE,
				CLOCKWISE
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

				/** the standard bond length for structure
				 *  diagrams
				 */
				static const char* STANDARD_BOND_LENGTH;
			};

			/// Default values for options
			struct BALL_EXPORT Default
			{
				static const bool SHOW_HYDROGENS;

				static const float STANDARD_BOND_LENGTH;
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
			 * Clear all internal data structures.
			 */
			void clear();

  	protected:

      /**
      * \brief Distinguishes between ring-atoms and core-chain-atoms, removes all H-Atoms from the System
      * @param molecule_sys
      */
      void prepare_(System& molecule_sys);

			/**
			* \brief Constructs a ringsystem, providing the atoms with relative 2D-coordinates, starting in the point of origin
			* @param current_ring_system consecutive numbering of the molecule's ringsystems
			*/
			void constructRingSystem_(Position current_ring_system);

			/** Build a regular polygon for a ring with two fixed points.
			 */
			void buildRegularPolygon_(RingAnalyser::Ring& ring, Position first_anchor_index, bool clockwise);

			/** Build an open polygon for a ring with two fixed points.
			 */
			void buildOpenPolygon_(RingAnalyser::Ring& ring, Position first_anchor_index, Position second_anchor_index);

			/**
			* \brief construct the core-ring as a regular polygon
			* @param current_ring the index of the ring to attach
			* @param current_system the ring system
			* @param x_start the ring is created in (x_start, 0, 0)
			*/
			void attachCore_(Position current_ring, std::vector<RingAnalyser::Ring>& current_system, float x_start);

			/**
			* \brief attach a ring template to a (partially) constructed ringsystem (no functionality yet)
			* @param current_ring the index of the ring to attach
			* @param current_system the ring system
			*/
			void attachTemplate_(Position current_ring, std::vector<RingAnalyser::Ring>& current_system);

			/**
			 * \brief attach a fused ring to a (partially) constructed ringsystem
			 * @param current_ring the index of the ring to attach
			 * @param current_system the index of the ring system
			 */
			void attachFused_(Position current_ring, std::vector<RingAnalyser::Ring>& current_system);

			/**
			 * \brief attach a bridged ring to a (partially) constructed ringsystem
			 * @param current_ring the index of the ring to attach
			 * @param current_system the index of the ring system
			 */
			void attachBridged_(Position current_ring, std::vector<RingAnalyser::Ring>& current_system);

			/**
 			 * \brief attach a spiro ring to a (partially) constructed ringsystem
			 * @param current_ring the index of the ring to attach
			 * @param current_system the index of the ring system
			 */
			void attachSpiro_(Position current_ring, std::vector<RingAnalyser::Ring>& current_system);

			/// Compute adjacency matrix of the given atoms
			void computeAdjacencyMatrix_(std::vector<Atom*>& chain, std::vector<bool>& result);

			/// Comparator for chains of atoms
			static bool compareChains_(const vector<Atom*>& x, const vector<Atom*>& y);

			/// recursively determine chain areas
			void visitChainAreas_(Size k, std::vector<bool>& adj_matrix, std::vector<int>& val, Size nodes, 
			                      Size id, std::vector<Atom*>& core_chain_atoms, std::vector<Atom*>& chain_area);

			///
			void visitChains_(Size& k, std::vector<bool>& adj_matrix, std::vector<int>& val, Size& id, 
			                  std::vector<Atom*>& chain_area, Size& end, bool& breaker, Size& t, std::vector<Atom*>& prev_nodes);

			/// 
			std::vector<Atom*> findPath_(Atom*& first_edge, Atom*& second_edge, std::vector<Atom*>& chain_area, 
			                             std::vector<bool>& adj_matrix);

			/**
			 * \brief cluster and arrange all chains in the system
			 */
			void treatChains_(AtomContainer& ac);

			/// The ring analyser containing all information about ring systems
			RingAnalyser ring_analyser_;

			/// all chains
			std::vector<std::vector<Atom*> > chains_;
	};

} // namepspace BALL

#endif
