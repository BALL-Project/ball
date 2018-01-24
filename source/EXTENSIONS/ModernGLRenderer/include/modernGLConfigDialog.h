#ifndef MODERNGL_CONFIG_DIALOG_H
#define MODERNGL_CONFIG_DIALOG_H

#include <modernGLRenderer.h>
#include <renderObjects/SSAO.h>
#include <renderObjects/transparent.h>

#include <BALL/VIEW/PLUGIN/VIEWPlugin.h>


class ModernGLRenderer;

namespace Ui
{
	class OpenGLSettings;
}

/**
 * @brief Config Dialog for the ModernGLRenderer.
 **/
class ModernGLConfigDialog : public BALL::VIEW::ConfigDialog
{
		Q_OBJECT

	public:
		/**
		 * @brief Constructor
		 *
		 * @param parent ... The parent widget. Default is 0.
		 * @param f ... WindowFlags. Default is 0.
		 **/
		ModernGLConfigDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

		/**
		 * @brief Destructor
		 *
		 **/
		virtual ~ModernGLConfigDialog();

		/**
		 * @brief Set the version of the renderer.
		 *
		 * @param version The version of the renderer.
		 * @return void
		 **/
		virtual void setVersion(QString version);
		/**
		 * @brief Set the name of the graphics card.
		 *
		 * @param name The name of the graphics card.
		 * @return void
		 **/
		virtual void setGraphicsCard(QString graphics_card);
		/**
		 * @brief Set the desciption for the renderer.
		 *
		 * @param description Some text describing the renderer.
		 * @return void
		 **/
		virtual void setDescription(QString description);
		/**
		 * @brief Set the name of the vendor of the renderer.
		 *
		 * @param vendor The name of the vendor.
		 * @return void
		 **/
		virtual void setVendor(QString vendor);

		/**
		 * @brief Set the renderer used for this config dialog.
		 *
		 * @param renderer The renderer used.
		 * @return void
		 **/
		virtual void setRenderer(ModernGLRenderer* renderer);

		/**
		 * @brief Returns the SSAO parameters created by this dialog.
		 *
		 * @return SSAOParameters The created SSAO parameters.
		 **/
		virtual SSAOParameters getSSAOParameters();

		/**
		 * @brief Returns the use transparent parameters created by this dialog.
		 *
		 * @return TransparentParameters The created transparent parameters.
		 **/
		virtual TransparentParameters getTransparentParameters();

		/**
		 * @brief Accepts the current configuration.
		 *
		 * It emits the new SSAO parameters.
		 *
		 * @return void
		 **/
		virtual void storeValues();

		/**
		 * Reimplemented from @href PreferencesEntry
		 *
		 * @copydoc PreferencesEntry::readPreferencesEntries
		 */
		virtual void readPreferenceEntries(const BALL::INIFile& ini);

	private slots:
		/**
		 * @brief Updates the "sample radius" label.
		 *
		 * @return void
		 **/

		virtual void sampleRadiusSliderChanged();
		/**
		 * @brief Updates the "intensity" label.
		 *
		 * @return void
		 **/
		virtual void intensitySliderChanged();

		/**
		 * @brief Updates the "scale" label.
		 *
		 * @return void
		 **/
		virtual void scaleSliderChanged();

		/**
		 * @brief Updates the "bias" label.
		 *
		 * @return void
		 **/
		virtual void biasSliderChanged();
		/**
		 * @brief Updates the "downsampling" label.
		 *
		 * @return void
		 **/
		virtual void downsamplingSliderChanged();

		/**
		 * @brief Updates the "numPasses" label.
		 *
		 * @return void
		 **/
		virtual void numPassesSliderChanged();

	signals:
		/**
		 * @brief Signal for changed SSO parameters.
		 *
		 * This signal can emitted when the SSAO parameters have changed.
		 *
		 * @param ssaoParameters The new parameters for SSAO.
		 * @return void
		 **/
		void ssaoParametersChanged(SSAOParameters ssaoParameters);

		/**
		 * @brief Signal for changed transparent parameters.
		 *
		 * This signal can emitted when the transparent parameters have changed.
		 *
		 * @param transparentParameters The new parameters for transparency.
		 * @return void
		 **/
		void transparentParametersChanged(TransparentParameters transparentParameters);


	private:

		Ui::OpenGLSettings* ui_;

		/**
		 * @brief The renderer used for this dialog.
		 **/
		const ModernGLRenderer* renderer_;
};

#endif // MODERNGL_CONFIG_DIALOG_H