#include <PresentaBALLPlugin.h>

#include <BALL/VIEW/KERNEL/mainControl.h>

namespace BALL
{
	namespace VIEW
	{
		PresentaBALLPlugin::PresentaBALLPlugin()
			: icon_(":pluginPresentaBALL.png"),
			  is_active_(false),
			  settings_(0)
		{
		}

		PresentaBALLPlugin::~PresentaBALLPlugin()
		{ }

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
			return settings_;
		}

		ModularWidget* PresentaBALLPlugin::createModularWidget(MainControl* main_control)
		{
			html_interface_ = new PresentaBALLView(main_control);

			HTMLViewDock* html_view = new HTMLViewDock(html_interface_, main_control, String(tr("PresentaBALL")).c_str());

			settings_ = html_interface_->getSettings();

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
