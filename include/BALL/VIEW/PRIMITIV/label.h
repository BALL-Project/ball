// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: label.h,v 1.14 2003/03/03 14:18:35 anhi Exp $

#ifndef BALL_VIEW_PRIMITIV_LABEL_H
#define BALL_VIEW_PRIMITIV_LABEL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT
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

		/** Label class.
				An instance of Label represents an instance of the geometric representation "label".
				A label is an information text that can be pinned to another  \link Composite Composite \endlink  or
				 \link GeometricObject GeometricObject \endlink . A label is both visible in the dynamic and static render
				mode of the  \link Scene Scene \endlink .
				A label has the following properties. 
				\begin{itemize}
				  \item color  - the color of the label
					\item text   - the text of the label
					\item vertex - the position of the label			
				\end{itemize}
				The class Label is derived from the classes  \link GeometricObject GeometricObject \endlink ,  \link ColorExtension ColorExtension \endlink 
				and  \link Vertex Vertex \endlink . See these classes for further information concerning
				interface and additional methods.  \par
				<b>Definition:</b> BALL/VIEW/PRIMITIV/label.h
		*/
		class Label
			: public GeometricObject,
				public ColorExtension,
				public Vertex
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new label.
					The properties of {\em *this} label are set to:
					\begin{itemize}
  				  \item color  - to the color black
						\item text   - to the text "unkown"
		  			\item vertex - to the vector (0,0,0)
					\end{itemize}
					@return      Label new constructed label
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
			*/
			Label()
				throw();

			/** Copy constructor with cloning facility.
					Construct new label by copying the label {\em label}.
					The copy is either deep (default) or shallow.
					@param       label the label to be copied (cloned)
					@param       deep make a deep (= <tt>true</tt>) or shallow (= <tt>false</tt>)
					@return      Label new constructed label copied from {\em label}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
			*/
			Label(const Label& label, bool deep = true)
				throw();

			/** Copy constructor from geometricObject.
					Construct new label by copying the internal values from geometricObject 
					{\em geometric_object}.
					@param       geometric_object the geometricObject which internal value should be copied
					@return      Label new constructed label initialized from {\em geometric_object}
					@see         GeometricObject
					@see         ColorExtension
					@see         Vertex
			*/
			Label(const GeometricObject& geometric_object)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} label.
					Calls  \link destroy destroy \endlink .
					@see  destroy
			*/
			virtual ~Label()
				throw();

			/** Explicit default initialization.
					Calls  \link GeometricObject::clear GeometricObject::clear \endlink 
					Calls  \link ColorExtension::clear ColorExtension::clear \endlink 
					Calls  \link Vertex::clear Vertex::clear \endlink 
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link GeometricObject::destroy GeometricObject::destroy \endlink 
					Calls  \link ColorExtension::destroy ColorExtension::destroy \endlink 
					Calls  \link Vertex::destroy Vertex::destroy \endlink 
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
					Assign the label {\em label} to {\em *this} label.
					The copy is either deep (default) or shallow.
					The value of {\em *this} label is initialized to the value of 
					the label {\em label}. \par
					@param       label the label to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em label}
			*/
			void set(const Label& label, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the label {\em label} to {\em *this} label.
					The copy is deep.
					Calls  \link set set \endlink .
					The value of {\em *this} label is initialized to the value 
					of the label {\em label}. \par
					@param       label the label to be copied
					@return      Label& {\em *this} label
					@see         set
			*/
			const Label& operator = (const Label& label)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} label to the label {\em label}.
					The copy is either deep (default) or shallow.
					Calls  \link set set \endlink .
					The value of the label {\em label} is initialized to the
					value of {\em *this} label. \par
					@param       label the label to be assigned to
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em label}
					@see         set
			*/
			void get(Label& label, bool deep = true) const
				throw();

			/** Swapping of label's.
					Swap the value of {\em *this} label with the label {\em label}.
					@param       label the label being swapped with {\em *this} label 
					@see         Label
			*/
			void swap(Label& label)
				throw();

			//@}
			/**	@name	Inspectors, Mutators, Accessors
			*/
			//@{

			/** Change the name of the label.
					Change the name of {\em *this} label represented by the parameter {\em text}.
					Calls  \link GeometricObject::setName GeometricObject::setName \endlink 
					@param       text the new text of {\em *this} label
					@see         GeometricObject::setName
			*/
			void setText(const String text)
				throw();

			/** Inspection of the name of the label.
					Access the name of {\em *this} label.
					Calls  \link GeometricObject::getName GeometricObject::getName \endlink 
					@return   String the name of {\em *this} label
					@see      GeometricObject::getName
			*/
			String getText() const
				throw();

			/** Return the type name of {\em *this} label.
					Virtually overridden method for specifying the type name of {\em *this} label.
					Important for identifying the type of the label by other objects.
					@return  String the type name of {\em *this} label ("Label")
					@see     GeometricObject::getTypeName
			*/
				virtual String getTypeName() const
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} label.
					If the internal state of {\em *this} label is correct (self-validated) and 
					consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls  \link GeometricObject::isValid GeometricObject::isValid \endlink .
					Calls  \link Vertex::isValid Vertex::isValid \endlink .
					@return			bool <tt>true</tt> if the internal state of {\em *this} label is correct (self-validated) and consistent, <tt>false</tt> otherwise
					@see        GeometricObject::isValid
					@see        Vertex::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} label to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GeometricObject::dump GeometricObject::dump \endlink .
					Calls  \link ColorExtension::dump ColorExtension::dump \endlink .
					Calls  \link Vertex::dump Vertex::dump \endlink .
					@param   s output stream where to output the value of {\em *this} label
					@param   depth the dumping depth
					@see     GeometricObject::dump
					@see     ColorExtension::dump
					@see     Vertex::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}


			protected:

			/** Export method.
					This method handles the export of {\em *this} label into another
					format (eg. POVRAY, VRML)   \par
				  <b>Note:</b> Not yet implemented.
					@return    bool <tt>true</tt> if successful,	<tt>false</tt> otherwise
			*/
			virtual bool extract()
				throw();

  		private:	
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/label.iC>
#		endif

  /** @} */
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LABEL_H
