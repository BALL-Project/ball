// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tube.h,v 1.18 2003/03/14 11:50:26 sturm Exp $

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

namespace BALL
{
	namespace VIEW
	{
        /**  \addtogroup  ViewPrimitives
         *  @{
         */
         
		/** Tube class.				
				An instance of Tube represents an instance of the geometric representation "tube".
				A tube has the following properties. 

				  - color   - the color of the tube
					- radius  - the radius of the tube
					- vertex1 - the first vertex of the tube
					- vertex2 - the second vertex of the tube
				
				The class Tube is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink ,
				 \link Radius Radius \endlink  and  \link Vertex2 Vertex2 \endlink . 
				See these classes for further information concerning
				interface and additional methods.  \par
				
		*/
		class Tube
			: public GeometricObject,
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

  				  - color   - to the color black
						- radius  - set to 1
		  			- vertex1 - to the vector (0,0,0)
		  			- vertex2 - to the vector (0,0,0)
					
					@return      Tube new constructed tube
					@see         GeometricObject
					@see         ColorExtension
					@see         Radius
					@see         Vertex2
			*/
			Tube()
				throw();

			/** Copy constructor with cloning facility.
					Construct new tube by copying the tube {\em tube}.
					The copy is either deep (default) or shallow.
					@param       tube the tube to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
					@return      Tube new constructed tube copied from {\em tube}
					@see         GeometricObject
					@see         ColorExtension
					@see         Radius
					@see         Vertex2
			*/
			Tube(const Tube& tube, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new tube by copying the internal values from geometricObject 
					{\em geometric\_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Tube new constructed tube initialized from {\em geometric\_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Radius
					@see         Vertex2
			*/
			Tube(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} tube.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Tube()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink 
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink 
					Calls  \link Radius::clear Radius::clear \endlink 
					Calls  \link Vertex2::clear Vertex2::clear \endlink 
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Radius::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink 
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink 
					Calls  \link Radius::destroy Radius::destroy \endlink 
					Calls  \link Vertex2::destroy Vertex2::destroy \endlink 
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
					the tube {\em tube}. \par
					@param       tube the tube to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em tube}
					@see         Tube
			*/
			void set(const Tube& tube, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the tube {\em tube} to {\em *this} tube.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} tube is initialized to the value 
					of the tube {\em tube}. \par
					@param       tube the tube to be copied
					@return      Tube& {\em *this} tube
					@see         set
			*/
			const Tube& operator = (const Tube& tube)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} tube to the tube {\em tube}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the tube {\em tube} is initialized to the
					value of {\em *this} tube. \par
					@param       tube the tube to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em tube}
					@see         set
			*/
			void get(Tube& tube, bool deep = true) const
				throw();

			/** Swapping of tube's.
					Swap the value of {\em *this} tube with the tube {\em tube}.
					@param       tube the tube being swapped with {\em *this} tube 
			*/
			void swap(Tube& tube)
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

			/** Inspection of the square length of the tube.
					Access the square length of {\em *this} tube.
					@return  Real the square length of {\em *this} tube
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
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					Calls  \link Vertex2::isValid Vertex2::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} tube is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
					@see        Vertex2::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} tube to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					Calls  \link Radius::dump Radius::dump \endlink .
					Calls  \link Vertex2::dump Vertex2::dump \endlink .
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
					This method handles the export of {\em *this} tube into another
					format (eg. POVRAY, VRML).  \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();

			private:

			Real radius_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/PRIMITIV/tube.iC>
#	endif
  /** @} */		
	} // namespace VIEW
	 
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_TUBE_H
