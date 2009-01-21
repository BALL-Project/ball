#include <BALL/PLUGIN/pluginManager.h>

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QMutex>
#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/COMMON/logStream.h>

namespace BALL
{
	PluginManager* PluginManager::manager_ = NULL;

	PluginManager::PluginManager()
	{
	}

	PluginManager::~PluginManager()
	{
		QHash<QString, QPluginLoader*>::iterator it = loaders_.begin();
		while(it != loaders_.end()) {
			it.value()->unload();
			delete it.value();
		}
	}

	PluginManager& PluginManager::instance()
	{
		//Make the PluginManager creation thread safe
		if(!manager_) {
			//Another thread could have taken over control right now
			//so lock a mutex to ensure the PluginManager is created only once.
			QMutex m;
			m.lock();
			//Check that the manager has not been created by a concurring thread
			if(!manager_) {
				manager_ = new PluginManager();
			}
		}

		return *manager_;
	}

	void PluginManager::setPluginDirectory(const QString& dir)
	{
		plugin_dir_ = dir;
		QDir plugin_dir(plugin_dir_, "plugin*.so", QDir::Name | QDir::IgnoreCase, QDir::Files);

		foreach(QString it, plugin_dir.entryList()) {
			loadPlugin(plugin_dir.absoluteFilePath(it));
		}
	}

	BALLPlugin* PluginManager::loadPlugin(const QString& plugin_name)
	{
		BALLPlugin* plugin = 0;
		QPluginLoader* loader = new QPluginLoader(plugin_name);

		qDebug() << "Trying to load plugin: " << plugin_name;
		if (loader->load())
		{
			plugin = qobject_cast<BALLPlugin*>(loader->instance());
		}
		else
		{
			qDebug() << "Error:" << loader->errorString();
		}

		if(plugin) {
			qDebug("loaded plugin");
			loaders_.insert(plugin->getName(), loader);
		}

		return plugin;
	}

	bool PluginManager::unloadPlugin(const QString& plugin)
	{
		QHash<QString, QPluginLoader*>::iterator it = loaders_.find(plugin);

		if(it != loaders_.end()) {
			it.value()->unload();
			delete it.value();
			loaders_.erase(it);

			return true;
		}

		return false;
	}

	QObject* PluginManager::getPluginInstance(const QString& plugin)
	{
		QHash<QString, QPluginLoader*>::iterator it = loaders_.find(plugin);

		if(it != loaders_.end()) {
			return (it.value()->instance());
		}

		return NULL;
	}

	QObject* PluginManager::getPluginInstance(int pos)
	{
		if(pos < 0 || pos >= getPluginCount()) {
			return NULL;
		}

		QHash<QString, QPluginLoader*>::const_iterator it = loaders_.begin();
		for(int i = 0; i < pos; ++i, ++it) {}

		return it.value()->instance();
	}

	int PluginManager::getPluginCount() const
	{
		return loaders_.size();
	}
}

