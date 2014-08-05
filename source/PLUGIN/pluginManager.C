#include <BALL/PLUGIN/pluginManager.h>
#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/PLUGIN/pluginHandler.h>

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>
#include <BALL/COMMON/logStream.h>

#if defined(BALL_OS_WINDOWS)
# define PLUGIN_MASK "plugin*.dll"
#elif defined(BALL_OS_DARWIN)
# define PLUGIN_MASK "plugin*.dylib"
#else
# define PLUGIN_MASK "plugin*.so"
#endif

namespace BALL
{
	const char* PluginManager::BETWEEN_PLUGINDIR_SEPERATOR  = "?";
	boost::shared_ptr<PluginManager> PluginManager::manager_;

	QMutex PluginManager::mutex_;

	PluginManager::PluginManager()
	{
	}

	PluginManager::~PluginManager()
	{
		unloadAllPlugins();
	}

	PluginManager& PluginManager::instance()
	{
		//Make the PluginManager creation thread safe
		if (!manager_)
		{
			//Another thread could have taken over control right now
			//so lock a mutex to ensure the PluginManager is created only once.
			mutex_.lock();
			//Check that the manager has not been created by a concurring thread
			if(!manager_)
			{
				manager_ = boost::shared_ptr<PluginManager>(new PluginManager());
			}
			mutex_.unlock();
		}

		return *manager_;
	}

	bool PluginManager::addPluginDirectory(const QString& dir)
	{
		std::map<QString, vector<BALLPlugin*> >::iterator to_load_it = loaded_plugin_dirs_.find(dir);
		if (to_load_it == loaded_plugin_dirs_.end())
		{
			vector<BALLPlugin*> loaded_plugins;
			QDir plugin_dir(dir, PLUGIN_MASK, QDir::Name | QDir::IgnoreCase, QDir::Files);

			if(!plugin_dir.exists())
			{
				return false;
			}

			// collect the loaded plugins in this dir
			foreach(QString it, plugin_dir.entryList())
			{
				BALLPlugin* new_plugin = loadPlugin(plugin_dir.absoluteFilePath(it));
				if (new_plugin)
				{
					loaded_plugins.push_back(new_plugin);
				}
			}

			// and store the entry
			loaded_plugin_dirs_[dir]=loaded_plugins;

			return true;
		}

		return false;
	}

	bool PluginManager::unloadDirectoryPlugins_(PluginDirMap::iterator it)
	{
		if(it == loaded_plugin_dirs_.end()) {
			return false;
		}

		vector<BALLPlugin*>::iterator to_unload = it->second.begin();
		for (; to_unload != it->second.end(); ++to_unload)
		{
			unloadPlugin((*to_unload)->getName());
		}

		// and the directory itself
		loaded_plugin_dirs_.erase(it);

		return true;
	}

	bool PluginManager::removePluginDirectory(const QString& dir)
	{
		std::map<QString, vector<BALLPlugin*> >::iterator to_unload_it = loaded_plugin_dirs_.find(dir);

		return unloadDirectoryPlugins_(to_unload_it);
	}

	vector<QString> PluginManager::getPluginDirectories() const
	{
		vector<QString> result;

		std::map<QString, vector<BALLPlugin*> >::const_iterator it = loaded_plugin_dirs_.begin();
		for (; it!=loaded_plugin_dirs_.end(); ++it)
			result.push_back(it->first);

		return result;
	}

