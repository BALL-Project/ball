// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox.h,v 1.17 2003/03/26 13:09:05 sturm Exp $

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
# include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

      
		/** SimpleBox class.	
				An instance of SimpleBox represents an instance of the geometric representation "Box".
				A simpleBox is a box whose sides are parallel to the coordinate axes. Therefore only
				two vertices are needed. The first vertex is the lower left corner and the second
				vertex is the upper right corner of the box (in 3-dimensional space).
				A simpleBox has the following properties. 

				  - color - the color of the simpleBox
					- vertex1 - the first vertex of the simpleBox (lower left corner)
					- vertex2 - the second vertex of the simpleBox (upper right corner)
				
				The class SimpleBox is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink 
				and  \link Vertex2 Vertex2 \endlink . See these classes for further information concerning
				interface and additional methods.  \par
			\ingroup ViewPrimitives	
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

  				  - color - to the color black
		  			- vertex1 - to the vector (0,0,0)
		  			- vertex2 - to the vector (0,0,0)
					
					@return      SimpleBox new constructed simpleBox
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex2
			*/
			SimpleBox()
				throw();

			/** Copy constructor with cloning facility.
					Construct new simpleBox by copying the simpleBox {\em simpleBox}.
					The copy is either deep (default) or shallow.
					@param       simpleBox the simpleBox to be copied (cloned)
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>)
					@return      SimpleBox new constructed simpleBox copied from {\em simpleBox}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex2
			*/
			SimpleBox(const SimpleBox& simpleBox, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new simpleBox by copying the internal values from geometricObject 
					{\em geometric\_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      SimpleBox new constructed simpleBox initialized from {\em geometric\_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex2
			*/
			SimpleBox(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} simpleBox.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~SimpleBox()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink 
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink 
					Calls  \link Vertex2::clear Vertex2::clear \endlink 
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink 
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink 
					Calls  \link Vertex2::destroy Vertex2::destroy \endlink 
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
					the simpleBox {\em simpleBox}. \par
					@param       simpleBox the simpleBox to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em simpleBox}
					@see         SimpleBox
			*/
			void set(const SimpleBox& simpleBox, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the simpleBox {\em simpleBox} to {\em *this} simpleBox.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} simpleBox is initialized to the value 
					of the simpleBox {\em simpleBox}. \par
					@param       simpleBox the simpleBox to be copied
					@return      SimpleBox& {\em *this} simpleBox
					@see         set
			*/
			const SimpleBox& operator = (const SimpleBox& simpleBox)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} simpleBox to the simpleBox {\em simpleBox}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the simpleBox {\em simpleBox} is initialized to the
					value of {\em *this} simpleBox. \par
					@param       simpleBox the simpleBox to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em simpleBox}
					@see         set
			*/
			void get(SimpleBox& simpleBox, bool deep = true) const
				throw();

			/** Swapping of simpleBox's.
					Swap the value of {\em *this} simpleBox with the simpleBox {\em simpleBox}.
					@param       simpleBox the simpleBox being swapped with {\em *this} simpleBox 
					@see         SimpleBox
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
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					Calls  \link Vertex2::isValid Vertex2::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} simpleBox is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
					@see        Vertex2::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} simpleBox to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					Calls  \link Vertex2::dump Vertex2::dump \endlink .
					@param   s output stream where to output the value of {\em *this} simpleBox
					@param   depth the dumping depth
					@see        GeometricObject::dump
					@see        ColorExtension::dump
					@see        Vertex2::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
		

			protected:

			/** Export method.
					This method handles the export of {\em *this} simpleBox into another
					format (eg. POVRAY, VRML)  \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();
		};
  
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SIMPLEBOX_H
