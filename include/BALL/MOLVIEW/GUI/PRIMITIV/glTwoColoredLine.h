// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glTwoColoredLine.h,v 1.12 2003/03/03 14:18:08 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H

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
    /**  \addtogroup  MolviewGuiPrimitves
     *  @{
     */
		using VIEW::GLObject;
		
		/** GLTwoColoredLine class.
				An instance of GLTwoColoredLine represents an instance of the geometric
				visualization of a two colored "line".
				The class GLTwoColoredLine is derived from the classes  \link TwoColoredLine TwoColoredLine \endlink  and
				 \link GLObject GLObject \endlink . Therefore the class GLTwoColoredLine is the graphical extension
				of the geometrical definition of the class twoColoredLine.
				The drawing method from  \link GLObject GLObject \endlink  is overridden to visualize the
				twoColoredLine. OpenGL code is used for the visualization.
				See these classes for further information.  \par
				<b>Definition:</b> BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredLine.h
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
					Constructs new glTwoColoredLine.
					@return      GLTwoColoredLine new constructed glTwoColoredLine
					@see         TwoColoredLine
					@see         GLObject
			*/
			GLTwoColoredLine()
				throw();

			/** Copy constructor with cloning facility.
					Constructs new glTwoColoredLine by copying the glTwoColoredLine 
					{\em GL_two_colored_line}.
					The copy is either deep (default) or shallow.
					@param       line the line to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      GLTwoColoredLine new constructed glTwoColoredLine copied from {\em GL_two_colored_line}
					@see         TwoColoredLine
					@see         GLObject
			*/
			GLTwoColoredLine(const GLTwoColoredLine& GL_two_colored_line, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Constructs new glTwoColoredLine by copying the internal values from
					 \link GeometricObject GeometricObject \endlink  {\em geometric_object}.
					@param       geometric_object the  \link GeometricObject GeometricObject \endlink  which internal value should be copied
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLTwoColoredLine()
				throw();

			/** Explicit default initialization.
					Calls  \link TwoColoredLine::clear TwoColoredLine::clear \endlink 
					Calls  \link GLObject::clear GLObject::clear \endlink 
					@see  TwoColoredLine::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link TwoColoredLine::destroy TwoColoredLine::destroy \endlink 
					Calls  \link GLObject::destroy GLObject::destroy \endlink 
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
					Assigns the glTwoColoredLine {\em gl_two_colored_line} to {\em *this}
					glTwoColoredLine.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glTwoColoredLine is initialized to the value of 
					the glTwoColoredLine {\em gl_two_colored_line}.
					This method does not copy the structure of {\em gl_two_colored_line} into {\em *this}
					glTwoColoredLine. Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are copied. 
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					Calls  \link GeometricObject::set GeometricObject::set \endlink .
					@param       gl_two_colored_line the glTwoColoredLine to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em gl_two_colored_line}
					@see         GeometricObject::set
					@see         GLObject::set
			*/
			void set(const GLTwoColoredLine& gl_two_colored_line, bool deep = true)
				throw();

			/** Assignment operator.
					Assigns the glTwoColoredLine {\em gl_two_colored_line} to {\em *this}
					glTwoColoredLine.	The copy is deep.
					This method does not copy the structure of {\em gl_two_colored_line} into {\em *this}
					glTwoColoredLine. 
					Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are copied.
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					Calls  \link set set \endlink .
					The value of {\em *this} glTwoColoredLine is initialized to the value 
					of the glTwoColoredLine {\em gl_two_colored_line}. \par
					@param       gl_two_colored_line the glTwoColoredLine to be copied
					@return      GLTwoColoredLine& constant reference of {\em *this} glTwoColoredLine
					@see         set
			*/
			const GLTwoColoredLine& operator = (const GLTwoColoredLine& gl_two_colored_line)
				throw();

			/** Copying with cloning facility.
					Copies {\em *this} glTwoColoredLine to the glTwoColoredLine {\em gl_two_colored_line}.
					The copy is either deep (default) or shallow.
					This method does not copy the structure of {\em *this} glTwoColoredLine into
					{\em gl_two_colored_line}. 
					Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are copied.
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					Calls  \link set set \endlink .
					The value of the glTwoColoredLine {\em gl_two_colored_line} is initialized to the
					value of {\em *this} glTwoColoredLine. \par
					@param       gl_two_colored_line the glTwoColoredLine to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em gl_two_colored_line}
					@see         set
			*/
			void get(GLTwoColoredLine& gl_two_colored_line, bool deep = true) const
				throw();

			/** Swapping of glTwoColoredLine's.
					Swaps the value of {\em *this} glTwoColoredLine with the glTwoColoredLine
					{\em gl_two_colored_line}.
					This method does not swap the structure of {\em gl_two_colored_line} with {\em *this}
					glTwoColoredLine. 
					Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are swapped.
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
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
					Overridden method from  \link GLObject GLObject \endlink . Uses the geometrical properties
					defined in class  \link TwoColoredLine TwoColoredLine \endlink  to create the graphical representation
					of the line. This method is used internally by the render engine
					of the  \link Scene Scene \endlink . Predefined shapes of lines from the class
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are used for the variable drawing resolutions.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					@param     with_names flag if the graphical objects must have a name
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
					@see       GLObject::draw
					@see       GLPrimitiveManager
			*/
			virtual bool draw(bool with_names = false)
				throw();

			/** Export method.
					This method handles the export of {\em *this} glTwoColoredLine into another
					format (eg. POVRAY, VRML) \par
					Calls  \link TwoColoredLine::extract TwoColoredLine::extract \endlink .
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};
     /** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDLINE_H 
