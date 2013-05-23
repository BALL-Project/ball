#ifndef BALL_PLUGINS_GLRENDERSETUP_H
#define BALL_PLUGINS_GLRENDERSETUP_H

#include <BALL/VIEW/RENDERING/renderSetup.h>

namespace BALL
{
	namespace VIEW
	{
		class BALL_PLUGIN_EXPORT GLRenderSetup : public RenderSetup
		{
			public:
				GLRenderSetup(Renderer* renderer, RenderTarget* target, Scene* scene)
					: RenderSetup(renderer, target, scene)
				{
				}

				void updateTarget();

			protected:
				GLRenderSetup(const GLRenderSetup&);
				GLRenderSetup* operator=(const GLRenderSetup&);

				void doRender_() { }
		};
	}
}

#endif //BALL_PLUGINS_GLRENDERSETUP_H
