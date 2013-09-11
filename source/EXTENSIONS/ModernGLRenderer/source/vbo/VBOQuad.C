#include <GL/glew.h>

#include <vbo/VBOQuad.h>

#include <iostream>


void VBOQuad::render()
{
	bindIndices();

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		enableAttribute(iter->first);
	}

	glDrawElements(GL_QUADS, n_elements_,  GL_UNSIGNED_INT, NULL);

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		disableAttribute(iter->first);
	}
}