// $Id: label.C,v 1.3 2000/12/12 16:18:46 oliver Exp $

#include <BALL/VIEW/PRIMITIV/label.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

		Label::Label()
			:	GeometricObject(),
				ColorExtension()
		{
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		Label::Label(const Label& label, bool deep)
			:	GeometricObject(label, deep),
				ColorExtension(label, deep),
				Vertex(label, deep)
		{
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_STATIC);
			PropertyManager::setProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			setProperty(GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
		}

		Label::Label(const GeometricObject& geometric_object)
			:	GeometricObject(geometric_object),
				ColorExtension()
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
		}

		void Label::destroy()
			throw()
		{ 
			GeometricObject::destroy();
			ColorExtension::destroy();
		}

		void Label::set(const Label& Label, bool deep)
		{
			GeometricObject::set(Label, deep);
			ColorExtension::set(Label, deep);
		}

		Label& Label::operator = (const Label& Label)
		{
			set(Label);

			return *this;
		}

		void Label::get(Label& Label, bool deep) const
		{
			Label.set(*this, deep);
		}

		void Label::swap(Label& Label)
		{
			GeometricObject::swap(Label);
			ColorExtension::swap(Label);
		}

	  String Label::getTypeName() const
    {
			return String("Label");
    }

		bool Label::isValid() const
		{
			return (bool)(GeometricObject::isValid() == true
										&& ColorExtension::isValid() == true);
		}

		void Label::dump
			(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Label Text: " << getName() << endl;

			GeometricObject::dump(s, depth + 1);
			ColorExtension::dump(s, depth + 1);

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void Label::read(istream & /* s */)
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void Label::write(ostream & /* s */) const
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool Label::extract()
		{
			return true;  
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/label.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
