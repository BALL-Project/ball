#ifndef COMPOSITING_H
#define COMPOSITING_H

#include <Eigen/Geometry>
#include <BALL/MATHS/vector3.h>
#include <renderObjects/RenderObject.h>

#include <ShaderManager.h>

#include <vbo/VBObject.h>

#include <list>
using std::list;

/**
 * @brief Compositing class.
 *
 * Each render path should inherit from this class. It provides the basic functionalites each render
 * path needs and dicates which methods to implement.
 *
 * It provides at least the texture IDs for SSAO and volume rendering as well as the clear color /
 * background color.
 **/
class Compositing
	: public RenderObject
{
		Q_OBJECT
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor
		 *
		 * Additionally to the initialization of the RenderObject constructor, the vbo_ should be created.
		 *
		 **/
		Compositing(Camera* camera);

		/**
		* @brief Destructor
		*
		**/
		virtual ~Compositing();

		/**
		 * @brief Set the texture ID for the SSAO texture.
		 *
		 * @param textureid The SSAO texture to be set.
		 * @return void
		 **/
		virtual void setSSAOTexture(GLuint textureid);

		/**
		 * @brief Set the texture ID for the volume texture.
		 *
		 * @param textureid The volume texture ID to be set.
		 * @return void
		 **/
		virtual void setVolumeTexture(GLuint textureid);

		/**
		 * @brief Initialize the render object.
		 *
		 * Performs a clean up and initializes the render object.
		 * Additionally, the vertex buffer object is initialized.
		 *
		 * @return void
		 **/
		virtual void init();

		/**
		 * @brief Prepare the rendering.
		 *
		 * This includes for example binding framebuffers, clearing buffers or setting blend equations.
		 *
		 *
		 * @param level The level for which the rendering should be preprared. Defaults to OPAQUE.
		 * @return void
		 **/
		virtual void prepare_rendering(RenderLevel level = OPAQUE) = 0;

	public slots:

		/**
		 * @brief Set true if SSAO should be used.
		 *
		 * @param value true, if SSAO should be used.
		 * @return void
		 **/
		virtual void setAO(bool value);

		/**
		 * @brief Set true if volume should be blended.
		 *
		 * @param value true, if volume should be blended.
		 * @return void
		 **/
		virtual void setVolume(bool value);

		/**
		 * @brief Set the clear color.
		 *
		 * The clear color should be the background color. Every color component must be in range [0,1].
		 *
		 * @param r Red channel of the clear color.
		 * @param g Green channel of the clear color.
		 * @param b Blue channel of the clear color.
		 * @param a Alpha channel of the clear color.
		 * @return void
		 **/
		virtual void setClearColor(float r, float g, float b, float a);

	protected:

		/**
		 * @brief Initialize the vertex buffer object.
		 *
		 * @return void
		 **/
		virtual void initVBO_() = 0;

		/**
		 * @brief Performs a clean up.
		 *
		 * Sets up the vbo_.
		 *
		 * @return void
		 **/
		virtual void cleanup_();

		/**
		 * @brief Texture ID for the SSAO texture.
		 **/
		GLuint ssao_texture_;

		/**
		 * @brief Texture ID for the volume texture.
		 **/
		GLuint volume_texture_;

		/**
		 * @brief Vertex buffer object of this render object.
		 **/
		VBObject* vbo_;
};

#endif //COMPOSITING_H