// $Id: glsimpleBox.h,v 1.7 2001/07/16 14:49:30 amoll Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSIMPLEBOX_H
#define BALL_VIEW_GUI_PRIMITIV_GLSIMPLEBOX_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SIMPLEBOX_H
#	include <BALL/VIEW/PRIMITIV/simpleBox.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** GLSimpleBox class.
				An instance of GLSimpleBox represents an instance of the geometric
				visualization "simpleBox".
				The class GLSimpleBox is derived from the classes \Ref{SimpleBox} and
				\Ref{GLObject}. Therefore the class glSimpleBox is the graphical extension
				of the geometrical definition of the class simpleBox.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				simpleBox. OpenGL code is used for the visualization.
				See these classes for further information.
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h}\\ \\
		*/
		class GLSimpleBox
			: public SimpleBox,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glSimpleBox.
					@return      GLSimpleBox new constructed glSimpleBox
					@see         SimpleBox
					@see         GLObject
			*/
			GLSimpleBox()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glSimpleBox by copying the glSimpleBox {\em simpleBox}.
					The copy is either deep (default) or shallow.
					@param       simpleBox the glSimpleBox to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLSimpleBox new constructed glSimpleBox copied from {\em simpleBox}
					@see         SimpleBox
					@see         GLObject
			*/
			GLSimpleBox(const GLSimpleBox& simpleBox, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glSimpleBox by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLSimpleBox new constructed glSimpleBox initialized from {\em geometric_object}
					@see         SimpleBox
					@see         GLObject
			*/
			GLSimpleBox(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glSimpleBox.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLSimpleBox()
				throw();

			/** Explicit default initialization.
					Calls \Ref{SimpleBox::clear}
					Calls \Ref{GLObject::clear}
					@see  SimpleBox::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{SimpleBox::destroy}
					Calls \Ref{GLObject::destroy}
					@see  SimpleBox::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glSimpleBox {\em simpleBox} to {\em *this} glSimpleBox.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glSimpleBox is initialized to the value of 
					the glSimpleBox {\em simpleBox}.\\
					@param       simpleBox the glSimpleBox to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em simpleBox}
					@see         SimpleBox
					@see         GLObject
			*/
			void set(const GLSimpleBox& simpleBox, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glSimpleBox {\em simpleBox} to {\em *this} glSimpleBox.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} glSimpleBox is initialized to the value 
					of the glSimpleBox {\em simpleBox}.\\
					@param       simpleBox the glSimpleBox to be copied
					@return      GLSimpleBox& constant reference {\em *this} glSimpleBox
					@see         set
			*/
			const GLSimpleBox& operator = (const GLSimpleBox& simpleBox)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glSimpleBox to the glSimpleBox {\em simpleBox}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the glSimpleBox {\em simpleBox} is initialized to the
					value of {\em *this} glSimpleBox.\\
					@param       simpleBox the glSimpleBox to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em simpleBox}
					@see         set
			*/
			void get(GLSimpleBox& simpleBox, bool deep = true) const
				throw();

			/** Swapping of SimpleBox's.
					Swap the value of {\em *this} SimpleBox with the SimpleBox {\em simpleBox}.
					@param       simpleBox the SimpleBox being swapped with {\em *this} SimpleBox 
					@see         GLSimpleBox
			*/
			void swap(GLSimpleBox& simpleBox)
				throw();
			//@}

			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{SimpleBox} to create the graphical representation
					of the simpleBox. This method is used internally from the render engine
					of the \Ref{Scene}. Predefined shapes of simpleBoxes from the class
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

			/** Experimental method.
					Please avoid using this method.
			*/
			virtual bool drawUserDefined()
				throw();

			/** Export method.
					This method handles the export of {\em *this} glSimpleBox into another
					format (eg. POVRAY, VRML)\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLSIMPLEBOX_H
