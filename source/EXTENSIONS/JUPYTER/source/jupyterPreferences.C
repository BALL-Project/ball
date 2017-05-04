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
			: ConfigDialog(parent, fl),
			  Ui_JupyterPreferencesData(),
			  conn_mode_(ConnectionMode::EXTERNAL)
		{
			setupUi(this);
			setObjectName(name);
			setINIFileSectionName("Jupyter");
			setWidgetStackName("JupyterWidget");

			connect(mode_edit,    static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
					this,         &JupyterPreferences::selectConnectionMode);
			connect(exe_button,   &QPushButton::clicked, this, &JupyterPreferences::selectExePath);
			connect(nbdir_button, &QPushButton::clicked, this, &JupyterPreferences::selectNbdir);
			registerWidgets_();
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
			selectConnectionMode(mode_edit->currentIndex());

			JupyterWidget* bi = JupyterWidget::getInstance(0);
			if(!bi)
			{
				return;
			}

			if(conn_mode_ == ConnectionMode::EXTERNAL)
			{
				bi->setDashboardURL(getDashboardUrl());
				bi->setServer(nullptr);
			}
			else // ConnectionMode::HOSTED
			{
				JupyterServer* server = bi->getServer();
				if(server)
				{
					server->setExePath(getExePath());
					server->setPort(getPort());
					server->setDebug(getDebug());
					server->setNbdir(getNbdir());
					server->setToken(getToken());
				}
				else
				{
					server = new JupyterServer(bi, getExePath(), getPort(), getDebug(), getNbdir(), getToken());
					bi->setServer(server);
				}
				if(server->isRunning())
				{
					Log.info() << "[JupyterPlugin] Please restart your Jupyter server manually for changes to take effect" << std::endl;
					return;
				}
				if(getAutostart())
				{
					server->start();
				}
			}
		}

		QString JupyterPreferences::getDashboardUrl() const
		{
			return url_edit->text();
		}

		void JupyterPreferences::selectConnectionMode(int index)
		{
			setConnectionMode(static_cast<ConnectionMode>(index));
		}

		void JupyterPreferences::setConnectionMode(ConnectionMode mode)
		{
			conn_mode_ = mode;
			external_group->setEnabled(conn_mode_ == ConnectionMode::EXTERNAL);
			hosted_group->setEnabled(conn_mode_ == ConnectionMode::HOSTED);
		}

		QString JupyterPreferences::getExePath() const
		{
			return exe_edit->text();
		}

		QString JupyterPreferences::getNbdir() const
		{
			return nbdir_edit->text();
		}

		QString JupyterPreferences::getToken() const
		{
			return token_edit->text();
		}

		unsigned int JupyterPreferences::getPort() const
		{
			return port_edit->value();
		}

		bool JupyterPreferences::getAutostart() const
		{
			return autostart_edit->checkState() == Qt::Checked;
		}

		bool JupyterPreferences::getDebug() const
		{
			return debug_edit->checkState() == Qt::Checked;
		}

		void JupyterPreferences::selectExePath()
		{
			QString exe = QFileDialog::getOpenFileName(this, tr("Select the Jupyter executable"));
			if (exe != "")
			{
				exe_edit->setText(exe);
			}
		}

		void JupyterPreferences::selectNbdir()
		{
			QString nbdir = QFileDialog::getExistingDirectory(this, tr("Select a notebook directory"));
			if (nbdir != "")
			{
				nbdir_edit->setText(nbdir);
			}
		}
	}
} // namespaces
