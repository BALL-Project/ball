// $Id: mesh.h,v 1.5 2000/12/21 17:03:45 amoll Exp $

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

			Mesh()
				throw();

			Mesh(const Mesh& mesh, bool deep = true)
				throw();

			Mesh(const GeometricObject& geometric_object)
				throw();

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

			void set(const Mesh& mesh, bool deep = true)
				throw();

			const Mesh& operator = (const Mesh& mesh)
				throw();

			void get(Mesh& mesh, bool deep = true) const
				throw();

			void swap(Mesh& mesh)
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
			virtual void read(std::istream&  s)
				throw();

			virtual void write(std::ostream& s) const
				throw();
			//@}


			protected:

			virtual bool extract()
				throw();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/PRIMITIV/mesh.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_MESH_H
