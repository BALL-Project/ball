// $Id: twoColoredLine.h,v 1.7 2001/05/13 14:55:25 hekl Exp $

#ifndef BALL_VIEW_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#define BALL_VIEW_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION2_H
#	include <BALL/VIEW/KERNEL/colorExtension2.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif
namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::GeometricObject;
		using VIEW::ColorExtension2;
		using VIEW::Vertex2;

		/** TwoColoredLine class.
				{\bf Framework:} BALL/MOLVIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/MOLVIEW/PRIMITIV/twoColoredLine.h}\\ \\
				An instance of TwoColoredLine represents an instance of the geometric
				representation of a two colored "line".
				A twoColoredLine has the following properties. 
				\begin{itemize}
				  \item color1 - the color for the first vertex of the line
				  \item color2 - the color for the second vertex of the line
					\item vertex1 - the first vertex of the line
					\item vertex2 - the second vertex of the line
				\end{itemize}
				The class TwoColoredLine is a line that is half colored in the color 1 and
				in color 2. The line is exactly divided in the middle. One half (starting
				from vertex1 til middle vertex) is colored by the color1 and the other half 
				(starting from middle vertex to vertex2) is colored by color2.
				The class TwoColoredLine is derived from the classes \Ref{GeometricObject},
				\Ref{ColorExtension2}	and \Ref{Vertex2}.
				See these classes for further information concerning
				interface and additional methods.
				@memo    TwoColoredLine class (BALL MOLVIEW primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.7 $
				@date    $Date: 2001/05/13 14:55:25 $
		*/
		class TwoColoredLine
			: public GeometricObject,
				public ColorExtension2,
				public Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new twoColoredLine.
					The properties of {\em *this} twoColoredLine are set to:
					\begin{itemize}
  				  \item color1 - set to the color black
						\item color2 - set to the color black
		  			\item vertex1 - set to the vector (0,0,0)
		  			\item vertex2 - set to the vector (0,0,0)
					\end{itemize}
					@return      TwoColoredLine new constructed twoColoredLine
					@see         GeometricObject
					@see         ColorExtension2
					@see         Vertex2
			*/
			TwoColoredLine()
				throw();

			/** Copy constructor with cloning facility.
					Construct new line by copying the twoColoredLine {\em two_colored_line}.
					The copy is either deep (default) or shallow.
					@param       two_colored_line the twoColoredLine to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      TwoColoredLine new constructed twoColoredLine copied from {\em two_colored_line}
					@see         GeometricObject
					@see         ColorExtension2
					@see         Vertex2
			*/
			TwoColoredLine(const TwoColoredLine& two_colored_line, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new twoColoredLine by copying the internal values from 
					\Ref{GeometricObject} {\em geometric_object}.
					@param       geometric_object the \Ref{GeometricObject} which internal values should be copied
					@return      TwoColoredLine new constructed twoColoredLine initialized from {\em geometric_object}
					@see         GeometricObject
					@see         ColorExtension2
					@see         Vertex2
			*/
			TwoColoredLine(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} twoColoredLine.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~TwoColoredLine()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}.
					Calls \Ref{ColorExtension2::clear}.
					Calls \Ref{Vertex2::clear}.
					@see  GeometricObject::clear
					@see  ColorExtension2::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}.
					Calls \Ref{ColorExtension2::destroy}.
					Calls \Ref{Vertex2::destroy}.
					@see  GeometricObject::destroy
					@see  ColorExtension2::destroy
					@see  Vertex2::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the twoColoredLine {\em two_colored_line} to {\em *this}
					twoColoredLine.
					The copy is either deep (default) or shallow.
					The value of {\em *this} twoColoredLine is initialized to the value of 
					the twoColoredLine {\em two_colored_line}.
					Calls \Ref{GeometricObject::set}.
					Calls \Ref{ColorExtension2::set}.
					Calls \Ref{Vertex2::set}.
					@param       two_colored_line the twoColoredLine to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em two_colored_line}
					@see         GeometricObject::set
					@see         ColorExtension2::set
					@see         Vertex2::set
			*/
			void set(const TwoColoredLine& two_colored_line, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the twoColoredLine {\em two_colored_line} to {\em *this}
					twoColoredLine.	The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} twoColoredLine is initialized to the value 
					of the twoColoredLine {\em two_colored_line}.\\
					@param       two_colored_line the twoColoredLine to be copied
					@return      TwoColoredLine& constant reference of {\em *this} twoColoredLine
					@see         set
			*/
			const TwoColoredLine& operator = (const TwoColoredLine& two_colored_line)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} twoColoredLine to the twoColoredLine {\em two_colored_line}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the twoColoredLine {\em two_colored_line} is initialized to the
					value of {\em *this} twoColoredLine.\\
					@param       two_colored_line the twoColoredLine to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em two_colored_line}
					@see         set
			*/
			void get(TwoColoredLine& two_colored_line, bool deep = true) const
				throw();

			/** Swapping of two_colored_line's.
					Swap the value of {\em *this} twoColoredLine with the twoColoredLine
					{\em two_colored_line}.
					@param       two_colored_line the twoColoredLine being swapped with {\em *this} twoColoredLine 
			*/
			void swap(TwoColoredLine& two_colored_line)
				throw();
			//@}


			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Inspection of the middle vector.
					Access the geometric middle vertex between vertex1 and vertex2 of
					{\em *this} twoColoredLine.
					@return. Vector3 the middle vertex between vertex1 and vertex 2.
					@see     Vector3
			*/
			Vector3 getMiddleVertex()
				throw();
			//@}


			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure
					consistencies	of {\em *this} twoColoredLine.
					If the internal state of {\em *this} twoColoredLine is correct 
					(self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {GeometricObject::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} twoColoredLine is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GeometricObject::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} twoColoredLine to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension2::dump}.
					Calls \Ref{Vertex2::dump}.
					@param   s output stream where to output the value of {\em *this} twoColoredLine
					@param   depth the dumping depth
					@see        GeometricObject::dump
					@see        ColorExtension2::dump
					@see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent twoColoredLine data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} twoColoredLine
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent twoColoredLine data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} twoColoredLine
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}


			protected:

			/** Export method.
					This method handles the export of {\em *this} twoColoredLine into another
					format (eg. POVRAY, VRML)\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_VIEW_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
