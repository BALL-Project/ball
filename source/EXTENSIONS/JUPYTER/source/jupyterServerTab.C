#include <jupyterServerTab.h>

#include <BALL/VIEW/DIALOGS/preferences.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <jupyterPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		using ProcessState = QProcess::ProcessState;

		JupyterServerTab::JupyterServerTab(QWidget* parent, JupyterServer* server)
			: QWidget(parent),
			  Ui_JupyterServerTab(),
			  server_(server)
		{
			setupUi(this);

			connect(server_, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(updateState(QProcess::ProcessState)));
			connect(server_, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
			connect(server_, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
			connect(this, SIGNAL(appendMessage(const QString&)), messageArea, SLOT(appendPlainText(const QString&)));
		}

		void JupyterServerTab::readStandardOutput()
		{
			emit appendMessage(server_->readStandardOutput().trimmed());
		}

		void JupyterServerTab::readStandardError()
		{
			emit appendMessage(server_->readStandardError().trimmed());
		}

		void JupyterServerTab::updateState(ProcessState state)
		{
			switch(state)
			{
				case ProcessState::NotRunning:
					statusLabel->setText("Stopped");
					break;
				case ProcessState::Starting:
					statusLabel->setText("Starting");
					break;
				case ProcessState::Running:
					statusLabel->setText("Running");
					break;
				default:
					statusLabel->setText("Unknown");
			}
		}
	}
}