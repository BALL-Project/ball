// $Id: sphere.h,v 1.1 1999/08/26 07:53:16 oliver Exp $

#ifndef BALL_VIEW_PRIMITIV_SPHERE_H
#define BALL_VIEW_PRIMITIV_SPHERE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_EXPORT_H
//#	include <BALL/VIEW/export.h>
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

			virtual ~Sphere();

			virtual void clear();

			virtual void destroy();
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
				(ostream&  s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{
			virtual void read(istream&  s);

			virtual void write(ostream& s) const;
			//@}


			protected:

			virtual bool export();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/sphere.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_SPHERE_H
