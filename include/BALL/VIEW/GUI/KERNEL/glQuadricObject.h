// $Id: glQuadricObject.h,v 1.3 2000/09/23 14:15:04 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLQUADRICOBJECT_H
#define BALL_VIEW_GUI_KERNEL_GLQUADRICOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

namespace BALL
{
	
	namespace VIEW
	{
		
		/**
		*/
		class GLQuadricObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			GLQuadricObject
				(int draw_style = GLU_FILL, int normals = GLU_FLAT,
				 int orientation = GLU_OUTSIDE, bool generate_texture_coordinates = false);

			GLQuadricObject
				(const GLQuadricObject& GL_quadric_object, bool deep = true);
		
			virtual ~GLQuadricObject();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set
				(const GLQuadricObject& GL_quadric_object, bool deep = true);

			GLQuadricObject& operator = (const GLQuadricObject& GL_quadric_object);

			void get
				(GLQuadricObject& GL_quadric_object, bool deep = true) const;

			void swap(GLQuadricObject& GL_quadric_object);
			//@}

			/**	@name	Accessors
			*/	
			//@{

			void setDrawStyle(int style);

			int getDrawStyle() const;

			void setOrientation(int orientation);

			int getOrientation() const;

			void setTextureCoordinateGeneration(bool generate);

			bool getTextureCoordinateGeneration() const;

			void setNormals(int normals);

			int getNormals() const;

			void drawPartialDisk
				(GLdouble inner_radius, GLdouble outer_radius,
				 int slices,int loops,
				 GLdouble start_angle, GLdouble sweep_angle);

			void drawDisk
				(GLdouble inner_radius, GLdouble outer_radius,
				 int slices, int loops);

			void drawCylinder
				(GLdouble base_radius, GLdouble top_radius, GLdouble height,
				 int slices, int stacks);

			void drawSphere
				(GLdouble radius, int longitude_subdiv, int latitude_subdiv);
			//@}
			
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

			
			private:

			void create_();

			int draw_style_;
			int normals_;
			int orientation_;
			bool generate_texture_coordinates_;

			GLUquadricObj* GLU_quadric_obj_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glQuadricObject.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLQUADRICOBJECT_H
