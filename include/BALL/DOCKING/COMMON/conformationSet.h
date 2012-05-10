// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conformationSet.h,v 1.3.18.2 2007/06/17 07:02:21 oliver Exp $
//
// Author:
//

#ifndef BALL_DOCKING_COMMON_CONFORMATIONSET_H
#define BALL_DOCKING_COMMON_CONFORMATIONSET_H

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

#include <vector>
#include <set>

namespace BALL
{
		/** Data structure for conformation results.
				\ingroup Docking
		*/
		class BALL_EXPORT ConformationSet
		{
		  public:

				/** The conformations are scored as indices into
				 * 	the vector of snapshots with a score
				 */
				typedef std::pair < Index, float > Conformation;

				ConformationSet() {};
				ConformationSet(const System& system);

				virtual ~ConformationSet() {};

				/** Operations
				*/
				void setup(const System& system)
					throw();

				void add(const float score, const System& conformation)
					throw();

				const System& getSystem() const
					throw();

				System& getSystem()
					throw();

				/** Use score as the new scoring information.
				 */
				void setScoring(std::vector < Conformation > & score)
					throw();

				/** Return the current scoring.
				 */
				const std::vector < Conformation > & getScoring() const
					throw();

				/** Reset the scoring vector to the identity permutation with
				 * 	identical scores of 0.
				 */
				void resetScoring()
					throw();

				/** Returns the _unranked_ list of conformations, i.e. the conformations
				 *  in the order in which they were added.
				 */
				const std::vector < SnapShot > & getUnscoredConformations() const
					throw();

				/** Returns the i-th snapshot using the current scoring information.
				 * 	TODO: Exception handling
				 */
				const SnapShot& operator [] (const Index pos) const;

				/** Export the first num results in the order of the current scoring as dcd file
				 *  with name filename. If num == 0, all results are exported.
				 */
				bool writeDCDFile(const String& filename, const Size num = 0);

				bool readDCDFile(const String& filename)
					throw();

				/** Return the number of conformations.
				 */
				Size size() const;

			protected:

				/** Attributes
				*/

				// Indices of the conformations in the SnapShotManager.
				// Sorted according to their score
				std::vector < Conformation > snapshot_order_;

				/** The original system on which the SnapShots are based
				*/
				System system_;

				/** The SnapShots
				*/
				vector<SnapShot> structures_;
		};

}
#endif
