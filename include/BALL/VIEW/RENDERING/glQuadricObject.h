// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glQuadricObject.h,v 1.6 2004/07/14 17:42:49 amoll Exp $

#ifndef BALL_VIEW_RENDERING_GLQUADRICOBJECT_H
#define BALL_VIEW_RENDERING_GLQUADRICOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <qgl.h>

namespace BALL
{
	namespace VIEW
	{
		
		/** GLQuadricObject class.
				The class QLQuadricObject is a wrapper class for the quadric objects
				of the OpenGL graphical library. It defines some useful methods to define
				and create quadric objects. Further this class hides the handling
				of OpenGL quadric objects and defines an easy interface for creating those
				quadric objects.
				See the documentation of the OpenGL graphical library for information
				concerning quadric objects.
				\ingroup ViewRendering
		*/
		class BALL_EXPORT GLQuadricObject
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** WrongDrawingStyle exception class.
					This exception will be thrown if a wrong drawing style for this
					glQuadricObject will be used. See the documentation of the OpenGL
					graphical library for drawing styles of quadric objects.
					\see         GeneralException			
			*/
			class WrongDrawingStyle:	public Exception::GeneralException
			{
				public:

				WrongDrawingStyle(const char* file, int line)
					throw();
			};

			/** WrongOrientationStyle exception class.
					This exception will be thrown if a wrong orientation style for this
					glQuadricObject will be used. See the documentation of the OpenGL
					graphical library for orientation styles of quadric objects.
					\see         GeneralException			
			*/
			class WrongOrientationStyle:	public Exception::GeneralException
			{
				public:

				WrongOrientationStyle(const char* file, int line)
					throw();
			};

			/** WrongNormalStyle exception class.
					This exception will be thrown if a wrong normal style for this
					glQuadricObject will be used. See the documentation of the OpenGL
					graphical library for normal styles of quadric objects.
					\see         GeneralException			
			*/
			class WrongNormalStyle:	public Exception::GeneralException
			{
				public:

				WrongNormalStyle(const char* file, int line)
					throw();
			};


			/** NoQuadricObjectAvailable exception class.
					This exception will be thrown if no memory for this glQuadricObject
					is available.
					\see         GeneralException			
			*/
			class NoQuadricObjectAvailable:	public Exception::GeneralException
			{
				public:

				NoQuadricObjectAvailable(const char* file, int line)
					throw();
			};

			//@}
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The state of this glQuadricObject is set to:
					  - drawing style is set to <tt> GLU_FILL</tt> (Default)
					  - normal style is set to <tt> GLU_FLAT_FILL</tt> (Default)
					  - orientation style is set to <tt> GLU_OUTSIDE</tt> (Default)
					  - generate texture coordinates is set to <tt> false</tt> (Default)
			*/
			GLQuadricObject
				(GLenum draw_style = GLU_FILL, GLenum normals = GLU_FLAT,
				 GLenum orientation = GLU_OUTSIDE, bool generate_texture_coordinates = false)
				throw();

			/** Copy constructor.
			*/
			GLQuadricObject(const GLQuadricObject& GL_quadric_object)
				throw();
		
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~GLQuadricObject()
				throw();

			/** Explicit default initialization.
					Resets the values of this glQuadricObject to the default values.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
			*/
			void set(const GLQuadricObject& GL_quadric_object)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const GLQuadricObject& operator = (const GLQuadricObject& GL_quadric_object)
				throw();

			/** Swapping of glQuadricObjects.
			*/
			void swap(GLQuadricObject& GL_quadric_object)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the drawing style.
					\see         getDrawStyle
					\exception   WrongDrawingStyle thrown whenever the drawing style is not allowed.
			*/
			void setDrawStyle(int style)
				throw(WrongDrawingStyle);

			/** Inspection of the drawing style.
					\see         setDrawStyle
			*/
			int getDrawStyle() const
				throw();

