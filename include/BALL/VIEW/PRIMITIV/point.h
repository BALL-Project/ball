// $Id: point.h,v 1.6 2000/12/21 17:03:45 amoll Exp $

#ifndef BALL_VIEW_PRIMITIV_POINT_H
#define BALL_VIEW_PRIMITIV_POINT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#	include <BALL/VIEW/KERNEL/vertex1.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/**
		*/
		class Point
			: public GeometricObject,
				public ColorExtension,
				public Vertex
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Point()
				throw();

			Point(const Point& point, bool deep = true)
				throw();

			Point(const GeometricObject& geometric_object)
				throw();

			virtual ~Point()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set(const Point& point, bool deep = true)
				throw();

			const Point& operator = (const Point& point)
				throw();

			void get(Point& point, bool deep = true) const
				throw();

			void swap(Point& point)
				throw();
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const
				throw();

			virtual void dump(std::ostream&  s = std::cout, Size depth = 0) const
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
#			include <BALL/VIEW/PRIMITIV/point.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_POINT_H
