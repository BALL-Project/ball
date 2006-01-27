// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_STRUCTURE_DOCKING_AMBEREVALUATION_H
#define BALL_STRUCTURE_DOCKING_AMBEREVALUATION_H

#ifndef BALL_STRUCTURE_FORCEFIELDEVALUATION_H
# include <BALL/STRUCTURE/DOCKING/forceFieldEvaluation.h>
#endif

#ifndef BALL_MOLMEC_AMBER_AMBER_H
# include <BALL/MOLMEC/AMBER/amber.h>
#endif

namespace BALL
{
		/** A class for energetic evaluators of docking results using
		 *  an AMBER force field as scoring function.
		 */
		class BALL_EXPORT AmberEvaluation : public ForceFieldEvaluation
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
