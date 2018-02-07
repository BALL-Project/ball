#ifndef JUPYTERPREFERENCES_H
#define JUPYTERPREFERENCES_H

#include <ui_jupyterPreferences.h>

#include <BALL/COMMON/global.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>

#include <QtWidgets/QWidget>

namespace BALL
{
	namespace VIEW
	{

		/**
		 * [JupyterPlugin]
		 * Settings dialog for the Jupyter plugin.
		 */
		class JupyterPreferences :
			public ConfigDialog,
			public Ui_JupyterPreferencesData
		{
			Q_OBJECT

			public:

				enum class ConnectionMode {EXTERNAL, HOSTED};

				JupyterPreferences(Qt::WindowFlags fl=0);
				~JupyterPreferences() override;

				void storeValues() override;
				void restoreValues(bool all = false) override;

				ConnectionMode getConnectionMode() const { return conn_mode_; }
				QString getDashboardUrl() const { return dashboard_url_; };
				QString getExePath() const { return exe_path_; }
				QString getNbdir() const { return nbdir_; }
				QString getToken() const { return token_; }
				unsigned int getPort() const { return port_;}
				bool getAutostart() const { return autostart_; }
				bool getDebug() const { return debug_; }

				/**
				 * Toggles the visibility of connection-mode-specific settings.
				 *
				 * @param mode connection mode
				 */
				void selectConnectionMode(ConnectionMode mode);

			public Q_SLOTS:
				/**
				 * Toggles the visibility of connection-mode-specific settings.
				 *
				 * @param index connection mode index according to the dialog's selection box
				 */
				void selectConnectionMode(int index);

				/**
				 * Sets the path to the Jupyter executable according to the settings.
				 */
				void selectExePath();

				/**
				 * Sets the Jupyter notebook directory (dashboard root) according to the settings.
				 */
				void selectNbdir();

			protected:
				/**
				 * Applies changes in the connection mode setting. 
				 */
				virtual void updateServer();

				ConnectionMode conn_mode_ {ConnectionMode::EXTERNAL};
				QString dashboard_url_ {};
				QString exe_path_ {};
				QString nbdir_ {};
				QString token_ {};
				unsigned int port_ {0};
				bool autostart_ {false};
				bool debug_ {false};
		};
	}
} // namespace

#endif // JUPYTERPREFERENCES_H
