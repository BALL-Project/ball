#ifndef BALL_STRUCTURE_DOCKING_RANKEDCONFORMATION_H
#define BALL_STRUCTURE_DOCKING_RANKEDCONFORMATION_H

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
# include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

#ifndef BALL_FORMAT_DCDFILE_H
# include <BALL/FORMAT/DCDFile.h>
#endif

#include <set>
#include <vector>

namespace BALL
{

		/** A class defining the interface for docking algorithms.
				This is a first iteration.
		*/
		class ConformationSet
		{
		  public:

				/** The conformations are scored as indices into 
				 * 	the vector of snapshots with a score
				 */
				typedef std::pair<Index, float> Conformation;

				ConformationSet() {};
				ConformationSet(const System& system);

				virtual ~ConformationSet() {};
				
				/** Operations
				*/
				void setup(const System& system)
					throw();

				void add(const float score, const System& conformations) 
					throw();

				const System& getSystem() const
					throw();

				/** Use score as the new scoring information.
				 */
				void setScoring(std::vector<Conformation>& score)
					throw();
				
				/** Return the current scoring.
				 */
				const std::vector<Conformation>& getScoring() const
					throw();

				/** Reset the scoring vector to the identity permutation with
				 * 	identical scores of 0.
				 */
				void resetScoring()
					throw();

				/** Returns the _unranked_ list of conformations, i.e. the conformations
				 *  in the order in which they were added.
				 */
				const std::vector<SnapShot>& getUnscoredConformations() const
					throw();

				/** Returns the i-th snapshot using the current scoring information.
				 * 	TODO: Exception handling
				 */
				const SnapShot& operator [] (const Index pos) const;

				/** Export the first num results in the order of the current scoring as dcd file
				 *  with name filename. If num == 0, all results are exported.
				 */
				void writeDCDFile(const String& filename, const Size num=0);
				
			protected:

				/** Attributes
				*/

				// Indices of the conformations in the SnapShotManager.
				// Sorted according to their score
				std::vector<Conformation> snapshot_order_;

				/** The original system on which the SnapShots are based
				*/	
				System system_;

				/** The SnapShots
				*/
				vector<SnapShot> structures_;
		};

}
#endif
