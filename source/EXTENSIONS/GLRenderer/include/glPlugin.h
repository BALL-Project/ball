#ifndef GLPLUGIN_H
#define GLPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/RENDERING/rendererPlugin.h>

namespace BALL {
	namespace VIEW {
		class Renderer;
	}
}

class GLConfigDialog;

class BALL_PLUGIN_EXPORT GLPlugin : public QObject, public BALL::BALLPlugin, public BALL::VIEW::VIEWPlugin, public BALL::VIEW::RendererPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Renderer.GLRenderer")
	Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::RendererPlugin)

	public:
		GLPlugin();
		virtual ~GLPlugin();

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
		GLConfigDialog* config_dialog_;
};

#endif //GLPLUGIN_H
