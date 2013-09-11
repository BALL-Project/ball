#include <GL/glew.h>

#include <vbo/VBOPoint.h>

#include <iostream>


void VBOPoint::render()
{
	bindIndices();

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		enableAttribute(iter->first);
	}

	glDrawArrays(GL_POINTS, 0, n_elements_);

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		disableAttribute(iter->first);
	}
}