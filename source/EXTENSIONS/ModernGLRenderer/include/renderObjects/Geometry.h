#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <renderObjects/RenderObject.h>
#include <vbo/VBObject.h>

#include <Eigen/Geometry>


class Geometry
	: public RenderObject
{
		Q_OBJECT
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		Geometry(Camera* camera);

		virtual ~Geometry();

		virtual void render(RenderLevel level = OPAQUE);

	protected:

		virtual void render_() = 0;

		virtual void setUniforms_ () {};

		virtual void bindTexturesOpaque_() {};

		virtual void bindTexturesTransparencyInit_() {};

		virtual void bindTexturesTransparencyPeel_() {};

		virtual void cleanup_();

		VBObject* vbo_;

		const char* transparency_init_;
		const char* transparency_peel_;
};

#endif //GEOMETRY_H