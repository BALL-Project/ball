// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/PRIMITIVES/quadMesh.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		QuadMesh::QuadMesh()
			:	GeometricObject(),
				MultiColorExtension()
		{
		}

		QuadMesh::QuadMesh(const QuadMesh& mesh)
			:	GeometricObject(mesh),
				MultiColorExtension(mesh),
				vertex(mesh.vertex),
				normal(mesh.normal),
				quad(mesh.quad)
		{
		}

	} // namespace VIEW
} // namespace BALL
