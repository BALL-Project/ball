// $Id: mesh.C,v 1.2 2000/04/30 15:13:25 hekl Exp $

#include <BALL/VIEW/PRIMITIV/mesh.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Mesh::Mesh()
			:	GeometricObject(),
				ColorExtension(),
				Surface()
		{
		}

		Mesh::Mesh(const Mesh& mesh, bool deep)
			:	GeometricObject(mesh, deep),
				ColorExtension(mesh, deep),
				Surface(mesh)
		{
		}

		Mesh::Mesh(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Surface()
		{
		}

		Mesh::~Mesh()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Mesh>() << endl;
			#endif 

			destroy();
		}

		void Mesh::clear()
		{
			GeometricObject::clear();
			ColorExtension::clear();
		}

		void Mesh::destroy()
		{ 
			GeometricObject::destroy();
			ColorExtension::destroy();
		}

		void Mesh::set(const Mesh& mesh, bool deep)
		{
			GeometricObject::set(mesh, deep);
			ColorExtension::set(mesh, deep);
			//			Surface::set(mesh);
		}

		Mesh& Mesh::operator = (const Mesh& mesh)
		{
			set(mesh);

			return *this;
		}

		void Mesh::get(Mesh& mesh, bool deep) const
		{
			mesh.set(*this, deep);
		}

		void Mesh::swap(Mesh& mesh)
		{
			GeometricObject::swap(mesh);
			ColorExtension::swap(mesh);
			//			Surface::swap(mesh);
		}

		bool Mesh::isValid() const
		{
			return (bool)(GeometricObject::isValid() == true
										&& ColorExtension::isValid() == true);
										//										&& Surface::isValid() == true);
		}

		void Mesh::dump
			(ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Mesh::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Mesh::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Mesh::extract()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/mesh.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
