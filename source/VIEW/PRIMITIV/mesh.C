// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mesh.C,v 1.9 2002/12/16 12:23:15 sturm Exp $

#include <BALL/VIEW/PRIMITIV/mesh.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Mesh::Mesh()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Surface(),
				colorList()

		{
		}

		Mesh::Mesh(const Mesh& mesh, bool deep)
			throw()
			:	GeometricObject(mesh, deep),
				ColorExtension(mesh),
				Surface(mesh),
				colorList(mesh.colorList)
		{
		}

		Mesh::Mesh(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
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

			destroy();
		}

		void Mesh::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
	//		colorList.clear();
		}

		void Mesh::destroy()
			throw()
		{ 
			GeometricObject::destroy();
			ColorExtension::destroy();
//			colorList.clear();
		}

		void Mesh::set(const Mesh& mesh, bool deep)
			throw()
		{
			GeometricObject::set(mesh, deep);
			ColorExtension::set(mesh);
			//			Surface::set(mesh);
			colorList = mesh.colorList;
		}

		const Mesh& Mesh::operator = (const Mesh& mesh)
			throw()
		{
			set(mesh);
			return *this;
		}

		void Mesh::get(Mesh& mesh, bool deep) const
			throw()
		{
			mesh.set(*this, deep);
		}

		void Mesh::swap(Mesh& mesh)
			throw()
		{
			GeometricObject::swap(mesh);
			ColorExtension::swap(mesh);
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
			ColorExtension::dump(s, depth + 1);
			//colorList.dump();
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Mesh::extract()
			throw()
		{
			return true;  
		}

	} // namespace VIEW

} // namespace BALL
