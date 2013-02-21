#ifndef PRESENTABALLPLUGIN_H
#define PRESENTABALLPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <HTMLBasedInterface.h>

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
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::ModularWidgetPlugin)

			public:
				PresentaBALLPlugin();
				virtual ~PresentaBALLPlugin();

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
		};
	}
}


#endif // PRESENTABALLPLUGIN_H
