// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#ifndef BALL_SCORING_FUNCTIONS_FORCEFIELDEVALUATION_H
#define BALL_SCORING_FUNCTIONS_FORCEFIELDEVALUATION_H

#ifndef BALL_SCORING_FUNCTIONS_ENERGETICEVALUATION_H
# include <BALL/SCORING/FUNCTIONS/energeticEvaluation.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif

namespace BALL
{
		/** Base class for energetic evaluators of docking results using
		    a force field as scoring function.
				\ingroup Docking
		 */
		class BALL_EXPORT ForceFieldEvaluation
			: public EnergeticEvaluation
		{
			public:

				/// Default constructor.
				ForceFieldEvaluation()
					throw();

				///
				ForceFieldEvaluation(ForceField& ff)
					throw();

				///
				virtual ~ForceFieldEvaluation()
					throw();

				/** Operations
				*/
				void setForceField(ForceField& ff)
					throw();

				///
				void setOptions(const Options& options)
					throw();

				///
				ForceField& getForceField()
					throw();

				///
				const ForceField& getForceField() const
					throw();

				///
				Options& getOptions()
					throw();

				///
				const Options& getOptions() const
					throw();

				///
				virtual std::vector < ConformationSet::Conformation > operator () (ConformationSet& conformations)
					throw(Exception::TooManyErrors);

			protected:

				ForceField* ff_;
				Options     options_;
				bool 				delete_force_field_;
		};
}

#endif // BALL_SCORING_FUNCTIONS_FORCEFIELDEVALUATION_H
