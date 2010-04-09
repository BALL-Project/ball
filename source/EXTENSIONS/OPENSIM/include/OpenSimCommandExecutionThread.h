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
		class BALLViewOpenSimPlugin;

		class OpenSimCommandExecutionThread: public QThread
		{
			Q_OBJECT

			public:
				OpenSimCommandExecutionThread(BALLViewOpenSimPlugin* plugin);
				~OpenSimCommandExecutionThread();

				void run();

				void deactivate();

			protected:
				BALLViewOpenSimPlugin* bvcmdplugin_;
				bool terminate_requested_;
		};

	}
}

#endif
