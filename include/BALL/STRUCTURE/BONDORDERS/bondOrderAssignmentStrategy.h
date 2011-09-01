#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
#define BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H

#ifndef BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENT_H
# include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#include <boost/shared_ptr.hpp>

namespace BALL
{
	class AssignBondOrderProcessor;

	/** \brief Base class for bond order assignment algorithms.
	 *  
	 *  This class forms the base of a hierarchy of solution strategies
	 *  for the bond order assignment problem. In itself, the class is not
	 *  terribly useful. Instead, it defines the interface for the algorithms
	 *  used by the \link AssignBondOrderProcessor AssignBondOrderProcessor \endlink
	 *  which follows the strategy pattern.
	 */
	class BALL_EXPORT BondOrderAssignmentStrategy
	{
		public:
			BondOrderAssignmentStrategy(AssignBondOrderProcessor* parent);

			virtual boost::shared_ptr<BondOrderAssignment> computeNextSolution() = 0;
			virtual bool readOptions(const Options& options);
			virtual void setDefaultOptions();
			virtual void clear();
			virtual void init() = 0;
		
			/// Our parent processor
			AssignBondOrderProcessor* abop;

	};
}
#endif // BALL_STRUCTURE_BONDORDERS_BONDORDERASSIGNMENTSTRATEGY_H
