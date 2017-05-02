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
		class JupyterPlugin
			: public QObject,
			  public BALLPlugin,
				public VIEWPlugin,
				public ModularWidgetPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.ModularWidget.Jupyter")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::ModularWidgetPlugin)

			public:
				JupyterPlugin();
				virtual ~JupyterPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				bool isActive() { return widget_; }

				bool activate();
				bool deactivate();

			private:
				QPixmap icon_;
				JupyterPreferences* preferences_;
				ModularWidget* widget_;
		};
	}
}

#endif // JUPYTERPLUGIN_H
