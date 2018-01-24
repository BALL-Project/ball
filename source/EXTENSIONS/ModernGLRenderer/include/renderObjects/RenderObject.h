#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include <GL/glew.h>

#include <Camera.h>
#include <ClippingPlanes.h>
#include <Material.h>
#include <ShaderManager.h>

#include <BALL/VIEW/KERNEL/geometricObject.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <Eigen/Geometry>


#define MAX_NUMBER_OF_LIGHTS 8

namespace BALL
{
	class System;
}

enum RenderLevel
{
	OPAQUE,
	TRANSPARENCY_INIT,
	TRANSPARENCY_PEEL,
	TRANSPARENCY_BLEND,
	TRANSPARENCY_FINAL
};

/**
 * @brief Renderobject.
 *
 * TODO some more description here.
 *
 * The init() method needs to be called to get an fully initialized object.
 **/
class RenderObject: public QObject
{
		Q_OBJECT

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor
		 *
		 * Gets instances of the shadermanager and the material.
		 * Registers signals and slots.
		 *
		 **/
		RenderObject(Camera* camera);

		/**
		 * @brief Destructor
		 *
		 * Calls the cleanup_() method.
		 *
		 **/
		virtual ~RenderObject();

		/**
		 * @brief Starting point for rendering.
		 * This method should be called whenever the window needs to be redrawn.
		 *
		 * The level can e.g. be used for different steps of transparency rendering.
		 *
		 * @param level The level the objects should be rendered with.
		 * @return void
		 **/
		virtual void render(RenderLevel level = OPAQUE) = 0;

		/**
		 * @brief Initializes the RenderObject.
		 *
		 * Performs an cleanup and initializes the GeometricObject.
		 *
		 * @return void
		 **/
		virtual void init();

		virtual bool isInitialized();

		/**
		 * @brief Setup the RenderObject.
		 *
		 * Performs an cleanup and initializes the GeometricObject.
		 *
		 * Takes a list of GeometricObjects and sends them to the GPU for later rendering.
		 *
		 * @param  list List of GeometricObjects to render.
		 * @return void
		 **/
		virtual void init(std::list<const BALL::VIEW::GeometricObject*>& list);

		/**
		 * @brief Set the camera.
		 *
		 * @param camera The camera to be set.
		 * @return void
		 **/
		virtual void setCamera(Camera* camera);

		/**
		 * @brief Set the drawing mode.
		 *
		 * Set the drawing mode the representation should be rendered with.
		 *
		 * @param drawing_mode The drawing mode the representation should be rendered with.
		 * @return void
		 **/
		virtual void setDrawingMode(BALL::VIEW::DrawingMode drawing_mode);

		/**
		 * @brief Return the drawing mode.
		 *
		 * Return the drawing mode the representation should be rendered with.
		 *
		 * @return BALL::VIEW::DrawingMode The drawing mode the representation should be rendered with.
		 **/
		virtual BALL::VIEW::DrawingMode getDrawingMode();

		/**
		 * @brief Return the openGL polygon mode.
		 *
		 * @return GLuint The drawing mode the representation should be rendered with.
		 **/
		virtual GLuint getGLDrawingMode();

		/**
		 * @brief Set the clipability of the renderobject.
		 *
		 * @param clipable True, if object is clipable.
		 * @return void
		 **/
		virtual void setClipable(bool clipable = true);

		/**
		 * @brief Set the lightsources for rendering.
		 *
		 * Querries the lightsources from the stage and set them up for the renderer.
		 * Directional, point and ambient lightsources are supported (color and intensity of ambient
		 * lightsources is cummulated).
		 *
		 * Up to <code>MAX_NUMBER_OF_LIGHTS</code> (normally 8) are supported.
		 *
		 * @param stage The stage we render on.
		 * @param reset_all Should the old lightsources be reset?
		 * @return void
		 **/
		void setLights(const BALL::VIEW::Stage* stage, bool reset_all);

	public slots:
		/**
		 * @brief Set the view matrix.
		 *
		 * @param view_matrix The view matrix to be set.
		 * @return void
		 **/
		virtual void setView(const Eigen::Affine3f& view_matrix);

		/**
		 * @brief Set the inverse view matrix.
		 *
		 * @param view_matrix The inverse view matrix to be set.
		 * @return void
		 **/
		virtual void setViewInverse(const Eigen::Affine3f& view_matrix_inv);

