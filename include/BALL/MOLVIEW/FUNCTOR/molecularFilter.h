// $Id: molecularFilter.h,v 1.5 2001/07/15 18:49:18 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H
#define BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_FILTER_H
#	include <BALL/VIEW/FUNCTOR/filter.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif


using namespace BALL::VIEW;

  
namespace BALL
{

	namespace MOLVIEW
	{
		
		/** MolecularFilter class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/molecularFilter.h} \\
				{\bf Concept:} visitor design pattern\\ \\
				The class MolecularFilter is a class that filters
				\Ref{Composite} objects. The \Ref{visit} method implements the filter
				criteria. In this case this method filters \Ref{Composite} objects
				that are of kind \Ref{Atom} or of kind \Ref{AtomContainer}.
				This class is used by the class \Ref{MolecularControl} to filter
				\Ref{Composite} objects that are of the kind specified above.
				@memo    MolecularFilter class (BALL MOLVIEW functor framework)
				@author  $Author: oliver $
				@version $Revision: 1.5 $
				@date    $Date: 2001/07/15 18:49:18 $
		*/
		class MolecularFilter: public Filter
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new filter.
					The state of {\em *this} filter is: result is {\tt false}.
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
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~MolecularFilter()
				throw();

			/** Explicit default initialization.
					Calls \Ref{Filter::clear}.
					@see  Filter::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Calls \Ref{Filter::destroy}.
					@see  Filter::destroy
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Visit method.
					This method sets the result to {\tt true} if the \Ref{Composite} object
					is of kind \Ref{Atom} or of kind \Ref{AtomContainer}, {\tt false}
					otherwise
					@param  composite the \Ref{Composite} object to be filtered by {\em *this} molecularFilter.
					@see    Filter
					@see    setResult_
			*/
			virtual void visit(Composite& composite);
			//@}
			
			protected:
			
  		private:
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/molecularFilter.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_MOLECULARFILTER_H
