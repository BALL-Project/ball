// $Id: mesh.h,v 1.3 2000/12/12 16:15:48 oliver Exp $

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#define BALL_VIEW_PRIMITIV_MESH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_SURFACE_H
#	include <BALL/MATHS/surface.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_COLOREXTENSION1_H
#	include <BALL/VIEW/KERNEL/colorExtension1.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class Mesh
			: public GeometricObject,
				public ColorExtension,
			  public Surface
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Mesh();

			Mesh(const Mesh& mesh, bool deep = true);

			Mesh(const GeometricObject& geometric_object);

			virtual ~Mesh()
				throw();

			virtual void clear()
				throw();

			virtual void destroy()
				throw();
			//@}

			
			/**	@name	Assignment
			*/
			//@{

			void set(const Mesh& mesh, bool deep = true);

			Mesh& operator = (const Mesh& mesh);

			void get(Mesh& mesh, bool deep = true) const;

			void swap(Mesh& mesh);
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
#			include <BALL/VIEW/PRIMITIV/mesh.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_MESH_H
