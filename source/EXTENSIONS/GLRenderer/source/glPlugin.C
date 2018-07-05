#include <glPlugin.h>
#include <glRenderer.h>
#include <glConfigDialog.h>

namespace BALL
{
	namespace GLRenderer
	{

		GLPlugin::GLPlugin()
				: is_active_(false),
				  icon_(":pluginGLRenderer.png"),
				  config_dialog_(new GLConfigDialog)
		{
		}

		GLPlugin::~GLPlugin()
		{
		}

		QString GLPlugin::getName() const
		{
			return QString("GLRenderer");
		}

		QString GLPlugin::getDescription() const
		{
			return QString("An OpenGL 1.x renderer.");
		}

		const QPixmap* GLPlugin::getIcon() const
		{
			return &icon_;
		}

		BALL::VIEW::ConfigDialog* GLPlugin::getConfigDialog()
		{
			return config_dialog_.get();
		}

		BALL::VIEW::Renderer* GLPlugin::createRenderer()
		{
			return new GLRenderer(config_dialog_.get());
		}

	} // namespace GLRenderer
} // namespace BALL
