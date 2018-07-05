#ifndef BALL_GLRENDERER_GLRENDERSETUP_H
#define BALL_GLRENDERER_GLRENDERSETUP_H

#include <BALL/VIEW/RENDERING/renderSetup.h>

namespace BALL
{
	namespace GLRenderer
	{
		class BALL_PLUGIN_EXPORT GLRenderSetup : public BALL::VIEW::RenderSetup
		{
			public:
				GLRenderSetup(BALL::VIEW::Renderer* renderer, BALL::VIEW::RenderTarget* target, BALL::VIEW::Scene* scene)
					: RenderSetup(renderer, target, scene)
				{
				}

				void updateTarget() override;

			protected:
				GLRenderSetup(const GLRenderSetup&);
				GLRenderSetup* operator=(const GLRenderSetup&);

				void doRender_() override { }
		};
	} // namespace GLRenderer
} // namespace BALL

#endif //BALL_GLRENDERER_GLRENDERSETUP_H