			/** Change the orientation style.
					\see         getOrientation
					\exception   WrongOrientationStyle thrown whenever the orientation style is not allowed.
			*/
			void setOrientation(GLenum orientation)
				throw(WrongOrientationStyle);

			/** Inspection of the orientation style.
					\see         setOrientation
			*/
			int getOrientation() const
				throw();

			/** Change the texture coordinate generation.
					\see         getTextureCoordinateGeneration
			*/
			void setTextureCoordinateGeneration(bool generate)
				throw();

			/** Inspection of the texture coordinate generation switch.
					\see       setTextureCoordinateGeneration
			*/
			bool getTextureCoordinateGeneration() const
				throw();

			/** Change the normal style.
					\see         getNormals
					\exception   WrongNormalStyle thrown whenever the normal style is not allowed.
			*/
			void setNormals(int normals)
				throw(WrongNormalStyle);

			/** Inspection of the normal style.
					\see         setNormals
			*/
			int getNormals() const
				throw();

			//@}
			/** @name Drawing methods for quadric objects.
			*/
			//@{
			
			/** Draw a partial disk.
					This method is a wrapper method for the OpenGL function <b> gluPartialDisk</b>.
					\param    inner_radius the inner radius of the partial disk.
					\param    outer_radius the outer radius of the partial disk.
					\param    slices the number of slices the partial disk has
					\param    rings the number of rings the partial disk has
					\param    start_angle the start angle of the partial disk.
					\param    sweep_angle the sweep angle of the partial disk.
					\exception  NoQuadricObjectAvailable thrown if no memory is available for this glQuadricObject
			*/
			void drawPartialDisk (GLdouble inner_radius, GLdouble outer_radius,
														int slices,int rings, GLdouble start_angle, GLdouble sweep_angle)
				throw(NoQuadricObjectAvailable);

			/** Draw a disk.
					This method is a wrapper method for the OpenGL function <b> gluDisk</b>.
					\param    inner_radius the inner radius of the disk.
					\param    outer_radius the outer radius of the disk.
					\param    slices the number of slices the disk has
					\param    rings the number of rings the disk has
					\exception  NoQuadricObjectAvailable thrown if no memory is available for this glQuadricObject
			*/
			void drawDisk (GLdouble inner_radius, GLdouble outer_radius, int slices, int rings)
				throw(NoQuadricObjectAvailable);

			/** Draw a cylinder.
					This method is a wrapper method for the OpenGL function <b> gluCylinder</b>.
					\param    base_radius the base radius of the cylinder.
					\param    top_radius the top radius of the cylinder.
					\param    height the height of the cylinder.
					\param    slices the number of slices the cylinder has
					\param    stacks the number of stacks the cylinder has
					\exception  NoQuadricObjectAvailable thrown if no memory is available for this glQuadricObject
			*/
			void drawCylinder (GLdouble base_radius, GLdouble top_radius, GLdouble height, int slices, int stacks)
				throw(NoQuadricObjectAvailable);

			/** Draw a sphere.
					This method is a wrapper method for the OpenGL function <b> gluSphere</b>.
					\param    radius the radius of the sphere.
					\param    slices the number of slices the sphere has
					\param    stacks the number of stacks the sphere has
					\exception  NoQuadricObjectAvailable thrown if no memory is available for this glQuadricObject
			*/
			void drawSphere(GLdouble radius, int slices, int stacks)
				throw(NoQuadricObjectAvailable);

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{
			
			/** Internal state and consistency self-validation.
					A glQuadricObject is valid if a quadric object is already defined
					(a graphical object is created).
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of this glQuadricObject to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this glQuadricObject
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			private:

			void create_()
				throw(NoQuadricObjectAvailable);

			GLenum 					draw_style_;
			GLenum 					normals_;
			GLenum 					orientation_;
			bool 						generate_texture_coordinates_;

			GLUquadricObj* 	GLU_quadric_obj_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/RENDERING/glQuadricObject.iC>
#		endif
		
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_GLQUADRICOBJECT_H
