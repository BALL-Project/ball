// $Id: geoprim.C,v 1.1 2000/09/23 13:28:35 hekl Exp $

#include <BALL/VIEW/geoprim.h>
#include <GL/gl.h>

namespace BALL
{

	namespace VIEW
	{

		// --------------------------------------------------------------------------
		// Member Functions of Class Sphere ---------------------------------------
		// --------------------------------------------------------------------------

		Sphere::Sphere()
			:	ColorExtension(),
				Point(),
				Radius(),
				Resolution(),
				GeometricObject()
		{
		}

		Sphere::Sphere(const Sphere& sphere)
			:	ColorExtension(sphere),
				Point(sphere),
				Radius(sphere),
				Resolution(sphere),
				GeometricObject(sphere)
		{
		}

		Sphere::Sphere(const Sphere& sphere, bool /* deep */)
			:	ColorExtension(sphere),
				Point(sphere),
				Radius(sphere),
				Resolution(sphere),
				GeometricObject(sphere)
		{
		}

		Sphere::Sphere(const GeometricObject& geometric_object)
			:	ColorExtension(),
				Point(),
				Radius(),
				Resolution(),
				GeometricObject(geometric_object)
		{
		}

		Sphere::~Sphere()
		{
		}

		void Sphere::dump(ostream& s) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_HEADER(s, Sphere, this);

			ColorExtension::dump(s);
			Point::dump(s);
			Radius::dump(s);
			Resolution::dump(s);
			GeometricObject::dump(s);
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Sphere::draw()
		{
			if (hasProperty(GeometricObject::PROPERTY__HIDDEN) == true)
			{
				return true;
			}

			if (hasProperty(GeometricObject::PROPERTY__DESELECTED) == true)
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

			DisplayListManager::ShapePrecision precision;
			DisplayListManager::ShapeDrawingMode drawing_mode;

			getPrecisionAndDrawingMode(precision, drawing_mode);
			
			glLoadName((unsigned int)((Tree *)this));

			glPushMatrix();
			
			glTranslatef((GLfloat)(getPoint().x),
									 (GLfloat)(getPoint().y),
									 (GLfloat)(getPoint().z));

			glScalef((GLfloat)getRadius(),
							 (GLfloat)getRadius(),
							 (GLfloat)getRadius());

			if (precision != DisplayListManager::SHAPE_PRECISION__USER_DEFINED)
			{
				glCallList(getDisplayListManager()->getDisplayList(DisplayListManager::SHAPE__SPHERE,
											 precision, drawing_mode));
			}
			else
			{
				// user defined sphere
			}
			
			glPopMatrix();

			return true;
		}

		bool Sphere::extract()
		{
			return true;  
		}

		// --------------------------------------------------------------------------
		// Member Functions of Class Tube ---------------------------------------
		// --------------------------------------------------------------------------


		Tube::Tube()
			:	ColorExtension(),
				OpenCloseExtension(),
				Point2(),
				Radius(),
				Resolution(),
				GeometricObject()
		{
		}

		Tube::Tube(const Tube& tube)
			:	ColorExtension(tube),
				OpenCloseExtension(tube),
				Point2(tube),
				Radius(tube),
				Resolution(tube),
				GeometricObject(tube)
		{
		}

		Tube::Tube(const Tube& tube, bool /* deep */)
			:	ColorExtension(tube),
				OpenCloseExtension(tube),
				Point2(tube),
				Radius(tube),
				Resolution(tube),
				GeometricObject(tube)
		{
		}

		Tube::Tube(const Cone& cone)
			:	ColorExtension(cone),
				OpenCloseExtension(cone),
				Point2(cone),
				Radius(cone),
				Resolution(cone),
				GeometricObject(cone)
		{
		}
		
		Tube::Tube(const Box& box)
			:	ColorExtension(box),
				OpenCloseExtension(),
				Point2(box),
				Radius(),
				Resolution(),
				GeometricObject(box)
		{
		}

		Tube::Tube(const GeometricObject& geometric_object)
			:	ColorExtension(),
				OpenCloseExtension(),
				Point2(),
				Radius(),
				Resolution(),
				GeometricObject(geometric_object)
		{
		}

