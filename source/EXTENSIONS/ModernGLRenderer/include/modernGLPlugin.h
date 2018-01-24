#ifndef MODERNGLPLUGIN_H
#define MODERNGLPLUGIN_H

#include <modernGLRenderer.h>

#include <BALL/PLUGIN/BALLPlugin.h>
#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>
#include <BALL/VIEW/RENDERING/rendererPlugin.h>

#include <QtCore/QObject>
#include <QtGui/QPixmap>


namespace BALL
{
	namespace VIEW
	{
		class Renderer;
	}
}

class ModernGLConfigDialog;


/**
 * @brief Plugin for the ModernGLRenderer.
 **/
class ModernGLPlugin : public QObject, public BALL::BALLPlugin, public BALL::VIEW::VIEWPlugin, public BALL::VIEW::RendererPlugin
{
		Q_OBJECT
		Q_INTERFACES(BALL::BALLPlugin BALL::VIEW::VIEWPlugin BALL::VIEW::RendererPlugin)
		Q_PLUGIN_METADATA(IID "org.ball-project.Plugin.Renderer.ModernGLRenderer")

	public:
		ModernGLPlugin();
		virtual ~ModernGLPlugin();

		/**
		 * @brief Return the icon of the renderer.
		 *
		 * @return QPixmap The icon of the renderer.
		 **/
		const QPixmap* getIcon() const;

		/**
		 * @brief Return the name of the renderer.
		 *
		 * @return QString The name of the renderer.
		 **/
		QString getName() const;

		/**
		 * @brief Return the name of the graphics card.
		 *
		 * @return QString The name of the graphics card.
		 **/
		QString getGraphicsCard() const;

		/**
		 * @brief Return the description of the renderer.
		 *
		 * @return QString The description of the renderer.
		 **/
		QString getDescription() const;

		/**
		 * @brief Return the version of the renderer.
		 *
		 * @return QString The version of the renderer.
		 **/
		QString getVersion() const;

		/**
		 * @brief Return the vendor of the renderer.
		 *
		 * @return QString The vendor of the renderer.
		 **/
		QString getVendor() const;

		/**
		 * @brief Return the config dialog for the renderer.
		 *
		 * Creates a new config dialog if this has not be done previously.
		 *
		 * @return :VIEW::ConfigDialog* The config dialog for the renderer.
		 **/
		BALL::VIEW::ConfigDialog* getConfigDialog();

		/**
		 * @brief Creates a new renderer and returns it.
		 *
		 * Creates a new renderer and set the config dialog for it.
		 *
		 * @return :VIEW::Renderer* The newly created renderer.
		 **/
		BALL::VIEW::Renderer* createRenderer();

		/**
		 * @brief Returns true if the renderer is active.
		 *
		 * @return bool True if the renderer is active.
		 **/
		bool isActive()
		{
			return is_active_;
		}

		/**
		 * @brief Activates the renderer.
		 *
		 * @return bool Returns the new active state of the renderer (always true).
		 **/
		bool activate()
		{
			return is_active_ = true;
		}

		/**
		 * @brief Deactivates the renderer.
		 *
		 * @return bool Returns the new active state of the renderer (always false).
		 **/
		bool deactivate()
		{
			is_active_ = false;
			return true;
		}

	public slots:
		void updateLabels();

	private:
		/**
		  * @brief The state of the renderer. True if renderer is active.
		  **/
		bool is_active_;

		/**
		 * @brief Stores the icon of the renderer.
		 **/
		QPixmap icon_;
		/**
		 * @brief The config dialog used for this renderer.
		 **/

		ModernGLConfigDialog* config_dialog_;

		/**
		 * @brief The renderer used for this plugin.
		 **/
		ModernGLRenderer* renderer_;
};

#endif //GLPLUGIN_H
