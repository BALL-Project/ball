// $Id: glsimpleBox.C,v 1.4 2001/02/04 16:14:26 hekl Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>
#include <GL/gl.h>


namespace BALL
{

	namespace VIEW
	{

		GLSimpleBox::GLSimpleBox()
			:	SimpleBox(),
				GLObject()
		{
		}

		GLSimpleBox::GLSimpleBox(const GLSimpleBox& simpleBox, bool deep)
			:	SimpleBox(simpleBox, deep),
				GLObject(simpleBox)
		{
		}

		GLSimpleBox::GLSimpleBox(const GeometricObject& geometric_object)
			:	SimpleBox(geometric_object),
				GLObject()
		{
		}

		GLSimpleBox::~GLSimpleBox()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLSimpleBox>() << endl;
			#endif 

			destroy();
		}

		void GLSimpleBox::clear()
			throw()
		{
			SimpleBox::clear();
			GLObject::clear();
		}

		void GLSimpleBox::destroy()
			throw()
		{
			SimpleBox::destroy();
			GLObject::destroy();
		}

		bool GLSimpleBox::draw(bool with_names)
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().red(),
									 (unsigned char)getColor().green(),
									 (unsigned char)getColor().blue(),
									 (unsigned char)getColor().alpha());
			}
			else
			{
				glColor4ub((unsigned char)getSelectedColor().red(),
									 (unsigned char)getSelectedColor().green(),
									 (unsigned char)getSelectedColor().blue(),
									 (unsigned char)getSelectedColor().alpha());
			}

			if (with_names)
			{
				glLoadName(getGLPrimitiveManager()->getName(*this));
			}

			glPushMatrix();
			
			glTranslatef((GLfloat)(getVertex1().x),
									 (GLfloat)(getVertex1().y),
									 (GLfloat)(getVertex1().z));

			glScalef((GLfloat)(getVertex2().x - getVertex1().x),
							 (GLfloat)(getVertex2().y - getVertex1().y),
							 (GLfloat)(getVertex2().z - getVertex1().z));

			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			getGLPrimitiveManager()->SimpleBox(mode, precision).draw();

			glPopMatrix();

			return true;
		}

		bool GLSimpleBox::drawUserDefined()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLSimpleBox::extract()
			throw()
		{
			return SimpleBox::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
