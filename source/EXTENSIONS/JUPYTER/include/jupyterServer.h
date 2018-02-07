#ifndef JUPYTERSERVER_H
#define JUPYTERSERVER_H

#include <QtCore/QProcess>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * [JupyterPlugin]
		 * Manager class for a plugin-hosted Jupyter server.
		 */
		class JupyterServer: public QObject
		{
			Q_OBJECT

			public:
				/**
				 * Creates a new server process.
				 *
				 * @param parent parent object for Qt-handled memory management
				 */
				JupyterServer(QObject* parent);

				/**
				 * Terminates the server process (if necessary).
				 */
				~JupyterServer() override;

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

				/**
				 * Checks whether the Jupyter server process is running.
				 *
				 * @return true iff the server process is running
				 */
				virtual bool isRunning() const;

				/**
				 * Starts the Jupyter server process. Does nothing if the server is already running.
				 */
				virtual void start();

				/**
				 * Stops the Jupyter server process. Terminates the process forcefully if the server does not stop
				 * within the given amount of time. Does nothing if the server is not running.
				 *
				 * @param kill_timer timeout in milliseconds
				 */
				virtual void terminate(int kill_timer = 10000);

				/**
				 * Returns all data from the server process' standard output channel.
				 *
				 * @return messages
				 */
				virtual QByteArray readStandardOutput();

				/**
				 * Returns all data from the server process' standard error channel.
				 *
				 * @return error messages
				 */
				virtual QByteArray readStandardError();

				/**
				 * Returns the server process' current state.
				 *
				 * @return current server state
				 */
				virtual QProcess::ProcessState state() const;

			Q_SIGNALS:
				/**
				 * Emitted when there is new data on the server process' standard output channel
				 * (cf. QProcess::readyReadStandardOutput)
				 */
				void readyReadStandardOutput();

				/**
				 * Emitted when there is new data on the server process' standard error channel
				 * (cf. QProcess::readyReadStandardError)
				 */
				void readyReadStandardError();

				/**
				 * Emitted when the server process' state changes (cf. QProcess::stateChanged)
				 */
				void stateChanged(QProcess::ProcessState);

				/**
				 * Emitted when the server process has been started (cf. QProcess::started)
				 */
				void started();

				/**
				 * Emitted when the server process encounters an error (cf. QProcess::errorOccured)
				 */
				void errorOccurred(QProcess::ProcessError);

			protected:
				QString exe_path_ {};
				unsigned int port_ {0};
				bool debug_ {false};
				QString nbdir_ {};
				QString token_ {};
				QProcess* proc_ {new QProcess(this)};
		};
	}
}

#endif // JUPYTERSERVER_H
