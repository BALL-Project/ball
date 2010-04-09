#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMWORKERTHREAD_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMWORKERTHREAD_H

#include <BALL/COMMON/global.h>

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class BALLViewOpenSimPlugin;

		class OpenSimWorkerThread: public QThread
		{
			Q_OBJECT

			public:
				void run();

				OpenSimWorkerThread(BALLViewOpenSimPlugin* plugin);
				~OpenSimWorkerThread();

				void deactivate();

			protected:
				BALLViewOpenSimPlugin* bvplugin_;
				bool terminate_requested_;
		};
	}
}

#endif

