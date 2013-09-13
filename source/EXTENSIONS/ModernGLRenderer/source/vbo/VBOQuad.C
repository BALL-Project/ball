#include <GL/glew.h>

#include <vbo/VBOQuad.h>

#include <iostream>


void VBOQuad::render_()
{
	glDrawElements(GL_QUADS, n_elements_,  GL_UNSIGNED_INT, NULL);
}

