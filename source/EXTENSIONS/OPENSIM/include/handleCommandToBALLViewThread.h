#ifndef BALL_EXTENSIONS_OPENSIM_HANDLECOMMANDTOBALLVIEWTHREAD_H
#define BALL_EXTENSIONS_OPENSIM_HANDLECOMMANDTOBALLVIEWTHREAD_H

#include <BALL/COMMON/global.h>

#include <QtCore/QThread>

namespace BALL
{
	namespace VIEW
	{
		class OpenSimPlugin;

		class HandleCommandToBALLViewThread: public QThread
		{
			Q_OBJECT

			public:
				void run();

				HandleCommandToBALLViewThread(OpenSimPlugin* plugin);
				~HandleCommandToBALLViewThread();

				void deactivate();

			protected:
				OpenSimPlugin* bvplugin_;
				bool terminate_requested_;
		};
	}
}

#endif

