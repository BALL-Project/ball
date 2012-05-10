// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig $
// ----------------------------------------------------

#ifndef BALL_DOCKING_COMMON_FLEXDEFINITION_H
#define BALL_DOCKING_COMMON_FLEXDEFINITION_H

#include <BALL/KERNEL/protein.h>

namespace BALL
{
		/**     FlexDefinition class.
					  This class is used to store flexibility information
					  about a receptor object.
		 */
		class BALL_EXPORT FlexDefinition
		{
			public:

				enum RotamerLibrary
				{
					DUNBRACK = 0,
					LOVELL
				};

				/** Make a rigid FlexDefinition.
				 */
				static FlexDefinition Rigid();

				BALL_CREATE(FlexDefinition)

				/** Constructor.
				 */
				FlexDefinition();

				/** Destructor.
				 */
				virtual	~FlexDefinition();

				/** Add a fully (all-atom) flexible residue.
							@param residue index
				 */
				void addFullyFlexibleResidue(Position idx);

				/** Get all fully-flexible residues.
							@param vector of residue indices
				 */
				vector<Position> getFullyFlexibleResidues() const;

				/** Add a rotamer-flexible residue.
							@param residue index
				 */
				void addRotamerFlexibleResidue(Position idx);

				/** Get all rotamer-flexible residues.
							@param vector of residue indices
				 */
				vector<Position> getRotamerFlexibleResidues() const;

			private:
				vector<Position> fully_flexible_;
				vector<Position> rotamer_flexible_;
		};
}

#endif /* BALL_DOCKING_COMMON_FLEXDEFINITION_H */
