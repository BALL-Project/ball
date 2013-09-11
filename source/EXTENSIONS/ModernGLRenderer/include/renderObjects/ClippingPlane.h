#ifndef CLIPPINGPLANE_H
#define CLIPPINGPLANE_H

#include <Eigen/Geometry>
#include <BALL/MATHS/vector3.h>
#include <renderObjects/RenderObject.h>
#include "Cylinder.h"

#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <ShaderManager.h>

#include <list>

/**
 * @brief RenderObject for rendering a clipping plane.
 *
 * The clipping plane consists of a transparent disc and a tube to drag it.
 **/
class ClippingPlane
	: public RenderObject
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
		ClippingPlane(Camera* camera);

		/**
		 * @brief Starting point for rendering.
		 * This method should be called whenever the window needs to be redrawn. It starts the rendering
		 * process for the clipping plane.
		 *
		 * @return void
		 **/
		void render(RenderLevel level = OPAQUE);

		virtual void init(std::vector<BALL::VIEW::ClippingPlane*>& clipping_planes);

		/**
		 * @brief Return the number of clipping planes stored in this object.
		 *
		 * @return int Number of clipping planes stored in this object.
		 **/
		virtual int getNumberOfClippingPlanes();

		virtual void setLights(const BALL::VIEW::Stage* stage, bool reset_all);

		virtual void setDepthBlenderTex(GLuint tex);

		virtual void setFrontBlenderTex(GLuint tex);

		virtual void setBackBlenderTex (GLuint tex);

	protected:

		virtual void register_() {};

		virtual void unregister_() {};

		virtual void compileShaderPrograms_() {};

		virtual void initShaderPrograms_() {};

	private:

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& /*list*/ ) {};

		/**
		 * @brief Render the box.
		 *
		 * The vertex attribut arrays (discribing the shap of the object) are enabled to draw the
		 * quads and are disabled after that.
		 *
		 * @return void
		 **/
		void renderClippingPlane_(RenderLevel level = OPAQUE);

		Cylinder cylinder_;

		std::vector<BALL::VIEW::ClippingPlane*> clipping_planes_;
};

#endif //CLIPPINGPLANE_H