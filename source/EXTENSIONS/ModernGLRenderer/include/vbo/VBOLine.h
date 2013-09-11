#ifndef VBOLINE_H
#define VBOLINE_H

#include <vbo/VBObject.h>

/**
 * @brief Stores and manages vertex buffer objects (VBO) for line data.
 **/
class VBOLine
	: public VBObject
{
	public:
		virtual void render();
};

#endif // VBOLINE_H
