// $Id: mesh.C,v 1.5 2000/12/21 17:03:46 amoll Exp $

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
				Surface()
		{
		}

		Mesh::Mesh(const Mesh& mesh, bool deep)
			throw()
			:	GeometricObject(mesh, deep),
				ColorExtension(mesh, deep),
				Surface(mesh)
		{
		}

		Mesh::Mesh(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Surface()
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
		}

		void Mesh::destroy()
			throw()
		{ 
			GeometricObject::destroy();
			ColorExtension::destroy();
		}

		void Mesh::set(const Mesh& mesh, bool deep)
			throw()
		{
			GeometricObject::set(mesh, deep);
			ColorExtension::set(mesh, deep);
			//			Surface::set(mesh);
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
		}

		bool Mesh::isValid() const
			throw()
		{
			return (GeometricObject::isValid()
										&& ColorExtension::isValid());
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

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Mesh::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Mesh::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Mesh::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/mesh.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
