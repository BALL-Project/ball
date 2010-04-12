#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMWORKERTHREAD_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMWORKERTHREAD_H

#include <BALL/COMMON/global.h>

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class OpenSimPlugin;

		class OpenSimWorkerThread: public QThread
		{
			Q_OBJECT

			public:
				void run();

				OpenSimWorkerThread(OpenSimPlugin* plugin);
				~OpenSimWorkerThread();

				void deactivate();

			protected:
				OpenSimPlugin* bvplugin_;
				bool terminate_requested_;
		};
	}
}

#endif

