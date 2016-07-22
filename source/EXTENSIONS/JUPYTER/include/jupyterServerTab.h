#ifndef JUPYTERSERVERTAB_H
#define JUPYTERSERVERTAB_H

#include <jupyterServer.h>
#include <ui_jupyterServerTab.h>

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

			public slots:
				void readStandardOutput();
				void readStandardError();
				void updateState(QProcess::ProcessState state);

			protected:
				JupyterServer* server_;

			signals:
				void appendMessage(const QString& /* message */);
		};
	}
}

#endif // JUPYTERSERVERTAB_H
