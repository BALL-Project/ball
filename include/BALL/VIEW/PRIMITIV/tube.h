// $Id: tube.h,v 1.5 2000/12/19 21:54:08 oliver Exp $

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

			Tube();

			Tube(const Tube& tube, bool deep = true);

			Tube(const GeometricObject& geometric_object);

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

			void set(const Tube& tube, bool deep = true);

			Tube& operator = (const Tube& tube);

			void get(Tube& tube, bool deep = true) const;

			void swap(Tube& tube);
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}
				 

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}

				
			protected:

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/tube.iC>
#		endif
		
	} // namespace VIEW
	 
} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_TUBE_H
