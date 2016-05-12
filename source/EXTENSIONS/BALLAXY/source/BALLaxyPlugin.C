#include <BALLaxyPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		BALLaxyPlugin::BALLaxyPlugin()
			: icon_(":pluginBALLaxy.png"),
			  is_active_(false),
			  preferences_(new BALLaxyInterfacePreferences())
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

		ModularWidget* BALLaxyPlugin::createModularWidget(MainControl* main_control)
		{
			BALLaxyInterface* ballaxy_interface = new BALLaxyInterface(main_control);
			HTMLViewDock*     ballaxy_view      = new HTMLViewDock(ballaxy_interface, main_control, String(tr("BALLaxy")).c_str());

			preferences_->storeValues();

			main_control->addDockWidget(Qt::BottomDockWidgetArea, ballaxy_view);

			return ballaxy_view;
		}

		bool BALLaxyPlugin::activate()
		{
			is_active_ = true;

			return true;
		}

		bool BALLaxyPlugin::deactivate()
		{
			is_active_ = false;

			return true;
		}

	}
}
