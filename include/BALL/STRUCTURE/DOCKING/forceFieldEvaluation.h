#ifndef FORCEFIELDEVALUATION_H
#define FORCEFIELDEVALUATION_H

#include <BALL/STRUCTURE/DOCKING/energeticEvaluation.h>

#include <BALL/MOLMEC/COMMON/forceField.h>

namespace BALL
{
		/** A basis class for energetic evaluators of docking results using
		 *  a force field as scoring function.
		 */
		class ForceFieldEvaluation : public EnergeticEvaluation
		{
			public:

				// Default constructor.
				ForceFieldEvaluation()
					throw();
				
				virtual ~ForceFieldEvaluation()
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
				
				std::vector<ConformationSet::Conformation> operator () (const ConformationSet& conformations)
					throw();

			protected:

				ForceField* ff_;
				Options     options_;
				bool 				delete_force_field_;
		};
}
#endif
