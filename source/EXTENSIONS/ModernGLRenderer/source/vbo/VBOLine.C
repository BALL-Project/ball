#include <GL/glew.h>

#include <vbo/VBOLine.h>


void VBOLine::render_()
{
	glDrawArrays(GL_LINES, 0, n_elements_);
}

void VBOLine::bindIndices()
{
}