	BALLPlugin* PluginManager::loadPlugin(const QString& plugin_name)
	{
		BALLPlugin* plugin = 0;
		QPluginLoader* loader = new QPluginLoader(plugin_name);

		Log.info() << "Trying to load plugin: " << plugin_name.toStdString() << std::endl;
		if (loader->load())
		{
			plugin = qobject_cast<BALLPlugin*>(loader->instance());
		}
		else
		{
			Log.info() << "Error:" << loader->errorString().toStdString() << std::endl;
		}

		if (plugin)
		{
			QHash<QString, QPluginLoader*>::iterator it = loaders_.find(plugin->getName());

			if(it != loaders_.end()) {
				Log.info() << "Plugin was already loaded." << std::endl;
				delete loader;
				return qobject_cast<BALLPlugin*>(*it);
			}

			Log.info() << "Loaded plugin " << plugin_name.toStdString() << "." << std::endl;
			loader_mutex_.lockForWrite();
			loaders_.insert(plugin->getName(), loader);
			loader_mutex_.unlock();

			if(autoactivate_plugins_.contains(plugin->getName())) {
				startPlugin(plugin);
				plugin->activate();
			}
		}

		return plugin;
	}

	bool PluginManager::unloadPlugin(const QString& plugin)
	{
		QWriteLocker locker(&loader_mutex_);
		QHash<QString, QPluginLoader*>::iterator it = loaders_.find(plugin);

		if (it != loaders_.end())
		{
			//Shutdown the plugin
			stopPlugin(qobject_cast<BALLPlugin*>(it.value()->instance()));
			
			// NOTE: unloading crashes BALLView, for some reason!
			//       it is not really crucial to unload the plugins, but we should have
			//       a look at this anyhow
			//Delete the loader
//			it.value()->unload();
//			delete it.value();

			loaders_.erase(it);

			return true;
		}

		return false;
	}

	void PluginManager::unloadAllPlugins()
	{
		loader_mutex_.lockForWrite();
		QHash<QString, QPluginLoader*>::iterator it = loaders_.begin();
		for (; it != loaders_.end(); ++it)
		{
			//Shutdown the plugin
			stopPlugin(qobject_cast<BALLPlugin*>(it.value()->instance()));
			//Delete the loader
			it.value()->unload();
			delete it.value();
		}
		loaders_.clear();
		loader_mutex_.unlock();
	}

	QObject* PluginManager::getPluginInstance(const QString& plugin)
	{
		QReadLocker locker(&loader_mutex_);
		QHash<QString, QPluginLoader*>::iterator it = loaders_.find(plugin);

		if(it != loaders_.end())
		{
			return (it.value()->instance());
		}

		return NULL;
	}

	QObject* PluginManager::getPluginInstance(int pos)
	{
		QReadLocker locker(&loader_mutex_);
		if(pos < 0 || pos >= getPluginCount())
		{
			return NULL;
		}

		QHash<QString, QPluginLoader*>::const_iterator it = loaders_.begin();
		for(int i = 0; i < pos; ++i, ++it) {}

		return it.value()->instance();
	}

	void PluginManager::registerHandler(boost::shared_ptr<PluginHandler> h)
	{
		handler_mutex_.lockForWrite();
		shared_handlers_.push_back(h);
		handlers_.push_back(h.get());
		handler_mutex_.unlock();
	}

	void PluginManager::registerHandler(PluginHandler* h)
	{
		handler_mutex_.lockForWrite();
		handlers_.push_back(h);
		handler_mutex_.unlock();
	}

	bool PluginManager::unregisterHandler(PluginHandler* h)
	{
		// First remove the handler from the list of PluginHandlers
		handler_mutex_.lockForRead();
		std::list<PluginHandler*>::iterator ht = std::find(handlers_.begin(), handlers_.end(), h);
		handler_mutex_.unlock();

		if(ht == handlers_.end())
		{
			return true;
		}

		handler_mutex_.lockForWrite();
		handlers_.erase(ht);
		handler_mutex_.unlock();

		// Now stop all plugins that are run by this handler
		QWriteLocker locker(&loader_mutex_);

		QHash<QString, QPluginLoader*>::iterator it = loaders_.begin();
		while(it != loaders_.end())
		{
			if (!it.value()->isLoaded())
			{
				++it;
				continue;
			}

			BALLPlugin* plugin = qobject_cast<BALLPlugin*>(it.value()->instance());

			if(!plugin || !h->isRunning(plugin))
			{
				++it;
				continue;
			}

			if(!h->stopPlugin(plugin) || !stopPlugin(plugin))
			{
				return false;
			}

			//Delete the loader
			it.value()->unload();
			delete it.value();

			it = loaders_.erase(it);
		}

		return true;
	}

