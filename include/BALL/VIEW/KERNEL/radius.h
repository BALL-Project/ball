// $Id: radius.h,v 1.2 1999/12/19 17:19:29 oliver Exp $

#ifndef BALL_VIEW_KERNEL_RADIUS_H
#define BALL_VIEW_KERNEL_RADIUS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		class Radius
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/
			//@{

			Radius();

			Radius(const Radius& r, bool deep = true);

			virtual ~Radius();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set(const Radius& r, bool deep = true);

			Radius& operator = (const Radius& r);

			void get(Radius& r, bool deep = true) const;

			void swap(Radius& r);
			//@}

			/**	@name	 Accessors
			*/
			//@{
			void setRadius(const Real r);

			Real getRadius();

			void getRadius(Real& r) const;
			//@}
			
			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump(std::ostream& s = std::cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}


			private:

			/* radius */
			Real radius_;

		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/radius.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_RADIUS_H
