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

		/** JupyterPreferences is a widget that will be inserted into the
				tab dialog Preferences.
				\ingroup ViewDialogs
		*/
		class JupyterPreferences :
			public ConfigDialog,
			public Ui_JupyterPreferencesData
		{
			Q_OBJECT

			public:

				enum class ConnectionMode {EXTERNAL, HOSTED};
			
				/// Default Constructor.
				JupyterPreferences(Qt::WindowFlags fl=0);
				
				/// Destructor.
				~JupyterPreferences() override;

				///
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

				void selectConnectionMode(ConnectionMode mode);

			public Q_SLOTS:
				void selectConnectionMode(int index);
				void selectExePath();
				void selectNbdir();

			protected:
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