		Tube::~Tube()
		{
		}

		void Tube::dump(ostream& s) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_HEADER(s, Tube, this);

			ColorExtension::dump(s);
			OpenCloseExtension::dump(s);
			Point2::dump(s);
			Radius::dump(s);
			Resolution::dump(s);
			GeometricObject::dump(s);
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Tube::draw()
		{
			if (hasProperty(GeometricObject::PROPERTY__HIDDEN) == true)
			{
				return true;
			}

			DisplayListManager::Shape shape;

			if (isOpen() == true)
			{
				shape = DisplayListManager::SHAPE__TUBE;
			}
			else
			{
				shape = DisplayListManager::SHAPE__CLOSED_TUBE;
			}
		 
			if (hasProperty(GeometricObject::PROPERTY__DESELECTED) == true)
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

			DisplayListManager::ShapePrecision precision;
			DisplayListManager::ShapeDrawingMode drawing_mode;

			getPrecisionAndDrawingMode(precision, drawing_mode);
			
			glLoadName((unsigned int)((Tree *)this));

			glPushMatrix();
			
			glTranslatef((GLfloat)(getPoint1().x),
									 (GLfloat)(getPoint1().y),
									 (GLfloat)(getPoint1().z));

			Vector3 z_unit(0.0,0.0,getLength());

			Vector3 tube = getPoint2() - getPoint1();
			Vector3 axis = z_unit % tube;

			glRotatef((GLfloat)(Vector3::getAngle(z_unit, tube).getDegree()),
								(GLfloat)(axis.x),
								(GLfloat)(axis.y),
								(GLfloat)(axis.z)); 

			glScalef((GLfloat)getRadius(),
							 (GLfloat)getRadius(), 
							 (GLfloat)getLength());

			if (precision != DisplayListManager::SHAPE_PRECISION__USER_DEFINED)
			{
				glCallList(getDisplayListManager()->getDisplayList
									 (shape, precision, drawing_mode));
			}
			else
			{
				// user defined tube
			}
			
			glPopMatrix();

			return true;
		}

		bool Tube::extract()
		{
			return true;  
		}

		// --------------------------------------------------------------------------
		// Member Functions of Class Cone ---------------------------------------
		// --------------------------------------------------------------------------


		Cone::Cone()
			:	ColorExtension(),
				OpenCloseExtension(),
				Point2(),
				Radius(),
				Resolution(),
				GeometricObject()
		{
		}

		Cone::Cone(const Cone& cone)
			:	ColorExtension(cone),
				OpenCloseExtension(cone),
				Point2(cone),
				Radius(cone),
				Resolution(cone),
				GeometricObject(cone)
		{
		}

		Cone::Cone(const Cone& cone, bool /* deep */)
			:	ColorExtension(cone),
				OpenCloseExtension(cone),
				Point2(cone),
				Radius(cone),
				Resolution(cone),
				GeometricObject(cone)
		{
		}

		Cone::Cone(const Tube& tube)
			:	ColorExtension(tube),
				OpenCloseExtension(tube),
				Point2(tube),
				Radius(tube),
				Resolution(tube),
				GeometricObject(tube)
		{
		}

		Cone::Cone(const Box& box)
			:	ColorExtension(box),
				OpenCloseExtension(),
				Point2(box),
				Radius(),
				Resolution(),
				GeometricObject(box)
		{
		}

		Cone::Cone(const GeometricObject& geometric_object)
			:	ColorExtension(),
				OpenCloseExtension(),
				Point2(),
				Radius(),
				Resolution(),
				GeometricObject(geometric_object)
		{
		}

		Cone::~Cone()
		{
		}

		void Cone::dump(ostream& s) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_HEADER(s, Cone, this);

