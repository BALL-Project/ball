#ifndef POINTVOLUME_H
#define POINTVOLUME_H

#include <GL/glew.h>

#include <renderObjects/Geometry.h>

#include <list>

namespace BALL
{
	namespace VIEW
	{
		class GridVisualisation;
	}
}

/**
 * @brief RenderObject for rendering a volume texture represented as points.
 **/
class PointVolume
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
		PointVolume(Camera* camera);

	protected:

		virtual void render_();

		virtual void register_();

		virtual void unregister_();

		virtual void compileShaderPrograms_();

		virtual void initShaderPrograms_();

		virtual void bindTexturesTransparencyPeel_();

		virtual void bindTexturesOpaque_();

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

		void createTextureFromGrid_(const BALL::VIEW::GridVisualisation* vis);

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& list );

		/**
		 * @brief Location of the 3D volume texture to be rendered.
		 **/
		GLuint volume_texture_;

		/**
		 * @brief Location of the attribute variable "midpoint".
		 *
		 * Needed to pass data of the midpoint attribut array.
		 **/
		GLuint midpoint_idx;

		/**
		 * @brief Location of the attribute variable "texcoords".
		 *
		 * Needed to pass data of the texcoords attribut array.
		 **/
		GLuint texcoords_idx;

		/**
		 * @brief Location of the attribute variable "radius".
		 *
		 * Needed to pass data of the radius attribut array.
		 **/
		GLuint radius_idx;
};

#endif //POINTVOLUME_H