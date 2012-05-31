#ifndef BALL_PLUGIN_PLUGINHANDLER_H
#define BALL_PLUGIN_PLUGINHANDLER_H


#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <list>
#include <QtCore/QReadWriteLock>

namespace BALL
{
	class BALLPlugin;

	/**
	 * A PluginHandler is a class that is responsible for initializing
	 * a certain plugin type. It provides the method canHandle() that
	 * checks whether a plugin can be initialized by the handler and
	 * has the purely virtual methods specificSetup_() and specificShutdown_()
	 * that provide plugin type specific initializations.
	 *
	 * Implementing a suitable PluginHandler is the second step besides
	 * defining the plugin interface itsself that is needed to create
	 * a new plugin api for BALL.
	 */
	class BALL_EXPORT PluginHandler
	{
		public:
			/**
			 * A virtual destructor. Does nothing at the moment.
			 */
			virtual ~PluginHandler();

			/**
			 * This function is used to check whether this PluginHandler can initialize
			 * the passed plugin. A usual implementation would be something like:
			 *
			 *   return qobject_cast<MyPlugin*>(plugin) != 0;
			 *
			 * @return true if the handler can make use of the plugin, false otherwise
			 */
			virtual bool canHandle(BALLPlugin* plugin) const = 0;

			/**
			 * This function checks whether the plugin can be handled and
			 * if it is not already running. Then the specificSetup()_ routine
			 * is called which should take care of all plugin specific initilizations.
			 *
			 * If specificSetup_() returns true, the plugin is added to the list of
			 * already running plugins.
			 *
			 * @return true if the plugin could be started, false otherwise.
			 */
			bool startPlugin(BALLPlugin* plugin);

			/**
			 * This function checks whether the plugin is currently running
			 * and atempts to stop it via the specificShutdown_() method. On
			 * success the plugin is removed from the list of running plugins.
			 *
			 * @return true if the plugin could be stopped, false otherwise.
			 */
			bool stopPlugin(BALLPlugin* plugin);

			/**
			 * This function must check if the passed plugin has been started
			 * by this handler
			 *
			 * @return true if the handler started the plugin, false otherwise
			 */
			virtual bool isRunning(BALLPlugin* plugin) const;

		protected:
			/**
			 * A purely virtual function which is responsible for
			 * properly initialising the passed plugin instance.
			 *
			 * The passed plugin may be assumed to be of a type that
			 * can be handled by this PluginHandler.
			 *
			 * @return true if the setup succeeded, false otherwise
			 */
			virtual bool specificSetup_(BALLPlugin* plugin) = 0;

			/**
			 * A purely virtual function which is responsible for
			 * properly deinitialising the passed plugin instance.
			 *
			 * The passed plugin may be assumed to be of a type that
			 * can be handled by this PluginHandler.
			 *
			 * @return true if the deinitialization succeeded, false otherwise
			 */
			virtual bool specificShutdown_(BALLPlugin* plugin) = 0;

			/**
			 * The list of plugins this handler is responsible for. Plugins should only
			 * be added/removed by using start/stopPlugin
			 */
			std::list<BALLPlugin*> running_plugins_;
		private:
			mutable QReadWriteLock mutex_;
	};
}

#endif //BALL_PLUGIN_PLUGINHANDLER_H

