// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.C,v 1.3 2003/08/29 10:53:24 amoll Exp $

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
				colorList()

		{
		}

		Mesh::Mesh(const Mesh& mesh)
			throw()
			:	GeometricObject(mesh),
				Surface(mesh),
				colorList(mesh.colorList)
		{
		}

		Mesh::Mesh(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				Surface(),
				colorList()
		{
		}

		Mesh::~Mesh()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Mesh>() << endl;
			#endif 
		}

		void Mesh::clear()
			throw()
		{
			GeometricObject::clear();
	//		colorList.clear();
		}

		void Mesh::set(const Mesh& mesh)
			throw()
		{
			GeometricObject::set(mesh);
			colorList = mesh.colorList;
		}

		const Mesh& Mesh::operator = (const Mesh& mesh)
			throw()
		{
			set(mesh);
			return *this;
		}

		void Mesh::get(Mesh& mesh) const
			throw()
		{
			mesh.set(*this);
		}

		void Mesh::swap(Mesh& mesh)
			throw()
		{
			GeometricObject::swap(mesh);
			//			Surface::swap(mesh);
			vector<ColorRGBA> dummy = mesh.colorList;
			mesh.colorList = colorList;
			colorList = dummy;
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
			//colorList.dump();
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

	} // namespace VIEW
} // namespace BALL
