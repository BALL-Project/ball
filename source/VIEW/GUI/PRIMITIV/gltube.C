// $Id: gltube.C,v 1.4 2001/02/04 16:14:26 hekl Exp $

#include <BALL/VIEW/GUI/PRIMITIV/gltube.h>
#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{

		GLTube::GLTube()
			:	Tube(),
				GLObject()
		{
		}

		GLTube::GLTube(const GLTube& GL_tube, bool deep)
			:	Tube(GL_tube, deep),
				GLObject(GL_tube)
		{
		}

		GLTube::GLTube(const GeometricObject& geometric_object)
			:	Tube(geometric_object),
				GLObject()
		{
		}

		GLTube::~GLTube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLTube>() << endl;
			#endif 

			destroy();
		}

		void GLTube::clear()
			throw()
		{
			Tube::clear();
			GLObject::clear();
		}

		void GLTube::destroy()
			throw()
		{
			Tube::destroy();
			GLObject::destroy();
		}

		bool GLTube::draw(bool with_names)
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

			Vector3 result;
			Vector3 rotation_axis;
			Real angle;

			result = getVertex2() - getVertex1();

			// cross product with z-axis-vector and result
			rotation_axis.set(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			angle = BALL_ANGLE_RADIAN_TO_DEGREE
								(acos(result.z / result.getLength()));

			glRotatef(angle, rotation_axis.x,
								rotation_axis.y, rotation_axis.z);

			glScalef((GLfloat)getRadius(),
							 (GLfloat)getRadius(),
							 (GLfloat)getLength());

			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			getGLPrimitiveManager()->Tube(mode, precision).draw();

			glPopMatrix();

			return true;
		}

		bool GLTube::drawUserDefined()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		bool GLTube::extract()
			throw()
		{
			return Tube::extract();
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/gltube.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL
