// $Id: glQuadricObject.C,v 1.4 2001/05/13 14:28:36 hekl Exp $

#include <BALL/VIEW/GUI/KERNEL/glQuadricObject.h>
#include <BALL/COMMON/exception.h>

using namespace std;

namespace BALL
{

	namespace VIEW
	{

	  GLQuadricObject::WrongDrawingStyle::WrongDrawingStyle(const char* file, int line)
			throw()
			: Exception::GeneralException(file, line, string("WrongDrawingStyle"), string("wrong drawing style for quadric objects."))
		{
    }
  
	  GLQuadricObject::WrongOrientationStyle::WrongOrientationStyle(const char* file, int line)
			throw()
			: Exception::GeneralException(file, line, string("WrongOrientationStyle"), string("wrong orientation style for quadric objects."))
		{
    }
  
	  GLQuadricObject::WrongNormalStyle::WrongNormalStyle(const char* file, int line)
			throw()
			: Exception::GeneralException(file, line, string("WrongNormalStyle"), string("wrong normal style for quadric objects."))
		{
    }
  
	  GLQuadricObject::NoQuadricObjectAvailable::NoQuadricObjectAvailable(const char* file, int line)
			throw()
			: Exception::GeneralException(file, line, string("NoQuadricObjectAvailable"), string("memory allocation for quadric objects failed."))
		{
    }
  
		GLQuadricObject::GLQuadricObject
			(int draw_style, int normals, int orientation,
			 bool generate_texture_coordinates)
			throw()
			:	GLU_quadric_obj_(0)
		{
			setDrawStyle(draw_style);
			setNormals(normals);
			setOrientation(orientation);
			setTextureCoordinateGeneration(generate_texture_coordinates);
		}

		GLQuadricObject::GLQuadricObject
			(const GLQuadricObject& GL_quadric_object)
			throw()
			:	draw_style_(GL_quadric_object.draw_style_),
				normals_(GL_quadric_object.normals_),
				orientation_(GL_quadric_object.orientation_),
				generate_texture_coordinates_(GL_quadric_object.generate_texture_coordinates_),
				GLU_quadric_obj_(0)
		{
		}

		GLQuadricObject::~GLQuadricObject()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<GLQuadricObject>() << endl;
			#endif 

			destroy();
		}

		void GLQuadricObject::clear()
			throw()
		{
			draw_style_ = GLU_FILL;
			normals_ = GLU_FLAT;
			orientation_ = GLU_OUTSIDE;
			generate_texture_coordinates_ = false;
		}

		void GLQuadricObject::destroy()
			throw()
		{
			if (GLU_quadric_obj_ != 0)
			{
				gluDeleteQuadric(GLU_quadric_obj_);
			}

			clear();
		}

		void GLQuadricObject::set
			(const GLQuadricObject& GL_quadric_object)
			throw()
		{
			draw_style_ = GL_quadric_object.draw_style_;
			normals_ = GL_quadric_object.normals_;
			orientation_ = GL_quadric_object.orientation_;
			generate_texture_coordinates_ = GL_quadric_object.generate_texture_coordinates_;
		}

		const GLQuadricObject& GLQuadricObject::operator =
			(const GLQuadricObject& GL_quadric_object)
			throw()
		{
			set(GL_quadric_object);

			return *this;
		}

		void GLQuadricObject::get
			(GLQuadricObject& GL_quadric_object) const
			throw()
		{
			GL_quadric_object.set(*this);
		}

		void GLQuadricObject::swap(GLQuadricObject& GL_quadric_object)
			throw()
		{
			int i = draw_style_;
			draw_style_ = GL_quadric_object.draw_style_;
			GL_quadric_object.draw_style_ = i;

			i = normals_;
			normals_ = GL_quadric_object.normals_;
			GL_quadric_object.normals_ = i;

			i = orientation_;
			orientation_ = GL_quadric_object.orientation_;
			GL_quadric_object.orientation_ = i;

			bool b = generate_texture_coordinates_;
			generate_texture_coordinates_ = GL_quadric_object.generate_texture_coordinates_;
			GL_quadric_object.generate_texture_coordinates_ = b;
		}

		void GLQuadricObject::setDrawStyle(int style)
			throw(WrongDrawingStyle)
		{
			if (style != GLU_FILL
					&& style != GLU_LINE
					&& style != GLU_SILHOUETTE
					&& style != GLU_POINT)
			{
				throw WrongDrawingStyle(__FILE__, __LINE__);
			}
			
			draw_style_ = style;;
		}

		void GLQuadricObject::setOrientation(int orientation)
			throw(WrongOrientationStyle)
		{
			if (orientation != GLU_INSIDE
					&& orientation != GLU_OUTSIDE)
			{
				throw WrongOrientationStyle(__FILE__, __LINE__);
			}

			orientation_ = orientation;
		}

