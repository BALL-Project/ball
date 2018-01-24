#ifndef MESH_H
#define MESH_H

#include <renderObjects/geometry.h>

#include <list>


/**
 * @brief RenderObject for rendering mesh data.
 **/
class Mesh
	: public Geometry
{
		Q_OBJECT
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor
		 * The renderer needs to be initialized before usage by calling init().
		 *
		 * @param camera The camera of the renderer.
		 *
		 **/
		Mesh(Camera* camera);

	protected:

		/**
		 * @brief Renders the VBO.
		 *
		 * Before rendering the VBO the glPolygonMode is set to GL_FRONT_AND_BACK with the given drawing
		 * mode.
		 * After rendering the VBO it is resest to GL_FRONT_AND_BACK and GL_FILL.
		 *
		 * @return void
		 **/
		virtual void render_();

		virtual void register_();

		virtual void unregister_();

		virtual void compileShaderPrograms_();

		virtual void initShaderPrograms_();

	private:

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list );

		/**
		 * @brief Location of the uniform variable "vertex_position".
		 *
		 * Needed to pass data of the vertex attribut array.
		 **/
		GLuint vertex_position_idx;

		/**
		 * @brief Location of the uniform variable "vertex_normal".
		 *
		 * Needed to pass data of the vertex attribut array.
		 **/
		GLuint vertex_normal_idx;

		/**
		 * @brief Location of the uniform variable "color".
		 *
		 * Needed to pass data of the color attribut array.
		 **/
		GLuint color_idx;
};

#endif //MESH_H