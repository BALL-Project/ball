#ifndef CYLINDER_H
#define CYLINDER_H

#include <renderObjects/Geometry.h>

#include <list>


/**
 * @brief RenderObject for rendering cylinders.
 **/
class Cylinder
	: public Geometry
{
		Q_OBJECT
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		/**
		 * @brief Default constructor
		 * The renderer needs to be initialized before usage by calling setup().
		 *
		 * @param camera The camera of the renderer.
		 *
		 **/
		Cylinder(Camera* camera);

	protected:

		virtual void setUniforms_();

		virtual void render_();

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
		void initialize_();

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list );

		/**
		 * @brief Location of the uniform variable "midpoint".
		 *
		 * Needed to pass data of the vertex attribut array.
		 **/
		GLuint midpoint_idx;

		/**
		 * @brief Location of the uniform variable "direction".
		 *
		 * Needed to pass data of the direction attribut array.
		 **/
		GLuint direction_idx;

		/**
		 * @brief Location of the uniform variable "color1".
		 *
		 * Needed to pass data of the color1 attribut array.
		 **/
		GLuint color1_idx;

		/**
		 * @brief Location of the uniform variable "color2".
		 *
		 * Needed to pass data of the color2 attribut array.
		 **/
		GLuint color2_idx;

		/**
		 * @brief Location of the uniform variable "radius".
		 *
		 * Needed to pass data of the radius attribut array.
		 **/
		GLuint radius_idx;

		/**
		 * @brief Location of the uniform variable "color_ratio".
		 *
		 * Needed to pass data of the color ratio attribut array.
		 **/
		GLuint color_ratio_idx;
};

#endif //CYLINDER_H