#include <iPythonPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

Q_EXPORT_PLUGIN2(pluginiPython, BALL::VIEW::IPythonPlugin)

namespace BALL
{
	namespace VIEW
	{
		IPythonPlugin::IPythonPlugin()
			: icon_(":pluginiPython.png"),
				is_active_(false),
				preferences_(new IPythonPreferences())
		{
		}

		IPythonPlugin::~IPythonPlugin()
		{
		}

		QString IPythonPlugin::getName() const
		{
			return QString("iPython");
		}

		QString IPythonPlugin::getDescription() const
		{
			return QString("An interface to iPython notebooks.");
		}

		const QPixmap* IPythonPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* IPythonPlugin::getConfigDialog()
		{
			return preferences_;
		}

		ModularWidget* IPythonPlugin::createModularWidget(MainControl* main_control)
		{
			IPythonWidget* ipython_widget = new IPythonWidget(main_control, "IPython");

			preferences_->storeValues();

			main_control->addDockWidget(Qt::BottomDockWidgetArea, ipython_widget);

			return ipython_widget;
		}

		bool IPythonPlugin::activate()
		{
			is_active_ = true;

			return true;
		}

		bool IPythonPlugin::deactivate()
		{
			is_active_ = false;

			return true;
		}
	}
}
