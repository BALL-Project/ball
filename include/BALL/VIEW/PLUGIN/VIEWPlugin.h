#ifndef VIEWPLUGIN_H
#define VIEWPLUGIN_H

#include <QtCore/QString>
#include <QtCore/QtPlugin>

class QPixmap;
class QDialog;

namespace BALL
{
	namespace VIEW
	{
		class VIEWPlugin
		{
			public:
				virtual ~VIEWPlugin() {}

				virtual QString getName() const = 0;
				virtual QString getDescription() const = 0;
				virtual const QPixmap* getIcon() const = 0;

				virtual QDialog* getConfigDialog() = 0;

				virtual bool isActive() = 0;
		};

	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::VIEWPlugin, "org.ball-project.Plugin.VIEWPlugin/1.0")

#endif //VIEWPLUGIN_H

