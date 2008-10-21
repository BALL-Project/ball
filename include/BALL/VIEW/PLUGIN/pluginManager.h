#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QtCore/QHash>

class QPluginLoader;

namespace BALL
{
	namespace VIEW
	{

		class VIEWPlugin;

		class PluginManager 
		{
			public:
				~PluginManager();

				static PluginManager& instance();

				void setPluginDirectory(const QString& dir);

				VIEWPlugin* loadPlugin(const QString& plugin_name);
				bool unloadPlugin(const QString& plugin); 

				VIEWPlugin* getPluginInstance(const QString& plugin);
				VIEWPlugin* getPluginInstance(int pos);

				int getPluginCount() const;

			private:
				PluginManager();
				PluginManager(const PluginManager&);
				const PluginManager& operator=(const PluginManager&);

				QString plugin_dir_;
				QHash<QString, QPluginLoader*> loaders_;
		};
	}
}

#endif //PLUGINMANAGER_H

