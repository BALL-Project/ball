#ifndef JUPYTERSERVER_H
#define JUPYTERSERVER_H

#include <QtCore/QProcess>

namespace BALL
{
	namespace VIEW
	{
		class JupyterServer: public QObject
		{
			Q_OBJECT

			public:
				JupyterServer(QObject* parent, QString exe_path, unsigned int port, bool autostart, bool debug, QString nbdir);
				virtual ~JupyterServer();

				void start();
				void terminate(int kill_timer = 30000);
				QByteArray readStandardOutput();
				QByteArray readStandardError();
				QProcess::ProcessState state();

			signals:
				void readyReadStandardOutput();
				void readyReadStandardError();
				void stateChanged(QProcess::ProcessState);
				void started();

			protected:
				QString exe_path_;
				unsigned int port_;
				bool debug_;
				QString nbdir_;
				QProcess* proc_;
		};
	}
}

#endif // JUPYTERSERVER_H
