// $Id: glQuadricObject.h,v 1.4 2001/02/11 13:04:39 hekl Exp $

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
		
		/** GLQuadricObject class.
				
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/KERNEL/glQuadricObject.h}
				\\
				The class QLQuadricObject is a wrapper class for the quadric objects
				of the OpenGL graphical library. It defines some useful methods to define
				and create quadric objects. Further this class hides the handling
				of OpenGL quadric objects and defines an easy interface for creating those
				quadric objects.
				See the documentation of th OpenGL graphical library for information
				concerning quadric objects.

				@memo    GLQuadricObject class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/02/11 13:04:39 $
		*/
		class GLQuadricObject
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** WrongDrawingStyle exception class.
					This exception will be thrown if a wrong drawing style for {\em *this}
					glQuadricObject will be used. See the documentation of the OpenGL
					graphical library for drawing styles of quadric objects.

					@see         Exception::GeneralException			
			*/
			class WrongDrawingStyle:	public Exception::GeneralException
			{
				public:

				WrongDrawingStyle(const char* file, int line)
					throw();
			};

			/** WrongOrientationStyle exception class.
					This exception will be thrown if a wrong orientation style for {\em *this}
					glQuadricObject will be used. See the documentation of the OpenGL
					graphical library for orientation styles of quadric objects.

					@see         Exception::GeneralException			
			*/
			class WrongOrientationStyle:	public Exception::GeneralException
			{
				public:

				WrongOrientationStyle(const char* file, int line)
					throw();
			};

			/** WrongNormalStyle exception class.
					This exception will be thrown if a wrong normal style for {\em *this}
					glQuadricObject will be used. See the documentation of the OpenGL
					graphical library for normal styles of quadric objects.

					@see         Exception::GeneralException			
			*/
			class WrongNormalStyle:	public Exception::GeneralException
			{
				public:

				WrongNormalStyle(const char* file, int line)
					throw();
			};


			/** NoQuadricObjectAvailable exception class.
					This exception will be thrown if no memory for {\em *this} glQuadricObject
					is available.

					@see         Exception::GeneralException			
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
					The state of {\em *this} glQuadricObject is set to:
					\begin{itemize}
					  \item drawing style is set to {\tt GLU_FILL} (Default)
					  \item normal style is set to {\tt GLU_FLAT_FILL} (Default)
					  \item orientation style is set to {\tt GLU_OUTSIDE} (Default)
					  \item generate texture coordinates is set to {\tt false} (Default)
					\end{itemize}
					See the documentation of the OpenGL	graphical library for the various
					styles of quadric objects.

					@return      GLQuadricObject - new constructed glQuadricObject
			*/
			GLQuadricObject
				(int draw_style = GLU_FILL, int normals = GLU_FLAT,
				 int orientation = GLU_OUTSIDE, bool generate_texture_coordinates = false);

			/** Copy constructor.
					Construct new glQuadricObject by copying the glQuadricObject 
					{\em GL_quadric_object}.
					{\em *this} glQuadricObject is initialized to the glQuadricObject
					{\em GL_quadric_object}.\\

					@param       GL_quadric_object the glQuadricObject to be copied 
					@return      GLQuadricObject - new constructed glQuadricObject copied from {\em GL_quadric_object}
					@see         GLQuadricObject::GLQuadricObject
			*/
			GLQuadricObject(const GLQuadricObject& GL_quadric_object);
		
			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} glQuadricObject.
					Calls \Ref{GLQuadricObject::destroy}.
					@see         GLQuadricObject::destroy
			*/
			virtual ~GLQuadricObject();

			/** Explicit default initialization.
					Resets the values of {\em *this} glQuadricObject to the default values.

					@see    GLQuadricObject::GLQuadricObject
			*/
			virtual void clear();

			/** Explicit destructor.
					Deletes the internally allocated OpenGL quadric object if already allocated.
					Calls \Ref{GLQuadricObject::clear}
					
					@see  GLQuadricObject::clear
			*/
			virtual void destroy();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glQuadricObject {\em GL_quadric_object} to {\em *this}
					glQuadricObject.
					The state of {\em *this} glQuadricObject is initialized to the state
					of the glQuadricObject {\em GL_quadric_object}

					@param       GL_quadric_object the glQuadricObject to be copied
					@see         GLQuadricObject::GLQuadricObject
			*/
			void set(const GLQuadricObject& GL_quadric_object);

			/** Assignment operator.
					Assign the glQuadricObject {\em GL_quadric_object} to {\em *this}
					glQuadricObject.
					The state of {\em *this} glQuadricObject is initialized to the state
					of the glQuadricObject {\em GL_quadric_object}
					Calls \Ref{GLQuadricObject::set}.

					@param       GL_quadric_object the glObject to be copied
					@return      GLQuadricObject& - {\em *this} glQuadricObject
					@see         GLQuadricObject::set
			*/
			GLQuadricObject& operator = (const GLQuadricObject& GL_quadric_object);

			/** Copying.
					Copy {\em *this} glQuadricObject to the glQuadricObject 
					{\em GL_quadric_object}.
					The state of {\em *this} glQuadricObject is initialized to the state
					of the glQuadricObject {\em GL_quadric_object}
					Calls \Ref{GLQuadricObject::set}.

					@param       GL_quadric_object the glQuadricObject to be assigned to
					@see         GLQuadricObject::set
			*/
			void get(GLQuadricObject& GL_quadric_object) const;

			/** Swapping of glQuadricObjects.
					Swap the state of {\em *this} glQuadricObject with the state of the
					glQuadricObject	{\em GL_quadric_object}.

					@param       GL_quadric_object the glQuadricObject being swapped with {\em *this} glQuadricObject 
					@see         GLQuadricObject::GLQuadricObject
			*/
			void swap(GLQuadricObject& GL_quadric_object);
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the drawing style.
					Change the drawing style of {\em *this} glQuadricObject to the value
					represented by the parameter {\em style}.
					See the documentation of the OpenGL	graphical library for drawing
					styles of quadric objects.

					@param       style the new drawing style of {\em *this} glQuadricObject
					@see         GLQuadricObject::getDrawStyle
					@exception   WrongDrawingStyle - thrown whenever the drawing style is not allowed.
			*/
			void setDrawStyle(int style)
				throw();

			/** Inspection of the drawing style.
					Access the drawing style of {\em *this} glQuadricObject.
					See the documentation of the OpenGL	graphical library for drawing
					styles of quadric objects.

					@return      int - the drawing style of {\em *this} glQuadricObject
					@see         GLQuadricObject::setDrawStyle
			*/
			int getDrawStyle() const;

			/** Change the orientation style.
					Change the orientation style of {\em *this} glQuadricObject to the value
					represented by the parameter {\em orientation}.
					See the documentation of the OpenGL	graphical library for orientation
					styles of quadric objects.

					@param       orientation the new orientation style of {\em *this} glQuadricObject
					@see         GLQuadricObject::getOrientation
					@exception   WrongOrientationStyle - thrown whenever the orientation style is not allowed.
			*/
			void setOrientation(int orientation)
				throw();

			/** Inspection of the orientation style.
					Access the orientation style of {\em *this} glQuadricObject.
					See the documentation of the OpenGL	graphical library for orientation
					styles of quadric objects.

					@return      int - the orientation style of {\em *this} glQuadricObject
					@see         GLQuadricObject::setOrientation
			*/
			int getOrientation() const;

			/** Change the texture coordinate generation.
					Turn the texture coordinate generation of {\em *this} glQuadricObject on or
					off according to the value represented by the parameter {\em generate}.
					See the documentation of the OpenGL	graphical library for information
					concerning texture coordinate generation of quadric objects.

					@param       generate if set to {\tt true} the texture coordinate generation of {\em *this} glQuadricObject will be turned on; turned off if set to {\tt false}
					@see         GLQuadricObject::getTextureCoordinateGeneration
			*/
			void setTextureCoordinateGeneration(bool generate);

			/** Inspection of the texture coordinate generation switch.
					Is the texture coordinate generation of {\em *this} glQuadricObject
					turned on {\tt true}, or turned off {\tt false}.
					
					@return    bool - {\tt true} if the texture coordinate generation of {\em *this} glQuadricObject will be turned on, {\tt false} otherwise
					@see         GLQuadricObject::setTextureCoordinateGeneration
			*/
			bool getTextureCoordinateGeneration() const;

			/** Change the normal style.
					Change the normal style of {\em *this} glQuadricObject to the value
					represented by the parameter {\em normals}.
					See the documentation of the OpenGL	graphical library for normal
					styles of quadric objects.

					@param       normals the new normal style of {\em *this} glQuadricObject
					@see         GLQuadricObject::getNormals
					@exception   WrongNormalStyle - thrown whenever the normal style is not allowed.
			*/
			void setNormals(int normals)
				throw();

			/** Inspection of the normal style.
					Access the normal style of {\em *this} glQuadricObject.
					See the documentation of the OpenGL	graphical library for normal
					styles of quadric objects.

					@return      int - the normal style of {\em *this} glQuadricObject
					@see         GLQuadricObject::setNormals
			*/
			int getNormals() const;

			//@}

			/** @name Drawing methods for quadric objects.
			*/
			//@{
			
			/** Draw a partial disk.
					This method draws a partial disk with the given parameter.
					It uses the properties of {\em *this} glQuadricObject for drawing
					the partial disk.
					This method is a wrapper method for the OpenGL function {\em gluPartialDisk}.
					See documentation of the OpenGL graphical library for information
					on the function {\em gluPartialDisk} in the quadric object section.

					@param    inner_radius the inner radius of the partial disk.
					@param    outer_radius the outer radius of the partial disk.
					@param    slices the number of slices the partial disk have
					@param    rings the number of rings the partial disk have
					@param    start_angle the start angle of the partial disk.
					@param    sweep_angle the sweep angle of the partial disk.
					@exception  NoQuadricObjectAvailable - thrown if no memory is available for {\em *this} glQuadricObject
			*/
			void drawPartialDisk
				(GLdouble inner_radius, GLdouble outer_radius,
				 int slices,int rings,
				 GLdouble start_angle, GLdouble sweep_angle)
				throw();

			/** Draw a disk.
					This method draws a disk with the given parameter.
					It uses the properties of {\em *this} glQuadricObject for drawing
					the disk.
					This method is a wrapper method for the OpenGL function {\em gluDisk}.
					See documentation of the OpenGL graphical library for information
					on the function {\em gluDisk} in the quadric object section.

					@param    inner_radius the inner radius of the disk.
					@param    outer_radius the outer radius of the disk.
					@param    slices the number of slices the disk have
					@param    rings the number of rings the disk have
					@exception  NoQuadricObjectAvailable - thrown if no memory is available for {\em *this} glQuadricObject
			*/
			void drawDisk
				(GLdouble inner_radius, GLdouble outer_radius,
				 int slices, int rings)
				throw();

			/** Draw a cylinder.
					This method draws a cylinder with the given parameter.
					It uses the properties of {\em *this} glQuadricObject for drawing
					the cylinder.
					This method is a wrapper method for the OpenGL function {\em gluCylinder}.
					See documentation of the OpenGL graphical library for information
					on the function {\em gluCylinder} in the quadric object section.

					@param    base_radius the base radius of the cylinder.
					@param    top_radius the top radius of the cylinder.
					@param    height the height of the cylinder.
					@param    slices the number of slices the cylinder have
					@param    stacks the number of stacks the cylinder have
					@exception  NoQuadricObjectAvailable - thrown if no memory is available for {\em *this} glQuadricObject
			*/
			void drawCylinder
				(GLdouble base_radius, GLdouble top_radius, GLdouble height,
				 int slices, int stacks)
				throw();

			/** Draw a sphere.
					This method draws a sphere with the given parameter.
					It uses the properties of {\em *this} glQuadricObject for drawing
					the sphere.
					This method is a wrapper method for the OpenGL function {\em gluSphere}.
					See documentation of the OpenGL graphical library for information
					on the function {\em gluSphere} in the quadric object section.

					@param    radius the radius of the sphere.
					@param    slices the number of slices the sphere have
					@param    stacks the number of stacks the sphere have
					@exception  NoQuadricObjectAvailable - thrown if no memory is available for {\em *this} glQuadricObject
			*/
			void drawSphere(GLdouble radius, int slices, int stacks)
				throw();

			//@}
			
			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure 
					consistencies	of {\em *this} glQuadricObject.
					If the internal state of {\em *this} glQuadricObject is correct
					(self-validated) and consistent {\tt true} is returned,
					{\tt false} otherwise. 
					{\em *this} glQuadricObject is valid if a quadric object is already defined
					(a graphical object is created).

					@return			bool -
											{\tt true} if the internal state of {\em *this} glQuadricObject is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state of {\em *this} glQuadricObject to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the state of {\em *this} glQuadricObject
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{
			/** Persistent stream output and state restorage.
  			 Read persistent glQuadricObject data from the input stream {\em s} and 
				 restore the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s input stream from where to restore the internal state of {\em *this} glQuadricObject
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			 Write persistent glQuadricObject data to the output stream {\em s} and 
				 store the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s output stream to where to store the internal state of {\em *this} glQuadricObject
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			
			private:

			void create_()
				throw();

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
