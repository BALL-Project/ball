// $Id: glTwoColoredTube.C,v 1.5 2001/05/13 15:02:41 hekl Exp $

#include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GeometricObject;

		GLTwoColoredTube::GLTwoColoredTube()
			throw()
			:	TwoColoredTube(),
				GLObject()
		{
		}

		GLTwoColoredTube::GLTwoColoredTube
			(const GLTwoColoredTube& GL_two_colored_tube, bool deep)
			throw()
			:	TwoColoredTube(GL_two_colored_tube, deep),
				GLObject(GL_two_colored_tube)
		{
		}

		GLTwoColoredTube::GLTwoColoredTube
			(const GeometricObject& geometric_object)
			throw()
			:	TwoColoredTube(geometric_object),
				GLObject()
		{
		}

		GLTwoColoredTube::~GLTwoColoredTube()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLTwoColordTube>() << endl;
			#endif 

			destroy();
		}

		void GLTwoColoredTube::clear()
			throw()
		{
			TwoColoredTube::clear();
			GLObject::clear();
		}

		void GLTwoColoredTube::destroy()
			throw()
		{
			TwoColoredTube::destroy();
			GLObject::destroy();
		}

		void GLTwoColoredTube::set(const GLTwoColoredTube& gl_two_colored_tube, bool deep)
			throw()
		{
			TwoColoredTube::set(gl_two_colored_tube, deep);
			GLObject::set(gl_two_colored_tube);
		}

		const GLTwoColoredTube& GLTwoColoredTube::operator = (const GLTwoColoredTube& gl_two_colored_tube)
			throw()
		{
			set(gl_two_colored_tube);

			return *this;
		}

		void GLTwoColoredTube::get(GLTwoColoredTube& gl_two_colored_tube, bool deep) const
			throw()
		{
			gl_two_colored_tube.set(*this, deep);
		}

		void GLTwoColoredTube::swap(GLTwoColoredTube& gl_two_colored_tube)
			throw()
		{
			TwoColoredTube::swap(gl_two_colored_tube);
			GLObject::swap(gl_two_colored_tube);
		}

		bool GLTwoColoredTube::draw(bool with_names)
			throw()
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN))
			{
				return true;
			}

			if (isSelected())
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

			Vector3 result;
			Vector3 rotation_axis;
			Real angle;

			result = getVertex2() - getVertex1();

			// cross product with z-axis-vector and result
			rotation_axis.set(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(result.z / result.getLength()));

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor1().getRed(),
									 (unsigned char)getColor1().getGreen(),
									 (unsigned char)getColor1().getBlue(),
									 (unsigned char)getColor1().getAlpha());
			}

			draw_(getVertex1(), getMiddleVertex(), rotation_axis, angle);

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor2().getRed(),
									 (unsigned char)getColor2().getGreen(),
									 (unsigned char)getColor2().getBlue(),
									 (unsigned char)getColor2().getAlpha());
			}

			draw_(getMiddleVertex(), getVertex2(), rotation_axis, angle);

			return true;
		}

		bool GLTwoColoredTube::extract()
			throw()
		{
			return TwoColoredTube::extract();
		}

		void GLTwoColoredTube::draw_
			(const Vector3& vertex_1, const Vector3& /* vertex_2 */,
			 const Vector3& rotation_axis, const Real angle)
			
		{
			glPushMatrix();
			
			glTranslatef((GLfloat)(vertex_1.x), (GLfloat)(vertex_1.y), (GLfloat)(vertex_1.z));

			glRotatef(angle, rotation_axis.x, rotation_axis.y, rotation_axis.z);

			glScalef((GLfloat)getRadius(),
							 (GLfloat)getRadius(),
							 (GLfloat)getLength() / (Real)2);

			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			getGLPrimitiveManager()->Tube(mode, precision).draw();

			glPopMatrix();
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
