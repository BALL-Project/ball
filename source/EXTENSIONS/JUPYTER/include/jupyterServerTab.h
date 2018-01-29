#ifndef JUPYTERSERVERTAB_H
#define JUPYTERSERVERTAB_H

#include <ui_jupyterServerTab.h>

#include <jupyterServer.h>

#include <QWidget>

namespace BALL
{
	namespace VIEW
	{
		class JupyterServerTab :
			public QWidget,
			public Ui_JupyterServerTab
		{
			Q_OBJECT

			public:
				JupyterServerTab(QWidget* parent, JupyterServer* server);
				virtual ~JupyterServerTab();

				JupyterServer* getServer();
				void setServer(JupyterServer* server);

			public Q_SLOTS:
				void readStandardOutput();
				void readStandardError();
				void updateState(QProcess::ProcessState state);
				void startStopServer();
				void processError(QProcess::ProcessError error);
				void scrollToEnd();

			Q_SIGNALS:
				void appendMessage(const QString& /* message */);

			protected:
				JupyterServer* server_;
		};
	}
}

#endif // JUPYTERSERVERTAB_H
