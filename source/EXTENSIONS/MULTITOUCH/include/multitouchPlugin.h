#ifndef MULTITOUCHPLUGIN_H
#define MULTITOUCHPLUGIN_H

#include <QtCore/QObject>
#include <QtGui/QPixmap>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>

namespace BALL
{
	namespace VIEW
	{

		class MultitouchPlugin 
			: public QObject, 
				public BALLPlugin, 
				public VIEWPlugin, 
				public InputPlugin
		{
			Q_OBJECT
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				MultitouchPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				QDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				void setReceiver(QWidget* receiver);
				InputDeviceDriver* startDriver();

				bool activate();
				bool deactivate();

				void setHostName(const String& hostname) 
				{ 
					hostname_=hostname; 
				}

				void setPort(const String& port) 
				{ 
					port_=port; 
				}
			private:
				QWidget* receiver_;
				QPixmap icon_;
				bool is_active_;
				String hostname_;
				String port_;
		};
	}
}

#endif //MULTITOUCHPLUGIN_H

