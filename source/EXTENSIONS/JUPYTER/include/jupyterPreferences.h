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
		class JupyterPreferences
			: public ConfigDialog,
			  public Ui_JupyterPreferencesData
		{
			Q_OBJECT

			public:

				enum class ConnectionMode {EXTERNAL, HOSTED};
			
				/// Default Constructor.
				JupyterPreferences(QWidget *parent = nullptr, const char *name = "JupyterPreferences", Qt::WindowFlags fl=0);
				
				/// Destructor.
				virtual ~JupyterPreferences();

				///
				virtual void storeValues();

				///
				void getSettings();

				QString getDashboardUrl() const;
				ConnectionMode getConnectionMode() const { return conn_mode_; }
				void setConnectionMode(ConnectionMode mode);
				QString getExePath() const;
				QString getNbdir() const;
				unsigned int getPort() const;
				bool getAutostart() const;
				bool getDebug() const;

			public slots:
				void selectConnectionMode(int index);
				void selectExePath();
				void selectNbdir();

			protected:
				ConnectionMode conn_mode_;
		};
	}
} // namespace

#endif // JUPYTERPREFERENCES_H
