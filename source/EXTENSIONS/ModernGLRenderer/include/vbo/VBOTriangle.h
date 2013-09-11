#ifndef VBOTRIANGLE_H
#define VBOTRIANGLE_H

#include <vbo/VBObject.h>

/**
 * @brief Stores and manages vertex buffer objects (VBO) for triangle data.
 **/
class VBOTriangle
	: public VBObject
{
	public:
		virtual void render();
};

#endif // VBOTRIANGLE_H
