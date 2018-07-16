#include <rtfactPlugin.h>

#include <rtfactRenderer.h>

using namespace BALL::VIEW;

namespace BALL
{
	namespace RTfactRenderer
	{
		RTfactPlugin::RTfactPlugin()
			: is_active_(false),
			  icon_(":pluginRTfactRenderer.png")
		{
		}

		QString RTfactPlugin::getName() const
		{
			return QString("RTfactRenderer");
		}

		QString RTfactPlugin::getDescription() const
		{
			return QString("A real time raytracing renderer using the RTfact library.");
		}

		const QPixmap* RTfactPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* RTfactPlugin::getConfigDialog()
		{
			return nullptr;
		}

		Renderer* RTfactPlugin::createRenderer()
		{
			return new RTfactRenderer;
		}

	}
}
