#ifndef BALL_PLUGIN_BALLPLUGIN_H
#define BALL_PLUGIN_BALLPLUGIN_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QString>
#include <QtCore/QtPlugin>
#include <QtCore/QVariant>

class QPixmap;
class QDialog;

namespace BALL
{

	class PluginHandler;

	class BALL_EXPORT BALLPlugin
	{
		public:
			virtual ~BALLPlugin() {}

			/**
			 * Returns the name of the plugin
			 *
			 * @return the plugins name
			 */
			virtual QString getName() const = 0;

			/**
			 * Returns a description of the plugin
			 *
			 * @return a description of the plugin
			 */
			virtual QString getDescription() const = 0;

			/**
			 * Get the current state of the plugin.
			 *
			 * @return true if the plugin is initialized and running, false otherwise
			 */
			virtual bool isActive() = 0;

			/**
			 * Activate the plugin.
			 *
			 * @return true if no problems during activation occured, false otherwise
			 */
			virtual bool activate() = 0;

			/**
			 * Deactivate the plugin.
			 *
			 * @return true if no problems during deactivation occured, false otherwise
			 */
			virtual bool deactivate() = 0;
	};
}

template<typename T>
T qobject_cast(BALL::BALLPlugin* plugin)
{
	return qobject_cast<T>(dynamic_cast<QObject*>(plugin));
}

Q_DECLARE_METATYPE(BALL::BALLPlugin*)
Q_DECLARE_INTERFACE(BALL::BALLPlugin, "org.ball-project.Plugin.BALLPlugin/1.0")

#endif //BALL_PLUGIN_BALLPLUGIN_H

