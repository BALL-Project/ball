#ifndef JUPYTERSERVERTAB_H
#define JUPYTERSERVERTAB_H

#include <ui_jupyterServerTab.h>

#include <jupyterServer.h>

#include <QWidget>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * [JupyterPlugin]
		 * Single server tab for the Jupyter widget, providing control buttons and log messages.
		 */
		class JupyterServerTab :
			public QWidget,
			public Ui_JupyterServerTab
		{
			Q_OBJECT

			public:
				/**
				 * Creates a new server tab and connects it to the given server.
				 *
				 * @param parent parent object for Qt-handled memory management
				 * @param server valid Jupyter server
				 */
				JupyterServerTab(QWidget* parent, JupyterServer* server);

				~JupyterServerTab() override = default;

				JupyterServer* getServer() { return server_; };

				/**
				 * Replaces the currently used server and connects it to the server tab.
				 *
				 * @param server valid Jupyter server
				 */
				void setServer(JupyterServer* server);

			public Q_SLOTS:
				/**
				 * Appends all messages from the server process' standard output channel to the log window.
				 * (cf. JupyterServer::readyReadStandardOutput)
				 */
				void readStandardOutput();

				/**
				 * Appends all messages from the server process' standard error channel to the log window.
				 * (cf. JupyterServer::readyReadStandardError)
				 */
				void readStandardError();

				/**
				 * Adjusts the control buttons to reflect the given server state.
				 *
				 * @param state server state
				 */
				void updateState(QProcess::ProcessState state);

				/**
				 * Starts or stops the server process.
				 */
				void startStopServer();

				/**
				 * Appends an error message (corresponding to the given error state) to the log window.
				 * @param error
				 */
				void processError(QProcess::ProcessError error);

				/**
				 * Scrolls to the end of the log window to show the latest messages.
				 */
				void scrollToEnd();

			Q_SIGNALS:
				/**
				 * This signal can be used to have the given message being appended to the log window.
				 */
				void appendMessage(const QString& message);

			protected:
				JupyterServer* server_ {nullptr};
		};
	}
}

#endif // JUPYTERSERVERTAB_H
