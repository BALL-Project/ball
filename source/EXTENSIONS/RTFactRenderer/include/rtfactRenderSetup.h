#ifndef BALL_PLUGINS_RTFACTRENDERSETUP_H
#define BALL_PLUGINS_RTFACTRENDERSETUP_H

#include <BALL/VIEW/RENDERING/renderSetup.h>
#include <config.h>

#ifdef USE_TBB
#include <tbb/task_scheduler_init.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class RTfactRenderSetup : public RenderSetup
		{
			public:
				RTfactRenderSetup(Renderer* renderer, RenderTarget* target, Scene* scene)
					: RenderSetup(renderer, target, scene)
				{
				}

				void updateTarget();

			protected:
				void startRunning_();

			private:

#ifdef USE_TBB
				boost::shared_ptr<tbb::task_scheduler_init> scheduler_;
#endif
		};
	}
}

#endif //BALL_PLUGINS_RTFACTRENDERSETUP_H