			ColorExtension::dump(s);
			OpenCloseExtension::dump(s);
			Point2::dump(s);
			Radius::dump(s);
			Resolution::dump(s);
			GeometricObject::dump(s);
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Cone::draw()
		{
			if (hasProperty(GeometricObject::PROPERTY__HIDDEN) == true)
			{
				return true;
			}

			DisplayListManager::Shape shape;

			if (isOpen() == true)
			{
				shape = DisplayListManager::SHAPE__CONE;
			}
			else
			{
				shape = DisplayListManager::SHAPE__CLOSED_CONE;
			}
		 
			if (hasProperty(GeometricObject::PROPERTY__DESELECTED) == true)
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

			DisplayListManager::ShapePrecision precision;
			DisplayListManager::ShapeDrawingMode drawing_mode;

			getPrecisionAndDrawingMode(precision, drawing_mode);
			
			glLoadName((unsigned int)((Tree *)this));

			glPushMatrix();
			
			glTranslatef((GLfloat)(getPoint1().x),
									 (GLfloat)(getPoint1().y),
									 (GLfloat)(getPoint1().z));

			Vector3 z_unit(0.0, 0.0, getLength());

			Vector3 cone = getPoint2() - getPoint1();
			Vector3 axis = z_unit % cone;

			glRotatef((GLfloat)(Vector3::getAngle(z_unit, cone).getDegree()),
								(GLfloat)(axis.x),
								(GLfloat)(axis.y),
								(GLfloat)(axis.z)); 

			glScalef((GLfloat)getRadius(),
							 (GLfloat)getRadius(), 
							 (GLfloat)getLength());

			if (precision != DisplayListManager::SHAPE_PRECISION__USER_DEFINED)
			{
				glCallList(getDisplayListManager()->getDisplayList
										(shape,precision,drawing_mode));
			}
			else
			{
				// user defined cone
			}
			
			glPopMatrix();

			return true;
		}

		bool Cone::extract()
		{
			return true;  
		}

		// --------------------------------------------------------------------------
		// Member Functions of Class Box ---------------------------------------
		// --------------------------------------------------------------------------


		Box::Box()
			:	ColorExtension(),
				Point2(),
				GeometricObject()
		{
		}

		Box::Box(const Box& box)
			:	ColorExtension(box),
				Point2(box),
				GeometricObject(box)
		{
		}

		Box::Box(const Box& box, bool /* deep */)
			:	ColorExtension(box),
				Point2(box),
				GeometricObject(box)
		{
		}

		Box::Box(const Tube& tube)
			:	ColorExtension(tube),
				Point2(tube),
				GeometricObject(tube)
		{
		}

		Box::Box(const Cone& cone)
			:	ColorExtension(cone),
				Point2(cone),
				GeometricObject(cone)
		{
		}

		Box::Box(const GeometricObject& geometric_object)
			:	ColorExtension(),
				Point2(),
				GeometricObject(geometric_object)
		{
		}

		Box::~Box()
		{
		}

		void Box::dump(ostream& s) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_HEADER(s, Box, this);

			ColorExtension::dump(s);
			Point2::dump(s);
			GeometricObject::dump(s);
			
			BALL_DUMP_STREAM_SUFFIX(s);
		}

		bool Box::draw()
		{
			if (hasProperty(GeometricObject::PROPERTY__HIDDEN) == true)
			{
				return true;
			}

			if (hasProperty(GeometricObject::PROPERTY__DESELECTED) == true)
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

			DisplayListManager::ShapeDrawingMode drawing_mode;

			getDrawingMode(drawing_mode);

			glLoadName((unsigned int)((Tree *)this));

			glPushMatrix();
			
			glTranslatef((GLfloat)(getPoint1().x),
									 (GLfloat)(getPoint1().y),
									 (GLfloat)(getPoint1().z));

			glScalef((GLfloat)((getPoint2().x) - (getPoint1().x)),
							 (GLfloat)((getPoint2().y) - (getPoint1().y)), 
							 (GLfloat)((getPoint2().z) - (getPoint1().z)));

			glCallList(getDisplayListManager()->getDisplayList(DisplayListManager::SHAPE__BOX,
										 DisplayListManager::SHAPE_PRECISION__LOW,
										 drawing_mode));
			glPopMatrix();

			return true;
		}

		bool Box::extract()
		{
			return true;  
		}


#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/geoprim.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
