#ifndef BALL_EXTENSIONS_OPENSIM_OPENSIMCOMMANDEXECUTIONTHREAD_H
#define BALL_EXTENSIONS_OPENSIM_OPENSIMCOMMANDEXECUTIONTHREAD_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class OpenSimPlugin;

		class OpenSimCommandExecutionThread: public QThread
		{
			Q_OBJECT

			public:
				OpenSimCommandExecutionThread(OpenSimPlugin* plugin);
				~OpenSimCommandExecutionThread();

				void run();

				void deactivate();

			protected:
				OpenSimPlugin* bvcmdplugin_;
				bool terminate_requested_;
		};

	}
}

#endif
