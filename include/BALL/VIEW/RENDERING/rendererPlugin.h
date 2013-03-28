#ifndef BALL_VIEW_RENDERING_RENDERERPLUGIN_H
#define BALL_VIEW_RENDERING_RENDERERPLUGIN_H

#include <QtCore/QtPlugin>

namespace BALL
{
	namespace VIEW
	{
		class Renderer;

		class RendererPlugin
		{
			public:
				virtual ~RendererPlugin() {}

				/**
				 * Create a new Renderer instance
				 *
				 * @return A pointer to the renderer
				 */
				virtual Renderer* createRenderer() = 0;
		};
	}
}

Q_DECLARE_INTERFACE(BALL::VIEW::RendererPlugin, "org.ball-project.Plugin.RendererPlugin/1.0")

#endif //BALL_VIEW_RENDERING_RENDERERPLUGIN_H