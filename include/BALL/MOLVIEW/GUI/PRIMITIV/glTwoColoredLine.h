// $Id: glTwoColoredLine.h,v 1.4 2001/05/13 14:55:24 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>
#endif


namespace BALL
{

	namespace MOLVIEW
	{
			
		using VIEW::GLObject;
		
		/** GLTwoColoredLine class.
				{\bf Framework:} BALL/MOLVIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h}\\ \\
				An instance of GLTwoColoredLine represents an instance of the geometric
				visualization of a two colored "line".
				The class GLTwoColoredLine is derived from the classes \Ref{TwoColoredLine} and
				\Ref{GLObject}. Therefore the class glTwoColoredLine is the graphical extension
				of the geometrical definition of the class twoColoredLine.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				twoColoredLine. OpenGL code is used for the visualization.
				See these classes for further information.
				@memo    GLTwoColoredLine class (BALL MOLVIEW gui primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.4 $
				@date    $Date: 2001/05/13 14:55:24 $
		*/
		class GLTwoColoredLine
			: public TwoColoredLine,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glTwoColoredLine.
					@return      GLTwoColoredLine new constructed glTwoColoredLine
					@see         TwoColoredLine
					@see         GLObject
			*/
			GLTwoColoredLine()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glTwoColoredLine by copying the glTwoColoredLine 
					{\em GL_two_colored_line}.
					The copy is either deep (default) or shallow.
					@param       line the line to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLTwoColoredLine new constructed glTwoColoredLine copied from {\em GL_two_colored_line}
					@see         TwoColoredLine
					@see         GLObject
			*/
			GLTwoColoredLine(const GLTwoColoredLine& GL_two_colored_line, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glTwoColoredLine by copying the internal values from
					\Ref{GeometricObject} {\em geometric_object}.
					@param       geometric_object the \Ref{GeometricObject} which internal value should be copied
					@return      GLTwoColoredLine new constructed glTwoColoredLine initialized from {\em geometric_object}
					@see         TwoColoredLine
					@see         GLObject
			*/
			GLTwoColoredLine(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glTwoColoredLine.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLTwoColoredLine()
				throw();

			/** Explicit default initialization.
					Calls \Ref{TwoColoredLine::clear}
					Calls \Ref{GLObject::clear}
					@see  TwoColoredLine::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{TwoColoredLine::destroy}
					Calls \Ref{GLObject::destroy}
					@see  TwoColoredLine::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glTwoColoredLine {\em gl_two_colored_line} to {\em *this}
					glTwoColoredLine.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glTwoColoredLine is initialized to the value of 
					the glTwoColoredLine {\em gl_two_colored_line}.
					This method does not copy the structure of {\em gl_two_colored_line} into {\em *this}
					glTwoColoredLine. Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are copied. 
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{GeometricObject::set}.
					@param       gl_two_colored_line the glTwoColoredLine to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em gl_two_colored_line}
					@see         GeometricObject::set
					@see         GLObject::set
			*/
			void set(const GLTwoColoredLine& gl_two_colored_line, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glTwoColoredLine {\em gl_two_colored_line} to {\em *this}
					glTwoColoredLine.	The copy is deep.
					This method does not copy the structure of {\em gl_two_colored_line} into {\em *this}
					glTwoColoredLine. 
					Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are copied.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{set}.
					The value of {\em *this} glTwoColoredLine is initialized to the value 
					of the glTwoColoredLine {\em gl_two_colored_line}.\\
					@param       gl_two_colored_line the glTwoColoredLine to be copied
					@return      GLTwoColoredLine& constant reference of {\em *this} glTwoColoredLine
					@see         set
			*/
			const GLTwoColoredLine& operator = (const GLTwoColoredLine& gl_two_colored_line)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glTwoColoredLine to the glTwoColoredLine {\em gl_two_colored_line}.
					The copy is either deep (default) or shallow.
					This method does not copy the structure of {\em *this} glTwoColoredLine into
					{\em gl_two_colored_line}. 
					Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are copied.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{set}.
					The value of the glTwoColoredLine {\em gl_two_colored_line} is initialized to the
					value of {\em *this} glTwoColoredLine.\\
					@param       gl_two_colored_line the glTwoColoredLine to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em gl_two_colored_line}
					@see         set
			*/
			void get(GLTwoColoredLine& gl_two_colored_line, bool deep = true) const
				throw();

			/** Swapping of glTwoColoredLine's.
					Swap the value of {\em *this} glTwoColoredLine with the glTwoColoredLine
					{\em gl_two_colored_line}.
					This method does not swap the structure of {\em gl_two_colored_line} with {\em *this}
					glTwoColoredLine. 
					Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are swapped.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					@param       gl_two_colored_line the glTwoColoredLine being swapped with {\em *this} glTwoColoredLine
			*/
			void swap(GLTwoColoredLine& gl_two_colored_line)
				throw();

			//@}


			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{TwoColoredLine} to create the graphical representation
					of the line. This method is used internally from the render engine
					of the \Ref{Scene}. Predefined shapes of lines from the class
					\Ref{GLPrimitiveManager} are used for the variable drawing resolutions.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					@param     with_names flag if the graphical objects must have a name
					@return    bool {\tt true} if successful,	{\tt false} otherwise
					@see       GLObject::draw
					@see       GLPrimitiveManager
			*/
			virtual bool draw(bool with_names = false)
				throw();

			/** Export method.
					This method handles the export of {\em *this} glTwoColoredLine into another
					format (eg. POVRAY, VRML)\\
					Calls \Ref{TwoColoredLine::extract}.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H 
