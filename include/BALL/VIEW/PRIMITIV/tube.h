// $Id: tube.h,v 1.8 2001/02/08 16:04:10 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#define BALL_VIEW_PRIMITIV_TUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif

#ifndef BALL_VIEW_KERNEL_RADIUS_H
#	include <BALL/VIEW/KERNEL/radius.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/** Tube class.				
				{\bf Framework:} BALL/VIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/PRIMITIV/tube.h}\\
				An instance of Tube represents an instance of the geometric representation "tube".
				A tube has the following properties. 
				\begin{itemize}
				  \item color   - the color of the tube
					\item radius  - the radius of the tube
					\item vertex1 - the first vertex of the tube
					\item vertex2 - the second vertex of the tube
				\end{itemize}
				The class Tube is derived from the classes \Ref{GeometricObject}, \Ref{ColorExtension},
				\Ref{Radius} and \Ref{Vertex2}. 
				See these classes for further information concerning
				interface and additional methods.
				@memo    Tube class (BALL VIEW primitiv framework)
				@author  $Author: amoll $
				@version $Revision: 1.8 $
				@date    $Date: 2001/02/08 16:04:10 $
		*/
		class Tube
			: public GeometricObject,
				public Radius,
			  public ColorExtension,
   			public Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new tube.
					The properties of {\em *this} tube are set to:
					\begin{itemize}
  				  \item color   - to the color black
						\item radius  - set to 1
		  			\item vertex1 - to the vector (0,0,0)
		  			\item vertex2 - to the vector (0,0,0)
					\end{itemize}
					@return      Tube - new constructed tube
					@see         GeometricObject::GeometricObject
					@see         ColorExtension::ColorExtension
					@see         Radius::Radius
					@see         Vertex2::Vertex2
			*/
			Tube()
				throw();

			/** Copy constructor with cloning facility.
					Construct new tube by copying the tube {\em tube}.
					The copy is either deep (default) or shallow.
					@param       tube the tube to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      Tube - new constructed tube copied from {\em tube}
					@see         GeometricObject::GeometricObject
					@see         ColorExtension::ColorExtension
					@see         Radius::Radius
					@see         Vertex2::Vertex2
			*/
			Tube(const Tube& tube, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new tube by copying the internal values from geometricObject 
					{\em geometric\_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Tube - new constructed tube initialized from {\em geometric\_object}
					@see         GeometricObject::GeometricObject
					@see         ColorExtension::ColorExtension
					@see         Radius::Radius
					@see         Vertex2::Vertex2
			*/
			Tube(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} tube.
					Calls \Ref{Tube::destroy}.
					@see         Tube::destroy
			*/
			virtual ~Tube()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}
					Calls \Ref{ColorExtension::clear}
					Calls \Ref{Radius::clear}
					Calls \Ref{Vertex2::clear}
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Radius::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}
					Calls \Ref{ColorExtension::destroy}
					Calls \Ref{Radius::destroy}
					Calls \Ref{Vertex2::destroy}
					@see  GeometricObject::destroy
					@see  ColorExtension::destroy
					@see  Radius::destroy
					@see  Vertex2::destroy
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the tube {\em tube} to {\em *this} tube.
					The copy is either deep (default) or shallow.
					The value of {\em *this} tube is initialized to the value of 
					the tube {\em tube}.\\
					@param       tube the tube to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em tube}
					@see         Tube::Tube
			*/
			void set(const Tube& tube, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the tube {\em tube} to {\em *this} tube.
					The copy is deep.
					Calls \Ref{Tube::set}.
					The value of {\em *this} tube is initialized to the value 
					of the tube {\em tube}.\\
					@param       tube the tube to be copied
					@return      Tube& - {\em *this} tube
					@see         Tube::set
			*/
			const Tube& operator = (const Tube& tube)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} tube to the tube {\em tube}.
					The copy is either deep (default) or shallow.
					Calls \Ref{Tube::set}.
					The value of the tube {\em tube} is initialized to the
					value of {\em *this} tube.\\
					@param       tube the tube to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em tube}
					@see         Tube::set
			*/
			void get(Tube& tube, bool deep = true) const
				throw();

			/** Swapping of tube's.
					Swap the value of {\em *this} tube with the tube {\em tube}.
					@param       tube the tube being swapped with {\em *this} tube 
					@see         Tube::Tube
			*/
			void swap(Tube& tube)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Inspection of the length of the tube.
					Access the length of {\em *this} tube.
					@return  Real - the length of {\em *this} tube
			*/
			Real getLength() const
				throw();

			/** Inspection of the square length of the tube.
					Access the square length of {\em *this} tube.
					@return  Real - the square length of {\em *this} tube
			*/
			Real getSquareLength() const
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} tube.
					If the internal state of {\em *this} tube is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {GeometricObject::isValid}.
					Calls {Vertex2::isValid}.
					@return			bool -
											{\tt true} if the internal state of {\em *this} tube is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GeometricObject::isValid
					@see        Vertex2::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} tube to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension::dump}.
					Calls \Ref{Radius::dump}.
					Calls \Ref{Vertex2::dump}.
					@param   s output stream where to output the value of {\em *this} tube
					@param   depth the dumping depth
					@see        GeometricObject::dump
					@see        ColorExtension::dump
					@see        Radius::dump
					@see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent tube data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
		 		  @param       s input stream from where to restore the internal state of {\em *this} tube
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent tube data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} tube
			*/
			virtual void write(std::ostream& s) const
				throw();

			//@}
				
			protected:

			/** Export method.
					This method handles the export of {\em *this} tube into another
					format (eg. POVRAY, VRML). \\
				  {\bf Note:} Not yet implemented.
					@return    bool - {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/tube.iC>
#		endif
		
	} // namespace VIEW
	 
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_TUBE_H
