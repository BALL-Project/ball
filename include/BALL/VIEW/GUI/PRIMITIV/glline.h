// $Id: glline.h,v 1.6 2001/05/13 13:59:13 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#define BALL_VIEW_GUI_PRIMITIV_GLLINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/** GLLine class.
				{\bf Framework:} BALL/VIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/glline.h}\\ \\
				An instance of GLLine represents an instance of the geometric
				visualization "line".
				The class GLLine is derived from the classes \Ref{Line} and
				\Ref{GLObject}. Therefore the class glLine is the graphical extension
				of the geometrical definition of the class line.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				line. OpenGL code is used for the visualization.
				See these classes for further information.
				@memo    GLLine class (BALL VIEW gui primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.6 $
				@date    $Date: 2001/05/13 13:59:13 $
		*/
		class GLLine
			: public Line,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glLine.
					@return      GLLine new constructed glLine
					@see         Line
					@see         GLObject
			*/
			GLLine()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glLine by copying the glLine {\em line}.
					The copy is either deep (default) or shallow.
					@param       line the glLine to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLLine new constructed glLine copied from {\em line}
					@see         Line
					@see         GLObject
			*/
			GLLine(const GLLine& line, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glLine by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLLine new constructed glLine initialized from {\em geometric_object}
					@see         Line
					@see         GLObject
			*/
			GLLine(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glLine.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLLine()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Line::clear}
					Calls \Ref{GLObject::clear}
					@see  Line::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Line::destroy}
					Calls \Ref{GLObject::destroy}
					@see  Line::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glLine {\em line} to {\em *this} glLine.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glLine is initialized to the value of 
					the glLine {\em line}.\\
					@param       line the glLine to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em line}
					@see         Line
					@see         GLObject
			*/
			void set(const GLLine& line, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glLine {\em line} to {\em *this} glLine.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} glLine is initialized to the value 
					of the glLine {\em line}.\\
					@param       line the glLine to be copied
					@return      GLLine& constant reference {\em *this} glLine
					@see         set
			*/
			const GLLine& operator = (const GLLine& line)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glLine to the glLine {\em line}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the glLine {\em line} is initialized to the
					value of {\em *this} glLine.\\
					@param       line the glLine to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em line}
					@see         set
			*/
			void get(GLLine& line, bool deep = true) const
				throw();

			/** Swapping of Line's.
					Swap the value of {\em *this} Line with the Line {\em line}.
					@param       line the Line being swapped with {\em *this} Line 
					@see         GLLine
			*/
			void swap(GLLine& line)
				throw();
			//@}

			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Line} to create the graphical representation
					of the line. This method is used internally from the render engine
					of the \Ref{Scene}.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					@param     with_names flag if the graphical objects must have a name
					@return    bool {\tt true} if successful,	{\tt false} otherwise
					@see       GLObject::draw
			*/
			virtual bool draw(bool with_names = false)
				throw();

			/** Export method.
					This method handles the export of {\em *this} glLine into another
					format (eg. POVRAY, VRML)\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glline.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLLINE_H 
