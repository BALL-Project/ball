#include <GL/glew.h>

#include <renderObjects/lightSources.h>

#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>


LightSources::LightSources(Camera* camera)
	: RenderObject(camera),
	  sphere_(camera),
	  cylinder_(camera)
{
	sphere_.setClipable(false);
	cylinder_.setClipable(false);
}

//here starts the rendering
void LightSources::render(RenderLevel level)
{
	sphere_.render(level);
	cylinder_.render(level);
}

void LightSources::init (const BALL::VIEW::Stage* stage )
{
	std::list<BALL::VIEW::LightSource> light_sources = stage->getLightSources();

	std::list<BALL::VIEW::GeometricObject const *> sphere_list;
	std::list<BALL::VIEW::GeometricObject const *> tube_list;

	std::list<BALL::VIEW::LightSource>::const_iterator lit = light_sources.begin();

	for (; lit != light_sources.end(); lit++)
	{
		BALL::Vector3 pos = (*lit).getPosition();
		BALL::Vector3 dir = (*lit).getDirection();

		if ((*lit).isRelativeToCamera())
		{
			pos = stage->getCamera().getViewPoint() + stage->calculateAbsoluteCoordinates(pos);
			dir = pos + stage->calculateAbsoluteCoordinates(dir);
		}

		BALL::VIEW::ColorRGBA bgi(stage->getBackgroundColor().getInverseColor());

		BALL::Vector3 diff = dir - pos;

		if (!BALL::Maths::isZero(diff.getSquareLength())) diff.normalize();

		diff *= 100.0;

		BALL::VIEW::Sphere* s = new BALL::VIEW::Sphere;
		s->setPosition(pos);
		s->setRadius(5);

		s->setColor(bgi);
		sphere_list.push_back(s);

		int steps = 10;

		for(int i = 0; i < steps; i++)
		{
			BALL::VIEW::Tube* t = new BALL::VIEW::Tube;
			t->setVertex1(pos);
			t->setVertex2(pos + diff/steps);
			t->setColor(bgi);
			tube_list.push_back(t);

			pos = pos + diff/steps;
		}
	}

	sphere_.init(sphere_list);
	cylinder_.init(tube_list);

	// free resources
	while(!tube_list.empty())
	{
		delete tube_list.front();
		tube_list.pop_front();
	}

	// free resources
	while(!sphere_list.empty())
	{
		delete sphere_list.front();
		sphere_list.pop_front();
	}
}



void LightSources::setLights(const BALL::VIEW::Stage* stage, bool reset_all)
{
	cylinder_.setLights(stage, reset_all);
	sphere_.setLights(stage, reset_all);
}

void LightSources::setDepthBlenderTex(GLuint tex)
{
	cylinder_.setDepthBlenderTex(tex);
	sphere_.setDepthBlenderTex(tex);
}

void LightSources::setFrontBlenderTex(GLuint tex)
{
	cylinder_.setFrontBlenderTex(tex);
	sphere_.setFrontBlenderTex(tex);
}

void LightSources::setBackBlenderTex ( GLuint tex )
{
	cylinder_.setBackBlenderTex(tex);
	sphere_.setBackBlenderTex(tex);
}