
// $Id: basetube.h,v 1.6 2000/12/21 17:03:45 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_BASETUBE_H
#define BALL_VIEW_PRIMITIV_BASETUBE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#	include <BALL/VIEW/KERNEL/vertex2.h>
#endif

#ifndef BALL_VIEW_KERNEL_RADIUS_H
#	include <BALL/VIEW/KERNEL/radius.h>
#endif

namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class BaseTube
			: public GeometricObject,
				public Radius,
				public Vertex2
		{
			public:
			/**	@name	Constructors and Destructors
			*/
			//@{

			BaseTube()
				throw();

			BaseTube(const BaseTube& base_tube, bool deep = true)
				throw();

			BaseTube(const GeometricObject& geometric_object)
				throw();

			virtual ~BaseTube()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set(const BaseTube& base_tube, bool deep = true)
				throw();

			const BaseTube& operator = (const BaseTube& base_tube)
				throw();

			void get(BaseTube& base_tube, bool deep = true) const
				throw();

			void swap(BaseTube& base_tube)
				throw();
			//@}

			/**	@name	Accessors
			*/
			//@{

			Real getLength() const
				throw();

			Real getSquareLength() const
				throw();
			//@}


			/**	@name Debugging and Diagnostics
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
#			include <BALL/VIEW/PRIMITIV/basetube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_BASETUBE_H
