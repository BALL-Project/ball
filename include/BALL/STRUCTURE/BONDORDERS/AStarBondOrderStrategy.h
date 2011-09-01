#ifndef BALL_STRUCTURE_BONDORDERS_ASTARBONDORDERSTRATEGY_H
#define BALL_STRUCTURE_BONDORDERS_ASTARBONDORDERSTRATEGY_H

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_PARTIALBONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/partialBondOrderAssignment.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#include <queue>

namespace BALL
{
	/** AStar algorithm for bond order assignment.
	 *  
	 *  This class implements an AStar approach for the bond order assignment
	 *  problem that can be used by the \link AssignBondOrderProcessor
	 *  AssignBondOrderProcessor \endlink.
	 */
	class AStarBondOrderStrategy
		: public BondOrderAssignmentStrategy
	{
		public:
			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/**	heuristic defining the tightness of the search critria 
				 * @see Option::Heuristic::SIMPLE
				 * @see Option::Heuristic::MEDIUM
				 * @see Option::Heuristic::TIGHT
				*/
				static const char* HEURISTIC;
			};

			struct BALL_EXPORT Default
			{
				static const String HEURISTIC;
			};

			struct BALL_EXPORT Heuristic
			{
				static const String SIMPLE;
				static const String MEDIUM;
				static const String TIGHT;
			};
			//@}

			AStarBondOrderStrategy(AssignBondOrderProcessor* parent);

			virtual ~AStarBondOrderStrategy();

			virtual void clear();
			virtual void init();

			virtual bool readOptions(const Options& options);
			virtual void setDefaultOptions();

			virtual boost::shared_ptr<BondOrderAssignment> computeNextSolution();

		protected:



			/// The chosen heuristic
			PartialBondOrderAssignment::HEURISTIC_INDEX heuristic_index_;

			std::priority_queue<PartialBondOrderAssignment> queue_;

			/// The priority queue. 
			// The current number of node expansions. 
			// step_ + queue_.size() gives the number of touched nodes.
			int step_;
	};

}
#endif // BALL_STRUCTURE_BONDORDERS_ASTARBONDORDERSTRATEGY_H
