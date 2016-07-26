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
			setWidgetStackName((String)tr("JupyterWidget"));

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
			if (bi == 0) return;

			if(conn_mode_ == ConnectionMode::EXTERNAL)
			{
				bi->setDashboardURL(ascii(url_edit->text()));
				bi->setServer(nullptr);
			}
			else if(bi->getServer()) // ConnectionMode::HOSTED (server already created)
			{
				JupyterServer* server = bi->getServer();
				server->setExePath(exe_edit->text());
				server->setPort(port_edit->value());
				server->setDebug(debug_edit->checkState() == Qt::Checked);
				server->setNbdir(nbdir_edit->text());
				if(server->isRunning())
				{
					Log.info() << "Please restart your Jupyter server manually for changes to take effect" << std::endl;
				}
				else if(autostart_edit->checkState() == Qt::Checked)
				{
					server->start();
				}
			}
			else // ConnectionMode::HOSTED (create server)
			{
				bi->setServer(new JupyterServer(bi, exe_edit->text(), port_edit->value(),
												autostart_edit->checkState() == Qt::Checked,
												debug_edit->checkState() == Qt::Checked, nbdir_edit->text()));
			}
		}

		void JupyterPreferences::setConnectionMode(ConnectionMode mode)
		{
			conn_mode_ = mode;
			external_group->setEnabled(conn_mode_ == ConnectionMode::EXTERNAL);
			hosted_group->setEnabled(conn_mode_ == ConnectionMode::HOSTED);
		}

		void JupyterPreferences::setConnectionMode(int index)
		{
			setConnectionMode(static_cast<ConnectionMode>(index));
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
