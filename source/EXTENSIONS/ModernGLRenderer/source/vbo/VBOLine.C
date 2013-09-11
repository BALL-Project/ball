#include <GL/glew.h>

#include <vbo/VBOLine.h>

#include <iostream>


void VBOLine::render()
{
	bindIndices();

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		enableAttribute(iter->first);
	}

	glDrawArrays(GL_LINES, 0, n_elements_);
	//glDrawElements(GL_LINES, n_elements_,  GL_UNSIGNED_INT, NULL);

	for(std::map< GLuint, Buffer >::const_iterator iter = buffers_.begin(); iter != buffers_.end(); iter++)
	{
		disableAttribute(iter->first);
	}
}
