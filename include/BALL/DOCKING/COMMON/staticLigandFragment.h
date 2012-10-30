// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_DOCKING_COMMON_STATICLIGANDFRAGMENT
#define BALL_DOCKING_COMMON_STATICLIGANDFRAGMENT

#ifndef BALL_MATH_MATRIX44_H
#include <BALL/MATHS/matrix44.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#include <BALL/KERNEL/atom.h>
#endif

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
