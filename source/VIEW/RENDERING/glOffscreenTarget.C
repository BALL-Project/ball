// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/RENDERING/glOffscreenTarget.h>

namespace BALL
{
	namespace VIEW
	{
		GLOffscreenTarget::GLOffscreenTarget(const GLRenderWindow& share_from, const String& filename)
			: GLRenderWindow(share_from, NULL),
				filename_(filename)
		{
		}
	}
}
