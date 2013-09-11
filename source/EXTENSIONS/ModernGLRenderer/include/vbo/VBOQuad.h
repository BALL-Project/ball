#ifndef VBOQUAD_H
#define VBOQUAD_H

#include <vbo/VBObject.h>

/**
 * @brief Stores and manages vertex buffer objects (VBO) for quad data.
 **/
class VBOQuad
	: public VBObject
{
	public:
		virtual void render();
};

#endif // VBOQUAD_H