		void GLQuadricObject::setTextureCoordinateGeneration(bool generate)
			throw()
		{
			generate_texture_coordinates_ = generate;
		}

		void GLQuadricObject::setNormals(int normals)
			throw(WrongNormalStyle)
		{
			if (normals != GLU_NONE
					&& normals != GLU_FLAT
					&& normals != GLU_SMOOTH)
			{
				throw WrongNormalStyle(__FILE__, __LINE__);
			}
			
			normals_ = normals;
		}

		void GLQuadricObject::drawPartialDisk
			(GLdouble inner_radius, GLdouble outer_radius,
			 int slices, int rings, GLdouble start_angle, GLdouble sweep_angle)
			throw(NoQuadricObjectAvailable)
		{
			create_();

			gluPartialDisk(GLU_quadric_obj_, inner_radius, outer_radius,
										 slices, rings, start_angle, sweep_angle);
		}

		void GLQuadricObject::drawDisk
			(GLdouble inner_radius, GLdouble outer_radius,
			 int slices, int rings)
			throw(NoQuadricObjectAvailable)
		{
			create_();

			gluDisk(GLU_quadric_obj_, inner_radius, outer_radius, slices, rings);
		}

		void GLQuadricObject::drawCylinder
			(GLdouble base_radius, GLdouble top_radius, GLdouble height,
			 int slices, int stacks)
			throw(NoQuadricObjectAvailable)
		{
			create_();

			gluCylinder(GLU_quadric_obj_, base_radius, top_radius, height, slices, stacks);  
		}

		void GLQuadricObject::drawSphere
			(GLdouble radius, int slices, int stacks)
			throw(NoQuadricObjectAvailable)
		{
			create_();
			gluSphere(GLU_quadric_obj_, radius, slices, stacks);
		}

		bool GLQuadricObject::isValid() const
			throw()
		{
			return (GLU_quadric_obj_ != 0);
		}

		void GLQuadricObject::dump(ostream& s, Size depth) const
			throw()
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "drawing style: ";

			switch (draw_style_)
			{
				case GLU_FILL:
					s << "GLU_FILL" << endl;
					break;

				case GLU_LINE:
					s << "GLU_LINE" << endl;
					break;

				case GLU_SILHOUETTE:
					s << "GLU_SILHOUTTE" << endl;
					break;

				case GLU_POINT:
					s << "GLU_POINT" << endl;
					break;

				default:
					s << "UNKOWN" << endl;
					break;
			}

			BALL_DUMP_DEPTH(s, depth);
			s << "normal style: "; 

			switch (draw_style_)
			{
				case GLU_NONE:
					s << "GLU_NONE" << endl;
					break;

				case GLU_FLAT:
					s << "GLU_FLAT" << endl;
					break;

				case GLU_SMOOTH:
					s << "GLU_SMOOTH" << endl;
					break;

				default:
					s << "UNKOWN" << endl;
					break;
			}

			BALL_DUMP_DEPTH(s, depth);
			s << "orientation style: ";

			switch (draw_style_)
			{
				case GLU_INSIDE:
					s << "GLU_INSIDE" << endl;
					break;

				case GLU_OUTSIDE:
					s << "GLU_OUTSIDE" << endl;
					break;

				default:
					s << "UNKOWN" << endl;
					break;
			}

			BALL_DUMP_DEPTH(s, depth);
			s << "create texture coordinates: "
					 << ((generate_texture_coordinates_ == true) ? "yes" : "no") 
					 << endl;; 

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void GLQuadricObject::read(istream & /* s */)
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}
		 
		void GLQuadricObject::write(ostream & /* s */) const
			throw()
		{
			throw ::BALL::Exception::NotImplemented(__FILE__, __LINE__);
		}

		void GLQuadricObject::create_()
			throw(NoQuadricObjectAvailable)
		{
			if (GLU_quadric_obj_ == 0)
			{
				GLU_quadric_obj_ = gluNewQuadric();

				if (GLU_quadric_obj_ == 0)
				{
					throw NoQuadricObjectAvailable(__FILE__, __LINE__);
				}
			}
				
			gluQuadricNormals(GLU_quadric_obj_, (GLenum)normals_);
			
			if(generate_texture_coordinates_ == true)
			{ 
				gluQuadricTexture(GLU_quadric_obj_, GL_TRUE);
			}
			else 
			{
				gluQuadricTexture(GLU_quadric_obj_, GL_FALSE);
			}
			
			gluQuadricDrawStyle(GLU_quadric_obj_, (GLenum)draw_style_);
			
			gluQuadricOrientation(GLU_quadric_obj_, (GLenum)orientation_);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glQuadricObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL
