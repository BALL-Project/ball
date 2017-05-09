#include <jupyterPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		JupyterPlugin::JupyterPlugin()
			: icon_(":pluginJupyter.png"),
			  preferences_(new JupyterPreferences()),
			  widget_(nullptr)
		{ }

		JupyterPlugin::~JupyterPlugin()
		{
			delete preferences_;
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

		bool JupyterPlugin::activate()
		{
			if(isActive()) return true;

			JupyterWidget* jupyter_widget = new JupyterWidget(main_control_, "Jupyter");

			preferences_->storeValues();

			main_control_->addDockWidget(Qt::BottomDockWidgetArea, jupyter_widget);

			widget_ = jupyter_widget;
			widget_->registerWidget(widget_);
			widget_->initializeWidget(*main_control_);

			return true;
		}

		bool JupyterPlugin::deactivate()
		{
			if(!isActive()) return true;

			widget_->finalizeWidget(*main_control_);
			delete widget_;
			widget_ = nullptr;

			return true;
		}
	}
}
