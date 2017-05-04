#include <jupyterPreferences.h>

#include <jupyterServer.h>
#include <jupyterWidget.h>

#include <BALL/VIEW/KERNEL/common.h>

#include <QtWidgets/QFileDialog>

namespace BALL
{
	namespace VIEW
	{
		JupyterPreferences::JupyterPreferences(QWidget* parent, const char* name, Qt::WindowFlags fl)
			: ConfigDialog{parent, fl},
			  Ui_JupyterPreferencesData{},
			  conn_mode_{ConnectionMode::EXTERNAL},
			  dashboard_url_{},
			  exe_path_{},
			  nbdir_{},
			  token_{},
			  port_{0},
			  autostart_{false},
			  debug_{false}
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("Jupyter");
			setWidgetStackName("JupyterWidget");

			connect(mode_edit,    static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this,         static_cast<void(JupyterPreferences::*)(int)>(&JupyterPreferences::selectConnectionMode));
			connect(exe_button,   &QPushButton::clicked, this, &JupyterPreferences::selectExePath);
			connect(nbdir_button, &QPushButton::clicked, this, &JupyterPreferences::selectNbdir);
			registerWidgets_();
			storeValues();
		}

		JupyterPreferences::~JupyterPreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "[JupyterPlugin] Destructing object " << (void *)this
							<< " of class JupyterPreferences" << std::endl;
			#endif 
		}

		void JupyterPreferences::storeValues()
		{
			conn_mode_ = static_cast<ConnectionMode>(mode_edit->currentIndex());
			selectConnectionMode(conn_mode_);
			dashboard_url_ = url_edit->text();
			exe_path_ = exe_edit->text();
			nbdir_ = nbdir_edit->text();
			token_ = token_edit->text();
			port_ = port_edit->value();
			autostart_ = autostart_edit->checkState() == Qt::Checked;
			debug_ = debug_edit->checkState() == Qt::Checked;
			updateServer();
		}

		void JupyterPreferences::updateServer()
		{
			auto widget = JupyterWidget::getInstance(0);
			if (!widget) return;

			switch (conn_mode_)
			{
				case ConnectionMode::EXTERNAL:
					widget->setDashboardURL(getDashboardUrl());
					widget->setServer(nullptr);
					break;

				case ConnectionMode::HOSTED:
					auto server = widget->getServer();
					if (!server) server = new JupyterServer(widget);

					server->setExePath(getExePath());
					server->setPort(getPort());
					server->setDebug(getDebug());
					server->setNbdir(getNbdir());
					server->setToken(getToken());

					widget->setDashboardURL(QString("http://localhost:%1?token=%2").arg(getPort()).arg(getToken()));
					widget->setServer(server);

					if (server->isRunning())
					{
						Log.info() << "[JupyterPlugin] Please restart your Jupyter server manually "
								   << "for changes to take effect" << std::endl;
					}
					else if (getAutostart()) server->start();
					break;
			}
		}

		void JupyterPreferences::restoreValues(bool all)
		{
			ConfigDialog::restoreValues(all);
			mode_edit->setCurrentIndex(static_cast<int>(conn_mode_));
			selectConnectionMode(conn_mode_);
			url_edit->setText(dashboard_url_);
			exe_edit->setText(exe_path_);
			nbdir_edit->setText(nbdir_);
			token_edit->setText(token_);
			port_edit->setValue(port_);
			autostart_edit->setCheckState(autostart_ ? Qt::Checked : Qt::Unchecked);
			debug_edit->setCheckState(debug_ ? Qt::Checked : Qt::Unchecked);
		}

		void JupyterPreferences::selectConnectionMode(int index)
		{
			selectConnectionMode(static_cast<ConnectionMode>(index));
		}

		void JupyterPreferences::selectConnectionMode(ConnectionMode mode)
		{
			external_group->setEnabled(mode == ConnectionMode::EXTERNAL);
			hosted_group->setEnabled(mode == ConnectionMode::HOSTED);
		}

		void JupyterPreferences::selectExePath()
		{
			QString exe = QFileDialog::getOpenFileName(this, tr("Select the Jupyter executable"));
			if (exe != "") exe_edit->setText(exe);
		}

		void JupyterPreferences::selectNbdir()
		{
			QString nbdir = QFileDialog::getExistingDirectory(this, tr("Select a notebook directory"));
			if (nbdir != "") nbdir_edit->setText(nbdir);
		}
	}
} // namespaces
