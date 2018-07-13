#ifndef BALL_RTFACTRENDERER_RTFACTPLUGIN_H
#define BALL_RTFACTRENDERER_RTFACTPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/RENDERING/rendererPlugin.h>

namespace BALL
{
	namespace RTfactRenderer
	{

		class RTFactPlugin :
			public QObject,
			public BALLPlugin,
			public BALL::VIEW::VIEWPlugin,
			public BALL::VIEW::RendererPlugin
		{
			Q_OBJECT
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::RendererPlugin)
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Renderer.RTFactRenderer")

			public:
				RTFactPlugin();
				virtual ~RTFactPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				BALL::VIEW::ConfigDialog* getConfigDialog();

				BALL::VIEW::Renderer* createRenderer();

				bool isActive() {return is_active_; }
				bool activate() { return is_active_ = true; }
				bool deactivate() { is_active_ = false; return true; }

			private:
				bool is_active_;
				QPixmap icon_;
		};
	}
}

#endif // BALL_RTFACTRENDERER_RTFACTPLUGIN_H
