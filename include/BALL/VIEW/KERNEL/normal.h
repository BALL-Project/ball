// $Id: normal.h,v 1.3 1999/12/28 18:37:47 oliver Exp $

#ifndef BALL_VIEW_KERNEL_NORMAL_H
#define BALL_VIEW_KERNEL_NORMAL_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	namespace VIEW
	{

		/**	
		*/
		class NormalVector
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			NormalVector();

			NormalVector(const NormalVector& n, bool deep = true);

			virtual ~NormalVector();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set(const NormalVector& n, bool deep = true);

			NormalVector& operator = (const NormalVector& n);

			void get(NormalVector& n, bool deep = true) const;

			void swap(NormalVector& n);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setNormalVector(const Vector3& n);

			void setNormalVector
				(const Real x, const Real y, const Real z);

			Vector3& getNormalVector();

			const Vector3& getNormalVector() const;

			void getNormalVector(Vector3& n) const;

			void getNormalVector
				(Real& x, Real& y, Real& z) const;	
			//@}

			/**	@name	Debugging and Diagnostics
			*/	
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
			//@}
			
			
			private:

			/* normal vector */
			Vector3 normal_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/normal.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_NORMAL_H
