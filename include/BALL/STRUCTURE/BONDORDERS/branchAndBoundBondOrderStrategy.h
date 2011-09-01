#ifndef BALL_STRUCTURE_BONDORDERS_BRANCHANDBOUNDBONDORDERSTRATEGY_H
#define BALL_STRUCTURE_BONDORDERS_BRANCHANDBOUNDBONDORDERSTRATEGY_H

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignmentStrategy.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#endif

#ifndef BALL_STRUCTURE_BONDORDERS_KGREEDYBONDORDERSTRATEGY_H
# include <BALL/STRUCTURE/BONDORDERS/KGreedyBondOrderStrategy.h>
#endif

namespace BALL
{
	/** Branch-and-bound algorithm for bond order assignment.
	 *  
	 *  This class implements a (possibly sub-optimal) branch and bound approach
	 *  for the bond order assignment problem that can be used by the 
	 *  \link AssignBondOrderProcessor AssignBondOrderProcessor \endlink. 
	 *  The implementation of this strategy
	 *  is very similar to the KGreedy approach and much of its functionality
	 *  can be reused. We thus derive from \link KGreedyBondOrderStrategy
	 *  KGreedyBondOrderStrategy \endlink.
	 */
	class BranchAndBoundBondOrderStrategy
		: public KGreedyBondOrderStrategy
	{
		public:
			/** @name Constant Definitions
			*/
			//@{
			/// Option names
			struct BALL_EXPORT Option
			{
				/** the percentage cutoff for keeping PQ-Entries in the branch and bound algorithm.
				 * Default is 1.2.
				 * @see Option::AlgorithmBRANCH_AND_BOUND;
				 * \par
				 *   <b>NOTE:</b> This option is still experimental.
				 */
				static const char* BRANCH_AND_BOUND_CUTOFF;
			};

			struct BALL_EXPORT Default
			{
				static const float BRANCH_AND_BOUND_CUTOFF;
			};
			//@}

			BranchAndBoundBondOrderStrategy(AssignBondOrderProcessor* parent);
			virtual ~BranchAndBoundBondOrderStrategy();

			virtual void clear();
			virtual void init();

			virtual bool readOptions(const Options& options);
			virtual void setDefaultOptions();

			virtual boost::shared_ptr<BondOrderAssignment> computeNextSolution();

		protected:
	};
}

#endif // BALL_STRUCTURE_BONDORDERS_KGREEDYBONDORDERSTRATEGY_H
