#include <GL/glew.h>

#include <vbo/VBOPoint.h>

#include <iostream>


void VBOPoint::render_()
{
	glDrawArrays(GL_POINTS, 0, n_elements_);
}

void VBOPoint::bindIndices()
{
}

