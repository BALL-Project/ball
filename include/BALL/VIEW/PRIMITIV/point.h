// $Id: point.h,v 1.2 1999/12/19 17:19:42 oliver Exp $

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

			Point();

			Point(const Point& point, bool deep = true);

			Point(const GeometricObject& geometric_object);

			virtual ~Point();

			virtual void clear();

			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			void set(const Point& point, bool deep = true);

			Point& operator = (const Point& point);

			void get(Point& point, bool deep = true) const;

			void swap(Point& point);
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream&  s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;
			//@}


			protected:

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/point.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_POINT_H
