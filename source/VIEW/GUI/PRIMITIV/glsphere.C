// $Id: glsphere.C,v 1.2 2000/12/12 16:18:45 oliver Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#include <GL/gl.h>


namespace BALL
{

	namespace VIEW
	{

		GLSphere::GLSphere()
			:	Sphere(),
				GLObject()
		{
		}

		GLSphere::GLSphere(const GLSphere& GL_sphere, bool deep)
			:	Sphere(GL_sphere, deep),
				GLObject(GL_sphere, deep)
		{
		}

		GLSphere::GLSphere(const GeometricObject& geometric_object)
			:	Sphere(geometric_object),
				GLObject()
		{
		}

		GLSphere::~GLSphere()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLSphere>() << endl;
			#endif 

			destroy();
		}

		void GLSphere::clear()
			throw()
		{
			Sphere::clear();
			GLObject::clear();
		}

		void GLSphere::destroy()
			throw()
		{
			Sphere::destroy();
			GLObject::destroy();
		}

		bool GLSphere::draw(bool with_names)
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
			
			glTranslatef((GLfloat)(getVertex().x),
									 (GLfloat)(getVertex().y),
									 (GLfloat)(getVertex().z));

			glScalef((GLfloat)getRadius(),
							 (GLfloat)getRadius(),
							 (GLfloat)getRadius());

			if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED)
					== true)
			{
				drawUserDefined();

				glPopMatrix();

				return true;
			}

			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			getGLPrimitiveManager()->Sphere(mode, precision).draw();

			glPopMatrix();

			return true;
		}

		bool GLSphere::drawUserDefined()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLSphere::extract()
		{
			return Sphere::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsphere.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
