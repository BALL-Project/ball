// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertexBuffer.h,v 1.1.2.4 2005/01/19 12:52:16 amoll Exp $

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

		/** Wrapper class to draw BALLView Mesh instances with OpenGL vertex buffer objects.
				Vertex Buffer Objects are an OpenGL extensions available since version 1.5.
				See http://oss.sgi.com/projects/ogl-sample/registry/EXT/pixel_buffer_object.txt \\
				Vertex Buffer Objects can drasticaly increase drawing speed for triangulated surfaces
				(up to 5 fold!) compared with OpenGL display lists.
				To ensure a maximum of platform independence we use method pointers in the source file.
				These must be initialises by calling MeshBuffer::initGL() (See below).
		*/
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

			/** Initialsie the GL methods needed.
			    Call this method one time after having a valid GL context (e.g. in GLRenderer::init).
			*/
			static bool initGL();

			///
			const MeshBuffer& operator = (const MeshBuffer& buffer)
				throw();

			/// Get the Mesh for this buffer object
			const Mesh* getMesh() { return mesh_;}

			/// Set the Mesh which shall be drawn with this buffer object
			void setMesh(const Mesh& mesh) {mesh_ = & mesh;}

			/// Transfer all vertex, normal, index and color data of the mesh into the vertex buffer object.
			bool initialize();

			/// Draw the mesh from the buffer.
			bool draw();

			/// Unset the mesh pointer and clear the buffer
			void clear();

			/// Free the reserved (and filled) vertex buffer.
			void clearBuffer();

			/** Set the GLRender.
					This GLRender pointer is used to know if to draw the mesh transparent solid or transparent,
					as solid triangles or as points.
					Meshes can currently not drawn in wireframe mode with vertex buffer.
			*/
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
