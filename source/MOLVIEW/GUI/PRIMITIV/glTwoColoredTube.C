// $Id: glTwoColoredTube.C,v 1.1 2000/09/23 15:39:16 hekl Exp $

#include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GeometricObject;

		GLTwoColoredTube::GLTwoColoredTube()
			:	TwoColoredTube(),
				GLObject()
		{
		}

		GLTwoColoredTube::GLTwoColoredTube
			(const GLTwoColoredTube& GL_two_colored_tube, bool deep)
			:	TwoColoredTube(GL_two_colored_tube, deep),
				GLObject(GL_two_colored_tube, deep)
		{
		}

		GLTwoColoredTube::GLTwoColoredTube
			(const GeometricObject& geometric_object)
			:	TwoColoredTube(geometric_object),
				GLObject()
		{
		}

		GLTwoColoredTube::~GLTwoColoredTube()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLTwoColordTube>() << endl;
			#endif 

			destroy();
		}

		void GLTwoColoredTube::clear()
		{
			TwoColoredTube::clear();
			GLObject::clear();
		}

		void GLTwoColoredTube::destroy()
		{
			TwoColoredTube::destroy();
			GLObject::destroy();
		}

		bool GLTwoColoredTube::draw(bool with_names)
		{
			if (hasProperty(GeometricObject::PROPERTY__OBJECT_HIDDEN) == true)
			{
				return true;
			}

			if (isSelected() == true)
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
				glColor4ub((unsigned char)getColor1().red(),
									 (unsigned char)getColor1().green(),
									 (unsigned char)getColor1().blue(),
									 (unsigned char)getColor1().alpha());
			}

			draw_(getVertex1(), getMiddleVertex(), rotation_axis, angle);

			if (isSelected() == false)
			{
				glColor4ub((unsigned char)getColor2().red(),
									 (unsigned char)getColor2().green(),
									 (unsigned char)getColor2().blue(),
									 (unsigned char)getColor2().alpha());
			}

			draw_(getMiddleVertex(), getVertex2(), rotation_axis, angle);

			return true;
		}

		bool GLTwoColoredTube::extract()
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

			if (hasProperty(GeometricObject::PROPERTY__DRAWING_PRECISION_USER_DEFINED))
			{
				drawUserDefined();

				glPopMatrix();

				return;
			}

			unsigned int precision;
			unsigned int mode;

			getDrawingModeAndPrecision(mode, precision);

			getGLPrimitiveManager()->Tube(mode, precision).draw();

			glPopMatrix();
		}

		bool GLTwoColoredTube::drawUserDefined()
		{
			throw Exception::NotImplemented(__FILE__, __LINE__);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL
