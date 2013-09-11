#ifndef POINT_H
#define POINT_H

#include <Eigen/Geometry>
#include <BALL/MATHS/vector3.h>
#include <renderObjects/Geometry.h>

#include <ShaderManager.h>

#include <vbo/VBOPoint.h>

#include <list>
using std::list;

/**
 * @brief RenderObject for rendering a point.
 **/
class Point
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
		Point(Camera* camera);

	protected:

		/**
		 * @brief Renders the VBO.
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
		 * @brief Location of the uniform variable "color".
		 *
		 * Needed to pass data of the color attribut array.
		 **/
		GLuint color_idx;
};

#endif //POINT_H