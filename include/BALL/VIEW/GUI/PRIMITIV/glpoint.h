// $Id: glpoint.h,v 1.7 2001/05/13 13:59:14 hekl Exp $

#ifndef BALL_VIEW_GUI_PRIMITIV_GLPOINT_H
#define BALL_VIEW_GUI_PRIMITIV_GLPOINT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#	include <BALL/VIEW/PRIMITIV/point.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/** GLPoint class.				
				{\bf Framework:} BALL/VIEW/GUI/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/PRIMITIV/glpoint.h}\\ \\
				An instance of GLPoint represents an instance of the geometric
				visualization "point".
				The class GLPoint is derived from the classes \Ref{Point} and
				\Ref{GLObject}. Therefore the class glPoint is the graphical extension
				of the geometrical definition of the class point.
				The drawing method from \Ref{GLObject} is overridden to visualize the
				point. OpenGL code is used for the visualization.
				See these classes for further information.
				@memo    GLPoint class (BALL VIEW gui primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.7 $
				@date    $Date: 2001/05/13 13:59:14 $
		*/
		class GLPoint
			: public Point,
				public GLObject
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glPoint.
					@return      GLPoint new constructed glPoint
					@see         Point
					@see         GLObject
			*/
			GLPoint()
				throw();

			/** Copy constructor with cloning facility.
					Construct new glPoint by copying the glPoint {\em point}.
					The copy is either deep (default) or shallow.
					@param       point the glPoint to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      GLPoint new constructed glPoint copied from {\em point}
					@see         Point
					@see         GLObject
			*/
			GLPoint(const GLPoint& point, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new glPoint by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      GLPoint new constructed glPoint initialized from {\em geometric_object}
					@see         Point
					@see         GLObject
			*/
			GLPoint(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glPoint.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLPoint()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Point::clear}
					Calls \Ref{GLObject::clear}
					@see  Point::clear
					@see  GLObject::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Point::destroy}
					Calls \Ref{GLObject::destroy}
					@see  Point::destroy
					@see  GLObject::destroy
			*/
			virtual void destroy()
				throw();

			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glPoint {\em point} to {\em *this} glPoint.
					The copy is either deep (default) or shallow.
					The value of {\em *this} glPoint is initialized to the value of 
					the glPoint {\em point}.\\
					@param       point the glPoint to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em point}
					@see         Point
					@see         GLObject
			*/
			void set(const GLPoint& point, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the glPoint {\em point} to {\em *this} glPoint.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} glPoint is initialized to the value 
					of the glPoint {\em point}.\\
					@param       point the glPoint to be copied
					@return      GLPoint& constant reference {\em *this} glPoint
					@see         set
			*/
			const GLPoint& operator = (const GLPoint& point)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} glPoint to the glPoint {\em point}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the glPoint {\em point} is initialized to the
					value of {\em *this} glPoint.\\
					@param       point the glPoint to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em point}
					@see         set
			*/
			void get(GLPoint& point, bool deep = true) const
				throw();

			/** Swapping of Point's.
					Swap the value of {\em *this} Point with the Point {\em point}.
					@param       point the Point being swapped with {\em *this} Point 
					@see         GLPoint
			*/
			void swap(GLPoint& point)
				throw();
			//@}

			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Overridden method from \Ref{GLObject}. Uses the geometrical properties
					defined in class \Ref{Point} to create the graphical representation
					of the point. This method is used internally from the render engine
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
					This method handles the export of {\em *this} glPoint into another
					format (eg. POVRAY, VRML) \\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/PRIMITIV/glpoint.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_PRIMITIV_GLPOINT_H
