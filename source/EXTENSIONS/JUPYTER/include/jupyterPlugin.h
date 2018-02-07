#ifndef JUPYTERPLUGIN_H
#define JUPYTERPLUGIN_H

#include <jupyterWidget.h>
#include <jupyterPreferences.h>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/PLUGIN/modularWidgetPlugin.h>

#include <QtGui/QPixmap>

#include <memory>

namespace BALL
{
	namespace VIEW
	{
		/**
		 * [JupyterPlugin]
		 * VIEW plugin to make Jupyter notebooks accessible in a dock widget. Jupyter notebook servers can either be
		 * hosted externally or via this plugin.
		 *
		 * Memory management of this plugin is isolated from the application it's running in. Do not attempt to delete
		 * objects obtained from this plugin.
		 */
		class JupyterPlugin :
			public QObject,
			public BALLPlugin,
			public VIEWPlugin,
			public ModularWidgetPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.ModularWidget.Jupyter")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::ModularWidgetPlugin)

			public:
				JupyterPlugin() = default;
				~JupyterPlugin() override = default;

				const QPixmap* getIcon() const override { return &icon_; };
				QString getName() const override { return QString("Jupyter"); };
				QString getDescription() const override { return QString("An interface to Jupyter notebooks."); };

				ConfigDialog* getConfigDialog() override { return preferences_.get(); };

				bool isActive() override { return widget_.get(); }

				bool activate() override;
				bool deactivate() override;

			private:
				QPixmap icon_ {":pluginJupyter.png"};
				std::unique_ptr<JupyterPreferences> preferences_ { new JupyterPreferences() };
				std::unique_ptr<JupyterWidget> widget_ {nullptr};
		};
	}
}

#endif // JUPYTERPLUGIN_H
