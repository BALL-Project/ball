// $Id: basetube.h,v 1.4 2000/12/12 16:15:47 oliver Exp $

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
#			include <BALL/VIEW/PRIMITIV/basetube.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_BASETUBE_H
