#include <jupyterPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		bool JupyterPlugin::activate()
		{
			if(isActive()) return true;

			widget_.reset(new JupyterWidget());

			preferences_->storeValues();

			main_control_->addDockWidget(Qt::BottomDockWidgetArea, widget_.get());

			widget_->registerWidget(widget_.get());
			widget_->initializeWidget(*main_control_);

			return true;
		}

		bool JupyterPlugin::deactivate()
		{
			if(!isActive()) return true;

			widget_->finalizeWidget(*main_control_);
			widget_.reset();

			return true;
		}
	}
}
