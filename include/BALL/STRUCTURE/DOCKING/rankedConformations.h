#ifndef BALL_STRUCTURE_DOCKING_RANKEDCONFORMATION_H
#define BALL_STRUCTURE_DOCKING_RANKEDCONFORMATION_H

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOTMANAGER_H
# include <BALL/MOLMEC/COMMON/snapShotManager.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
# include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_FORMAT_DCDFILE_H
# include <BALL/FORMAT/DCDFile.h>
#endif

#include <set>

namespace BALL
{

		/** A class defining the interface for docking algorithms.
				This is a first iteration.
		*/
		class RankedConformations
		{

		  public:

				/** Nested class for conformations with a rank.
				 */
				class Conformation
				{
					public:
						Conformation()
							throw()
						{};
						
						Conformation(Index new_index, float new_score)
							: index(new_index),
								score(new_score)
						{};
						
						bool operator < (const Conformation& c) const
							throw()
						{
							return score < c.score;
						}

						Index index;
						float score;
				};
						
				RankedConformations() {};
				RankedConformations(const System& system, const String& filename = "docking.dcd");

				virtual ~RankedConformations() {};
				
				/** Operations
				*/
				void setup(const System& system, const String& filename = "docking.dcd")
					throw();

				void add(float score, const  System& conformations) 
					throw();

				const System& getSystem() const
					throw();

				void closeTrajectoryFile()
					throw();

			protected:

				/** Attributes
				*/

				// Indices of the conformations in the SnapShotManager.
				// Sorted according to their score
				std::multiset<Conformation> snapshot_order_;

				/** The original system on which the SnapShotManager is based
				*/	
				System system_;

				/** We need a force field for the SnapShotManager, but we won't use it.
				 */
				ForceField ff_;

				/** The DCD file in which the snapshots will be stored
				 */
				DCDFile dcdfile_;

				/** The SnapShots
				*/
				SnapShotManager structures_;
		};

}
#endif
