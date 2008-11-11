#ifndef BALL_PLUGIN_BALLPLUGIN_H
#define BALL_PLUGIN_BALLPLUGIN_H

#include <QtCore/QString>
#include <QtCore/QtPlugin>

class QPixmap;
class QDialog;

namespace BALL
{
	class BALLPlugin 
	{
		public:
			virtual ~BALLPlugin() {}

			virtual QString getName() const = 0;
			virtual QString getDescription() const = 0;

			virtual bool isActive() = 0;

			virtual void activate() = 0;
			virtual void deactivate() = 0;
	};
}

Q_DECLARE_INTERFACE(BALL::BALLPlugin, "org.ball-project.Plugin.BALLPlugin/1.0")

#endif //BALL_PLUGIN_BALLPLUGIN_H

