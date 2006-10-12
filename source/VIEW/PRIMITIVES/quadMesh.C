// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: quadMesh.C,v 1.1.2.1 2006/10/12 19:22:42 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/quadMesh.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		QuadMesh::QuadMesh()
			throw()
			:	GeometricObject(),
				colors()

		{
		}

		QuadMesh::QuadMesh(const QuadMesh& mesh)
			throw()
			:	GeometricObject(mesh),
				vertex(mesh.vertex),
				normal(mesh.normal),
				quad(mesh.quad),
				colors(mesh.colors)
		{
		}

	} // namespace VIEW
} // namespace BALL
