// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <jupyterPreferences.h>

#include <jupyterServer.h>
#include <jupyterWidget.h>
#include <BALL/VIEW/KERNEL/common.h>

#include <QtWidgets/QLineEdit>
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

			connect(mode_edit, SIGNAL(currentIndexChanged(int)), this, SLOT(setConnectionMode(int)));
			connect(exe_button, SIGNAL(clicked()), this, SLOT(selectExePath()));
			connect(nbdir_button, SIGNAL(clicked()), this, SLOT(selectNbdir()));
			registerWidgets_();
		}

		JupyterPreferences::~JupyterPreferences()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
							<< " of class JupyterPreferences" << std::endl;
			#endif 
		}

		void JupyterPreferences::storeValues()
		{
			setConnectionMode(mode_edit->currentIndex());

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
				}
				else
				{
					server = new JupyterServer(bi, getExePath(), getPort(), getDebug(), getNbdir());
					bi->setServer(server);
				}
				if(server->isRunning())
				{
					Log.info() << "Please restart your Jupyter server manually for changes to take effect" << std::endl;
					return;
				}
				if(getAutostart())
				{
					server->start();
				}
			}
		}

		QString JupyterPreferences::getDashboardUrl()
		{
			return url_edit->text();
		}

		JupyterPreferences::ConnectionMode JupyterPreferences::getConnectionMode()
		{
			return conn_mode_;
		}

		void JupyterPreferences::setConnectionMode(int index)
		{
			setConnectionMode(static_cast<ConnectionMode>(index));
		}

		void JupyterPreferences::setConnectionMode(ConnectionMode mode)
		{
			conn_mode_ = mode;
			external_group->setEnabled(conn_mode_ == ConnectionMode::EXTERNAL);
			hosted_group->setEnabled(conn_mode_ == ConnectionMode::HOSTED);
		}

		QString JupyterPreferences::getExePath()
		{
			return exe_edit->text();
		}

		QString JupyterPreferences::getNbdir()
		{
			return nbdir_edit->text();
		}

		unsigned int JupyterPreferences::getPort()
		{
			return port_edit->value();
		}

		bool JupyterPreferences::getAutostart()
		{
			return autostart_edit->checkState() == Qt::Checked;
		}

		bool JupyterPreferences::getDebug()
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
