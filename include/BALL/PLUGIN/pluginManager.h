#ifndef BALL_PLUGIN_PLUGINMANAGER_H
#define BALL_PLUGIN_PLUGINMANAGER_H

#include <QtCore/QString>
#include <QtCore/QHash>

class QPluginLoader;
class QObject;

namespace BALL
{
	class BALLPlugin;

	class PluginManager
	{
		public:
			~PluginManager();

			static PluginManager& instance();

			void setPluginDirectory(const QString& dir);

			BALLPlugin* loadPlugin(const QString& plugin_name);
			bool unloadPlugin(const QString& plugin);

			QObject* getPluginInstance(const QString& plugin);
			QObject* getPluginInstance(int pos);

			int getPluginCount() const;

		private:
			PluginManager();
			PluginManager(const PluginManager&);
			const PluginManager& operator=(const PluginManager&);

			QString plugin_dir_;
			QHash<QString, QPluginLoader*> loaders_;

			static PluginManager* manager_;
	};
}

#endif //BALL_PLUGIN_PLUGINMANAGER_H

