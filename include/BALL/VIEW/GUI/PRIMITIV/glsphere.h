// $Id: glsphere.h,v 1.8 2001/05/27 10:26:37 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
#define BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#	include <BALL/VIEW/PRIMITIV/sphere.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** GLSphere class.
				{\bf Framework:} BALL/VIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/glsphere.h}	\\ \\
				An instance of GLSphere represents an instance of the geometric
				visualization "sphere".
				The class GLSphere is derived from the classes \Ref{Sphere} and
				\Ref{GLObject}. Therefore the class glSphere is the graphical extension
				of the geometrical definition of the class sphere.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				sphere. OpenGL code is used for the visualization.
				See these classes for further information.
				@memo    GLSphere class (BALL VIEW gui primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.8 $
				@date    $Date: 2001/05/27 10:26:37 $
		*/
		class GLSphere
			: public Sphere,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glSphere.
					@return      GLSphere new constructed glSphere
					@see         Sphere
					@see         GLObject
			*/
			GLSphere()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glSphere by copying the glSphere {\em glSphere}.
					The copy is either deep (default) or shallow.
					@param       glSphere the glSphere to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLSphere new constructed glSphere copied from {\em glSphere}
					@see         Sphere
					@see         GLObject
			*/
			GLSphere(const GLSphere& glSphere, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glSphere by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLSphere new constructed glSphere initialized from {\em geometric_object}
					@see         Sphere
					@see         GLObject
			*/
			GLSphere(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glSphere.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLSphere()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Sphere::clear}
					Calls \Ref{GLObject::clear}
					@see  Sphere::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Sphere::destroy}
					Calls \Ref{GLObject::destroy}
					@see  Sphere::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();

			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glSphere {\em sphere} to {\em *this} glSphere.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glSphere is initialized to the value of 
					the glSphere {\em sphere}.\\
					@param       sphere the glSphere to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em sphere}
					@see         Sphere
					@see         GLObject
			*/
			void set(const GLSphere& sphere, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glSphere {\em sphere} to {\em *this} glSphere.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} glSphere is initialized to the value 
					of the glSphere {\em sphere}.\\
					@param       sphere the glSphere to be copied
					@return      GLSphere& constant reference {\em *this} glSphere
					@see         set
			*/
			const GLSphere& operator = (const GLSphere& sphere)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glSphere to the glSphere {\em sphere}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the glSphere {\em sphere} is initialized to the
					value of {\em *this} glSphere.\\
					@param       sphere the glSphere to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em sphere}
					@see         set
			*/
			void get(GLSphere& sphere, bool deep = true) const
				throw();

			/** Swapping of Sphere's.
					Swap the value of {\em *this} Sphere with the Sphere {\em sphere}.
					@param       sphere the Sphere being swapped with {\em *this} Sphere 
					@see         GLSphere
			*/
			void swap(GLSphere& sphere)
				throw();
			//@}

			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Sphere} to create the graphical representation
					of the sphere. This method is used internally from the render engine
					of the \Ref{Scene}. Predefined shapes of spheres from the class
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
					This method handles the export of {\em *this} glSphere into another
					format (eg. POVRAY, VRML) \\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
			//@}
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glsphere.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLSPHERE_H
