// $Id: label.h,v 1.7 2001/05/13 13:40:37 hekl Exp $

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

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#	include <BALL/VIEW/KERNEL/vertex1.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** Label class.
				{\bf Framework:} BALL/VIEW/PRIMITIV\\
				{\bf Definition:} \URL{BALL/VIEW/PRIMITIV/label.h}\\ \\
				An instance of Label represents an instance of the geometric representation "label".
				A label is an information text that can be pinned to another \Ref{Composite} or
				\Ref{GeometricObject}. A label is both visible in the dynamic and static render
				mode of the \Ref{Scene}.
				A label has the following properties. 
				\begin{itemize}
				  \item color  - the color of the label
					\item text   - the text of the label
					\item vertex - the position of the label			
				\end{itemize}
				The class Label is derived from the classes \Ref{GeometricObject}, \Ref{ColorExtension}
				and \Ref{Vertex}. See these classes for further information concerning
				interface and additional methods.
				@memo    Label class (BALL VIEW primitiv framework)
				@author  $Author: hekl $
				@version $Revision: 1.7 $
				@date    $Date: 2001/05/13 13:40:37 $
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
					@param       deep make a deep (= {\tt true}) or shallow (= {\tt false})
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
					Calls \Ref{destroy}.
					@see  destroy
			*/
			virtual ~Label()
				throw();

			/** Explicit default initialization.
					Calls \Ref{GeometricObject::clear}
					Calls \Ref{ColorExtension::clear}
					Calls \Ref{Vertex::clear}
					@see  GeometricObject::clear
					@see  ColorExtension::clear
					@see  Vertex::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{GeometricObject::destroy}
					Calls \Ref{ColorExtension::destroy}
					Calls \Ref{Vertex::destroy}
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
					the label {\em label}.\\
					@param       label the label to be copied
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em label}
			*/
			void set(const Label& label, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the label {\em label} to {\em *this} label.
					The copy is deep.
					Calls \Ref{set}.
					The value of {\em *this} label is initialized to the value 
					of the label {\em label}.\\
					@param       label the label to be copied
					@return      Label& {\em *this} label
					@see         set
			*/
			const Label& operator = (const Label& label)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} label to the label {\em label}.
					The copy is either deep (default) or shallow.
					Calls \Ref{set}.
					The value of the label {\em label} is initialized to the
					value of {\em *this} label.\\
					@param       label the label to be assigned to
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em label}
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
					Calls \Ref{GeometricObject::setName}
					@param       text the new text of {\em *this} label
					@see         GeometricObject::setName
			*/
			void setText(const String text)
				throw();

			/** Inspection of the name of the label.
					Access the name of {\em *this} label.
					Calls \Ref{GeometricObject::getName}
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
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls \Ref{GeometricObject::isValid}.
					Calls \Ref{Vertex::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} label is correct (self-validated) and consistent, {\tt false} otherwise
					@see        GeometricObject::isValid
					@see        Vertex::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of {\em *this} label to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GeometricObject::dump}.
					Calls \Ref{ColorExtension::dump}.
					Calls \Ref{Vertex::dump}.
					@param   s output stream where to output the value of {\em *this} label
					@param   depth the dumping depth
					@see     GeometricObject::dump
					@see     ColorExtension::dump
					@see     Vertex::dump
			*/
			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent label data from the input stream {\em s} and 
				  restore the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.	 
				  @param       s input stream from where to restore the internal state of {\em *this} label
			*/
			virtual void read(std::istream&  s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent label data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} label
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}


			protected:

			/** Export method.
					This method handles the export of {\em *this} label into another
					format (eg. POVRAY, VRML)  \\
				  {\bf Note:} Not yet implemented.
					@return    bool {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract()
				throw();

  		private:	
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/label.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_LABEL_H