		/**
		 * @brief Set the projection matrix.
		 *
		 * @param projection_matrix The projection matrix to be set.
		 * @return void
		 **/
		virtual void setProjection(const Eigen::Matrix4f& projection_matrix);

		/**
		 * @brief Set the inverse projection matrix.
		 *
		 * @param projection_matrix_inv The inverse projection matrix to be set.
		 * @return void
		 **/
		virtual void setProjectionInverse(const Eigen::Matrix4f& projection_matrix_inv);

		/**
		 * @brief Set the transpose view projection matrix.
		 *
		 * @param view_projection_matrix_transp The transpose view projection matrix to be set.
		 * @return void
		 **/
		virtual void setViewProjectionTranspose(const Eigen::Matrix4f& view_projection_matrix_transp);

		/**
		 * @brief Set the viewport.
		 *
		 * @param w Width of the viewport.
		 * @param h Height of the viewport.
		 * @param x Horizontal position of the lower left corner of the viewport rectangle.
		 * @param y Vertical position of the lower left corner of the viewport rectangle.
		 * @return void
		 **/
		virtual void setViewport(unsigned int w, unsigned int h, unsigned int x=0, unsigned int y=0 );

		/**
		 * @brief Set the frustum.
		 *
		 * @param left Coordinate for the left vertical clipping plane.
		 * @param right Coordinate for the right vertical clipping plane.
		 * @param bottom Coordinate for the bottom horizontal clipping plane.
		 * @param top Coordinate for the top horizontal clipping plane.
		 * @return void
		 **/
		virtual void setFrustum(float left, float right, float bottom, float top );

		/**
		 * @brief Set the near distance.
		 *
		 * @param near Distance to the near plane to be set
		 * @return void
		 **/
		virtual void setNearDistance(float near);

		/**
		 * @brief Set the shininess.
		 *
		 * @param shininess The shininess to be set.
		 * @return void
		 **/
		virtual void setMaterialShininess(const float shininess);

		/**
		 * @brief Set the diffuse material.
		 *
		 * @param r Red channel of diffuse material in range [0,1].
		 * @param g Green channel of diffuse material in range [0,1].
		 * @param b Blue channel of diffuse material in range [0,1].
		 * @param a Alpha channel of diffuse material in range [0,1].
		 * @return void
		 **/
		virtual void setMaterialDiffuse(float r, float g, float b, float a);

		/**
		 * @brief Set the ambient material.
		 *
		 * @param r Red channel of ambient material in range [0,1].
		 * @param g Green channel of ambient material in range [0,1].
		 * @param b Blue channel of ambient material in range [0,1].
		 * @param a Alpha channel of ambient material in range [0,1].
		 * @return void
		 **/
		virtual void setMaterialAmbient(float r, float g, float b, float a);

		/**
		 * @brief Set the specular material.
		 *
		 * @param r Red channel of specular material in range [0,1].
		 * @param g Green channel of specular material in range [0,1].
		 * @param b Blue channel of specular material in range [0,1].
		 * @param a Alpha channel of specular material in range [0,1].
		 * @return void
		 **/
		virtual void setMaterialSpecular(float r, float g, float b, float a);

		/**
		 * @brief Set the clipping planes and calls clippingPlanesChanged().
		 *
		 * The renderer only supports MAX_CLIPPING_PLANES. Thus, only the first MAX_CLIPPING_PLANES are
		 * set. Components a, b, c, d of each clipping plane will be normalized.
		 *
		 * @param clipping_planes The clipping planes to be set.
		 * @return bool Returns the success of this operation.
		 **/
		bool setClippingPlanes(const std::vector< Eigen::Vector4f>& clipping_planes);

		/**
		 * @brief Set a clipping plane and calls clippingPlanesChanged().
		 *
		 * The renderer only supports MAX_CLIPPING_PLANES. Thus, only the first MAX_CLIPPING_PLANES are
		 * set. Components a, b, c, d of each clipping plane will be normalized.
		 *
		 * @param clipping_plane The clipping plane to be set.
		 * @return bool Returns the success of this operation.
		 **/
		bool setClippingPlane(const Eigen::Vector4f& clipping_plane);

		bool clearClippingPlanes();

		/**
		 * @brief Notifies that the clipping planes have changed.
		 *
		 * @return void
		 **/
		virtual void clippingPlanesChanged();

