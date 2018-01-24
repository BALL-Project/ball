#ifndef LIGHTSOURCES_H
#define LIGHTSOURCES_H

#include <renderObjects/cylinder.h>
#include <renderObjects/renderObject.h>
#include <renderObjects/sphere.h>

#include <list>


class LightSources
	: public RenderObject
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
		LightSources(Camera* camera);

		/**
		 * @brief Starting point for rendering.
		 * This method should be called whenever the window needs to be redrawn. It starts the rendering
		 * process for the box.
		 * The imposter_box shader program is used.
		 *
		 * @return void
		 **/
		void render(RenderLevel level = OPAQUE);

		virtual void init(const BALL::VIEW::Stage* stage);

		virtual void setLights(const BALL::VIEW::Stage* stage, bool reset_all);

		virtual void setDepthBlenderTex(GLuint tex);

		virtual void setFrontBlenderTex(GLuint tex);

		virtual void setBackBlenderTex (GLuint tex);

	protected:

		virtual void register_() { };

		virtual void unregister_() { };

		virtual void compileShaderPrograms_() {};

		virtual void initShaderPrograms_() {};

	private:

		virtual void initVBO_ ( std::list< const BALL::VIEW::GeometricObject* >& /*list*/ ) {};

		Sphere sphere_;
		Cylinder cylinder_;
};

#endif //LIGHTSOURCES_H