// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RINGANALYSER_H
#define BALL_STRUCTURE_RINGANALYSER_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
# include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
# include <BALL/CONCEPT/property.h>
#endif

#include <vector>
#include <set>
#include <deque>

namespace BALL
{
	class Atom;
	class Bond;

	/**
	 * \brief Methods for the analysis of the structure of ringsystems
	 */
	class BALL_EXPORT RingAnalyser
		: public UnaryProcessor<AtomContainer>
	{
		public:

			enum RingType 
			{
				NONE,
				TEMPLATE,
				FUSED,
				BRIDGED,
				SPIRO,
				CORE
			};

			class BALL_EXPORT Ring
				: public PropertyManager
			{
				public:
					/// Default constructor
					Ring();

					/// Detailed constructor
					Ring(std::vector<Atom*> const& atoms_);

					/// The predecessor of atom i in the ring
					Position predecessor(Position i) const;

					/// The successor of atom i in the ring
					Position successor(Position i) const;

					/// the atoms of this ring
					std::vector<Atom*> atoms;

					/// the type of this ring
					RingType type;

					/// the bonds shared with other rings
					std::set<Bond const*> shared_bonds;
			};

			/**	@name	Constructors and destructors. */
			//@{

			/**
			 * \brief Default-Constructor
			 */
			RingAnalyser();

			/**
			 * \brief Destructor
			 */
			virtual ~RingAnalyser();
			//@}

			/**	@name Processor-related methods */
			//@{

			/// Processor method which is called before the operator()-call.
			virtual bool start();
	
			/** Operator () for the processor 
			 *
			 * 	@param  ac  the AtomContainer to which the processor is applied.
			 */
			virtual Processor::Result operator ()(AtomContainer& ac);

			/// Processor method which is called after the operator()-call.
			virtual bool finish();

			//@}

			/** Clears the datastructures.
			* 	<b>NOTE:</b> The options remain!
			* 	Use setDefaultOptions() to clear the options.
			*/
			void clear();

			/**
			 * \brief helper-function; determines, whether an atom is part of a certain ring
			 * @param atom the atom
			 * @param ring the ring
			 * @return true, if the atom is part of the ring, otherwise returns false
			 */
			bool isInRing(const Atom* atom, std::vector<Atom*> const& ring) const;

			/**
			 * \brief helper-function; determines, whether an atom is part of a certain ring
			 * @param atom the atom
			 * @param i the index of the ring system to test
			 * @return true, if the atom is part of the ring system, otherwise returns false
			 */
			bool isInRingSystem(const Atom* atom, Index i) const;

      /**
      * Puts the Atoms in the input ring into the correct order
			* 
			* After calling this function, for i, i+1 it holds that atom i is bonded 
			* to atom i+1, and that the last is bonded to the first entry
      * @param ring the input ring to sequence
      */
      void sequenceRing(std::vector<Atom*>& ring);

		  /** Return the number of ring systems.
			 */
			Size getNumberOfRingSystems();

			/** Returns the i-th ring system.
			 */
			std::vector<Ring> getRingSystem(Position i);

			/** Returns the peeling order for the i-th ring system.
			 */
			std::vector<Position> getPeelingOrder(Position i);

		protected:

			/**
			 * \brief performs an agglomerative clustering of single rings to ringsystems
			 */
			void clusterRings_();

			/** Find the bonds shared between rings in the sssr
			 *  Precondition: the rings must be sorted such that a_i, a_i+1 share a bond (and a_size-1, a_0)
			 */
			void findSharedBonds_();

			/**
			 * \brief performs an analysis of the structure of a ringsystem
			 */
			void peelRings_();

			///
			bool isCentralRing_(std::list<Position>::iterator ring, std::list<Position>& unassigned_rings);

			/**
			 *
			 */
			bool peelNextRing_(std::list<Position>& unassigned_rings, bool peel_fused = false);

			/**
			 * \brief interface for a Ring Template Database (RTD), no functionality yet
			 * @return always returns false by now
			 */
			bool assignRTD_(std::list<Position>& trial_system);

			/// the smallest set of smallest rings of our molecule
			std::vector<Ring> rings_;

			/// the ring systems, stored as indices into the smallest set of smallest rings
			std::vector<std::vector<Position> > ring_systems_;

			/// the ring memberships per bond
			HashMap<Bond const*, std::deque<Position> > rings_per_bond_;

			/// the order in which rings are peeled from their respective ring systems
			std::vector<std::vector<Position> > peeling_order_;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_RINGANALYSER_H
