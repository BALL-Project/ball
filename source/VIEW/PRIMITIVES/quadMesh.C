// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: quadMesh.C,v 1.1.4.1 2007-03-25 22:02:35 oliver Exp $

#include <BALL/VIEW/PRIMITIVES/quadMesh.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		QuadMesh::QuadMesh()
			throw()
			:	GeometricObject(),
				MultiColorExtension()
		{
		}

		QuadMesh::QuadMesh(const QuadMesh& mesh)
			throw()
			:	GeometricObject(mesh),
				MultiColorExtension(mesh),
				vertex(mesh.vertex),
				normal(mesh.normal),
				quad(mesh.quad)
		{
		}

	} // namespace VIEW
} // namespace BALL
