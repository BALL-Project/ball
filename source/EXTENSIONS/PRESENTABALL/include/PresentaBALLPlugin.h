#ifndef PRESENTABALLPLUGIN_H
#define PRESENTABALLPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <PresentaBALLView.h>
#include <PresentaBALLSettings.h>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/PLUGIN/modularWidgetPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		class PresentaBALLPlugin
			: public QObject,
			  public BALLPlugin,
			  public VIEWPlugin,
			  public ModularWidgetPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.ModularWidget.PresentaBALL")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::ModularWidgetPlugin)

			public:
				PresentaBALLPlugin();
				virtual ~PresentaBALLPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				bool isActive() { return widget_; }

				bool activate();
				bool deactivate();

			private:
				QPixmap icon_;
				PresentaBALLView* html_interface_;
				PresentaBALLSettings* settings_;
				ModularWidget* widget_;
		};
	}
}

#endif // PRESENTABALLPLUGIN_H
