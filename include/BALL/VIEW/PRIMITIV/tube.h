// $Id: tube.h,v 1.6 2000/12/21 17:03:45 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#define BALL_VIEW_PRIMITIV_TUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_BASETUBE_H
#	include <BALL/VIEW/PRIMITIV/basetube.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class Tube
			: public BaseTube,
				public ColorExtension
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Tube()
				throw();

			Tube(const Tube& tube, bool deep = true)
				throw();

			Tube(const GeometricObject& geometric_object)
				throw();

			virtual ~Tube()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set(const Tube& tube, bool deep = true)
				throw();

			const Tube& operator = (const Tube& tube)
				throw();

			void get(Tube& tube, bool deep = true) const
				throw();

			void swap(Tube& tube)
				throw();
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}
				 

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream& s)
				throw();

			virtual void write(std::ostream& s) const
				throw();
			//@}

				
			protected:

			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/tube.iC>
#		endif
		
	} // namespace VIEW
	 
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_TUBE_H
