// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
// $Id: amberEvaluation.h,v 1.3.18.2 2007/06/17 07:02:21 oliver Exp $
//
// Author:
//   
//

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
		    an AMBER force field as scoring function.
				\ingroup Docking
		 */
		class BALL_EXPORT AmberEvaluation : public ForceFieldEvaluation
		{
			public:

				// Default constructor.
				AmberEvaluation()
					;
				
				AmberEvaluation(AmberFF& ff)
					;

				AmberEvaluation(AmberFF& ff, Options& option)
					;

				virtual ~AmberEvaluation()
					;
				
				/** Operations
				*/
				void setForceField(ForceField& ff)
					;

				void setOptions(const Options& options)
					;

				ForceField& getForceField()
					;

				const ForceField& getForceField() const
					;

				Options& getOptions()
					;

				const Options& getOptions() const
					;
				
		};
}
#endif
