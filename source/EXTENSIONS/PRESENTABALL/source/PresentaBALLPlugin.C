#include <PresentaBALLPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

Q_EXPORT_PLUGIN2(pluginPresentaBALL, BALL::VIEW::PresentaBALLPlugin)

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLPlugin::PresentaBALLPlugin()
			: icon_(":pluginPresentaBALL.png"),
				is_active_(false)
		{
		}

		PresentaBALLPlugin::~PresentaBALLPlugin()
		{
		}

		QString PresentaBALLPlugin::getName() const
		{
			return QString("PresentaBALL");
		}

		QString PresentaBALLPlugin::getDescription() const
		{
			return QString("An interactive, HTML-based presentation environment for BALLView.");
		}

		const QPixmap* PresentaBALLPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* PresentaBALLPlugin::getConfigDialog()
		{
			return 0;
		}

		ModularWidget* PresentaBALLPlugin::createModularWidget(MainControl* main_control)
		{
			HTMLBasedInterface* html_interface = new HTMLBasedInterface(main_control);
			HTMLViewDock*       html_view      = new HTMLViewDock(html_interface, main_control, String(tr("PresentaBALL")).c_str());

			if (UIOperationMode::instance().getMode() > UIOperationMode::MODE_ADVANCED)
			{
				html_view->setFeatures(0);
				html_view->setMaximumWidth(730);
				html_view->setMinimumWidth(730);
			}
			else
			{
				html_view->registerWidget(html_view);
			}

			main_control->addDockWidget(Qt::LeftDockWidgetArea, html_view);

			return html_view;
		}

		bool PresentaBALLPlugin::activate()
		{
			is_active_ = true;

			return true;
		}

		bool PresentaBALLPlugin::deactivate()
		{
			is_active_ = false;

			return true;
		}

	}
}
