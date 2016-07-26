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
				JupyterServer(QObject* parent, const QString& exe_path, unsigned int port, bool autostart, bool debug, const QString& nbdir);
				virtual ~JupyterServer();

				const QString& getExePath();
				void setExePath(const QString& exe_path);
				unsigned int getPort();
				void setPort(unsigned int port);
				bool getDebug();
				void setDebug(bool debug);
				const QString& getNbdir();
				void setNbdir(const QString& nbdir);

				bool isRunning();
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
