// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_GLOFFSCREENTARGET_H
#define BALL_VIEW_RENDERING_GLOFFSCREENTARGET_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERWINDOW_H
# include <BALL/VIEW/RENDERING/glRenderWindow.h>
#endif

namespace BALL
{
	namespace VIEW
	{				
		/**
		 * Model of the \link RenderWindow \endlink which uses OpenGL to render its buffer into a file
		 */
		class BALL_VIEW_EXPORT GLOffscreenTarget
			: public GLRenderWindow
		{
			public:
				/** Create a new GLOffscreenTarget with context shared from an existing GLRenderWindow.
				 */
				GLOffscreenTarget(const GLRenderWindow& share_from, const String& filename);

			protected:
				String filename_;
		};

	}
}
#endif // BALL_VIEW_RENDERING_GLOFFSCREENTARGET_H
