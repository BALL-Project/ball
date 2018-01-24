#include <GL/glew.h>

#include <renderObjects/ClippingPlane.h>

#include <BALL/VIEW/PRIMITIVES/tube.h>


ClippingPlane::ClippingPlane(Camera* camera)
	: RenderObject(camera),
	  cylinder_(camera)
{
	cylinder_.setClipable(false);
}

//here starts the rendering
void ClippingPlane::render(RenderLevel level)
{
	renderClippingPlane_(level);
}

void ClippingPlane::renderClippingPlane_(RenderLevel level)
{
	cylinder_.render(level);
}

void ClippingPlane::init (std::vector<BALL::VIEW::ClippingPlane*>& clipping_planes)
{
	clipping_planes_ = clipping_planes;

	std::list<BALL::VIEW::GeometricObject const *> tube_list;

	std::vector<BALL::VIEW::ClippingPlane*>::const_iterator it = clipping_planes.begin();

	for (; it != clipping_planes.end(); it++)
	{
		BALL::VIEW::ClippingPlane clipping_plane = **it;
		const BALL::Vector3& point(clipping_plane.getPoint());
		const BALL::Vector3& n(clipping_plane.getNormal());

		BALL::VIEW::Tube* tube = new BALL::VIEW::Tube;
		tube->setVertex1(point);
		tube->setVertex2(point - (n * 10.0));
		tube->setColor(BALL::VIEW::ColorRGBA(0,255,255));
		tube->setRadius(1);

		tube_list.push_back(tube);

		BALL::VIEW::Tube* tube2 = new BALL::VIEW::Tube;
		tube2->setVertex1(point);
		tube2->setVertex2(point + (n * 0.01));
		BALL::VIEW::ColorRGBA capping_color = clipping_plane.getCappingColor();
		capping_color.setAlpha(128);
		tube2->setColor(capping_color);
		tube2->setRadius(10);
		tube_list.push_back(tube2);
	}

	cylinder_.init(tube_list);

	// free resources
	while(!tube_list.empty())
	{
		delete tube_list.front();
		tube_list.pop_front();
	}
}

int ClippingPlane::getNumberOfClippingPlanes()
{
	int numberOfClippingPlanes = 0;
	std::vector<BALL::VIEW::ClippingPlane*>::const_iterator it = clipping_planes_.begin();

	for (; it != clipping_planes_.end(); it++)
	{
		BALL::VIEW::ClippingPlane clipping_plane = **it;

		if(!clipping_plane.isHidden())
		{
			numberOfClippingPlanes ++;
		}
	}

	return numberOfClippingPlanes;
}

void ClippingPlane::setLights(const BALL::VIEW::Stage* stage, bool reset_all)
{
	cylinder_.setLights(stage, reset_all);
}

void ClippingPlane::setDepthBlenderTex(GLuint tex)
{
	cylinder_.setDepthBlenderTex(tex);
}

void ClippingPlane::setFrontBlenderTex(GLuint tex)
{
	cylinder_.setFrontBlenderTex(tex);
}

void ClippingPlane::setBackBlenderTex ( GLuint tex )
{
	cylinder_.setBackBlenderTex(tex);
}
