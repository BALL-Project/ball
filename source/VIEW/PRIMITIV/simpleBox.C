// $Id: simpleBox.C,v 1.3 2000/12/19 22:07:28 oliver Exp $

#include <BALL/VIEW/PRIMITIV/simpleBox.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		SimpleBox::SimpleBox()
			:	GeometricObject(),
				ColorExtension(),
				Vertex2()
		{
		}

		SimpleBox::SimpleBox(const SimpleBox& SimpleBox, bool deep)
			:	GeometricObject(SimpleBox, deep),
				ColorExtension(SimpleBox, deep),
				Vertex2(SimpleBox, deep)
		{
		}

		SimpleBox::SimpleBox(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex2()
		{
		}

		SimpleBox::~SimpleBox()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<SimpleBox>() << endl;
			#endif 

			destroy();
		}

		void SimpleBox::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex2::clear();
		}

		void SimpleBox::destroy()
			throw()
		{
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex2::destroy();
		}

		void SimpleBox::set(const SimpleBox& SimpleBox, bool deep)
		{
			GeometricObject::set(SimpleBox, deep);
			ColorExtension::set(SimpleBox, deep);
			Vertex2::set(SimpleBox, deep);
		}

		SimpleBox& SimpleBox::operator = (const SimpleBox& SimpleBox)
		{
			set(SimpleBox);

			return *this;
		}

		void SimpleBox::get(SimpleBox& SimpleBox, bool deep) const
		{
			SimpleBox.set(*this, deep);
		}

		void SimpleBox::swap(SimpleBox& SimpleBox)
		{
			GeometricObject::swap(SimpleBox);
			ColorExtension::swap(SimpleBox);
			Vertex2::swap(SimpleBox);
		}

		bool SimpleBox::isValid() const
			throw()
		{
			return (bool)(GeometricObject::isValid() == true
										&& ColorExtension::isValid() == true
										&& Vertex2::isValid() == true);
		}

		void SimpleBox::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);
			Vertex2::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void SimpleBox::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void SimpleBox::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool SimpleBox::extract()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/simpleBox.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
