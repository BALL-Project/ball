// $Id: mesh.h,v 1.1 2000/04/25 15:25:11 hekl Exp $

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#define BALL_VIEW_PRIMITIV_MESH_H

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
				public ColorExtension
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Mesh();

			Mesh(const Mesh& mesh, bool deep = true);

			Mesh(const GeometricObject& geometric_object);

			virtual ~Mesh();

			virtual void clear();

			virtual void destroy();
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
				(std::ostream&  s = std::cout, Size depth = 0) const;
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
