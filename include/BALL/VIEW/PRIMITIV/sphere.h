// $Id: sphere.h,v 1.4 2000/12/12 16:15:48 oliver Exp $

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#define BALL_VIEW_PRIMITIV_SPHERE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_EXPORT_H
//#	include <BALL/VIEW/extract.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#	include <BALL/VIEW/KERNEL/vertex1.h>
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
		class Sphere
			: public GeometricObject,
				public ColorExtension,
				public Vertex,
				public Radius
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Sphere();

			Sphere(const Sphere& sphere, bool deep = true);

			Sphere(const GeometricObject& geometric_object);

			virtual ~Sphere()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set(const Sphere& sphere, bool deep = true);

			Sphere& operator = (const Sphere& sphere);

			void get(Sphere& sphere, bool deep = true) const;

			void swap(Sphere& sphere);
			//@}


			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream&  s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{
			virtual void read(std::istream&  s);

			virtual void write(std::ostream& s) const;
			//@}


			protected:

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/sphere.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SPHERE_H
