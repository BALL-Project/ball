// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glQuadricObject.h,v 1.2 2003/08/26 08:05:08 oliver Exp $

#ifndef BALL_VIEW_OPENGL_GLQUADRICOBJECT_H
#define BALL_VIEW_OPENGL_GLQUADRICOBJECT_H

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
				concerning quadric objects.\par
		*/
		class GLQuadricObject
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
					Construct new glQuadricObject.
					The state of this glQuadricObject is set to:
					  - drawing style is set to <tt> GLU_FILL</tt> (Default)
					  - normal style is set to <tt> GLU_FLAT_FILL</tt> (Default)
					  - orientation style is set to <tt> GLU_OUTSIDE</tt> (Default)
					  - generate texture coordinates is set to <tt> false</tt> (Default)
					\par
					See the documentation of the OpenGL	graphical library for the various
					styles of quadric objects.
					\return      GLQuadricObject new constructed glQuadricObject
			*/
			GLQuadricObject
				(int draw_style = GLU_FILL, int normals = GLU_FLAT,
				 int orientation = GLU_OUTSIDE, bool generate_texture_coordinates = false)
				throw();

			/** Copy constructor.
					Construct new glQuadricObject by copying the glQuadricObject 
					<b> GL_quadric_object</b>.
					this glQuadricObject is initialized to the glQuadricObject
					<b> GL_quadric_object</b>.\par
					\param       GL_quadric_object the glQuadricObject to be copied 
					\return      GLQuadricObject new constructed glQuadricObject copied from <b> GL_quadric_object</b>
			*/
			GLQuadricObject(const GLQuadricObject& GL_quadric_object)
				throw();
		
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of this glQuadricObject.
					Calls destroy.
					\see         destroy
			*/
			virtual ~GLQuadricObject()
				throw();

			/** Explicit default initialization.
					Resets the values of this glQuadricObject to the default values.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Deletes the internally allocated OpenGL quadric object if already allocated.
					Calls clear
					\see  clear
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glQuadricObject <b> GL_quadric_object</b> to this
					glQuadricObject.
					The state of this glQuadricObject is initialized to the state
					of the glQuadricObject <b> GL_quadric_object</b>
					\param       GL_quadric_object the glQuadricObject to be copied
			*/
			void set(const GLQuadricObject& GL_quadric_object)
				throw();

			/** Assignment operator.
					Assign the glQuadricObject <b> GL_quadric_object</b> to this glQuadricObject.
					The state of this glQuadricObject is initialized to the state
					of the glQuadricObject <b> GL_quadric_object</b>
					Calls set.
					\param       GL_quadric_object the glObject to be copied
					\return      GLQuadricObject& constant reference of this glQuadricObject
					\see         set
			*/
			const GLQuadricObject& operator = (const GLQuadricObject& GL_quadric_object)
				throw();

			/** Copying.
					Copy this glQuadricObject to the glQuadricObject <b> GL_quadric_object</b>.
					The state of this glQuadricObject is initialized to the state
					of the glQuadricObject <b> GL_quadric_object</b>
					Calls set.
					\param       GL_quadric_object the glQuadricObject to be assigned to
					\see         set
			*/
			void get(GLQuadricObject& GL_quadric_object) const
				throw();

			/** Swapping of glQuadricObjects.
					Swap the state of this glQuadricObject with the state of <b> GL_quadric_object</b>.
					\param       GL_quadric_object the glQuadricObject being swapped with this glQuadricObject 
			*/
			void swap(GLQuadricObject& GL_quadric_object)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the drawing style.
					Change the drawing style of this glQuadricObject to the value
					represented by the parameter <b> style</b>.
					See the documentation of the OpenGL	graphical library for drawing styles of quadric objects.
					\param       style the new drawing style of this glQuadricObject
					\see         getDrawStyle
					\exception   WrongDrawingStyle thrown whenever the drawing style is not allowed.
			*/
			void setDrawStyle(int style)
				throw(WrongDrawingStyle);

			/** Inspection of the drawing style.
					Access the drawing style of this glQuadricObject.
					See the documentation of the OpenGL	graphical library for drawing
					styles of quadric objects.
					\return      int the drawing style of this glQuadricObject
					\see         setDrawStyle
			*/
			int getDrawStyle() const
				throw();

			/** Change the orientation style.
					Change the orientation style of this glQuadricObject to the value
					represented by the parameter <b> orientation</b>.
					See the documentation of the OpenGL	graphical library for orientation
					styles of quadric objects.
					\param       orientation the new orientation style of this glQuadricObject
					\see         getOrientation
					\exception   WrongOrientationStyle thrown whenever the orientation style is not allowed.
			*/
			void setOrientation(int orientation)
				throw(WrongOrientationStyle);

			/** Inspection of the orientation style.
					Access the orientation style of this glQuadricObject.
					See the documentation of the OpenGL	graphical library for orientation
					styles of quadric objects.
					\return      int the orientation style of this glQuadricObject
					\see         setOrientation
			*/
			int getOrientation() const
				throw();

			/** Change the texture coordinate generation.
					Turn the texture coordinate generation of this glQuadricObject on or
					off according to the value represented by the parameter <b> generate</b>.
					See the documentation of the OpenGL	graphical library for information
					concerning texture coordinate generation of quadric objects.
					\param       generate if set to <tt> true</tt> the texture coordinate generation of this glQuadricObject will be turned on; 
											 turned off if set to <tt> false</tt>
					\see         getTextureCoordinateGeneration
			*/
			void setTextureCoordinateGeneration(bool generate)
				throw();

			/** Inspection of the texture coordinate generation switch.
					Is the texture coordinate generation of this glQuadricObject
					turned on <tt> true</tt>, or turned off <tt> false</tt>.
					\return    bool <tt> true</tt> if the texture coordinate generation of this glQuadricObject will be turned on, <tt> false</tt> otherwise
					\see       setTextureCoordinateGeneration
			*/
			bool getTextureCoordinateGeneration() const
				throw();

			/** Change the normal style.
					Change the normal style of this glQuadricObject to the value
					represented by the parameter <b> normals</b>.
					See the documentation of the OpenGL	graphical library for normal
					styles of quadric objects.
					\param       normals the new normal style of this glQuadricObject
					\see         getNormals
					\exception   WrongNormalStyle thrown whenever the normal style is not allowed.
			*/
			void setNormals(int normals)
				throw(WrongNormalStyle);

			/** Inspection of the normal style.
					Access the normal style of this glQuadricObject.
					See the documentation of the OpenGL	graphical library for normal
					styles of quadric objects.
					\return      int the normal style of this glQuadricObject
					\see         setNormals
			*/
			int getNormals() const
				throw();

			//@}

			/** @name Drawing methods for quadric objects.
			*/
			//@{
			
			/** Draw a partial disk.
					This method draws a partial disk with the given parameter.
					It uses the properties of this glQuadricObject for drawing
					the partial disk.
					This method is a wrapper method for the OpenGL function <b> gluPartialDisk</b>.
					See documentation of the OpenGL graphical library for information
					on the function <b> gluPartialDisk</b> in the quadric object section.
					\param    inner_radius the inner radius of the partial disk.
					\param    outer_radius the outer radius of the partial disk.
					\param    slices the number of slices the partial disk has
					\param    rings the number of rings the partial disk has
					\param    start_angle the start angle of the partial disk.
					\param    sweep_angle the sweep angle of the partial disk.
					\exception  NoQuadricObjectAvailable thrown if no memory is available for this glQuadricObject
			*/
			void drawPartialDisk
				(GLdouble inner_radius, GLdouble outer_radius,
				 int slices,int rings,
				 GLdouble start_angle, GLdouble sweep_angle)
				throw(NoQuadricObjectAvailable);

			/** Draw a disk.
					This method draws a disk with the given parameter.
					It uses the properties of this glQuadricObject for drawing
					the disk.
					This method is a wrapper method for the OpenGL function <b> gluDisk</b>.
					See documentation of the OpenGL graphical library for information
					on the function <b> gluDisk</b> in the quadric object section.
					\param    inner_radius the inner radius of the disk.
					\param    outer_radius the outer radius of the disk.
					\param    slices the number of slices the disk has
					\param    rings the number of rings the disk has
					\exception  NoQuadricObjectAvailable thrown if no memory is available for this glQuadricObject
			*/
			void drawDisk (GLdouble inner_radius, GLdouble outer_radius, int slices, int rings)
				throw(NoQuadricObjectAvailable);

			/** Draw a cylinder.
					This method draws a cylinder with the given parameter.
					It uses the properties of this glQuadricObject for drawing
					the cylinder.
					This method is a wrapper method for the OpenGL function <b> gluCylinder</b>.
					See documentation of the OpenGL graphical library for information
					on the function <b> gluCylinder</b> in the quadric object section.
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
					This method draws a sphere with the given parameter.
					It uses the properties of this glQuadricObject for drawing the sphere.
					This method is a wrapper method for the OpenGL function <b> gluSphere</b>.
					See documentation of the OpenGL graphical library for information
					on the function <b> gluSphere</b> in the quadric object section.
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
					Initiate self-validation of the internal state and data structure 
					consistencies	of this glQuadricObject.
					If the internal state of this glQuadricObject is correct
					(self-validated) and consistent <tt> true</tt> is returned,
					<tt> false</tt> otherwise. 
					this glQuadricObject is valid if a quadric object is already defined
					(a graphical object is created).
					\return			bool <tt> true</tt> if the internal state of this glQuadricObject is correct (self-validated) and consistent,
					 						<tt> false</tt> otherwise
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

			int 						draw_style_;
			int 						normals_;
			int 						orientation_;
			bool 						generate_texture_coordinates_;

			GLUquadricObj* 	GLU_quadric_obj_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/OPENGL/glQuadricObject.iC>
#		endif
		
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLQUADRICOBJECT_H
