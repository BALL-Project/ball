/* gridAnalysis.h
*
* Copyright (C) 2011 Marcel Schumann
*
* This program free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at
* your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_DOCKING_COMMON_GRIDANALYSIS_H
#define BALL_DOCKING_COMMON_GRIDANALYSIS_H

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <map>

namespace BALL
{
	class BALL_EXPORT GridAnalysis
	{
		public:
			GridAnalysis(ScoringFunction* sf, AtomContainer* probe_group);

			void setResolution(const double& resolution);

			void enableSavingBestPoses(Size number_best_poses);

			RegularData3D* calculate();

			list<AtomContainer*> getBestPoses();

		protected:
			typedef multimap<double, pair<Vector3, Vector3> > PoseList;

			/** Translate probe_group_ to the given destination. */
			void moveProbeGroup_(const Vector3& destination);

			/** Rotates probe_group_ around a specified axis.
			@param axis 0 == rotation around X axis, 1 == rotation around Y axis, 2 == rotation around Z axis */
			void rotateProbeGroup_(int axis, int degree);

			ScoringFunction* scoring_function_;

			AtomContainer probe_group_;

			double resolution_;

			Vector3 origin_;

			/** the current geometrical center of the probe group */
			Vector3 center_;

			Size no_best_poses_;

			/** contains pairs of  < score, < position, rotation > > . */
			PoseList best_poses_;

			Size border_;

	};
}

#endif // BALL_DOCKING_COMMON_GRIDANALYSIS_H
