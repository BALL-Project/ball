#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <Eigen/Geometry>
#include <BALL/MATHS/vector3.h>
#include <renderObjects/RenderObject.h>

#include <ShaderManager.h>

#include <vbo/VBObject.h>

#include <list>
using std::list;

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