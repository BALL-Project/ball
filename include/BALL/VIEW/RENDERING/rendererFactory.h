#ifndef BALL_VIEW_RENDERING_RENDERERFACTORY_H
#define BALL_VIEW_RENDERING_RENDERERFACTORY_H

#ifndef BALL_PLUGIN_PLUGINHANDLER_H
# include <BALL/PLUGIN/pluginHandler.h>
#endif

#include <boost/shared_ptr.hpp>
#include <QtCore/QObject>
#include <QtCore/QMutex>

namespace BALL
{
	class BALLPlugin;

	namespace VIEW
	{
		class Renderer;

		/**
		 * See PluginHandler for detailed documentation
		 */
		class BALL_VIEW_EXPORT RendererFactory : public QObject, public PluginHandler
		{
			Q_OBJECT

			public:
				static RendererFactory& instance();
				virtual Renderer* createRenderer(const QString& name) const;
				virtual bool hasRenderer(const QString& name) const;

				// Inherited from PluginHandler
				virtual bool canHandle(BALLPlugin* plugin) const;
				virtual std::list<QString> getAvailableRenderers() const;

			signals:
				void rendererRemoved(QString name);
				void rendererAdded(QString name);

			protected:
				// Inherited from PluginHandler
				virtual bool specificSetup_(BALLPlugin* plugin);
				virtual bool specificShutdown_(BALLPlugin* plugin);

			private:
				RendererFactory() {};
				RendererFactory(const RendererFactory&);
				RendererFactory& operator=(const RendererFactory&);
				static RendererFactory* factory_;
				static QMutex mutex_;
		};

	}
}

#endif //BALL_VIEW_RENDERING_RENDERERFACTORY_H