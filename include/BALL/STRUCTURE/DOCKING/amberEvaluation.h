#ifndef AMBEREVALUATION_H
#define AMBEREVALUATION_H

#include <BALL/STRUCTURE/DOCKING/forceFieldEvaluation.h>
#include <BALL/MOLMEC/AMBER/amber.h>

namespace BALL
{
		/** A class for energetic evaluators of docking results using
		 *  an AMBER force field as scoring function.
		 */
		class AmberEvaluation : public ForceFieldEvaluation
		{
			public:

				// Default constructor.
				AmberEvaluation()
					throw();
				
				AmberEvaluation(AmberFF& ff)
					throw();

				AmberEvaluation(AmberFF& ff, Options& option)
					throw();

				virtual ~AmberEvaluation()
					throw();
				
				/** Operations
				*/
				void setForceField(ForceField& ff)
					throw();

				void setOptions(const Options& options)
					throw();

				ForceField& getForceField()
					throw();

				const ForceField& getForceField() const
					throw();

				Options& getOptions()
					throw();

				const Options& getOptions() const
					throw();
				
		};
}
#endif
