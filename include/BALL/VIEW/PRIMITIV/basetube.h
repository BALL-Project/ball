// $Id: basetube.h,v 1.2 1999/12/19 17:19:41 oliver Exp $

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

			BaseTube();

			BaseTube(const BaseTube& base_tube, bool deep = true);

			BaseTube(const GeometricObject& geometric_object);

			virtual ~BaseTube();

			virtual void clear();

			virtual void destroy();
			//@}


			/**	@name	Assignment
			*/
			//@{

			void set(const BaseTube& base_tube, bool deep = true);

			BaseTube& operator = (const BaseTube& base_tube);

			void get(BaseTube& base_tube, bool deep = true) const;

			void swap(BaseTube& base_tube);
			//@}

			/**	@name	Accessors
			*/
			//@{

			Real getLength() const;

			Real getSquareLength() const;
			//@}


			/**	@name Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
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
#			include <BALL/VIEW/PRIMITIV/basetube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_BASETUBE_H
