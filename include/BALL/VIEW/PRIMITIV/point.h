// $Id: point.h,v 1.9 2001/05/13 13:40:38 hekl Exp $

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

		/** Point class.
				{\bf Framework:} BALL/VIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/PRIMITIV/point.h}\\ \\
				An instance of Point represents an instance of the geometric representation "point".
				A point has the following properties. 
				\begin{itemize}
				  \item color - the color of the point
					\item vertex - the position of the point
				\end{itemize}
				The class Point is derived from the classes \Ref{GeometricObject}, \Ref{ColorExtension}
				and \Ref{Vertex}. See these classes for further information concerning
				interface and additional methods.
				@memo    Point class (BALL VIEW primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.9 $
				@date    $Date: 2001/05/13 13:40:38 $
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
					\begin{itemize}
  				  \item color - to the color black
		  			\item vertex - to the vector (0,0,0)
					\end{itemize}
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Point()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}
					Calls \Ref{ColorExtension::clear}
					Calls \Ref{Vertex::clear}
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}
					Calls \Ref{ColorExtension::destroy}
					Calls \Ref{Vertex::destroy}
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
					the point {\em point}.\\
					@param       point the point to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em point}
					@see         Point
			*/
			void set(const Point& point, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the point {\em point} to {\em *this} point.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} point is initialized to the value 
					of the point {\em point}.\\
					@param       point the point to be copied
					@return      Point& {\em *this} point
					@see         set
			*/
			const Point& operator = (const Point& point)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} point to the point {\em point}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the point {\em point} is initialized to the
					value of {\em *this} point.\\
					@param       point the point to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em point}
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
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls \Ref{GeometricObject::isValid}.
					Calls \Ref{Vertex::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} point is correct (self-validated) and consistent, {\tt false} otherwise
					@see        GeometricObject::isValid
					@see        Vertex::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} point to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension::dump}.
					Calls \Ref{Vertex::dump}.
					@param   s output stream where to output the value of {\em *this} point
					@param   depth the dumping depth
					@see     GeometricObject::dump
					@see     ColorExtension::dump
					@see     Vertex::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent point data from the input stream {\em s} and 
				  restore the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented. 
				  @param       s input stream from where to restore the internal state of {\em *this} point
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent point data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} point
			*/
			virtual void write(std::ostream& s) const
				throw();

			//@}

			protected:

			/** Export method.
					This method handles the export of {\em *this} point into another
					format (eg. POVRAY, VRML) \\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/point.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_POINT_H
