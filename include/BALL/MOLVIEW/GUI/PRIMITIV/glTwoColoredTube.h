// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glTwoColoredTube.h,v 1.13 2003/03/26 13:56:54 anhi Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
		using VIEW::GLObject;
		
		/** GLTwoColoredTube class.
				An instance of GLTwoColoredTube represents an instance of the geometric
				visualization of a two colored "tube".
				The class GLTwoColoredTube is derived from the classes  \link TwoColoredTube TwoColoredTube \endlink  and
				 \link GLObject GLObject \endlink . Therefore the class GLTwoColoredTube is the graphical extension
				of the geometrical definition of the class twoColoredTube.
				The drawing method from  \link GLObject GLObject \endlink  is overridden to visualize the
				twoColoredTube. OpenGL code is used for the visualization.
				See these classes for further information.  \par
				
    		\ingroup  MolviewGuiPrimitves
		*/
		class GLTwoColoredTube
			: public TwoColoredTube,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new glTwoColoredTube.
					@return      GLTwoColoredTube new constructed glTwoColoredTube
					@see         TwoColoredTube
					@see         GLObject
			*/
			GLTwoColoredTube()
				throw();

			/** Copy constructor with cloning facility.
					Constructs new glTwoColoredTube by copying the glTwoColoredTube 
					{\em GL_two_colored_tube}.
					The copy is either deep (default) or shallow.
					@param       tube the tube to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      GLTwoColoredTube new constructed glTwoColoredTube copied from {\em GL_two_colored_tube}
					@see         TwoColoredTube
					@see         GLObject
			*/
			GLTwoColoredTube(const GLTwoColoredTube& GL_two_colored_tube, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Constructs new glTwoColoredTube by copying the internal values from
					 \link GeometricObject GeometricObject \endlink  {\em geometric_object}.
					@param       geometric_object the  \link GeometricObject GeometricObject \endlink  which internal value should be copied
					@return      GLTwoColoredTube new constructed glTwoColoredTube initialized from {\em geometric_object}
					@see         TwoColoredTube
					@see         GLObject
			*/
			GLTwoColoredTube(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glTwoColoredTube.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLTwoColoredTube()
				throw();

			/** Explicit default initialization.
					Calls  \link TwoColoredTube::clear TwoColoredTube::clear \endlink 
					Calls  \link GLObject::clear GLObject::clear \endlink 
					@see  TwoColoredTube::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link TwoColoredTube::destroy TwoColoredTube::destroy \endlink 
					Calls  \link GLObject::destroy GLObject::destroy \endlink 
					@see  TwoColoredTube::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}


			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assigns the glTwoColoredTube {\em gl_two_colored_tube} to {\em *this}
					glTwoColoredTube.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glTwoColoredTube is initialized to the value of 
					the glTwoColoredTube {\em gl_two_colored_tube}.
					This method does not copy the structure of {\em gl_two_colored_tube} into {\em *this}
					glTwoColoredTube. Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are copied. 
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					Calls  \link GeometricObject::set GeometricObject::set \endlink .
					@param       gl_two_colored_tube the glTwoColoredTube to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em gl_two_colored_tube}
					@see         GeometricObject::set
					@see         GLObject::set
			*/
			void set(const GLTwoColoredTube& gl_two_colored_tube, bool deep = true)
				throw();

			/** Assignment operator.
					Assigns the glTwoColoredTube {\em gl_two_colored_tube} to {\em *this}
					glTwoColoredTube.	The copy is deep.
					This method does not copy the structure of {\em gl_two_colored_tube} into {\em *this}
					glTwoColoredTube. 
					Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are copied.
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					Calls  \link set set \endlink .
					The value of {\em *this} glTwoColoredTube is initialized to the value 
					of the glTwoColoredTube {\em gl_two_colored_tube}. \par
					@param       gl_two_colored_tube the glTwoColoredTube to be copied
					@return      GLTwoColoredTube& constant reference of {\em *this} glTwoColoredTube
					@see         set
			*/
			const GLTwoColoredTube& operator = (const GLTwoColoredTube& gl_two_colored_tube)
				throw();

			/** Copying with cloning facility.
					Copies {\em *this} glTwoColoredTube to the glTwoColoredTube {\em gl_two_colored_tube}.
					The copy is either deep (default) or shallow.
					This method does not copy the structure of {\em *this} glTwoColoredTube into
					{\em gl_two_colored_tube}. 
					Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are copied.
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					Calls  \link set set \endlink .
					The value of the glTwoColoredTube {\em gl_two_colored_tube} is initialized to the
					value of {\em *this} glTwoColoredTube. \par
					@param       gl_two_colored_tube the glTwoColoredTube to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em gl_two_colored_tube}
					@see         set
			*/
			void get(GLTwoColoredTube& gl_two_colored_tube, bool deep = true) const
				throw();

			/** Swapping of glTwoColoredTube's.
					Swaps the value of {\em *this} glTwoColoredTube with the glTwoColoredTube
					{\em gl_two_colored_tube}.
					This method does not swap the structure of {\em gl_two_colored_tube} with {\em *this}
					glTwoColoredTube. 
					Only the geometric properties and the the access to the
					 \link GLPrimitiveManager GLPrimitiveManager \endlink  are swapped.
					See  \link GeometricObject GeometricObject \endlink  and	 \link GLObject GLObject \endlink  for information.
					@param       gl_two_colored_tube the glTwoColoredTube being swapped with {\em *this} glTwoColoredTube
			*/
			void swap(GLTwoColoredTube& gl_two_colored_tube)
				throw();

			//@}

			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from  \link GLObject GLObject \endlink . Uses the geometrical properties
					defined in class  \link TwoColoredTube TwoColoredTube \endlink  to create the graphical representation
					of the twoColoredTube. This method is used internally by the render engine
					of the  \link Scene Scene \endlink . Predefined shapes of tube objects from the class
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
					This method handles the export of {\em *this} glTwoColoredTube into another
					format (eg. POVRAY, VRML) \par
					Calls  \link TwoColoredTube::extract TwoColoredTube::extract \endlink .
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();
			//@}

			
			private:

			void draw_
				(const Vector3& vertex_1, const Vector3& vertex_2,
				 const Vector3& rotation_axis, const Real angle);
		};
	} // namespace MOLVIEW

} // namespace BALL
			
#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
