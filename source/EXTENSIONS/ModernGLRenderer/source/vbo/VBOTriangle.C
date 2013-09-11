#include <GL/glew.h>

#include <vbo/VBOTriangle.h>

#include <iostream>


void VBOTriangle::render()
{
	bindIndices();

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		enableAttribute(iter->first);
	}

	glDrawElements(GL_TRIANGLES, n_elements_,  GL_UNSIGNED_INT, NULL);

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		disableAttribute(iter->first);
	}
}

