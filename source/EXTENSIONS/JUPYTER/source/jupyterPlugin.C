#include <jupyterPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterPlugin::JupyterPlugin()
			: icon_(":pluginJupyter.png"),
				is_active_(false),
				preferences_(new JupyterPreferences())
		{
		}

		JupyterPlugin::~JupyterPlugin()
		{
		}

		QString JupyterPlugin::getName() const
		{
			return QString("Jupyter");
		}

		QString JupyterPlugin::getDescription() const
		{
			return QString("An interface to Jupyter notebooks.");
		}

		const QPixmap* JupyterPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* JupyterPlugin::getConfigDialog()
		{
			return preferences_;
		}

		ModularWidget* JupyterPlugin::createModularWidget(MainControl* main_control)
		{
			JupyterWidget* jupyter_widget = new JupyterWidget(main_control, "Jupyter");

			preferences_->storeValues();

			main_control->addDockWidget(Qt::BottomDockWidgetArea, jupyter_widget);

			return jupyter_widget;
		}

		bool JupyterPlugin::activate()
		{
			is_active_ = true;

			return true;
		}

		bool JupyterPlugin::deactivate()
		{
			is_active_ = false;

			return true;
		}
	}
}
