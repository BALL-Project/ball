#include <BALLaxyPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		BALLaxyPlugin::BALLaxyPlugin()
			: icon_(":pluginBALLaxy.png"),
			  preferences_(new BALLaxyInterfacePreferences()),
			  widget_(nullptr)
		{ }

		BALLaxyPlugin::~BALLaxyPlugin()
		{ }

		QString BALLaxyPlugin::getName() const
		{
			return QString("BALLaxy");
		}

		QString BALLaxyPlugin::getDescription() const
		{
			return QString("An interface to the BALLaxy workflow engine.");
		}

		const QPixmap* BALLaxyPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* BALLaxyPlugin::getConfigDialog()
		{
			return preferences_;
		}

		bool BALLaxyPlugin::activate()
		{
			if(isActive()) return true;

			if(!main_control_)
			{
				Log.error() << "BALLaxy plugin: Undefined main control" << std::endl;
				return false;
			}

			BALLaxyInterface* ballaxy_interface = new BALLaxyInterface(main_control_);
			HTMLViewDock*     ballaxy_view      = new HTMLViewDock(ballaxy_interface, main_control_, String(tr("BALLaxy")).c_str());

			preferences_->storeValues();

			main_control_->addDockWidget(Qt::RightDockWidgetArea, ballaxy_view);

			widget_ = ballaxy_view;
			widget_->registerWidget(widget_);
			widget_->initializeWidget(*main_control_);

			return true;
		}

		bool BALLaxyPlugin::deactivate()
		{
			if(!isActive()) return true;

			widget_->finalizeWidget(*main_control_);
			delete widget_;
			widget_ = nullptr;

			return true;
		}

	}
}
