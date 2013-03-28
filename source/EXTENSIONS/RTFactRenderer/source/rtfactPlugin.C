#include <rtfactPlugin.h>

#include <rtfactRenderer.h>

namespace BALL
{
	namespace VIEW
	{
		RTFactPlugin::RTFactPlugin()
			: is_active_(false),
			  icon_(":pluginRTfactRenderer.png")
		{
		}

		RTFactPlugin::~RTFactPlugin()
		{
		}

		QString RTFactPlugin::getName() const
		{
			return QString("RTFactRenderer");
		}

		QString RTFactPlugin::getDescription() const
		{
			return QString("A real time raytracing renderer using the RTFact library.");
		}

		const QPixmap* RTFactPlugin::getIcon() const
		{
			return &icon_;
		}

		ConfigDialog* RTFactPlugin::getConfigDialog()
		{
			return NULL;
		}

		Renderer* RTFactPlugin::createRenderer()
		{
			return new RTfactRenderer;
		}

	}
}
