// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: line.h,v 1.14 2003/02/25 13:28:26 sturm Exp $

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#define BALL_VIEW_PRIMITIV_LINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOLEXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif


namespace BALL
{

	namespace VIEW
	{
        /** \ingroup ViewPrimitives
         *  @{
         */
         
         
		/** Line class.
				An instance of Line represents an instance of the geometric representation "line".
				A line has the following properties. 
				\begin{itemize}
				  \item color - the color of the line
					\item vertex1 - the first vertex of the line
					\item vertex2 - the second vertex of the line
				\end{itemize}
				The class Line is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink 
				and  \link Vertex2 Vertex2 \endlink . See these classes for further information concerning
				interface and additional methods.  \par
				<b>Definition:</b> BALL/VIEW/PRIMITIV/line.h
		*/
		class Line
			: public GeometricObject,
				public ColorExtension,
				public Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new line.
					The properties of {\em *this} line are set to:
					\begin{itemize}
  				  \item color - to the color black
		  			\item vertex1 - to the vector (0,0,0)
		  			\item vertex2 - to the vector (0,0,0)
					\end{itemize}
					@return      Line new constructed line
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex2
			*/
			Line()
				throw();

			/** Copy constructor with cloning facility.
					Construct new line by copying the line {\em line}.
					The copy is either deep (default) or shallow.
					@param       line the line to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      Line new constructed line copied from {\em line}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex2
			*/
			Line(const Line& line, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new line by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Line new constructed line initialized from {\em geometric_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex2
			*/
			Line(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} line.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Line()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink .
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink .
					Calls  \link Vertex2::clear Vertex2::clear \endlink .
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink .
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink .
					Calls  \link Vertex2::destroy Vertex2::destroy \endlink .
					@see  GeometricObject::destroy
					@see  ColorExtension::destroy
					@see  Vertex2::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the line {\em line} to {\em *this} line.
					The copy is either deep (default) or shallow.
					The value of {\em *this} line is initialized to the value of 
					the line {\em line}. \par
					@param       line the line to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em line}
			*/
			void set(const Line& line, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the line {\em line} to {\em *this} line.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} line is initialized to the value 
					of the line {\em line}. \par
					@param       line the line to be copied
					@return      Line& {\em *this} line
					@see         set
			*/
			const Line& operator = (const Line& line)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} line to the line {\em line}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the line {\em line} is initialized to the
					value of {\em *this} line. \par
					@param       line the line to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em line}
					@see         set
			*/
			void get(Line& line, bool deep = true) const
				throw();

			/** Swapping of line's.
					Swap the value of {\em *this} line with the line {\em line}.
					@param       line the line being swapped with {\em *this} line 
					@see         Line
			*/
			void swap(Line& line)
				throw();
			//@}


			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} line.
					If the internal state of {\em *this} line is correct (self-validated) and 
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					Calls  \link Vertex2::isValid Vertex2::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} line is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
					@see        Vertex2::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} line to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					Calls  \link Vertex2::dump Vertex2::dump \endlink .
					@param   s output stream where to output the value of {\em *this} line
					@param   depth the dumping depth
					@see        GeometricObject::dump
					@see        ColorExtension::dump
					@see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			protected:

			/** Export method.
					This method handles the export of {\em *this} line into another
					format (eg. POVRAY, VRML) \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();
		};
  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LINE_H
