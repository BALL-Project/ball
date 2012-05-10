// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig $
// ----------------------------------------------------

#ifndef BALL_DOCKING_COMMON_RECEPTOR_H
#define BALL_DOCKING_COMMON_RECEPTOR_H

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/DATATYPE/hashSet.h>

#include <BALL/DOCKING/COMMON/conformation.h>
#include <BALL/DOCKING/COMMON/flexibleMolecule.h>
#include <BALL/DOCKING/COMMON/flexDefinition.h>

namespace BALL
{
	/**     Receptor class.
				  This class represents protein receptors.
	 */
	class BALL_EXPORT Receptor : public FlexibleMolecule
	{
		public:
			Receptor();
			Receptor(Protein* p);
			virtual ~Receptor();
			FlexDefinition getFlexDefinition(Position i);
			void setFlexDefinition(Position i, FlexDefinition& fd);
			vector<FlexDefinition> getFlexDefinitions();

			/** Add a conformation with flexible side chains
					@param conformation pointer
					@param FlexDefinition
			 */
			void addFlexConformation(Conformation* conf, FlexDefinition &fd);

			/** Add a conformation
					@param conformation pointer
			 */
			void addConformation(Conformation* conf);

		protected:
			vector<FlexDefinition> flexinfo_;
	};
}
#endif /* BALL_DOCKING_COMMON_RECEPTOR_H */
