#ifndef BALL_GLRENDERER_GLPLUGIN_H
#define BALL_GLRENDERER_GLPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/RENDERING/rendererPlugin.h>

#include <memory>

namespace BALL {
	namespace GLRenderer {
		class GLConfigDialog;

		class BALL_PLUGIN_EXPORT GLPlugin :
			public QObject,
			public BALL::BALLPlugin,
			public BALL::VIEW::VIEWPlugin,
			public BALL::VIEW::RendererPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Renderer.GLRenderer")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::RendererPlugin)

			public:
				GLPlugin();
				~GLPlugin() override;

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
				std::unique_ptr<GLConfigDialog> config_dialog_;
		};

	} // namespace GLRenderer
} // namespace BALL

#endif // BALL_GLRENDERER_GLPLUGIN_H
