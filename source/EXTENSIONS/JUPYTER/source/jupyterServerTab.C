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
			  server_(nullptr)
		{
			setupUi(this);
			setServer(server);
			connect(this, SIGNAL(appendMessage(const QString&)), message_edit, SLOT(appendPlainText(const QString&)));
			connect(clear_button, SIGNAL(clicked()), message_edit, SLOT(clear()));
			connect(start_stop_button, SIGNAL(clicked()), this, SLOT(startStopServer()));
		}

		JupyterServerTab::~JupyterServerTab()
		{ }

		JupyterServer* JupyterServerTab::getServer()
		{
			return server_;
		}

		void JupyterServerTab::setServer(JupyterServer* server)
		{
			server_ = server;
			connect(server_, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(updateState(QProcess::ProcessState)));
			connect(server_, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
			connect(server_, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
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
					status_edit->setText(tr("Stopped"));
					start_stop_button->setText(tr("Start server"));
					break;
				case ProcessState::Starting:
					status_edit->setText(tr("Starting"));
					start_stop_button->setText(tr("Stop server"));
					break;
				case ProcessState::Running:
					status_edit->setText(tr("Running"));
					start_stop_button->setText(tr("Stop server"));
					break;
				default:
					status_edit->setText(tr("Unknown"));
					start_stop_button->setText(tr("Start server"));
			}
		}

		void JupyterServerTab::startStopServer()
		{
			start_stop_button->setEnabled(false);
			if(server_->isRunning())
			{
				server_->terminate();
			}
			else
			{
				server_->start();
			}
			start_stop_button->setEnabled(true);
		}
	}
}