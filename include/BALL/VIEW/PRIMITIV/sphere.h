// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sphere.h,v 1.18 2003/03/14 11:50:26 sturm Exp $

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

namespace BALL
{
	
	namespace VIEW
	{
        /**  \addtogroup  ViewPrimitives
         *  @{
         */
       
		/** Sphere class.			
				An instance of Sphere represents an instance of the geometric representation "sphere".
				A sphere has the following properties. 

				  - color - the color of the sphere
					- radius - the radius of the sphere
					- vertex - the position of the sphere
				
				The class Sphere is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink ,
				 \link Vertex Vertex \endlink  and  \link Radius Radius \endlink . See these classes for further information concerning
				interface and additional methods.  \par
				
		*/
		class Sphere
			: public GeometricObject,
				public ColorExtension,
				public Vertex
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new sphere.
					The properties of {\em *this} sphere are set to:

  				  - color - to the color black
	  				- radius - to the value 1
		  			- vertex - to the vector (0,0,0)
					
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
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Sphere()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink 
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink 
					Calls  \link Vertex::clear Vertex::clear \endlink 
					Calls  \link Radius::clear Radius::clear \endlink 
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex::clear
					@see  Radius::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink 
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink 
					Calls  \link Vertex::destroy Vertex::destroy \endlink 
					Calls  \link Radius::destroy Radius::destroy \endlink 
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
					the sphere {\em sphere}. \par
					@param       sphere the sphere to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em sphere}
					@see         Sphere
			*/
			void set(const Sphere& sphere, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the sphere {\em sphere} to {\em *this} sphere.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} sphere is initialized to the value 
					of the sphere {\em sphere}. \par
					@param       sphere the sphere to be copied
					@return      Sphere& {\em *this} sphere
					@see         set
			*/
			const Sphere& operator = (const Sphere& sphere)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} sphere to the sphere {\em sphere}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the sphere {\em sphere} is initialized to the
					value of {\em *this} sphere. \par
					@param       sphere the sphere to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em sphere}
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
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					Calls  \link Vertex::isValid Vertex::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} sphere is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
					@see        Vertex::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} sphere to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					Calls  \link Vertex::dump Vertex::dump \endlink .
					Calls  \link Radius::dump Radius::dump \endlink .
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

			/**	Get the radius.
			 */
			Real getRadius() const
				throw();

			/** Set the radius.
			 */
			void setRadius(Real radius)
				throw();

			//@}

			protected:

			/** Export method.
					This method handles the export of {\em *this} sphere into another
					format (eg. POVRAY, VRML) \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();

			private:

			Real radius_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/PRIMITIV/sphere.iC>
#	endif
  /** @} */		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SPHERE_H
