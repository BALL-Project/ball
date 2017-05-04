#include <jupyterServerTab.h>

#include <jupyterPlugin.h>

#include <QtWidgets/QScrollBar>

namespace BALL
{
	namespace VIEW
	{
		using ProcessState = QProcess::ProcessState;
		using ProcessError = QProcess::ProcessError;

		JupyterServerTab::JupyterServerTab(QWidget* parent, JupyterServer* server)
			: QWidget(parent),
			  Ui_JupyterServerTab(),
			  server_(nullptr)
		{
			setupUi(this);
			setServer(server);
			connect(this, &JupyterServerTab::appendMessage, message_edit, &QPlainTextEdit::appendHtml);
			connect(clear_button, &QPushButton::clicked, message_edit, &QPlainTextEdit::clear);
			connect(start_stop_button, &QPushButton::clicked, this, &JupyterServerTab::startStopServer);
		}

		JupyterServerTab::~JupyterServerTab()
		{ }

		JupyterServer* JupyterServerTab::getServer()
		{
			return server_;
		}

		void JupyterServerTab::setServer(JupyterServer* server)
		{
			if(server_ == server)
			{
				return;
			}
			server_ = server;
			connect(server_,      &JupyterServer::stateChanged,            this, &JupyterServerTab::updateState);
			connect(server_,      &JupyterServer::readyReadStandardOutput, this, &JupyterServerTab::readStandardOutput);
			connect(server_,      &JupyterServer::readyReadStandardError,  this, &JupyterServerTab::readStandardError);
			connect(server_,      &JupyterServer::errorOccurred,           this, &JupyterServerTab::processError);
			connect(message_edit, &QPlainTextEdit::textChanged,            this, &JupyterServerTab::scrollToEnd);
		}

		void JupyterServerTab::readStandardOutput()
		{
			message_edit->appendPlainText(server_->readStandardOutput().trimmed());
		}

		void JupyterServerTab::readStandardError()
		{
			message_edit->appendPlainText(server_->readStandardError().trimmed());
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

		void JupyterServerTab::processError(ProcessError error)
		{
			switch(error)
			{
				case ProcessError::FailedToStart:
					message_edit->appendHtml("<strong>The server failed to start! Please make sure that the path to the\
											Jupyter executable is set correctly and that you have the permission to\
											invoke the program.</strong>");
					break;
				case ProcessError::Crashed:
					message_edit->appendHtml("<strong>The server crashed during runtime!</strong>");
					break;
				case ProcessError::Timedout:
					message_edit->appendHtml("<strong>The server took too long to shut down and will be killed!</strong>");
					break;
				default:
					message_edit->appendHtml("<strong>An unknown error occured!</strong>");
			}
		}

		void JupyterServerTab::scrollToEnd()
		{
			auto sb = message_edit->verticalScrollBar();
			if (sb) sb->setValue(sb->maximum());
		}
	}
}