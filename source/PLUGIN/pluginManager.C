#include <BALL/PLUGIN/pluginManager.h>
#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/PLUGIN/pluginHandler.h>

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	const char* PluginManager::BETWEEN_PLUGINDIR_SEPERATOR  = "?";
	boost::shared_ptr<PluginManager> PluginManager::manager_;

	QMutex PluginManager::mutex_;

	PluginManager::PluginManager()
		: PreferencesObject()
	{
		setObjectName("PluginManager");
	}

	PluginManager::~PluginManager()
	{
		unloadAllPlugins();

		std::list<PluginHandler*>::iterator ht = handlers_.begin();
		for (; ht != handlers_.end(); ++ht)
		{
			delete *ht;
		}
		//unregisterThis();
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

	void PluginManager::addPluginDirectory(const QString& dir, bool autoactivate)
	{
		std::map<QString, vector<BALLPlugin*> >::iterator to_load_it = loaded_plugin_dirs_.find(dir);
		if (to_load_it == loaded_plugin_dirs_.end())
		{
			vector<BALLPlugin*> loaded_plugins;
#if defined(BALL_OS_WINDOWS)
			QDir plugin_dir(dir, "plugin*.dll", QDir::Name | QDir::IgnoreCase, QDir::Files);
#elif defined(BALL_OS_DARWIN)
			QDir plugin_dir(dir, "plugin*.dylib", QDir::Name | QDir::IgnoreCase, QDir::Files);
#else
			QDir plugin_dir(dir, "plugin*.so", QDir::Name | QDir::IgnoreCase, QDir::Files);
#endif

			// collect the loaded plugins in this dir
			foreach(QString it, plugin_dir.entryList())
			{
				BALLPlugin* new_plugin = loadPlugin(plugin_dir.absoluteFilePath(it));
				if (new_plugin)
				{
					loaded_plugins.push_back(new_plugin);
					if (autoactivate)
						startPlugin(new_plugin);
				}
			}

			// and store the entry
			loaded_plugin_dirs_[dir]=loaded_plugins;
		}
	}

	bool PluginManager::removePluginDirectory(const QString& dir)
	{
		std::map<QString, vector<BALLPlugin*> >::iterator to_unload_it = loaded_plugin_dirs_.find(dir);

		if (to_unload_it != loaded_plugin_dirs_.end())
		{
			// remove the corresponding plugins
			vector<BALLPlugin*> to_unload = to_unload_it->second;
			for (Size i=0; i<to_unload.size(); i++)
			{
				unloadPlugin(to_unload[i]->getName());
			}

			// and the directory itself
			loaded_plugin_dirs_.erase(to_unload_it);
		}
		else
		{
			return false;
		}
		return true;
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
			//Delete the loader
			it.value()->unload();
			delete it.value();
			loaders_.erase(it);

			return true;
		}

		return false;
	}

	void PluginManager::unloadAllPlugins()
	{
		QHash<QString, QPluginLoader*>::iterator it = loaders_.begin();
		for (; it != loaders_.end(); ++it)
		{
			stopPlugin(qobject_cast<BALLPlugin*>(it.value()->instance()));
			//Delete the loader
			it.value()->unload();
			delete it.value();
		}
		loaders_.clear();
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

	void PluginManager::registerHandler(PluginHandler* h)
	{
		handler_mutex_.lockForWrite();
		handlers_.push_back(h);
		handler_mutex_.unlock();
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

	bool PluginManager::getValue(String& value) const
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

	bool PluginManager::setValue(const String& value)
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
}
