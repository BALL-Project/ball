// $Id: label.C,v 1.6 2001/02/04 16:14:28 hekl Exp $

#include <BALL/VIEW/PRIMITIV/label.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Label::Label()
			throw()
			:	GeometricObject(),
				ColorExtension(),
				Vertex()
		{
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		Label::Label(const Label& label, bool deep)
			throw()
			:	GeometricObject(label, deep),
				ColorExtension(label),
				Vertex(label)
		{
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		Label::Label(const GeometricObject& geometric_object)
			throw()
			:	GeometricObject(geometric_object),
				ColorExtension(),
				Vertex()
		{
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		Label::~Label()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<Label>() << endl;
			#endif 

			destroy();
		}

		void Label::clear()
			throw()
		{
			GeometricObject::clear();
			ColorExtension::clear();
			Vertex::clear();
		}

		void Label::destroy()
			throw()
		{ 
			GeometricObject::destroy();
			ColorExtension::destroy();
			Vertex::destroy();
		}

		void Label::set(const Label& label, bool deep)
			throw()
		{
			GeometricObject::set(label, deep);
			ColorExtension::set(label);
			Vertex::set(label);
		}

		const Label& Label::operator = (const Label& label)
			throw()
		{
			set(label);
			return *this;
		}

		void Label::get(Label& label, bool deep) const
			throw()
		{
			label.set(*this, deep);
		}

		void Label::swap(Label& label)
			throw()
		{
			GeometricObject::swap(label);
			ColorExtension::swap(label);
			Vertex::swap(label);
		}

	  String Label::getTypeName() const
			throw()
    {
			return String("Label");
    }

		bool Label::isValid() const
			throw()
		{
			return (GeometricObject::isValid() &&
							Vertex::isValid());
		}

		void Label::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Label Text: " << getName() << endl;

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);
			Vertex::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Label::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Label::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Label::extract()
			throw()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/label.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
