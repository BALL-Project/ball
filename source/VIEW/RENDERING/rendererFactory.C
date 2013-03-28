#include <BALL/VIEW/RENDERING/rendererFactory.h>

#include <BALL/PLUGIN/BALLPlugin.h>

#include <BALL/VIEW/RENDERING/rendererPlugin.h>
#include <BALL/solvation.h>

namespace BALL
{
	namespace VIEW
	{
		RendererFactory* RendererFactory::factory_ = 0;
		QMutex RendererFactory::mutex_;

		RendererFactory& RendererFactory::instance()
		{
			//Make the PluginManager creation thread safe
			if (!factory_)
			{
				//Another thread could have taken over control right now
				//so lock a mutex to ensure the PluginManager is created only once.
				mutex_.lock();
				//Check that the manager has not been created by a concurring thread
				if(!factory_)
				{
					factory_ = new RendererFactory();
				}
				mutex_.unlock();
			}

			return *factory_;
		}


		Renderer* RendererFactory::createRenderer(const QString& name) const
		{
			for(std::list<BALLPlugin*>::const_iterator it = running_plugins_.begin(); it != running_plugins_.end(); ++it)
			{
				if((*it)->getName() == name)
				{
					return qobject_cast<RendererPlugin*>(*it)->createRenderer();
				}
			}

			return 0;
		}

		bool RendererFactory::hasRenderer(const QString& name) const
		{
			for(std::list<BALLPlugin*>::const_iterator it = running_plugins_.begin(); it != running_plugins_.end(); ++it)
			{
				if((*it)->getName() == name)
				{
					return true;
				}
			}

			return false;
		}

		std::list<QString> RendererFactory::getAvailableRenderers() const
		{
			std::list<QString> result;

			for(std::list<BALLPlugin*>::const_iterator it = running_plugins_.begin(); it != running_plugins_.end(); ++it)
			{
				result.push_back((*it)->getName());
			}

			return result;
		}

		bool RendererFactory::canHandle(BALLPlugin* plugin) const
		{
			return qobject_cast<RendererPlugin*>(plugin) != 0;
		}

		bool RendererFactory::specificSetup_(BALLPlugin* plugin)
		{
			bool result = plugin->activate();

			if(result) {
				emit rendererAdded(plugin->getName());
			}

			return result;
		}

		bool RendererFactory::specificShutdown_(BALLPlugin* plugin)
		{
			bool result = plugin->deactivate();

			if(result) {
				emit rendererRemoved(plugin->getName());
			}

			return plugin->deactivate();
		}
	}
}

