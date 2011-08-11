#ifndef BALL_STRUCTURE_BONDORDERS_PARTIALBONDORDERASSIGNMENT_H
#define BALL_STRUCTURE_BONDORDERS_PARTIALBONDORDERASSIGNMENT_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
# include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
# include <BALL/KERNEL/bond.h>
#endif

#include <boost/shared_ptr.hpp>
#include <vector>

namespace BALL
{
	class AssignBondOrderProcessor;
	class BondOrderAssignment;

	/** \brief A full or partial solution to the AStar-based bond order assignment problem.
	 *  
	 *  This class represents a full or partial bond order assignment. It is a very basic representation
	 *  without any convenience functionality. This class exists because sometimes we need a
	 *  representation that is as small as possible - we need many of these during the AStar 
	 *  runs, e.g., and memory quickly becomes an issue. Please note that we explicitly avoid 
	 *  virtual functions here to save the vtable - inheriting from this class makes no sense, anyhow.
	 *  
	 */
	class BALL_EXPORT PartialBondOrderAssignment
	{
		public:
			// this enum allows faster access to the type of the chosen heuristic than a string compare
			enum HEURISTIC_INDEX
			{
				SIMPLE,
				MEDIUM,
				TIGHT
			};

			// Constructor
			PartialBondOrderAssignment(AssignBondOrderProcessor* parent);

			// Destructor
			~PartialBondOrderAssignment();

			boost::shared_ptr<BondOrderAssignment> convertToFullAssignment();

			// 
			void clear();

			// the less operator.
			// NOTE: we want a reverse sort, hence we actually return a "greater" 
			bool operator < (const PartialBondOrderAssignment& b) const;

			// the penalty
			float coarsePenalty(float atom_type_penalty, float bond_length_penalty) const;

			// the combined penalty of structure and type penalty
			float coarsePenalty() const;

			// the bond length penalty 
			float finePenalty() const {return estimated_bond_length_penalty;}

			/// Convenience function to obtain total atom type penalty value
			float getAtomTypePenalty(bool include_heuristic_term = true, HEURISTIC_INDEX heuristic_index = SIMPLE);

			/** Estimates the objective function f = g* + h* of the ASTAR - algorithm, if
			 *  include_heuristic_term == true, otherwise compute only f = g*. The
			 *  result is stored in the PartialBondOrderAssignment entry's member estimated_atom_type_penalty.
			 *
			 *  @retval bool - true, if the entry is still valid.
			 *  @retval bool - false otherwise.
			 */
			bool estimatePenalty_(bool include_heuristic_term = true, HEURISTIC_INDEX heuristic_index = SIMPLE);

			/// Estimates the atom type penalty for a given unclosed atom.
			float estimateAtomTypePenalty_(Atom* atom,
																		 Index atom_index,    // the atom index
																		 int fixed_valence,   // its so far fixed valence (incl. virtual H's)
																		 int fixed_virtual_order, // its so far fixed virtual H's
																		 int num_free_bonds,  // its number of unfixed original bonds
																		 HEURISTIC_INDEX heuristic_index);

			/// Estimates the bond length penalty for a given unclosed atom.
			//  NOTE: virtual bonds are excluded!
			float estimateBondLengthPenalty_(Index atom_index, // the atom index
																			 const vector<Bond*>& free_bonds,
																			 int fixed_virtual_order,
																			 int fixed_valence,
																			 int num_free_bonds);


			// the estimated atom type penalty
			float estimated_atom_type_penalty;
			// the estimated bond length penalty
			float estimated_bond_length_penalty;

			// the bond orders 
			// the i-th entry denotes the bondorder of the i-th bond
			// unset bonds get the order 0
			vector<short> bond_orders;

			// the index of the bond last considered 
			Position last_bond;

			AssignBondOrderProcessor* abop;
	};
}

#endif // BALL_STRUCTURE_BONDORDERS_PARTIALBONDORDERASSIGNMENT_H
