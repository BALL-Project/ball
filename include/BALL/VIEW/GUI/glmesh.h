// $Id: glmesh.h,v 1.1 2000/09/23 12:50:11 hekl Exp $

#ifndef BALL_VIEW_OPENGL_PRIMITIV_GLMESH_H
#define BALL_VIEW_OPENGL_PRIMITIV_GLMESH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_OPENGL_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/OPENGL/KERNEL/glObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
#	include <BALL/VIEW/PRIMITIV/mesh.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class GLMesh
			: public Mesh,
				public GLObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			GLMesh();

			GLMesh(const GLMesh& mesh, bool deep = true);

			GLMesh(const GeometricObject& geometric_object);

			virtual ~GLMesh();

			virtual void clear();

			virtual void destroy();
			//@}


			protected:

			virtual bool draw(bool with_names = false);

			virtual bool drawUserDefined();

			virtual bool extract();
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/OPENGL/PRIMITIV/glmesh.iC>
#		endif
		
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_OPENGL_PRIMITIV_GLMESH_H
