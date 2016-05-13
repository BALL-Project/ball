#ifndef BALLAXYPLUGIN_H
#define BALLAXYPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALLaxyInterface.h>
#include <BALLaxyInterfacePreferences.h>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/PLUGIN/modularWidgetPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		class BALLaxyPlugin
			: public QObject,
			  public BALLPlugin,
			  public VIEWPlugin,
			  public ModularWidgetPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.ModularWidget.BALLaxy")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::ModularWidgetPlugin)

			public:
				BALLaxyPlugin();
				virtual ~BALLaxyPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				ModularWidget* createModularWidget(MainControl* main_control);

				bool isActive() { return is_active_; }

				bool activate();
				bool deactivate();

			private:
				QPixmap icon_;
				bool is_active_;
				BALLaxyInterfacePreferences* preferences_;
		};
	}
}


#endif // BALLAXYPLUGIN_H
