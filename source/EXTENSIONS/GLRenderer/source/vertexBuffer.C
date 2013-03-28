// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertexBuffer.C,v 1.6.16.1 2007/03/25 21:57:02 oliver Exp $

#include <BALL/CONFIG/config.h>

#ifdef BALL_HAS_GLEW
#include <GL/glew.h>

#include <BALL/VIEW/RENDERING/RENDERERS/glRenderer.h>
#include <BALL/VIEW/RENDERING/vertexBuffer.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{

		GLRenderer* MeshBuffer::gl_renderer_ = 0;

		MeshBuffer::MeshBuffer()
		: mesh_(0),
			buffer_(),
			filled_(false),
			busy_(false),
			multiple_colors_(false),
			color_("0000FF"),
			vertices_(0),
			triangles_(0)
		{
			initializeOpenGLFunctions();
			buffer_[0] = buffer_[1] = buffer_[2] = buffer_[3] = 0;
		}

		MeshBuffer::MeshBuffer(const MeshBuffer& mesh_buffer)
		: QOpenGLFunctions(mesh_buffer),
			mesh_(mesh_buffer.mesh_),
			buffer_(),
			filled_(false),
			busy_(false),
			multiple_colors_(false),
			color_("0000FF")
		{
			buffer_[0] = buffer_[1] = buffer_[2] = buffer_[3] = 0;
		}

		const MeshBuffer& MeshBuffer::operator = (const MeshBuffer& mesh_buffer)
		{
			mesh_ = mesh_buffer.mesh_;
			filled_ = false;
			buffer_[0] = buffer_[1] = buffer_[2] = buffer_[3] = 0;
			return *this;
		}

		bool MeshBuffer::initialize()
		{
			if (mesh_ == 0 || busy_) return false;

			busy_ = true;

			clearBuffer();

			vertices_  = mesh_->vertex.size();
			triangles_ = mesh_->triangle.size();
			
			// colors, normals, indices, vertex
			// Get valid Names
			glGenBuffers(4, buffer_);

			////////////////////////////////////////////////////////////
			/// upload colors, normals and vertices
			////////////////////////////////////////////////////////////
			float* data = new float[vertices_ * 4];
			if (data == 0) return false;
			for (Size index = 0; index < vertices_; ++index)
			{
				const Size start = index * 3;
				data[start] = mesh_->vertex[index].x;
				data[start + 1] = mesh_->vertex[index].y;
				data[start + 2] = mesh_->vertex[index].z;
			}

			glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_ * 3, data, GL_STATIC_DRAW);

			for (Size index = 0; index < vertices_; ++index)
			{
				const Size start = index * 3;
				data[start] = mesh_->normal[index].x;
				data[start + 1] = mesh_->normal[index].y;
				data[start + 2] = mesh_->normal[index].z;
			}

			glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_ * 3, data, GL_STATIC_DRAW);

			if (mesh_->colors.size() > 1)
			{
				multiple_colors_ = true;
				for (Size index = 0; index < vertices_; ++index)
				{
					const Size start = index * 4;
					data[start] = (float) mesh_->colors[index].getRed();
					data[start + 1] = (float) mesh_->colors[index].getGreen();
					data[start + 2] = (float) mesh_->colors[index].getBlue();
					data[start + 3] = (float) mesh_->colors[index].getAlpha();
				}

				glBindBuffer(GL_ARRAY_BUFFER, buffer_[2]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_ * 4, data, GL_STATIC_DRAW);
			}
			else
			{
				multiple_colors_ = false;
				if (mesh_->colors.size() == 1)
				{
					color_ = mesh_->colors[0];
				}
			}

			delete[] data;

			////////////////////////////////////////////////////////////
			/// upload triangle indices
			////////////////////////////////////////////////////////////
			unsigned int* indices = new unsigned int[triangles_ * 3];
			if (indices == 0) return false;
			for (Size index = 0; index < triangles_; ++index)
			{
				const Size start = index * 3;
				indices[start] = mesh_->triangle[index].v1;
				indices[start + 1] = mesh_->triangle[index].v2;
				indices[start + 2] = mesh_->triangle[index].v3;
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[3]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * triangles_ * 3,
											indices, GL_STATIC_DRAW);
			delete[] indices;

			////////////////////////////////////////////////////////////
			/// cleanup
			////////////////////////////////////////////////////////////
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			filled_ = true;
			busy_ = false;
			return true;
		}

		void MeshBuffer::clear()
		{
			while (busy_)
			{
		#ifdef BALL_OS_WINDOWS
				Sleep(1);
		#else
				// sleep(1);
		#endif
			}
			mesh_ = 0;
			clearBuffer();
		}

		void MeshBuffer::clearBuffer()
		{
			if (!filled_) return;

			glDeleteBuffers(4, buffer_);
			filled_ = false;
		}

		bool MeshBuffer::draw()
		{
			if (!filled_ || gl_renderer_ == 0 || busy_) return false;

			busy_ = true;

			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_INDEX_ARRAY);

			// vertices
			glBindBuffer(GL_ARRAY_BUFFER, buffer_[0]);
			glVertexPointer(3, GL_FLOAT, 0, 0); 

			// indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_[3]);
			glIndexPointer(GL_UNSIGNED_INT, 0, 0);

			// colors
			if (multiple_colors_)
			{
				glEnableClientState(GL_COLOR_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, buffer_[2]);
				glColorPointer (4, GL_FLOAT, 0, 0);
			}
			else
			{
				glColor4ub(color_.getRed(), color_.getGreen(), color_.getBlue(), color_.getAlpha());
			}


			////////////////////////////////////////////////////////////
			/// draw it
			////////////////////////////////////////////////////////////
			DrawingMode drawing_mode = gl_renderer_->getDrawingMode();
			if (drawing_mode == DRAWING_MODE_SOLID)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, buffer_[1]);
				glNormalPointer(GL_FLOAT, 0, 0);

				glDrawElements(GL_TRIANGLES, triangles_ * 3, GL_UNSIGNED_INT, 0);
				glDisableClientState(GL_NORMAL_ARRAY);
			}
			else if (drawing_mode == DRAWING_MODE_WIREFRAME)
			{
				// do nothing
			}
			else // dots
			{
				glDrawElements(GL_POINTS, vertices_ * 3, GL_UNSIGNED_INT, 0);
			}

			////////////////////////////////////////////////////////////
			/// cleanup
			////////////////////////////////////////////////////////////
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_INDEX_ARRAY);
			
			busy_ = false;
			return true;
		}

		MeshBuffer::~MeshBuffer()
		{
			clear();
		}

	} 
}

