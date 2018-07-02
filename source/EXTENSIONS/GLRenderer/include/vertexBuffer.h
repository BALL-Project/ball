#ifndef BALL_GLRENDERER_VERTEXBUFFER_H
#define BALL_GLRENDERER_VERTEXBUFFER_H

#include <BALL/common.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/DATATYPE/colorRGBA.h>

#include <QtGui/QOpenGLFunctions>

namespace BALL
{
	namespace VIEW
	{
		class Mesh;
	}

	namespace GLRenderer
	{
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
		class BALL_DEPRECATED BALL_VIEW_EXPORT MeshBuffer : protected QOpenGLFunctions
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
			const BALL::VIEW::Mesh* getMesh() { return mesh_;}

			/// Set the Mesh which shall be drawn with this buffer object
			void setMesh(const BALL::VIEW::Mesh& mesh) {mesh_ = & mesh;}

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
			
			const BALL::VIEW::Mesh* 				mesh_;
 			Buffer 							buffer_;
			bool 								filled_;
			static GLRenderer* 	gl_renderer_;
			bool 								busy_;
			bool 								multiple_colors_;
			BALL::VIEW::ColorRGBA 					color_;
			Size 								vertices_;
			Size 								triangles_;
		};

	} // namespace GLRenderer
} // namespace BALL

#endif // BALL_GLRENDERER_VERTEXBUFFER_H
