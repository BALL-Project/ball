// $Id: sphere.h,v 1.10 2001/07/14 12:55:55 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#define BALL_VIEW_PRIMITIV_SPHERE_H

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
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#	include <BALL/VIEW/KERNEL/vertex1.h>
#endif

#ifndef BALL_VIEW_KERNEL_RADIUS_H
#	include <BALL/VIEW/KERNEL/radius.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** Sphere class.			
				{\bf Framework:} BALL/VIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/PRIMITIV/sphere.h}	\\ \\
				An instance of Sphere represents an instance of the geometric representation "sphere".
				A sphere has the following properties. 
				\begin{itemize}
				  \item color - the color of the sphere
					\item radius - the radius of the sphere
					\item vertex - the position of the sphere
				\end{itemize}
				The class Sphere is derived from the classes \Ref{GeometricObject}, \Ref{ColorExtension},
				\Ref{Vertex} and \Ref{Radius}. See these classes for further information concerning
				interface and additional methods.
				@memo    Sphere class (BALL VIEW primitiv framework)
				@author  $Author: amoll $
				@version $Revision: 1.10 $
				@date    $Date: 2001/07/14 12:55:55 $
		*/
		class Sphere
			: public GeometricObject,
				public ColorExtension,
				public Vertex,
				public Radius
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new sphere.
					The properties of {\em *this} sphere are set to:
					\begin{itemize}
  				  \item color - to the color black
	  				\item radius - to the value 1
		  			\item vertex - to the vector (0,0,0)
					\end{itemize}
					@return      Sphere new constructed sphere
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
					@see         Radius
			*/
			Sphere()
				throw();

			/** Copy constructor with cloning facility.
					Construct new sphere by copying the sphere {\em sphere}.
					The copy is either deep (default) or shallow.
					@param       sphere the sphere to be copied (cloned)
					@param       deep make a deep (={\tt true}) or shallow (={\tt false})
					@return      Sphere new constructed sphere copied from {\em sphere}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
					@see         Radius
			*/
			Sphere(const Sphere& sphere, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new sphere by copying the internal values from geometricObject 
					{\em geometric\_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Sphere new constructed sphere initialized from {\em geometric\_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
					@see         Radius
			*/
			Sphere(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} sphere.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Sphere()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}
					Calls \Ref{ColorExtension::clear}
					Calls \Ref{Vertex::clear}
					Calls \Ref{Radius::clear}
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex::clear
					@see  Radius::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}
					Calls \Ref{ColorExtension::destroy}
					Calls \Ref{Vertex::destroy}
					Calls \Ref{Radius::destroy}
					@see  GeometricObject::destroy
					@see  ColorExtension::destroy
					@see  Vertex::destroy
					@see  Radius::destroy
			*/
			virtual void destroy()
				throw();

			//@}	
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the sphere {\em sphere} to {\em *this} sphere.
					The copy is either deep (default) or shallow.
					The value of {\em *this} sphere is initialized to the value of 
					the sphere {\em sphere}.\\
					@param       sphere the sphere to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em sphere}
					@see         Sphere
			*/
			void set(const Sphere& sphere, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the sphere {\em sphere} to {\em *this} sphere.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} sphere is initialized to the value 
					of the sphere {\em sphere}.\\
					@param       sphere the sphere to be copied
					@return      Sphere& {\em *this} sphere
					@see         set
			*/
			const Sphere& operator = (const Sphere& sphere)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} sphere to the sphere {\em sphere}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the sphere {\em sphere} is initialized to the
					value of {\em *this} sphere.\\
					@param       sphere the sphere to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em sphere}
					@see         set
			*/
			void get(Sphere& sphere, bool deep = true) const
				throw();

			/** Swapping of sphere's.
					Swap the value of {\em *this} sphere with the sphere {\em sphere}.
					@param       sphere the sphere being swapped with {\em *this} sphere 
			*/
			void swap(Sphere& sphere)
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} sphere.
					If the internal state of {\em *this} sphere is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls \Ref{GeometricObject::isValid}.
					Calls \Ref{Vertex::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} sphere is correct (self-validated) and consistent, {\tt false} otherwise
					@see        GeometricObject::isValid
					@see        Vertex::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} sphere to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension::dump}.
					Calls \Ref{Vertex::dump}.
					Calls \Ref{Radius::dump}.
					@param   s output stream where to output the value of {\em *this} sphere
					@param   depth the dumping depth
					@see     GeometricObject::dump
					@see     ColorExtension::dump
					@see     Vertex::dump
					@see     Radius::dump					
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{
			/** Persistent stream output and state restorage.
  			  Read persistent sphere data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.	 
				  @param       s input stream from where to restore the internal state of {\em *this} sphere
			*/
			virtual void read(std::istream&  s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent sphere data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.		 
				  @param       s output stream to where to store the internal state of {\em *this} sphere
			*/
			virtual void write(std::ostream& s) const
				throw();

			//@}

			protected:

			/** Export method.
					This method handles the export of {\em *this} sphere into another
					format (eg. POVRAY, VRML)\\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/sphere.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SPHERE_H
