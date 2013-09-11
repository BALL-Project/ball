#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>
#include <Eigen/Geometry>
#include <BALL/MATHS/vector3.h>
#include <renderObjects/Geometry.h>

#include <ShaderManager.h>
#include <vbo/VBOPoint.h>

#include <list>

/**
 * @brief RenderObject for rendering spheres.
 **/
class Sphere
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
		Sphere(Camera* camera);

	protected:

		virtual void render_();

		virtual void setUniforms_();

		virtual void register_();

		virtual void unregister_();

		virtual void compileShaderPrograms_();

		virtual void initShaderPrograms_();

	private:

		/**
		 * @brief Initializes the system.
		 *
		 * Additionally to super class it enables server-side GL capabilities GL_POINT_SPRITE and
		 * GL_VERTEX_PROGRAM_POINT_SIZE which are used for this render object.
		 *
		 * @return void
		 **/
		virtual void initialize_();

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list );

		/**
		 * @brief Location of the attribute variable "midpoint".
		 *
		 * Needed to pass data of the vertex attribut array.
		 **/
		GLuint midpoint_idx;

		/**
		 * @brief Location of the attribute variable "color".
		 *
		 * Needed to pass data of the color attribut array.
		 **/
		GLuint color_idx;

		/**
		 * @brief Location of the attribute variable "radius".
		 *
		 * Needed to pass data of the radius attribut array.
		 **/
		GLuint radius_idx;
};

#endif //SPHERE_H