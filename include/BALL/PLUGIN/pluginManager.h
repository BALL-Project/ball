#ifndef BALL_PLUGIN_PLUGINMANAGER_H
#define BALL_PLUGIN_PLUGINMANAGER_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <QtCore/QReadWriteLock>

#include <boost/shared_ptr.hpp>

#include <list>
#include <map>

class QPluginLoader;
class QObject;

namespace BALL
{
	class BALLPlugin;
	class PluginHandler;

	/**
	 * A singleton that takes care of handling all plugins.
	 * It is responsible for loading, starting and stopping plugins.
	 * For the task of properly initializing the plugins it utilizes
	 * the PluginHandler helper classes, that need to be supplemented
	 * for each new plugin type.
	 */
	class BALL_EXPORT PluginManager
	{
		public:

			virtual ~PluginManager();

			/**
			 * Use this method to obtain the PluginManager instance.
			 *
			 * This method is thread safe.
			 */
			static PluginManager& instance();

			/**
			 * Tries to load all plugins (files named like: pluginMyPlugin.$LIBRARY_SUFFIX)
			 * located in the specified directoy dir.
			 *
			 * @param dir the directory to search for plugins.
			 * @return true if the directory could successfully be loaded.
			 *         false if it has already been loaded or is invalid.
			 */
			bool addPluginDirectory(const QString& dir);

			/**
			 * Tries to unload all plugins (files named like: pluginMyPlugin.$LIBRARY_SUFFIX)
			 * located in the specified directoy dir.
			 *
			 * @param dir the directory to search for plugins to remove.
			 * @return true if the plugin could be successfully removed.
			 */
			bool removePluginDirectory(const QString& dir);

			/** Return a list of directories currently searched for plugins.
			 */
			std::vector<QString> getPluginDirectories() const;

			/**
			 * Loads the plugin specified by plugin_name.
			 *
			 * @param plugin_name The path to the plugin to load
			 *
			 * @return NULL if the plugin could not be loaded. A valid pointer
			 *         to the loaded plugin otherwise.
			 */
			BALLPlugin* loadPlugin(const QString& plugin_name);

			/**
			 * Unloads the plugin specified by plugin_name.
			 *
			 * @param plugin_name The name of the plugin to unload
			 */
			bool unloadPlugin(const QString& plugin);

			/**
			 * Searches for the plugin and returns a pointer
			 * to it if it has been loaded.
			 *
			 * @param plugin The name of the plugin to search
			 *
			 * @return NULL if the plugin could not be found, a pointer to
			 *         it otherwise
			 */
			QObject* getPluginInstance(const QString& plugin);

			/**
			 * Searches for the plugin and returns a pointer
			 * to it if it has been loaded.
			 *
			 * @param pos The position of the plugin in the plugin list
			 *
			 * @return NULL if the plugin could not be found, a pointer to
			 *         it otherwise
			 */
			QObject* getPluginInstance(int pos);

			/**
			 * See startPlugin(BALLPlugin* plugin)
			 */
			bool startPlugin(int plugin);

			/**
			 * See startPlugin(BALLPlugin* plugin)
			 */
			bool startPlugin(const QString& plugin);

			/**
			 * Starts the specified plugin through a applicable
			 * PluginHandler
			 *
			 * @param plugin A pointer to the plugin to be started
			 *
			 * @return false if no suitable handler could be found,
			 *         true otherwise.
			 */
			bool startPlugin(BALLPlugin* plugin);

			/**
			 * See stopPlugin(BALLPlugin* plugin)
			 */
			bool stopPlugin(int plugin);

			/**
			 * See stopPlugin(BALLPlugin* plugin)
			 */
			bool stopPlugin(const QString& plugin);

			/**
			 * Stops the specified plugin by letting all PluginHandlers
			 * stop it.
			 *
			 * @param plugin A pointer to the plugin to be stopped
			 *
			 * @return false if one handler could not stop the plugin,
			 *         true otherwise.
			 */
			bool stopPlugin(BALLPlugin* plugin);

			/** Unload all registered plugins.
			 */
			void unloadAllPlugins();

			/**
			 * Returns the number of loaded plugins.
			 */
			int getPluginCount() const;

			/**
			 * Register a new PluginHandler. This handler will then
			 * be available for starting new plugins.
			 */
			void registerHandler(PluginHandler* h);

			/**
			 * Register a new PluginHandler. This handler will then
			 * be available for starting new plugins.
			 *
			 * This method is a convenience funtion that takes a boost
			 * shared_ptr which is stored in the PluginManager and
			 * hence destroyed once the PluginManager is unloaded.
			 */
			void registerHandler(boost::shared_ptr<PluginHandler> h);

			/**
			 * Unregister a PluginHandler.
			 *
			 * If the handler was not registered this function does nothing.
			 * Otherwise all plugins for which the handler is responsible will be
			 * unloaded and the handler will no longer be available.
			 *
			 * Every registered PluginHandler should call this function before it
			 * is destroyed. Otherwise segmentation faults will happen!
			 *
			 * @warning All plugins run by this handler will be unloaded, even if there
			 * are other handlers handling them!
			 *
			 * @param h the handler that should be unregistered
			 *
			 * @return true if the handler was unregistered successfully;
			 *         false if there was an error during the deactivation of running plugins
			 */
			bool unregisterHandler(PluginHandler* h);

			// needed for storing this classes' preferences
			virtual bool getPluginDirectories(String& value) const;
			virtual bool setPluginDirectories(const String&);

			virtual QString getAutoActivatePlugins() const;
			virtual bool setAutoActivatePlugins(const QString&);

			void autoActivatePlugin(const QString& str);
			void doNotAutoActivatePlugin(const QString& str);

		protected:
			static const char* BETWEEN_PLUGINDIR_SEPERATOR;

			typedef std::map<QString, vector<BALLPlugin*> > PluginDirMap;

			PluginManager();
			PluginManager(const PluginManager&);
			PluginManager& operator=(const PluginManager&);

			bool unloadDirectoryPlugins_(PluginDirMap::iterator it);
			PluginDirMap loaded_plugin_dirs_;

			QHash<QString, QPluginLoader*> loaders_;
			std::list<PluginHandler*> handlers_;
			std::list<boost::shared_ptr<PluginHandler> > shared_handlers_;

			QStringList autoactivate_plugins_;

			static boost::shared_ptr<PluginManager> manager_;

			//This mutex is used in the creation of the singleton
			static QMutex mutex_;
			//This mutex guards the handlers_ list
			mutable QReadWriteLock handler_mutex_;
			//This mutex guards the loaders_ list
			mutable QReadWriteLock loader_mutex_;
	};
}

#endif //BALL_PLUGIN_PLUGINMANAGER_H
