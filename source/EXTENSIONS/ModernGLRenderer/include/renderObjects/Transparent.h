#ifndef TRANSPARENT_H
#define TRANSPARENT_H

#include <renderObjects/Compositing.h>


namespace BALL
{
	class System;
}

struct TransparentParameters
{
	bool useOQ;
	unsigned int numPasses;
};

class Transparent
	: public Compositing
{
		Q_OBJECT
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor
		 * The renderer needs to be initialized before usage by calling setup().
		 * @param camera The camera of the renderer.
		 *
		 **/
		Transparent(Camera* camera);

		virtual void prepare_rendering(RenderLevel level);

		/**
		 * @brief Starting point for rendering.
		 * This method should be called whenever the window needs to be redrawn.
		 * The transparent shader program is used.
		 *
		 * @return void
		 **/
		void render(RenderLevel level = OPAQUE);

		/**
		 * @brief ...
		 *
		 * @param pass ...
		 * @param level ... Defaults to RenderLevel::OPAQUE.
		 * @return bool True if sample count > 0
		 **/
		bool render(unsigned int pass, RenderLevel level = OPAQUE);

		/**
		 * @brief Setup the render targets.
		 *
		 * The textures for diffuse and specular color, the normal texture and the depth texture are
		 * setup (2D textures). All textures use RGBA and linear interpolation.
		 *
		 * The renderbuffer for the z-test is setup.
		 *
		 * The textures are attached to the framebuffer. If an error occurs during the attachment an
		 * error message is printed.
		 *
		 * The diffuse, specular, normal, and depth texture are used for drawing. The binding of the
		 * framebuffer is broken.
		 *
		 * @param w The width of the current render context.
		 * @param h The height of the current render context.
		 * @return void
		 **/
		void setupRenderTargets(unsigned int w, unsigned int h);

		void setNormalTexture(GLuint normal_texture);

		GLuint getDepthTex();
		GLuint getPrevDepthTex();
		GLuint getFrontBlenderTex();
		GLuint getPrevFrontBlenderTex();
		GLuint getBackBlenderTex();

		void setTransparentParameters(TransparentParameters parameters);

		TransparentParameters getTransparentParameters();

	public slots:

		virtual void setViewport(unsigned int w, unsigned int h, unsigned int x=0, unsigned int y=0 );

		virtual void setClearColor(float r, float g, float b, float a);

	protected:

		virtual void register_();

		virtual void unregister_();

		virtual void compileShaderPrograms_();

		virtual void initShaderPrograms_();

		/**
		 * @brief Initializes the system.
		 *
		 * Only initializes the system when the initialized_ flag is not already set to true.
		 * It performs the OpenGL generation calls for nedded buffers (vertex and index buffer objects).
		 * The initialized_ flag is set to true.
		 *
		 * @return void
		 **/
		void initialize_();

		/**
		 * @brief Performs an cleanup.
		 *
		 * Delets buffers.
		 * After calling cleanup_() the initialized_ flag is set to false.
		 *
		 * @return void
		 **/
		void cleanup_();

		/**
		 * @brief Sets up the transparent.
		 *
		 * @return void
		 **/
		void initVBO_();

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& /*list*/ ) {};

		/**
		 * @brief Location of the uniform variable "normal_map" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program (index of the rendertarget_textures).
		 **/
		GLuint normal_map_idx;

		/**
		 * @brief Location of the uniform variable "depth_map" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program (index of the rendertarget_textures).
		 **/
		GLuint depth_map_idx;

		/**
		 * @brief Location of the uniform variable "ssao_map" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program (index of the rendertarget_textures).
		 **/
		GLuint ssao_map_idx;

		/**
		 * @brief Location of the uniform variable "volume_map" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program (index of the rendertarget_textures).
		 **/
		GLuint volume_map_idx;

		/**
		 * @brief Location of the uniform variable "quad_position" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program.
		 **/
		GLuint quad_position_idx;

		/**
		 * @brief Location of the uniform variable "quad_texcoord" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program.
		 **/
		GLuint quad_texcoord_idx;

		/**
		 * @brief Location of the uniform variable "projection_matrix_inv" for the composite surface used by a shader program.
		 *
		 * Needed to pass data to the shader program.
		 **/
		GLuint projection_matrix_inv_idx;

		/**
		 * @brief Location of the uniform variable "screen_size" for the transparent used by a shader program.
		 *
		 * Needed to update the width and height.
		 **/
		GLuint viewport_idx;

		GLuint normal_texture_;

		/*****************************************New Stuff */

		GLuint temp_tex_idx;

		GLuint depth_tex_idx;

		GLuint front_blender_tex_idx;

		GLuint back_blender_tex_idx;

		GLuint g_dualBackBlenderFboId;
		GLuint g_dualPeelingSingleFboId;
		GLuint g_dualDepthTexId[2];
		GLuint g_dualFrontBlenderTexId[2];
		GLuint g_dualBackTempTexId[2];
		GLuint g_dualBackBlenderTexId;

		GLuint g_frontFboId[2];
		GLuint g_frontDepthTexId[2];
		GLuint g_frontColorTexId[2];
		GLuint g_frontColorBlenderTexId;
		GLuint g_frontColorBlenderFboId;

		GLenum g_drawBuffers[7];

		GLuint g_queryId;

		int currId_;

		const char* shader_blend_;

		const char* shader_final_;

		BALL::Vector4 background_color_;

		TransparentParameters tp_;


};

#endif //TRANSPARENT_H