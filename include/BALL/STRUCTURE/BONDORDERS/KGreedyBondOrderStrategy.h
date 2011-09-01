#ifndef BALL_STRUCTURE_BONDORDERS_KGREEDYBONDORDERSTRATEGY_H
#define BALL_STRUCTURE_BONDORDERS_KGREEDYBONDORDERSTRATEGY_H

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_ASTARBONDORDERSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/AStarBondOrderStrategy.h>
#endif

namespace BALL
{
	/** K-Greedy algorithm for bond order assignment.
	 *  
	 *  This class implements a K-Greedy approach for the bond order assignment
	 *  problem that can be used by the \link AssignBondOrderProcessor
	 *  AssignBondOrderProcessor \endlink. The implementation of this strategy
	 *  is very similar to the AStar approach and much of its functionality
	 *  can be reused. We thus derive from \link AStarBondOrderStrategy
	 *  AStarBondOrderStrategy \endlink.
	 */
	class KGreedyBondOrderStrategy
		: public AStarBondOrderStrategy
	{
		public:
			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/** the size of priority queue for the greedy algorithm.
				 * Default is 1. 
				 * @see Option::Algorithm::K_GREEDY;
				 * \par
				 *   <b>NOTE:</b> This option is still experimental.
				 */
				static const char* GREEDY_K_SIZE;
			};

			struct BALL_EXPORT Default
			{
				static const int GREEDY_K_SIZE;
			};
			//@}

			KGreedyBondOrderStrategy(AssignBondOrderProcessor* parent);
			virtual ~KGreedyBondOrderStrategy();

			virtual void clear();
			virtual void init();

			virtual bool readOptions(const Options& options);
			virtual void setDefaultOptions();

			virtual boost::shared_ptr<BondOrderAssignment> computeNextSolution();

		protected:

			float greedy_atom_type_penalty_;
			float greedy_bond_length_penalty_;

			Size greedy_k_;

			vector<PartialBondOrderAssignment> greedy_set_;

			Position current_solution_;

			int greedy_node_expansions_;
	};
}

#endif // BALL_STRUCTURE_BONDORDERS_KGREEDYBONDORDERSTRATEGY_H
