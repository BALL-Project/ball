#ifndef BALL_RTFACTRENDERER_RTFACTRENDERSETUP_H
#define BALL_RTFACTRENDERER_RTFACTRENDERSETUP_H

#include <BALL/VIEW/RENDERING/renderSetup.h>

#include <tbb/task_scheduler_init.h>

namespace BALL
{
	namespace RTfactRenderer
	{
		class RTfactRenderSetup : public BALL::VIEW::RenderSetup
		{
			public:
				RTfactRenderSetup(BALL::VIEW::Renderer* renderer, BALL::VIEW::RenderTarget* target, BALL::VIEW::Scene* scene)
					: BALL::VIEW::RenderSetup(renderer, target, scene)
				{
				}

				void updateTarget();

			protected:
				void startRunning_();

			private:
				boost::shared_ptr<tbb::task_scheduler_init> scheduler_;
		};
	}
}

#endif //BALL_RTFACTRENDERER_RTFACTRENDERSETUP_H
