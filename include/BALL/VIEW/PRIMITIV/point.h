// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: point.h,v 1.17 2003/03/14 11:50:25 sturm Exp $

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#define BALL_VIEW_PRIMITIV_POINT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#	include <BALL/VIEW/KERNEL/vertex1.h>
#endif


namespace BALL
{

	namespace VIEW
	{
        /**  \addtogroup  ViewPrimitives
         *  @{
         */
    
		/** Point class.
				An instance of Point represents an instance of the geometric representation "point".
				A point has the following properties. 

				  - color - the color of the point
					- vertex - the position of the point
				
				The class Point is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink 
				and  \link Vertex Vertex \endlink . See these classes for further information concerning
				interface and additional methods.  \par
				
		*/
		class Point
			: public GeometricObject,
				public ColorExtension,
				public Vertex
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new point.
					The properties of {\em *this} point are set to:

  				  - color - to the color black
		  			- vertex - to the vector (0,0,0)
					
					@return      Point new constructed point
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
			*/
			Point()
				throw();

			/** Copy constructor with cloning facility.
					Construct new point by copying the point {\em point}.
					The copy is either deep (default) or shallow.
					@param       point the point to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      Point new constructed point copied from {\em point}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
			*/
			Point(const Point& point, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new point by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Point new constructed point initialized from {\em geometric_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
			*/
			Point(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} point.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Point()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink 
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink 
					Calls  \link Vertex::clear Vertex::clear \endlink 
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink 
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink 
					Calls  \link Vertex::destroy Vertex::destroy \endlink 
					@see  GeometricObject::destroy
					@see  ColorExtension::destroy
					@see  Vertex::destroy
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the point {\em point} to {\em *this} point.
					The copy is either deep (default) or shallow.
					The value of {\em *this} point is initialized to the value of 
					the point {\em point}. \par
					@param       point the point to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em point}
					@see         Point
			*/
			void set(const Point& point, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the point {\em point} to {\em *this} point.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} point is initialized to the value 
					of the point {\em point}. \par
					@param       point the point to be copied
					@return      Point& {\em *this} point
					@see         set
			*/
			const Point& operator = (const Point& point)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} point to the point {\em point}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the point {\em point} is initialized to the
					value of {\em *this} point. \par
					@param       point the point to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em point}
					@see         set
			*/
			void get(Point& point, bool deep = true) const
				throw();

			/** Swapping of point's.
					Swap the value of {\em *this} point with the point {\em point}.
					@param       point the point being swapped with {\em *this} point 
			*/
			void swap(Point& point)
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} point.
					If the internal state of {\em *this} point is correct (self-validated) and 
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					Calls  \link Vertex::isValid Vertex::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} point is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
					@see        Vertex::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} point to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					Calls  \link Vertex::dump Vertex::dump \endlink .
					@param   s output stream where to output the value of {\em *this} point
					@param   depth the dumping depth
					@see     GeometricObject::dump
					@see     ColorExtension::dump
					@see     Vertex::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}

			protected:

			/** Export method.
					This method handles the export of {\em *this} point into another
					format (eg. POVRAY, VRML)  \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();
		};
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_POINT_H
