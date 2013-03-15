#ifndef XML3DPLUGIN_H
#define XML3DPLUGIN_H

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
		class XML3DSettings;

		class XML3DPlugin 
			: public QObject, 
				public BALLPlugin, 
				public VIEWPlugin, 
				public InputPlugin
		{
			Q_OBJECT
			Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Input.XML3D")
			Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::InputPlugin)

			public:
				XML3DPlugin();

				const QPixmap* getIcon() const;
				QString getName() const;
				QString getDescription() const;

				ConfigDialog* getConfigDialog();

				bool isActive() { return is_active_; }

				void setReceiver(QWidget* receiver);
				InputDeviceDriver* startDriver();

				bool activate();
				bool deactivate();

				void setHostName(const String& hostname) 
				{ 
					hostname_ = hostname; 
				}

				void setPort(const String& port) 
				{ 
					port_ = port; 
				}

				void setSensitivity(const Size sensitivity)
				{
					sensitivity_ = sensitivity;
				}
			private:
				QWidget* receiver_;
				QPixmap icon_;
				bool is_active_;
				String hostname_;
				String port_;
				Size sensitivity_;
				XML3DSettings* settings_;
		};
	}
}

#endif //XML3DPLUGIN_H

