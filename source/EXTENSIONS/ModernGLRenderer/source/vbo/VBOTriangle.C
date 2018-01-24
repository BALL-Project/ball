#include <GL/glew.h>

#include <vbo/VBOTriangle.h>


void VBOTriangle::render_()
{
	glDrawElements(GL_TRIANGLES, n_elements_,  GL_UNSIGNED_INT, NULL);
}
