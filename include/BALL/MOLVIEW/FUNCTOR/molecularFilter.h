// $Id: molecularFilter.h,v 1.3 2000/12/12 16:16:52 oliver Exp $

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
		
		/**
		*/
		class MolecularFilter
			: public Filter
		{
			public:
			
			/**	@name	Enums
			 */
			//@{
			//@}


			/**	@name	Type definitions
			*/
			//@{
			//@}

			/**	@name	Constructors and Destructors
			*/
			//@{

			MolecularFilter();

			virtual ~MolecularFilter()
				throw();
			//@}

			/**	@name	Asignment
			*/
			//@{
			//@}

			
			/**	@name	Accessors
			*/
			//@{
			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{
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
