// $Id: filter.h,v 1.2 2000/12/12 16:15:40 oliver Exp $

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
		
		/**
		*/
		class Filter
			: public Visitor<Composite>
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

			Filter();

			virtual ~Filter()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			/**	@name	Asignment
			*/
			//@{
			//@}

			
			/**	@name	Accessors
			*/
			//@{
			bool getResult() const;

			operator bool () const;

			operator int () const;

			operator long () const;

			operator void* () const;

			virtual void visit(Composite& composite);
			//@}
			

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			protected:

			void setResult_(bool result);

			
  		private:

			bool object_passed_;
		};

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/VIEW/FUNCTOR/filter.iC>
#			endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_FUNCTOR_FILTER_H
