// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: filter.h,v 1.7 2003/02/21 16:07:13 anhi Exp $

#ifndef BALL_VIEW_FUNCTOR_FILTER_H
#define BALL_VIEW_FUNCTOR_FILTER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_VISITOR_H
#	include <BALL/CONCEPT/visitor.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

  
namespace BALL
{

	namespace VIEW
	{
		
		/** Filter class.
				The class Filter is a base class for filter objects. This class filters
				 \link Composite Composite \endlink  objects. Override the method  \link visit visit \endlink  to
				specify the needed filter criteria und use the method  \link setResult_ setResult_ \endlink  
				to set the result of the applied filter criteria.
				In this class the default implementation filters for  \link GeometricObjects GeometricObjects \endlink .
				So if a  \link Composite Composite \endlink  object that if of kind  \link GeometricObject GeometricObject \endlink  is visited 
				from this class the method  \link getResult getResult \endlink  returns <tt>true</tt>, otherwise
				<tt>false</tt>.
				This class is used by the class  \link Control Control \endlink  to filter for  \link GeometricObject GeometricObject \endlink 
				objects.  \par
				<b>Definition:</b> BALL/VIEW/FUNCTOR/filter.h
				@see     Control
		*/
		class Filter: public Visitor<Composite>
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new filter.
					The state of {\em *this} filter is: result is <tt>false</tt>.
					@return      Filter new constructed filter
			*/
			Filter()
				throw();
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} filter.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Filter()
				throw();

			/** Explicit default initialization.
					Set the result of {\em *this} filter to <tt>false</tt>.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
					@see  ~Filter
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Inspection of the result.
					Access the result of {\em *this} filter.
					@return bool <tt>true</tt> if the  \link Composite Composite \endlink  object passed through {\em *this} filter, <tt>false</tt> otherwise
					@see    setResult_
			*/
			bool getResult() const
				throw();

			/** Visit method.
					Override this method for defining the filter criteria. 
					Set the result if the  \link Composite Composite \endlink  object passed with the 
					 \link setResult_ setResult_ \endlink  method.
					This method sets the result to <tt>true</tt> if the  \link Composite Composite \endlink  object
					is of kind  \link GeometricObject GeometricObject \endlink  (Default implementation). 
					@param  composite the  \link Composite Composite \endlink  object to be filtered by {\em *this} filter.
					@see    setResult_
			*/
			virtual void visit(Composite& composite);
			//@}
			
			/** @name Conversion methods
			*/
			//@{
			/** Conversion to bool.
					Cast {\em *this} filter to a boolean value.
					@return  bool <tt>true</tt> if the  \link Composite Composite \endlink  object passed through {\em *this} filter, <tt>false</tt> otherwise
					@see     getResult
					@see     setResult_
			*/
			operator bool () const
				throw();

			/** Conversion to int.
					Cast {\em *this} filter to a integer value.
					@return  int <tt>1</tt> if the  \link Composite Composite \endlink  object passed through {\em *this} filter, <tt>0</tt> otherwise
					@see     getResult
					@see     setResult_
			*/
			operator int () const
				throw();

			/** Conversion to long.
					Cast {\em *this} filter to a long value.
					@return  int <tt>1</tt> if the  \link Composite Composite \endlink  object passed through {\em *this} filter, <tt>0</tt> otherwise
					@see     getResult
					@see     setResult_
			*/
			operator long () const
				throw();

			/** Conversion to void*.
					Cast {\em *this} filter to a void* value.
					@return  int <tt>(void*)1</tt> if the  \link Composite Composite \endlink  object passed through {\em *this} filter, <tt>(void*)0</tt> otherwise
					@see     getResult
					@see     setResult_
			*/
			operator void* () const
				throw();
			//@}

			protected:

			/** @name Protected methods.
			*/
			//@{
			/** Change the result.
					Change the result of {\em *this} filter to the value of the parameter
					{\em result}. With this method a derived class can set the result
					value of {\em *this} filter so all the implemented access methods
					are still functioning.
					Use this method to set the result value in a derived class in the overriden
					method  \link visit visit \endlink .
					@param  result the new result value of {\em *this} filter.
					@see    visit
					@see    getResult
			*/
			void setResult_(bool result)
				throw();
			//@}
			
  		private:

			bool object_passed_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/VIEW/FUNCTOR/filter.iC>
#			endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_FUNCTOR_FILTER_H
