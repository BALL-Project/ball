// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: gltube.h,v 1.12 2003/02/25 13:28:17 sturm Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#define BALL_VIEW_GUI_PRIMITIV_GLTUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif


namespace BALL
{

	namespace VIEW
	{
    /** \ingroup ViewGuiPrimitives
     *  @{
     */
		/** GLTube class.
				An instance of GLTube represents an instance of the geometric
				visualization "tube".
				The class GLTube is derived from the classes  \link Tube Tube \endlink  and
				 \link GLObject GLObject \endlink . Therefore the class glTube is the graphical extension
				of the geometrical definition of the class tube.
				The drawing method from  \link GLObject GLObject \endlink  is overridden to visualize the
				tube. OpenGL code is used for the visualization.
				See these classes for further information.
				<b>Definition:</b> BALL/VIEW/GUI/PRIMITIV/gltube.h
		*/
		class GLTube
			: public Tube,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glTube.
					@return      GLTube new constructed glTube
					@see         Tube
					@see         GLObject
			*/
			GLTube()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glTube by copying the glTube {\em tube}.
					The copy is either deep (default) or shallow.
					@param       tube the tube to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      GLTube new constructed glTube copied from {\em tube}
					@see         Tube
					@see         GLObject
			*/
			GLTube(const GLTube& tube, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glTube by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLTube new constructed glTube initialized from {\em geometric_object}
					@see         Tube
					@see         GLObject
			*/
			GLTube(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glTube.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLTube()
				throw();

			/** Explicit default initialization.
					Calls  \link Tube::clear Tube::clear \endlink 
					Calls  \link GLObject::clear GLObject::clear \endlink 
					@see  Tube::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link Tube::destroy Tube::destroy \endlink 
					Calls  \link GLObject::destroy GLObject::destroy \endlink 
					@see  Tube::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}
				
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glTube {\em tube} to {\em *this} glTube.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glTube is initialized to the value of 
					the glTube {\em tube}. \par
					@param       tube the glTube to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em tube}
					@see         Tube
					@see         GLObject
			*/
			void set(const GLTube& tube, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glTube {\em tube} to {\em *this} glTube.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} glTube is initialized to the value 
					of the glTube {\em tube}. \par
					@param       tube the glTube to be copied
					@return      GLTube& constant reference {\em *this} glTube
					@see         set
			*/
			const GLTube& operator = (const GLTube& tube)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glTube to the glTube {\em tube}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the glTube {\em tube} is initialized to the
					value of {\em *this} glTube. \par
					@param       tube the glTube to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em tube}
					@see         set
			*/
			void get(GLTube& tube, bool deep = true) const
				throw();

			/** Swapping of Tube's.
					Swap the value of {\em *this} Tube with the Tube {\em tube}.
					@param       tube the Tube being swapped with {\em *this} Tube 
					@see         GLTube
			*/
			void swap(GLTube& tube)
				throw();
			//@}



			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from  \link GLObject GLObject \endlink . Uses the geometrical properties
					defined in class  \link Tube Tube \endlink  to create the graphical representation
					of the tube. This method is used internally from the render engine
					of the  \link Scene Scene \endlink . Predefined shapes of tubes from the class
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
					This method handles the export of {\em *this} glTube into another
					format (eg. POVRAY, VRML) \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLTUBE_H 
