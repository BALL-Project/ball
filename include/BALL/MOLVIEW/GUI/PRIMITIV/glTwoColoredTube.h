// $Id: glTwoColoredTube.h,v 1.5 2001/07/06 15:06:32 aubertin Exp $

#ifndef BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
#define BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

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
				{\bf Framework:} BALL/MOLVIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.h}\\ \\
				An instance of GLTwoColoredTube represents an instance of the geometric
				visualization of a two colored "tube".
				The class GLTwoColoredTube is derived from the classes \Ref{TwoColoredTube} and
				\Ref{GLObject}. Therefore the class GLTwoColoredTube is the graphical extension
				of the geometrical definition of the class twoColoredTube.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				twoColoredTube. OpenGL code is used for the visualization.
				See these classes for further information.
				@memo    GLTwoColoredTube class (BALL MOLVIEW gui primitiv framework)
				@author  $Author: aubertin $
				@version $Revision: 1.5 $
				@date    $Date: 2001/07/06 15:06:32 $
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLTwoColoredTube new constructed glTwoColoredTube copied from {\em GL_two_colored_tube}
					@see         TwoColoredTube
					@see         GLObject
			*/
			GLTwoColoredTube(const GLTwoColoredTube& GL_two_colored_tube, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Constructs new glTwoColoredTube by copying the internal values from
					\Ref{GeometricObject} {\em geometric_object}.
					@param       geometric_object the \Ref{GeometricObject} which internal value should be copied
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLTwoColoredTube()
				throw();

			/** Explicit default initialization.
					Calls \Ref{TwoColoredTube::clear}
					Calls \Ref{GLObject::clear}
					@see  TwoColoredTube::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{TwoColoredTube::destroy}
					Calls \Ref{GLObject::destroy}
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
					\Ref{GLPrimitiveManager} are copied. 
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{GeometricObject::set}.
					@param       gl_two_colored_tube the glTwoColoredTube to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em gl_two_colored_tube}
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
					\Ref{GLPrimitiveManager} are copied.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{set}.
					The value of {\em *this} glTwoColoredTube is initialized to the value 
					of the glTwoColoredTube {\em gl_two_colored_tube}.\\
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
					\Ref{GLPrimitiveManager} are copied.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
					Calls \Ref{set}.
					The value of the glTwoColoredTube {\em gl_two_colored_tube} is initialized to the
					value of {\em *this} glTwoColoredTube.\\
					@param       gl_two_colored_tube the glTwoColoredTube to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em gl_two_colored_tube}
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
					\Ref{GLPrimitiveManager} are swapped.
					See \Ref{GeometricObject} and	\Ref{GLObject} for information.
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
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{TwoColoredTube} to create the graphical representation
					of the twoColoredTube. This method is used internally by the render engine
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
					This method handles the export of {\em *this} glTwoColoredTube into another
					format (eg. POVRAY, VRML)\\
					Calls \Ref{TwoColoredTube::extract}.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
			//@}

			
			private:

			void draw_
				(const Vector3& vertex_1, const Vector3& vertex_2,
				 const Vector3& rotation_axis, const Real angle);
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/PRIMITIV/glTwoColoredTube.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL
			
#endif // BALL_MOLVIEW_GUI_PRIMITIV_GLTWOCOLOREDTUBE_H
