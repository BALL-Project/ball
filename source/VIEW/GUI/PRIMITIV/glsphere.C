// $Id: glsphere.C,v 1.6 2001/05/13 14:28:37 hekl Exp $

#include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#include <GL/gl.h>


namespace BALL
{

	namespace VIEW
	{

		GLSphere::GLSphere()
			throw()
			:	Sphere(),
				GLObject()
		{
		}

		GLSphere::GLSphere(const GLSphere& GL_sphere, bool deep)
			throw()
			:	Sphere(GL_sphere, deep),
				GLObject(GL_sphere)
		{
		}

		GLSphere::GLSphere(const GeometricObject& geometric_object)
			throw()
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

		void GLSphere::set(const GLSphere& sphere, bool deep)
			throw()
		{
			Sphere::set(sphere, deep);
			GLObject::set(sphere);
		}

		const GLSphere& GLSphere::operator = (const GLSphere& sphere)
			throw()
		{
			set(sphere);
			return *this;
		}

		void GLSphere::get(GLSphere& sphere, bool deep) const
			throw()
		{
			sphere.set(*this, deep);
		}

		void GLSphere::swap(GLSphere& sphere)
			throw()
		{
			Sphere::swap(sphere);
			GLObject::swap(sphere);
		}

		bool GLSphere::draw(bool with_names)
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
				glColor4ub((unsigned char)getSelectedColor().getRed(),
									 (unsigned char)getSelectedColor().getGreen(),
									 (unsigned char)getSelectedColor().getBlue(),
									 (unsigned char)getSelectedColor().getAlpha());
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

			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			getGLPrimitiveManager()->Sphere(mode, precision).draw();

			glPopMatrix();

			return true;
		}

		bool GLSphere::drawUserDefined()
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLSphere::extract()
			throw()
		{
			return Sphere::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsphere.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