		/**
		 * @brief Set the clip range.
		 *
		 * @param clipRange The clip range to be set.
		 * @return void
		 **/
		virtual void setClipRange(ClipRange clipRange);

		void setDepthBlenderTex(GLuint tex);

		void setFrontBlenderTex(GLuint tex);

		void setBackBlenderTex(GLuint tex);

	protected:

		/**
		 * @brief Initializes the system.
		 *
		 * Only initializes the system when the initialized_ flag is not already set to true.
		 *
		 * @return void
		 **/
		virtual void initialize_();

		/**
		 * @brief Performs an cleanup.
		 *
		 * Deletes all resources.
		 * Last active shader will be released.
		 * After calling cleanup_() the initialized_ flag is set to false.
		 *
		 * @return void
		 **/
		virtual void cleanup_();

		/**
		 * @brief Register at camera for all data changes needed by this class.
		 *
		 * @return void
		 **/
		virtual void register_() {};

		/**
		 * @brief Unregister at camera for all data changes needed by this class.
		 *
		 * @return void
		 **/
		virtual void unregister_() {};

		/**
		 * @brief Compiles the shader programs.
		 *
		 * The shader program are compiled and linked.
		 *
		 * @return void
		 **/
		virtual void compileShaderPrograms_() = 0;

		/**
		 * @brief Initializes the shader programs.
		 *
		 * The uniform and attribut locations are stored.
		 *
		 * @return void
		 **/
		virtual void initShaderPrograms_() = 0;

		/**
		 * @brief Setup the Vertex Buffer Object (VBO).
		 *
		 * @param list List of GeometricObjects to add to the VBO.
		 * @return void
		 **/
		virtual void initVBO_(std::list<const BALL::VIEW::GeometricObject*>& list) = 0;

		void setUniform4fv_(QString uniform_name, int count, float* v1);

		void setUniform1f_(QString uniform_name, float v1);

		void setUniform2f_(QString uniform_name, float v1, float v2);

		void setUniform3f_(QString uniform_name, float v1, float v2, float v3);

		void setUniform4f_(QString uniform_name, float v1, float v2, float v3, float v4);

		void setUniform1i_(QString uniform_name, int v1);

		void setUniform1b_(QString uniform_name, bool v1);

		void setUniformMatrix4fv_(QString uniform_name, float* v1);

		void setLights_(const BALL::VIEW::Stage* stage, bool reset_all, const char* shader_name);

		/**
		 * @brief Is the renderer initialized?
		 *
		 * Is set to true if RenderObject is initialized.
		 **/
		bool initialized_;

		/**
		 * @brief The shader manager.
		 *
		 * Manages the shader programs.
		 **/
		ShaderManager* shader_manager_;

		/**
		 * @brief The material.
		 *
		 * Manages signals and slots for material changes.
		 **/
		Material* material_;

		/**
		 * @brief Name of the shader used for this render object.
		 **/
		const char* shader_name_; //TODO unused

		/**
		 * @brief Stores the names of the shaders.
		 **/
		QVector<const char*> shaders_;

		/**
		 * @brief Stores and manages the clipping planes.
		 **/
		ClippingPlanes clipping_planes_;

		/**
		 * @brief Pointer to the camera object.
		 **/
		Camera* camera_;

		BALL::VIEW::DrawingMode drawing_mode_;

		/**
		 * @brief Is the object clipable?
		 **/
		bool clipable_;

		GLuint depth_tex_idx;

		GLuint front_blender_tex_idx;

		GLuint back_blender_tex_idx;
};

class RenderObjectList
{
	public:
		void clear()
		{
			while(!render_objects.empty())
			{
				delete render_objects.front();
				render_objects.pop_front();
			}
		};

		bool isDisabled;

		std::list<RenderObject*> render_objects;

		void clearClippingPlanes()
		{
			std::list<RenderObject*>::iterator it;

			for(it = render_objects.begin(); it != render_objects.end(); it++)
			{
				(**it).clearClippingPlanes();
			}
		}

		void addClippingPlane(const Eigen::Vector4f& clipping_plane)
		{
			std::list<RenderObject*>::iterator it;

			for(it = render_objects.begin(); it != render_objects.end(); it++)
			{
				(**it).setClippingPlane(clipping_plane);
			}
		}
};


#endif //RENDEROBJECT_H