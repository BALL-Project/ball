// $Id: glBackbone.h,v 1.5 2001/07/06 14:49:30 aubertin Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLLINE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glline.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#	include <BALL/VIEW/GUI/PRIMITIV/glsphere.h>
#endif

#ifndef BALL_VIEW_GUI_PRIMITIV_GLTUBE_H
#	include <BALL/VIEW/GUI/PRIMITIV/gltube.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_BACKBONE_H
#	include <BALL/MOLVIEW/PRIMITIV/backbone.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{

		/** GlBackbone class.
				{\bf Framework:} BALL/MOLVIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.h}\\ \\
				An instance of GLBackbone represents an instance of the geometric
				visualization of a \Ref{Backbone}.
				The class GLBackbone is derived from the classes \Ref{Backbone} and
				\Ref{GLObject}. Therefore the class GLBackbone is the graphical extension
				of the geometrical definition of the class backbone.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				backbone. OpenGL code is used for the visualization.
				See these classes for further information.
				@memo    GlBackbone class (BALL MOLVIEW gui primitiv framework)
				@author  $Author: aubertin $
				@version $Revision: 1.5 $
				@date    $Date: 2001/07/06 14:49:30 $
		*/
		class GLBackbone
			: public Backbone,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Constructs new glBackbone.
					@return      GlBackbone new constructed glBackbone
					@see         Backbone
					@see         GLObject
			*/
			GLBackbone()
				throw();

			/** Copy constructor with cloning facility.
					Constructs new glBackbone by copying the glBackbone 
					{\em GL_two_colored_tube}.
					The copy is either deep (default) or shallow.
					@param       tube the tube to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GlBackbone new constructed glBackbone copied from {\em GL_two_colored_tube}
					@see         Backbone
					@see         GLObject
			*/
			GLBackbone(const GLBackbone& backbone, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Constructs new glBackbone by copying the internal values from
					\Ref{GeometricObject} {\em geometric_object}.
					@param       geometric_object the \Ref{GeometricObject} which internal value should be copied
					@return      GlBackbone new constructed glBackbone initialized from {\em geometric_object}
					@see         Backbone
					@see         GLObject
			*/
			GLBackbone(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glBackbone.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLBackbone()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Backbone::clear}
					Calls \Ref{GLObject::clear}
					@see  Backbone::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Backbone::destroy}
					Calls \Ref{GLObject::destroy}
					@see  Backbone::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();

			//@}


			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assigns the glBackbone {\em glbackbone} to {\em *this}
					glBackbone.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glBackbone is initialized to the value of 
					the glBackbone {\em glbackbone}.
					This method does not copy the structure of {\em glbackbone} into {\em *this}
					glBackbone. Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are copied. 
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{GeometricObject::set}.
					@param       glbackbone the glBackbone to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em glbackbone}
					@see         GeometricObject::set
					@see         GLObject::set
			*/
			void set(const GLBackbone& glbackbone, bool deep = true)
				throw();

			/** Assignment operator.
					Assigns the glBackbone {\em glbackbone} to {\em *this}
					glBackbone.	The copy is deep.
					This method does not copy the structure of {\em glbackbone} into {\em *this}
					glBackbone. 
					Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are copied.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{set}.
					The value of {\em *this} glBackbone is initialized to the value 
					of the glBackbone {\em glbackbone}.\\
					@param       glbackbone the glBackbone to be copied
					@return      GLBackbone& constant reference of {\em *this} glBackbone
					@see         set
			*/
			const GLBackbone& operator = (const GLBackbone& glbackbone)
				throw();

			/** Copying with cloning facility.
					Copies {\em *this} glBackbone to the glBackbone {\em glbackbone}.
					The copy is either deep (default) or shallow.
					This method does not copy the structure of {\em *this} glBackbone into
					{\em glbackbone}. 
					Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are copied.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{set}.
					The value of the glBackbone {\em glbackbone} is initialized to the
					value of {\em *this} glBackbone.\\
					@param       glbackbone the glBackbone to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em glbackbone}
					@see         set
			*/
			void get(GLBackbone& glbackbone, bool deep = true) const
				throw();

			/** Swapping of glBackbone's.
					Swaps the value of {\em *this} glBackbone with the glBackbone
					{\em glbackbone}.
					This method does not swap the structure of {\em glbackbone} with {\em *this}
					glBackbone. 
					Only the geometric properties and the the access to the
					\Ref{GLPrimitiveManager} are swapped.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					@param       glbackbone the glBackbone being swapped with {\em *this} glBackbone
			*/
			void swap(GLBackbone& glbackbone)
				throw();

			//@}


			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Backbone} to create the graphical representation
					of the backbone. This method is used internally by the render engine
					of the \Ref{Scene}. Predefined shapes of tube objects from the class
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
					This method handles the export of {\em *this} glBackbone into another
					format (eg. POVRAY, VRML)\\
					Calls \Ref{Backbone::extract}.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
			//@}

  		private:

			//_
			virtual Sphere* createSphere_();

			//_
			virtual Tube* createTube_();		

			//_
			virtual Line* createLine_();		
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/PRIMITIV/glBackbone.iC>
#		endif
		
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLBACKBONE_H
