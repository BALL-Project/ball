#ifndef BALL_EXTENSIONS_OPENSIM_HANDLECOMMANDFROMBALLVIEWTHREAD_H
#define BALL_EXTENSIONS_OPENSIM_HANDLECOMMANDFROMBALLVIEWTHREAD_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class OpenSimPlugin;

		class HandleCommandFromBALLViewThread: public QThread
		{
			Q_OBJECT

			public:
				HandleCommandFromBALLViewThread(OpenSimPlugin* plugin);
				~HandleCommandFromBALLViewThread();

				void run();

				void deactivate();

			protected:
				OpenSimPlugin* bvcmdplugin_;
				bool terminate_requested_;
		};

	}
}

#endif
