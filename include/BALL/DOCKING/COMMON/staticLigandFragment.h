/* staticLigandFragment.h
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

#ifndef BALL_DOCKING_COMMON_STATICLIGANDFRAGMENT
#define BALL_DOCKING_COMMON_STATICLIGANDFRAGMENT

#include <BALL/MATHS/matrix44.h>
#include <BALL/KERNEL/atom.h>


namespace BALL
{
	class BALL_EXPORT StaticLigandFragment
	{
		public:
			class Connection;

			/** the position of this fragment within ligand_fragments_ */
			int ID;

			/** all atoms of this ligand fragment */
			list<Atom*> atoms;

			vector<Connection> connections;

			/** the overall force of the entire fragment */
			Vector3 force;

			/** direction of the fragment force; i.e. the sum of the absolute values of the atom forces */
			Vector3 force_axis;

			/** the geometrical center of the ligand fragment */
			Vector3 center;

			/** rotates the current and all connected ligand fragments (except static_fragment) around given axis (--> bond between two fragments) */
			//void rotate(const Connection* rotation_bond, int degree, int static_fragment);

			/** rotates the current and all connected ligand fragments (except static_fragment) according to a given transformation matrix */
			//void rotate(TMatrix4x4<float>& M, Vector3& origin, int static_fragment);

			/** A Connection represent a single rotatable bond which connects two StaticLigandFragments */
			struct Connection
			{
				// /** the position of this connection within neighbor_connections */
				//int ID;

				/** atom of *this* fragment that is part of this intra-fragment bond */
				Atom* atom;

				/** atom of the neighbor fragment that is part of this intra-fragment bond */
				Atom* neighbor_atom;

				StaticLigandFragment* fragment; // the neighboring fragment
			};
	};
}

#endif // BALL_DOCKING_COMMON_STATICLIGANDFRAGMENT