	bool PluginManager::startPlugin(int plugin)
	{
		return startPlugin(qobject_cast<BALLPlugin*>(getPluginInstance(plugin)));
	}

	bool PluginManager::startPlugin(const QString& plugin)
	{
		return startPlugin(qobject_cast<BALLPlugin*>(getPluginInstance(plugin)));
	}

	bool PluginManager::startPlugin(BALLPlugin* plugin)
	{
		if (!plugin)
		{
			return true;
		}

		bool started = false;
		handler_mutex_.lockForRead();
		std::list<PluginHandler*>::iterator it = handlers_.begin();
		for (; it != handlers_.end(); ++it)
		{
			if ((*it)->canHandle(plugin))
			{
				started = (*it)->startPlugin(plugin) && !started;
			}
		}
		handler_mutex_.unlock();

		return started;
	}

	bool PluginManager::stopPlugin(int plugin)
	{
		return stopPlugin(qobject_cast<BALLPlugin*>(getPluginInstance(plugin)));
	}

	bool PluginManager::stopPlugin(const QString& plugin)
	{
		return stopPlugin(qobject_cast<BALLPlugin*>(getPluginInstance(plugin)));
	}

	bool PluginManager::stopPlugin(BALLPlugin* plugin)
	{
		if (!plugin)
		{
			return true;
		}

		bool all_stopped = true;
		handler_mutex_.lockForRead();
		std::list<PluginHandler*>::iterator it = handlers_.begin();
		for (; it != handlers_.end(); ++it)
		{
			if ((*it)->isRunning(plugin))
			{
				all_stopped = (*it)->stopPlugin(plugin) && all_stopped;
			}
		}
		handler_mutex_.unlock();

		return all_stopped;
	}

	int PluginManager::getPluginCount() const
	{
		QReadLocker locker(&loader_mutex_);
		return loaders_.size();
	}

	bool PluginManager::getPluginDirectories(String& value) const
	{
		if (!loaded_plugin_dirs_.empty())
		{
			std::map<QString, vector<BALLPlugin*> >::const_iterator it = loaded_plugin_dirs_.begin();

			if ( it != loaded_plugin_dirs_.end())
			{

				value += it->first.toAscii().toPercentEncoding().data();

				for (++it; it != loaded_plugin_dirs_.end(); ++it)
				{
					value += BETWEEN_PLUGINDIR_SEPERATOR;
					value += it->first.toAscii().toPercentEncoding().data();
				}
			}
		}
		return true;
	}

	bool PluginManager::setPluginDirectories(const String& value)
	{
		std::vector<String> plugin_directories;

		String tmp(QByteArray::fromPercentEncoding(QByteArray(value.c_str())).data());
		tmp.split(plugin_directories, BETWEEN_PLUGINDIR_SEPERATOR, 0);

		for (size_t i = 0; i < plugin_directories.size(); ++i)
		{
			addPluginDirectory(plugin_directories[i].c_str());
		}

		return true;
	}

	QString PluginManager::getAutoActivatePlugins() const
	{
		return autoactivate_plugins_.join(";");
	}

	bool PluginManager::setAutoActivatePlugins(const QString& value)
	{
		autoactivate_plugins_ = value.split(";", QString::SkipEmptyParts);

		return true;
	}

	void PluginManager::autoActivatePlugin(const QString& str)
	{
		if(!autoactivate_plugins_.contains(str)) {
			autoactivate_plugins_.append(str);
		}
	}

	void PluginManager::doNotAutoActivatePlugin(const QString& str)
	{
		autoactivate_plugins_.removeOne(str);
	}
}
