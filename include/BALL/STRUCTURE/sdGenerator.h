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

#ifndef BALL_KERNEL_PDBATOM_H
# include <BALL/KERNEL/PDBAtom.h>
#endif

#include <vector>
#include <queue>

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
				FIRST_SDGENERATOR_PROPERTY = PDBAtom::NUMBER_OF_PROPERTIES+1,
				IN_RING,
				CORE_CHAIN,
				DEPOSITED,
				FXAS,
				EQAS,
				HEAD,
				EDGE,
				ASSEMBLED,
				ZIG,
				ZAG,
				INITIALIZED_HEAD_CFS,
				LAST_SDGENERATOR_PROPERTY
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
			
			/** A nested class for our atom priorization
			 */
			class AtomComparator
			{
				public:
					bool operator() (Atom const* first, Atom const* second) const
					{
						Size first_value  =  first->getProperty("SDGenerator::PRIORITY").getUnsignedInt();
						Size second_value = second->getProperty("SDGenerator::PRIORITY").getUnsignedInt();

						return first_value < second_value;
					}
			};

      /**
      * \brief Distinguishes between ring-atoms and core-chain-atoms, removes all H-Atoms from the System
      * @param molecule_sys
      */
      void prepare_();

			/**
			* \brief Constructs a ringsystem, providing the atoms with relative 2D-coordinates, starting in the point of origin
			* @param current_ring_system consecutive numbering of the molecule's ringsystems
			*/
			void constructRingSystem_(Position current_ring_system);

			// Obtain the CFS from the properties of the atom...
			Angle getCFS_(Atom const* atom, bool hi);

			// Convert a vector into the corresponding CFS angle
			Angle computeCFS_(Vector3 const& input);

			// store the CFS for an atom
			void setCFS_(Atom* atom, Angle cfs, bool high);

			// push the CFS before it is overwritten
			void pushCFS_(Atom* atom);

			// retrieve backup CVS values
			Angle getBackupCFS_(Atom const*, bool hi);

			//  Compute the CFS values for a full regular polygon
			void computeCoreCFS_(RingAnalyser::Ring& ring, bool clockwise);

			//
			Angle computeAngularSeparation_(Atom* seed);

			//
			Angle computeAngularDemand_(Atom* seed);

			//
			std::vector<Atom*> sequenceSubstituents_(Atom* seed);

			/** Compute the Shelley priority values for each atom
			 */
			void computeShelleyPriorities_();

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

			/**
			 * \brief cluster and arrange all chains in the system
			 */
			void treatChains_();

			//
			void smoothCFSAngle_(Atom* seed);

			//
			void placeSubstituent_(Atom* seed, Atom* head, Atom* next);

			//
			void depositPFU_(Atom* seed_atom, Atom* next_neighbour);

			//
			void checkOverlap_(Atom* atom);

			//Check whether the ring is clockwise or not
			bool ringIsClockwise_(const RingAnalyser::Ring& ring, Index start_index = 0) const;

			/**
			 * Assemble the final structure diagram
			 */
			void assembleSD_();

			// The backtracking for our Floyd-Warshall implementation
			void findFloydWarshallPath_(std::vector<int>& path, std::vector<Index>& next, Size remaining_atoms, Position i, Position j, std::list<Index>& output);

			/// The ring analyser containing all information about ring systems
			RingAnalyser ring_analyser_;

			/// all chains
			std::list<std::list<Atom*> > chains_;

			/// our redraw queue
			std::priority_queue<Atom*, std::vector<Atom*>, AtomComparator> redraw_queue_;

			/// the system we are working on
			System* system_;
	};

} // namepspace BALL

#endif
