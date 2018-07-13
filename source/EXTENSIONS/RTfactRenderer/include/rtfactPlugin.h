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

		class RTfactPlugin :
			public QObject,
			public BALLPlugin,
			public BALL::VIEW::VIEWPlugin,
			public BALL::VIEW::RendererPlugin
		{
			Q_OBJECT
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::RendererPlugin)
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Renderer.RTfactRenderer")

			public:
				RTfactPlugin();
				~RTfactPlugin() noexcept override = default;

				const QPixmap* getIcon() const override;
				QString getName() const override;
				QString getDescription() const override;

				BALL::VIEW::ConfigDialog* getConfigDialog() override;

				BALL::VIEW::Renderer* createRenderer() override;

				bool isActive() override {return is_active_; }
				bool activate() override { return is_active_ = true; }
				bool deactivate() override { is_active_ = false; return true; }

			private:
				bool is_active_;
				QPixmap icon_;
		};
	}
}

#endif // BALL_RTFACTRENDERER_RTFACTPLUGIN_H
