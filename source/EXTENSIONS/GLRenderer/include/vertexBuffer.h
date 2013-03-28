// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertexBuffer.h,v 1.6 2005/12/23 17:02:21 amoll Exp $
//

#ifndef BALL_VIEW_RENDERING_RENDERERS_VERTEXBUFFER_H
#define BALL_VIEW_RENDERING_RENDERERS_VERTEXBUFFER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
#	include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
#	include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

//#include <QtGui/QOpenGLFunctions>

namespace BALL
{
	namespace VIEW
	{
		class Mesh;
		class GLRenderer;

		/** Wrapper class to draw BALLView Mesh instances with OpenGL vertex buffer objects.
				Vertex Buffer Objects are an OpenGL extension available since OpenGL version 1.5.
				\par
				See http://oss.sgi.com/projects/ogl-sample/registry/EXT/pixel_buffer_object.txt 
				\par
				Vertex Buffer Objects can drasticaly increase drawing speed for triangulated surfaces
				(up to 5 fold) compared with OpenGL display lists.
				To ensure a maximum of platform independence we use method pointers in the source file.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT MeshBuffer //: protected QOpenGLFunctions
		{
			public:

			BALL_CREATE(MeshBuffer)

			typedef GLuint Buffer[4];

			///
			MeshBuffer();
			
			///
			MeshBuffer(const MeshBuffer& mesh_buffer);

			///
			virtual ~MeshBuffer();

			///
			const MeshBuffer& operator = (const MeshBuffer& buffer);

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

			/// Return true, if currently updateing or drawing.
			bool isBusy() const { return busy_;}

			/** Set the GLRender.
					This GLRender pointer is used to know if to draw the mesh transparent solid or transparent,
					as solid triangles or as points.
					Meshes can currently not drawn in wireframe mode with vertex buffer.
			*/
			static void setGLRenderer(GLRenderer* renderer) { gl_renderer_ = renderer;}
			
			protected:
			
			const Mesh* 				mesh_;
 			Buffer 							buffer_;
			bool 								filled_;
			static GLRenderer* 	gl_renderer_;
			bool 								busy_;
			bool 								multiple_colors_;
			ColorRGBA 					color_;
			Size 								vertices_;
			Size 								triangles_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_VERTEXBUFFER_H
