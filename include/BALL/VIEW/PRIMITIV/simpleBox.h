// $Id: simpleBox.h,v 1.6 2001/02/08 16:04:10 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_SIMPLEBOX_H
#define BALL_VIEW_PRIMITIV_SIMPLEBOX_H

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
#include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** SimpleBox class.	
				{\bf Framework:} BALL/VIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/PRIMITIV/simpleBox.h}\\
				An instance of SimpleBox represents an instance of the geometric representation "Box".
				A simpleBox is a box whose sides are parallel to the coordinate axes. Therefore only
				two vertices are needed. The first vertex is the lower left corner and the second
				vertex is the upper right corner of the box (in 3-dimensional space).
				A simpleBox has the following properties. 
				\begin{itemize}
				  \item color - the color of the simpleBox
					\item vertex1 - the first vertex of the simpleBox (lower left corner)
					\item vertex2 - the second vertex of the simpleBox (upper right corner)
				\end{itemize}
				The class SimpleBox is derived from the classes \Ref{GeometricObject}, \Ref{ColorExtension}
				and \Ref{Vertex2}. See these classes for further information concerning
				interface and additional methods.
				@memo    SimpleBox class (BALL VIEW primitiv framework)
				@author  $Author: amoll $
				@version $Revision: 1.6 $
				@date    $Date: 2001/02/08 16:04:10 $
		*/
		class SimpleBox
			: public GeometricObject,
				public ColorExtension,
				public Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new simpleBox.
					The properties of {\em *this} simpleBox are set to:
					\begin{itemize}
  				  \item color - to the color black
		  			\item vertex1 - to the vector (0,0,0)
		  			\item vertex2 - to the vector (0,0,0)
					\end{itemize}
					@return      SimpleBox - new constructed simpleBox
					@see         GeometricObject::GeometricObject
					@see         ColorExtension::ColorExtension
					@see         Vertex2::Vertex2
			*/
			SimpleBox()
				throw();

			/** Copy constructor with cloning facility.
					Construct new simpleBox by copying the simpleBox {\em simpleBox}.
					The copy is either deep (default) or shallow.
					@param       simpleBox the simpleBox to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      SimpleBox - new constructed simpleBox copied from {\em simpleBox}
					@see         GeometricObject::GeometricObject
					@see         ColorExtension::ColorExtension
					@see         Vertex2::Vertex2
			*/
			SimpleBox(const SimpleBox& simpleBox, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new simpleBox by copying the internal values from geometricObject 
					{\em geometric\_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      SimpleBox - new constructed simpleBox initialized from {\em geometric\_object}
					@see         GeometricObject::GeometricObject
					@see         ColorExtension::ColorExtension
					@see         Vertex2::Vertex2
			*/
			SimpleBox(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} simpleBox.
					Calls \Ref{SimpleBox::destroy}.
					@see         SimpleBox::destroy
			*/
			virtual ~SimpleBox()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}
					Calls \Ref{ColorExtension::clear}
					Calls \Ref{Vertex2::clear}
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}
					Calls \Ref{ColorExtension::destroy}
					Calls \Ref{Vertex2::destroy}
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
					Assign the simpleBox {\em simpleBox} to {\em *this} simpleBox.
					The copy is either deep (default) or shallow.
					The value of {\em *this} simpleBox is initialized to the value of 
					the simpleBox {\em simpleBox}.\\
					@param       simpleBox the simpleBox to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em simpleBox}
					@see         SimpleBox::SimpleBox
			*/
			void set(const SimpleBox& simpleBox, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the simpleBox {\em simpleBox} to {\em *this} simpleBox.
					The copy is deep.
					Calls \Ref{SimpleBox::set}.
					The value of {\em *this} simpleBox is initialized to the value 
					of the simpleBox {\em simpleBox}.\\
					@param       simpleBox the simpleBox to be copied
					@return      SimpleBox& - {\em *this} simpleBox
					@see         SimpleBox::set
			*/
			const SimpleBox& operator = (const SimpleBox& simpleBox)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} simpleBox to the simpleBox {\em simpleBox}.
					The copy is either deep (default) or shallow.
					Calls \Ref{SimpleBox::set}.
					The value of the simpleBox {\em simpleBox} is initialized to the
					value of {\em *this} simpleBox.\\
					@param       simpleBox the simpleBox to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em simpleBox}
					@see         SimpleBox::set
			*/
			void get(SimpleBox& simpleBox, bool deep = true) const
				throw();

			/** Swapping of simpleBox's.
					Swap the value of {\em *this} simpleBox with the simpleBox {\em simpleBox}.
					@param       simpleBox the simpleBox being swapped with {\em *this} simpleBox 
					@see         SimpleBox::SimpleBox
			*/
			void swap(SimpleBox& simpleBox)
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} simpleBox.
					If the internal state of {\em *this} simpleBox is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {GeometricObject::isValid}.
					Calls {Vertex2::isValid}.
					@return			bool -
											{\tt true} if the internal state of {\em *this} simpleBox is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GeometricObject::isValid
					@see        Vertex2::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} simpleBox to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension::dump}.
					Calls \Ref{Vertex2::dump}.
					@param   s output stream where to output the value of {\em *this} simpleBox
					@param   depth the dumping depth
					@see        GeometricObject::dump
					@see        ColorExtension::dump
					@see        Vertex2::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{
			/** Persistent stream output and state restorage.
  			  Read persistent simpleBox data from the input stream {\em s} and 
				  restore the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented. 
				  @param       s input stream from where to restore the internal state of {\em *this} simpleBox
			*/
			virtual void read(std::istream&  s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent simpleBox data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.	 
				  @param       s output stream to where to store the internal state of {\em *this} simpleBox
			*/
			virtual void write(std::ostream& s) const
				throw();

			//@}

			protected:

			/** Export method.
					This method handles the export of {\em *this} simpleBox into another
					format (eg. POVRAY, VRML) \\
				  {\bf Note:} Not yet implemented.
					@return    bool - {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/simpleBox.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SIMPLEBOX_H
