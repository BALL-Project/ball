// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertexBuffer.h,v 1.1.2.2 2005/01/18 16:24:10 amoll Exp $

#ifndef BALL_VIEW_RENDERING_VERTEXBUFFER_H
#define BALL_VIEW_RENDERING_VERTEXBUFFER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
#	include <BALL/VIEW/KERNEL/common.h>
#endif


namespace BALL
{
	namespace VIEW
	{
		class Mesh;
		class GLRenderer;

		class BALL_EXPORT MeshBuffer
		{
			public:

			typedef unsigned int Buffer;

			///
			MeshBuffer();
			
			///
			MeshBuffer(const MeshBuffer& mesh_buffer);

			///
			~MeshBuffer();

			///
			const MeshBuffer& operator = (const MeshBuffer& buffer)
				throw();

			///
			const Mesh* getMesh() { return mesh_;}

			///
			void setMesh(const Mesh& mesh) {mesh_ = & mesh;}

			///
			bool initialize();

			///
			bool draw();

			///
			void clear();

			///
			void clearBuffer();

			static void setGLRenderer(GLRenderer* renderer) { gl_renderer_ = renderer;}
			
			protected:
			
			const Mesh* mesh_;
			Buffer buffers_[4];
			bool filled_;
			static GLRenderer* gl_renderer_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_VERTEXBUFFER_H
