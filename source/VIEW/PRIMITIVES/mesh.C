// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.C,v 1.6.2.1 2006/03/30 14:26:45 amoll Exp $

#include <BALL/VIEW/PRIMITIVES/mesh.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		Mesh::Mesh()
			throw()
			:	GeometricObject(),
				Surface(),
				colors()

		{
		}

		Mesh::Mesh(const Mesh& mesh)
			throw()
			:	GeometricObject(mesh),
				Surface(mesh),
				colors(mesh.colors)
		{
		}

		Mesh::~Mesh()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<Mesh>() << endl;
			#endif 
		}

		void Mesh::clear()
			throw()
		{
			GeometricObject::clear();
	//		colors.clear();
		}

		void Mesh::set(const Mesh& mesh)
			throw()
		{
			GeometricObject::set(mesh);
			colors = mesh.colors;
		}

		const Mesh& Mesh::operator = (const Mesh& mesh)
			throw()
		{
			set(mesh);
			return *this;
		}

		void Mesh::swap(Mesh& mesh)
			throw()
		{
			GeometricObject::swap(mesh);
			//			Surface::swap(mesh);
			vector<ColorRGBA> dummy = mesh.colors;
			mesh.colors = colors;
			colors = dummy;
		}

		bool Mesh::isValid() const
			throw()
		{
			return (GeometricObject::isValid());
										//										&& Surface::isValid());
		}

		void Mesh::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			//colors.dump();
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Mesh::getVertices(vector<Vector3>& vertices) const
		{
			vertices.reserve(vertices.size() + vertex.size());
			for (Position p = 0; p < vertex.size(); p++)
			{
				vertices.push_back(vertex[p]);
			}
		}

	} // namespace VIEW
} // namespace BALL
