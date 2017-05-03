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
				JupyterServer(QObject* parent, const QString& exe_path, unsigned int port, bool debug, const QString& nbdir);
				virtual ~JupyterServer();

				const QString& getExePath() { return exe_path_; }
				unsigned int getPort() { return port_; }
				bool getDebug() { return debug_; }
				const QString& getNbdir() { return nbdir_; }

				void setExePath(const QString& exe_path) { exe_path_ = exe_path; }
				void setPort(unsigned int port) { port_ = port; }
				void setDebug(bool debug) { debug_ = debug; }
				void setNbdir(const QString& nbdir) { nbdir_ = nbdir; }

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
				void errorOccurred(QProcess::ProcessError);

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
