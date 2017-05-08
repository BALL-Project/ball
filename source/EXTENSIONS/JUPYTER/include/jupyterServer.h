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
				JupyterServer(QObject* parent);
				virtual ~JupyterServer();

				const QString& getExePath() const { return exe_path_; }
				unsigned int getPort() const { return port_; }
				bool getDebug() const { return debug_; }
				const QString& getNbdir() const { return nbdir_; }
				const QString& getToken() const { return token_; }

				void setExePath(const QString& exe_path) { exe_path_ = exe_path; }
				void setPort(unsigned int port) { port_ = port; }
				void setDebug(bool debug) { debug_ = debug; }
				void setNbdir(const QString& nbdir) { nbdir_ = nbdir; }
				void setToken(const QString& token) { token_ = token; }

				bool isRunning() const;
				void start();
				void terminate(int kill_timer = 10000);
				QByteArray readStandardOutput();
				QByteArray readStandardError();
				QProcess::ProcessState state() const;

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
				QString token_;
				QProcess* proc_;
		};
	}
}

#endif // JUPYTERSERVER_H
