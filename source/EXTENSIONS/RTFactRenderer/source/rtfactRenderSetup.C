#include <rtfactRenderSetup.h>

#include <BALL/VIEW/RENDERING/renderTarget.h>

namespace BALL
{
	namespace VIEW
	{
		void RTfactRenderSetup::updateTarget()
		{
			target->refresh();
		}

		void RTfactRenderSetup::startRunning_()
		{
#ifdef USE_TBB
			const char* num_threads_s = getenv("BALL_NUM_RENDER_THREADS");

			Index num_threads = tbb::task_scheduler_init::automatic;
			if (num_threads_s != 0)
				num_threads = atoi(num_threads_s);

			printf("Using %d render threads\n", num_threads);
			scheduler_ = boost::shared_ptr<tbb::task_scheduler_init>(new tbb::task_scheduler_init(num_threads));
#endif
		}
	}
}
