// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glsimpleBox.C,v 1.5.2.1 2003/01/07 13:23:28 anker Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>

namespace BALL
{
	namespace VIEW
	{

		GLSimpleBox::GLSimpleBox()
			throw()
			:	SimpleBox(),
				GLObject()
		{
		}

		GLSimpleBox::GLSimpleBox(const GLSimpleBox& simpleBox, bool deep)
			throw()
			:	SimpleBox(simpleBox, deep),
				GLObject(simpleBox)
		{
		}

		GLSimpleBox::GLSimpleBox(const GeometricObject& geometric_object)
			throw()
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

		void GLSimpleBox::set(const GLSimpleBox& simpleBox, bool deep)
			throw()
		{
			SimpleBox::set(simpleBox, deep);
			GLObject::set(simpleBox);
		}

		const GLSimpleBox& GLSimpleBox::operator = (const GLSimpleBox& simpleBox)
			throw()
		{
			set(simpleBox);
			return *this;
		}

		void GLSimpleBox::get(GLSimpleBox& simpleBox, bool deep) const
			throw()
		{
			simpleBox.set(*this, deep);
		}

		void GLSimpleBox::swap(GLSimpleBox& simpleBox)
			throw()
		{
			SimpleBox::swap(simpleBox);
			GLObject::swap(simpleBox);
		}

		bool GLSimpleBox::draw(bool with_names)
			throw()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor().getRed(),
									 (unsigned char)getColor().getGreen(),
									 (unsigned char)getColor().getBlue(),
									 (unsigned char)getColor().getAlpha());
			}
			else
			{
				glColor4ub((unsigned char)BALL_SELECTED_COLOR.getRed(),
									 (unsigned char)BALL_SELECTED_COLOR.getGreen(),
									 (unsigned char)BALL_SELECTED_COLOR.getBlue(),
									 (unsigned char)BALL_SELECTED_COLOR.getAlpha());
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


		bool GLSimpleBox::extract()
			throw()
		{
			return SimpleBox::extract();
		}

	} // namespace VIEW

} // namespace BALL
