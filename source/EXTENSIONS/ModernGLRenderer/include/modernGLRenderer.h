#ifndef MODERN_GL_RENDERER_H
#define MODERN_GL_RENDERER_H

#include <camera.h>
#include <material.h>
#include <modernGLRenderSetup.h>
#include <renderObjects/clippingPlane.h>
#include <renderObjects/lightSources.h>
#include <renderObjects/opaque.h>
#include <renderObjects/SSAO.h>
#include <renderObjects/transparent.h>
#include <renderObjects/volume.h>

#include <BALL/VIEW/RENDERING/RENDERERS/renderer.h>

#include <Eigen/Geometry>


namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class Stage;
		class RenderTarget;
	}
}

/**
 * @brief Renderer class.
 *
 * The renderer uses deferred lightning.
 *
 * After initializing the renderer by calling initialize() and adding objects to be rendered. The
 * render() method should be called to redraw the window.
 **/


class  ModernGLRenderer
	: public QObject,
	public BALL::VIEW::Renderer
{
		Q_OBJECT
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor
		 * Note: The renderer needs to be initialized before usage by calling init().
		 *
		 **/
		ModernGLRenderer();

		/**
		 * @brief Destructor
		 *
		 * Calls the cleanup_() method to delets the shader, shader programs, buffers, and textures.
		 * After calling the destructor the initialized_ flag is set to false.
		 *
		 **/
		virtual ~ModernGLRenderer();

		/**
		 * @brief Initializes the renderer.
		 *
		 * Only initializes the renderer when the initialized_ flag is not already set to true.
		 * It performs the OpenGL generation calls for textures, framebuffers as well as
		 * the renderbuffer. The initialized_ flag is set to true.
		 *
		 * @param stage The stage used for initialization
		 * @return bool
		 **/
		virtual bool init(const BALL::VIEW::Stage& stage, float width, float height);

		virtual bool isInitialized() const;

		/**
		 * @brief Starting point for rendering.
		 * This method should be called whenever the window needs to be redrawn. It starts rendering the
		 * scene and the opaque.
		 *
		 * @return void
		 **/
		void render();

		/** Buffer a Representation for later rendering.
		 */
		virtual void bufferRepresentation(const BALL::VIEW::Representation& rep);

		/** Remove a representation from the buffer.
		 */
		virtual void removeRepresentation(const BALL::VIEW::Representation& rep);

		/**
		 * @brief Set the view matrix.
		 *
		 * @param view_matrix The view matrix to be set.
		 * @return void
		 **/
		void setView(const Eigen::Affine3f& view_matrix);

		/**
		 * @brief Set the projection matrix.
		 *
		 * @param matrix_type The projection matrix type to use.
		 * @return void
		 **/
		void setProjection(ProjectionMatrixType matrix_type);

		/**
		 * @brief Sets the Window size.
		 *
		 * Should be called whenever the size of the window changes.
		 *
		 * @param width The new width.
		 * @param height The new height.
		 * @return void
		 **/
		virtual void setSize(float width, float height);

		/**
		 * @brief Sets the near distance of the view frustum.
		 *
		 * @param near_dist The near distance to be set.
		 * @return void
		 **/
		void setNearDist(float near_dist);

		/**
		 * @brief Sets the far distance of the view frustum.
		 *
		 * @param far_dist The far distance to be set.
		 * @return void
		 **/
		void setFarDist(float far_dist);

		/**
		 * @brief Returns the far distance of the view frustum.
		 *
		 * @return float The far distance.
		 **/
		float getNearDist();

		/**
		 * @brief Returns the near distance of the view frustum.
		 *
		 * @return float The near distance.
		 **/
		float getFarDist();

		void setZoom(float zoom);

		/** This function renders into the buffer of the RenderTarget which has to be
		 *  made current before this function is called. Buffers are not automatically
		 *  swapped afterwards!
		 **/
		virtual void renderToBuffer(BALL::VIEW::RenderTarget* renderTarget);

		virtual void renderToBufferImpl(boost::shared_ptr<BALL::VIEW::FrameBuffer> /*buffer*/)
		{
			renderToBuffer(0);
		}

		virtual bool supports(const BALL::VIEW::PixelFormat&) const
		{
			return true;
		}
		virtual bool supports(const BALL::VIEW::FrameBufferFormat&) const
		{
			return true;
		}
		virtual void formatUpdated() {}

		virtual void getFrustum(float& near_f, float& far_f, float& left_f, float& right_f,
		                        float& top_f,  float& bottom_f);

		virtual void updateBackgroundColor();

		virtual boost::shared_ptr<BALL::VIEW::RenderSetup> createRenderSetup(BALL::VIEW::RenderTarget* target, BALL::VIEW::Scene* scene);

		virtual void updateCamera(const BALL::VIEW::Camera* camera);

		virtual void setLights(bool reset_all = false);

		/**
		 * @brief Update materials from stage.
		 *
		 * Take the materials stored in stage and send it to all representations if the material has
		 * changed since the last call.
		 *
		 * @return void
		 **/
		virtual void updateMaterialFromStage();

		/**
		 * @brief Update material for given representation.
		 *
		 * Update material for given representation. However, currently all representations have the same
		 * material.
		 *
		 * @param  rep The representation to update.
		 * @return void
		 **/
		virtual void updateMaterialForRepresentation(const BALL::VIEW::Representation* /*rep*/);

	signals:

		void useAOChanged(bool useAO);

		void initialized();

	public slots:
		void ssaoParametersChanged(SSAOParameters ssao_parameters);
		void transparentParametersChanged(TransparentParameters tp);

	protected:

		/**
		 * @brief Render the scene as oqaque objects.
		 *
		 * This renderpath should be used when there is no transparent object in the scene
		 * (except volume rendering).
		 *
		 * @return void
		 **/
		void renderOpaque_();

		/**
		 * @brief Render the scene as transparent objects.
		 *
		 * This renderpath should be used when there is at least one transparent object in the scene
		 * (except volume rendering).
		 *
		 * @return void
		 **/
		void renderTransparent_();

		/**
		 * @brief Render the additional effects.
		 *
		 * This renderpath should be called to render effects like ambient occlusion. It requires that
		 * the textures storing the normal and the depth information are set up correctly.
		 *
		 * @return void
		 **/
		void renderEffects_();

		/**
		 * @brief Renders all objects stored in object_ with the given RenderLevel.
		 *
		 * @param level The level the objects should be rendered with.
		 * @return void
		 **/
		void renderRenderObjects_(RenderLevel level);

		/**
		 * @brief Renders all geometry with the given RenderLevel.
		 *
		 * This includes all objects stored in obejct_, clippingplanes and lightsources (if enabled).
		 *
		 * @param level The level the objects should be rendered with.
		 * @return void
		 **/
		void renderGeometry_(RenderLevel level);

		void setDepthTex_(GLuint tex_id);

		void setFrontBlenderTex_(GLuint tex_id);

		/**
		 * @brief Connect signals and slots.
		 *
		 * Iterates over all RenderObjects and connects their signals and slots. This includes
		 *  - use ao
		 * changes.
		 *
		 * @return void
		 **/
		void connectSignalsAndSlots_();

		/**
		 * @brief Add the clipping planes to the representations.
		 *
		 * First, all clipping planes of all representations are reset.
		 * Then, clipping planes are set for all representations they belong to.
		 *
		 * @param active_planes Clipping planes to be set.
		 * @return void
		 **/
		void setActiveClippingPlanes_( std::vector<BALL::VIEW::ClippingPlane*>& active_planes );

		/**
		 * @brief Querries the clipping planes.
		 *
		 * Pointers to all active clipping planes will be stored in active_planes.
		 * Pointers to all inactive clipping planes will be stored in inactive_planes.
		 *
		 * @param active_planes Pointers to the active clipping planes.
		 * @param inactive_planes Pointers to the inactive clipping planes.
		 * @return void
		 **/
		void getClippingPlanes_(std::vector<BALL::VIEW::ClippingPlane*>* active_planes, std::vector<BALL::VIEW::ClippingPlane*>* inactive_planes );

		/**
		 * @brief Updates the RenderObjects which hold the geometry for rendering the clipping planes.
		 *
		 * @param inactive_planes The clipping planes to be rendered.
		 * @return void
		 **/
		void updateClippingPlanes_(std::vector<BALL::VIEW::ClippingPlane*>& inactive_planes);

		/**
		 * @brief Checks if support for rendering transparent objects is available.
		 *
		 * Support for rendering transparent objects is available if all neccessary extensions are
		 * available.
		 *
		 * @return bool True, if support is available.
		 **/
		bool checkTransparencySupport_();

		/**
		 * @brief Tests if a transparent object is buffered and updates has_transparency_.
		 *
		 * @return void
		 **/
		void updateHasTransparency_();

		/**
		 * @brief Is the renderer initialized?
		 *
		 * Is set to true if the textures and buffers are initialized.
		 **/
		bool initialized_;

		/**
		 * @brief Pointer to opaque object which finally computes the lightning of the scene.
		 **/
		Opaque* opaque_;

		/**
		 * @brief Pointer to transparent object which finally computes the lightning of the scene.
		 **/
		Transparent* transparent_;

		/**
		 * @brief Pointer to SSAO object.
		 **/
		SSAO* ssao_;

		/**
		 * @brief Pointer to volume object.
		 **/
		Volume* volume_;

		/**
		 * @brief Pointer to the clipping plane RenderObjects.
		 **/
		ClippingPlane* clipping_plane_;

		/**
		 * @brief Manages the shaders.
		 **/
		ShaderManager* shader_manager_;

		LightSources* light_sources_;

		/**
		 * @brief The camera used for this renderer.
		 **/
		Camera* camera_;

		/**
		 * @brief The material used in this renderer.
		 **/
		Material* material_;

		BALL::HashMap<BALL::VIEW::Representation const*, RenderObjectList > objects_;

		BALL::VIEW::Representation const* volume_rep_;

		/**
		 * @brief True, if ambient occlusion should be used.
		 **/
		bool useAO_;

		/**
		 * @brief True, if at least one transparent object is buffered
		 **/
		bool has_transparency_;
};

#endif //MODERN_GL_RENDERER_H
