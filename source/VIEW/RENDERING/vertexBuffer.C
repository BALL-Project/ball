// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertexBuffer.C,v 1.1.2.2 2005/01/16 20:47:56 amoll Exp $
//
#include <BALL/VIEW/RENDERING/vertexBuffer.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <qgl.h>
#include <GL/glext.h>

namespace BALL
{
	namespace VIEW
	{

MeshBuffer::MeshBuffer()
: mesh_(0),
	buffers_(),
	filled_(false)
{
	buffers_[0] = buffers_[1] = buffers_[2] = buffers_[3] = 0;
}

MeshBuffer::MeshBuffer(const MeshBuffer& mesh_buffer)
: mesh_(mesh_buffer.mesh_),
	buffers_(),
	filled_(mesh_buffer.filled_)
{
	buffers_[0] = mesh_buffer.buffers_[0];
	buffers_[1] = mesh_buffer.buffers_[1];
	buffers_[2] = mesh_buffer.buffers_[2];
	buffers_[3] = mesh_buffer.buffers_[3];
}

const MeshBuffer& MeshBuffer::operator = (const MeshBuffer& mesh_buffer)
	throw()
{
  mesh_ = mesh_buffer.mesh_;
	filled_ = mesh_buffer.filled_;
	buffers_[0] = mesh_buffer.buffers_[0];
	buffers_[1] = mesh_buffer.buffers_[1];
	buffers_[2] = mesh_buffer.buffers_[2];
	buffers_[3] = mesh_buffer.buffers_[3];
	return *this;
}

bool MeshBuffer::initialize()
{
	clearBuffer();
	if (mesh_ == 0) return false;

	Size nr_vertices = mesh_->vertex.size();
	Size nr_triangles = mesh_->triangle.size();

	float* vertex = new float[nr_vertices * 3];
	for (Size index = 0; index < nr_vertices; ++index)
	{
		const Size start = index * 3;
		vertex[start] = mesh_->vertex[index].x;
		vertex[start + 1] = mesh_->vertex[index].y;
		vertex[start + 2] = mesh_->vertex[index].z;
	}

	float* colors = new float[nr_vertices* 4];
	for (Size index = 0; index < nr_vertices; ++index)
	{
		const Size start = index * 4;
		colors[start] = (float) mesh_->colorList[index].getRed();
		colors[start + 1] = (float) mesh_->colorList[index].getGreen();
		colors[start + 2] = (float) mesh_->colorList[index].getBlue();
		colors[start + 3] = (float) mesh_->colorList[index].getAlpha();
	}

	float* normals= new float[nr_vertices* 3];
	for (Size index = 0; index < nr_vertices; ++index)
	{
		const Size start = index * 3;
		normals[start] = mesh_->normal[index].x;
		normals[start + 1] = mesh_->normal[index].y;
		normals[start + 2] = mesh_->normal[index].z;
	}

	unsigned int* indices = new unsigned int[nr_triangles * 3];
	for (Size index = 0; index < nr_triangles; ++index)
	{
		const Size start = index * 3;
		indices[start] = mesh_->triangle[index].v1;
		indices[start + 1] = mesh_->triangle[index].v2;
		indices[start + 2] = mesh_->triangle[index].v3;
	}

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
//        			glEnableClientState(GL_INDEX_ARRAY); 
	
	// colors, normals, indices, vertex
	// Get valid Names
	glGenBuffersARB( 4, buffers_);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers_[0]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * nr_vertices * 4, colors, GL_STATIC_DRAW_ARB);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers_[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * nr_vertices * 3, normals, GL_STATIC_DRAW_ARB);
	
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, buffers_[2]);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned int) * nr_triangles * 3, 
									indices, GL_STATIC_DRAW_ARB);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers_[3]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float) * nr_vertices * 3, vertex, GL_STATIC_DRAW_ARB);

	delete[] normals;
	delete[] vertex;
	delete[] indices;
	delete[] colors;

	filled_ = true;

	return true;
}

void MeshBuffer::clear()
{
	mesh_ = 0;
	clearBuffer();

}

void MeshBuffer::clearBuffer()
{
	if (!filled_) return;
	glDeleteBuffersARB(4, buffers_);
	filled_ = false;
}

bool MeshBuffer::draw()
{
	if (!filled_) return false;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers_[0]);
	glColorPointer (4, GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers_[1]);
	glNormalPointer(GL_FLOAT, 0, 0);

	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, buffers_[2]);
	glIndexPointer(GL_UNSIGNED_INT, 0, 0);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffers_[3]);
	glVertexPointer(3, GL_FLOAT, 0, 0); 
	glDrawElements(GL_TRIANGLES, mesh_->triangle.size() * 3, GL_UNSIGNED_INT, 0);
	return true;
}

MeshBuffer::~MeshBuffer()
{
	clear();
}

} }

