// $Id: twoColoredTube.h,v 1.7 2001/05/13 14:55:26 hekl Exp $

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#define BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H

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

#ifndef BALL_VIEW_KERNEL_RADIUS_H
#	include <BALL/VIEW/KERNEL/radius.h>
#endif


namespace BALL
{
	
	namespace MOLVIEW
	{


		using VIEW::GeometricObject;
		using VIEW::ColorExtension2;
		using VIEW::Radius;
		using VIEW::Vertex2;

		/** TwoColoredTube class.
				{\bf Framework:} BALL/MOLVIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/MOLVIEW/PRIMITIV/twoColoredTube.h}\\ \\
				An instance of TwoColoredTube represents an instance of the geometric
				representation of a two colored "tube".
				A twoColoredTube has the following properties. 
				\begin{itemize}
				  \item color1 - the color for the first vertex of the tube
				  \item color2 - the color for the second vertex of the tube
					\item vertex1 - the first vertex of the tube
					\item vertex2 - the second vertex of the tube
				\end{itemize}
				The class TwoColoredTube is a tube that is half colored in the color 1 and
				in color 2. The tube is exactly divided in the middle. One half (starting
				from vertex1 til middle vertex) is colored by the color1 and the other half 
				(starting from middle vertex to vertex2) is colored by color2.
				The class TwoColoredTube is derived from the classes \Ref{GeometricObject},
				\Ref{ColorExtension2}, \Ref{Radius}	and \Ref{Vertex2}.
				See these classes for further information concerning
				interface and additional methods.
				@memo    TwoColoredTube class (BALL MOLVIEW primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.7 $
				@date    $Date: 2001/05/13 14:55:26 $
		*/
		class TwoColoredTube
			: public GeometricObject,
			  public ColorExtension2,
				public Radius,
   			public Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new twoColoredTube.
					The properties of {\em *this} twoColoredTube are set to:
					\begin{itemize}
  				  \item color1 - set to the color black
						\item color2 - set to the color black
						\item radius - set to 1
		  			\item vertex1 - set to the vector (0,0,0)
		  			\item vertex2 - set to the vector (0,0,0)
					\end{itemize}
					@return      TwoColoredTube new constructed twoColoredTube
					@see         GeometricObject
					@see         ColorExtension2
					@see         Radius
					@see         Vertex2
			*/
			TwoColoredTube()
				throw();

			/** Copy constructor with cloning facility.
					Construct new twoColoredTube by copying the twoColoredTube {\em two_colored_tube}.
					The copy is either deep (default) or shallow.
					@param       two_colored_tube the twoColoredTube to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      TwoColoredTube new constructed twoColoredTube copied from {\em two_colored_tube}
					@see         GeometricObject
					@see         ColorExtension2
					@see         Radius
					@see         Vertex2
			*/
			TwoColoredTube
				(const TwoColoredTube& two_colored_tube, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new twoColoredTube by copying the internal values from 
					\Ref{GeometricObject} {\em geometric_object}.
					@param       geometric_object the \Ref{GeometricObject} which internal values should be copied
					@return      TwoColoredTube new constructed twoColoredTube initialized from {\em geometric_object}
					@see         GeometricObject
					@see         ColorExtension2
					@see         Radius
					@see         Vertex2
			*/
			TwoColoredTube
				(const GeometricObject& geometric_object)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} twoColoredTube.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~TwoColoredTube()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}.
					Calls \Ref{ColorExtension2::clear}.
					Calls \Ref{Radius::clear}.
					Calls \Ref{Vertex2::clear}.
					@see  GeometricObject::clear
					@see  ColorExtension2::clear
					@see  Radius::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}.
					Calls \Ref{ColorExtension2::destroy}.
					Calls \Ref{Radius::destroy}.
					Calls \Ref{Vertex2::destroy}.
					@see  GeometricObject::destroy
					@see  ColorExtension2::destroy
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
					Assign the twoColoredTube {\em two_colored_tube} to {\em *this}
					twoColoredTube.
					The copy is either deep (default) or shallow.
					The value of {\em *this} twoColoredTube is initialized to the value of 
					the twoColoredTube {\em two_colored_tube}.
					Calls \Ref{GeometricObject::set}.
					Calls \Ref{ColorExtension2::set}.
					Calls \Ref{Radius::set}.
					Calls \Ref{Vertex2::set}.
					@param       two_colored_tube the twoColoredTube to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em two_colored_tube}
					@see         GeometricObject::set
					@see         ColorExtension2::set
					@see         Radius::set
					@see         Vertex2::set
			*/
			void set(const TwoColoredTube& two_colored_tube, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the twoColoredTube {\em two_colored_tube} to {\em *this}
					twoColoredTube.	The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} twoColoredTube is initialized to the value 
					of the twoColoredTube {\em two_colored_tube}.\\
					@param       two_colored_tube the twoColoredTube to be copied
					@return      TwoColoredTube& constant reference of {\em *this} twoColoredTube
					@see         set
			*/
			const TwoColoredTube& operator = (const TwoColoredTube& two_colored_tube)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} twoColoredTube to the twoColoredTube {\em two_colored_tube}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the twoColoredTube {\em two_colored_tube} is initialized to the
					value of {\em *this} twoColoredTube.\\
					@param       two_colored_tube the twoColoredTube to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em two_colored_tube}
					@see         set
			*/
			void get(TwoColoredTube& two_colored_tube, bool deep = true) const
				throw();

			/** Swapping of two_colored_tube's.
					Swap the value of {\em *this} twoColoredTube with the twoColoredTube
					{\em two_colored_tube}.
					@param       two_colored_tube the twoColoredTube being swapped with {\em *this}  twoColoredTube
			*/
			void swap(TwoColoredTube& two_colored_tube)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Inspection of the length of the tube.
					Access the length of {\em *this} tube.
					@return  Real the length of {\em *this} tube
			*/
			Real getLength() const
				throw();

			/** Inspection of the middle vector.
					Access the geometric middle vertex between vertex1 and vertex2 of
					{\em *this} twoColoredTube.
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
					consistencies	of {\em *this} twoColoredTube.
					If the internal state of {\em *this} twoColoredTube is correct 
					(self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {GeometricObject::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} twoColoredTube is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GeometricObject::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} twoColoredTube to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension2::dump}.
					Calls \Ref{Radius::dump}.
					Calls \Ref{Vertex2::dump}.
					@param   s output stream where to output the value of {\em *this} twoColoredTube
					@param   depth the dumping depth
					@see        GeometricObject::dump
					@see        ColorExtension2::dump
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
  			  Read persistent twoColoredTube data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} twoColoredTube
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent twoColoredTube data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} twoColoredTube
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}


			
			protected:

			/** Export method.
					This method handles the export of {\em *this} twoColoredTube into another
					format (eg. POVRAY, VRML)\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
