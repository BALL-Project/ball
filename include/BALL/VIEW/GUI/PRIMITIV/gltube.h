// $Id: gltube.h,v 1.5 2001/02/11 13:04:40 hekl Exp $

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

		/** GLTube class.
				
				{\bf Framework:} BALL/VIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/gltube.h}
				\\

				An instance of GLTube represents an instance of the geometric
				visualization "tube".
				The class GLTube is derived from the classes \Ref{Tube} and
				\Ref{GLObject}. Therefore the class glTube is the graphical extension
				of the geometrical definition of the class tube.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				tube. OpenGL code is used for the visualization.
				See these classes for further information.

				@memo    GLTube class (BALL VIEW gui primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/02/11 13:04:40 $
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

					@return      GlTube - new constructed glTube
					@see         Tube::Tube
					@see         GLObject::GLObject
			*/
			GLTube();

			/** Copy constructor with cloning facility.
					Construct new glTube by copying the glTube {\em tube}.
					The copy is either deep (default) or shallow.

					@param       tube the tube to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GlTube - new constructed glTube copied from {\em tube}
					@see         Tube::Tube
					@see         GLObject::GLObject
			*/
			GLTube(const GLTube& tube, bool deep = true);

			/** Copy constructor from geometricObject.
					Construct new glTube by copying the internal values from geometricObject 
					{\em geometric_object}.

					@param       geometric_object the geometricObject which internal value should be copied
					@return      GlTube - new constructed glTube initialized from {\em geometric_object}
					@see         Tube::Tube
					@see         GLObject::GLObject
			*/
			GLTube(const GeometricObject& geometric_object);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} glTube.
					Calls \Ref{GlTube::destroy}.
					@see         GlTube::destroy
			*/
			virtual ~GLTube()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Tube::clear}
					Calls \Ref{GLObject::clear}

					@see  Tube::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Tube::destroy}
					Calls \Ref{GLObject::destroy}

					@see  Tube::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();
			//@}
				


			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Tube} to create the graphical representation
					of the tube. This method is used internally from the render engine
					of the \Ref{Scene}. Predefined shapes of tubes from the class
					\Ref{GLPrimitiveManager} are used for the variable drawing resolutions.
					The parameter {\em with_names} indicates whether the openGL command 
					{\em glLoadName} must be used for naming the graphical object 
					(necessary for picking mode in the scene).
					
					@param     with_names flag if the graphical objects must have a name
					@return    bool - {\tt true} if successful,	{\tt false} otherwise
					@see       GLObject::draw
					@see       GLPrimitiveManager::GLPrimitiveManager
			*/
			virtual bool draw(bool with_names = false);

			/** Experimental method.
					Please avoid using this method.
			*/
			virtual bool drawUserDefined();

			/** Export method.
					This method handles the export of {\em *this} glTube into another
					format (eg. POVRAY, VRML)
				  \\
				  {\bf Note:} Not yet implemented.

					@return    bool - {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

			//@}
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/gltube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLTUBE_H 
