#include <BALL/VIEW/PLUGIN/pluginManager.h>

#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/INPUT/inputPlugin.h>

namespace BALL
{
	namespace VIEW
	{
		
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
			static PluginManager manager;

			return manager;
		}

		void PluginManager::setPluginDirectory(const QString& dir)
		{
				plugin_dir_ = dir;
		 		QDir plugin_dir(plugin_dir_, "plugin*.so", QDir::Name | QDir::IgnoreCase, QDir::Files);

				foreach(QString it, plugin_dir.entryList()) {
					loadPlugin(plugin_dir.absoluteFilePath(it));
				}
		}

		VIEWPlugin* PluginManager::loadPlugin(const QString& plugin_name)
		{
			VIEWPlugin* plugin = 0;
			QPluginLoader* loader = new QPluginLoader(plugin_name);

			qDebug() << "Trying to load plugin: " << plugin_name;
			plugin = qobject_cast<VIEWPlugin*>(loader->instance());

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

		VIEWPlugin* PluginManager::getPluginInstance(const QString& plugin)
		{
			QHash<QString, QPluginLoader*>::iterator it = loaders_.find(plugin);
			
			if(it != loaders_.end()) {
				//static_cast should be ok here as we already checked the plugin 
				//in loadPlugin()
				return qobject_cast<VIEWPlugin*>(it.value()->instance());
			}

			return NULL;
		}

		VIEWPlugin* PluginManager::getPluginInstance(int pos)
		{
			if(pos < 0 || pos >= getPluginCount()) {
				return NULL;
			}

			QHash<QString, QPluginLoader*>::const_iterator it = loaders_.begin();
			for(int i = 0; i < pos; ++i, ++it) {}

			return qobject_cast<VIEWPlugin*>(it.value()->instance());
		}

		int PluginManager::getPluginCount() const
		{
			return loaders_.size();
		}

	}
}

