#ifndef BALL_VIEW_PLUGIN_INPUTDEVPLUGINHANDLER_H
#define BALL_VIEW_PLUGIN_INPUTDEVPLUGINHANDLER_H

#ifndef BALL_PLUGIN_PLUGINHANDLER_H
# include <BALL/PLUGIN/pluginHandler.h>
#endif

namespace BALL
{
	class BALLPlugin;

	namespace VIEW
	{

		/**
		 * See PluginHandler for detailed documentation
		 */
		class BALL_VIEW_EXPORT InputDevPluginHandler : public PluginHandler
		{
			public:
				virtual bool canHandle(BALLPlugin* plugin) const;

			protected:
				virtual bool specificSetup_(BALLPlugin* plugin);
				virtual bool specificShutdown_(BALLPlugin* plugin);
		};

	}
}

#endif //BALL_VIEW_PLUGIN_INPUTDEVPLUGINHANDLER_H

