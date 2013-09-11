#ifndef BALL_PLUGINS_MODERNGLRENDERSETUP_H
#define BALL_PLUGINS_MODERNGLRENDERSETUP_H

#include <BALL/VIEW/RENDERING/renderSetup.h>

class ModernGLRenderSetup : public BALL::VIEW::RenderSetup
{
	public:
		ModernGLRenderSetup(BALL::VIEW::Renderer* renderer, BALL::VIEW::RenderTarget* target, BALL::VIEW::Scene* scene)
			: RenderSetup(renderer, target, scene)
		{
		}

		void updateTarget();

	protected:
		void doRender_() { }
};


#endif //BALL_PLUGINS_MODERNGLRENDERSETUP_H
