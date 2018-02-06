#ifndef JUPYTERPLUGIN_H
#define JUPYTERPLUGIN_H

#include <jupyterWidget.h>
#include <jupyterPreferences.h>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/PLUGIN/modularWidgetPlugin.h>

#include <QtGui/QPixmap>

namespace BALL
{
	namespace VIEW
	{
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
				JupyterPlugin();
				~JupyterPlugin() override;

				const QPixmap* getIcon() const override { return &icon_; };
				QString getName() const override { return QString("Jupyter"); };
				QString getDescription() const override { return QString("An interface to Jupyter notebooks."); };

				ConfigDialog* getConfigDialog() override { return preferences_; };

				bool isActive() override { return widget_; }

				bool activate() override;
				bool deactivate() override;

			private:
				QPixmap icon_;
				JupyterPreferences* preferences_;
				ModularWidget* widget_;
		};
	}
}

#endif // JUPYTERPLUGIN_H
