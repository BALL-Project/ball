// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularFilter.h,v 1.10 2003/02/25 15:53:36 sturm Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H
#define BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H

#ifndef BALL_VIEW_FUNCTOR_FILTER_H
#	include <BALL/VIEW/FUNCTOR/filter.h>
#endif

namespace BALL
{
	namespace MOLVIEW
	{
    /** \ingroup MolviewFunctorsHelper
     *  @{
     */			
		/** MolecularFilter class.
				The class MolecularFilter is a class that filters
				 \link Composite Composite \endlink  objects. The  \link visit visit \endlink  method implements the filter
				criteria. In this case this method filters  \link Composite Composite \endlink  objects
				that are of kind  \link Atom Atom \endlink  or of kind  \link AtomContainer AtomContainer \endlink .
				This class is used by the class  \link MolecularControl MolecularControl \endlink  to filter
				 \link Composite Composite \endlink  objects that are of the kind specified above.  \par
				<b>Definition:</b> BALL/MOLVIEW/FUNCTOR/molecularFilter.h
		*/
		class MolecularFilter: public VIEW::Filter
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new filter.
					The state of {\em *this} filter is: result is <tt>false</tt>.
					@return      Filter new constructed filter
					@see         Filter
			*/
			MolecularFilter()
			  throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} molecularFilter.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~MolecularFilter()
				throw();

			/** Explicit default initialization.
					Calls  \link Filter::clear Filter::clear \endlink .
					@see  Filter::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls  \link Filter::destroy Filter::destroy \endlink .
					@see  Filter::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Visit method.
					This method sets the result to <tt>true</tt> if the  \link Composite Composite \endlink  object
					is of kind  \link Atom Atom \endlink  or of kind  \link AtomContainer AtomContainer \endlink , <tt>false</tt>
					otherwise
					@param  composite the  \link Composite Composite \endlink  object to be filtered by {\em *this} molecularFilter.
					@see    Filter
					@see    setResult_
			*/
			virtual void visit(Composite& composite);
			//@}
			
			protected:
			
  		private:
		};

	/** @} */
	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H
