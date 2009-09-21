// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: renderer.C,v 1.12.16.1 2007/03/25 21:57:02 oliver Exp $

#include <BALL/VIEW/RENDERING/renderer.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

namespace BALL
{
	namespace VIEW
	{

		Renderer::Renderer()
			: Object(),
				scene_(0),
				stage_(0),
				width_(0),
				height_(0),
				show_preview_(false),
				volume_width_(0.),
				show_light_sources_(false),
				use_continuous_loop_(false)
		{
			stage_ = &RTTI::getDefault<Stage>();
		}

		Renderer::Renderer(const Renderer& renderer)
		: Object(renderer),
			scene_(renderer.scene_),
			stage_(renderer.stage_),
			width_(renderer.width_),
			height_(renderer.height_),
			show_preview_(renderer.show_preview_),
			volume_width_(renderer.volume_width_),
			show_light_sources_(renderer.show_light_sources_),
			use_continuous_loop_(renderer.use_continuous_loop_)
		{}


		bool Renderer::renderOneRepresentation(const Representation& representation)
		{
			if (representation.isHidden()) return true;

			if (!representation.isValid())
			{
				Log.error() << "Representation " << &representation 
										<< "not valid, so aborting." << std::endl;
				return false;
			}

			list<GeometricObject*>::const_iterator it;
			for (it =  representation.getGeometricObjects().begin();
					 it != representation.getGeometricObjects().end();
					 it++)
			{
				render_(*it);
			}

			return true;
		}


		void Renderer::render_(const GeometricObject* object)
		{
			// most used geometric objects first
			if 			(RTTI::isKindOf<Sphere>(*object))  				renderSphere_(*(const 								Sphere*) object);
			else if (RTTI::isKindOf<TwoColoredTube>(*object)) renderTwoColoredTube_(*(const TwoColoredTube*) object);
			else if (RTTI::isKindOf<Mesh>(*object))   				renderMesh_(*(const   									Mesh*) object);
			else if (RTTI::isKindOf<TwoColoredLine>(*object)) renderTwoColoredLine_(*(const TwoColoredLine*) object);
			else if (RTTI::isKindOf<Point>(*object))  	 			renderPoint_(*(const  								 Point*) object);
			else if (RTTI::isKindOf<Disc>(*object))   				renderDisc_(*(const   									Disc*) object);
			else if (RTTI::isKindOf<Line>(*object))  	 				renderLine_(*(const   									Line*) object);
			else if (RTTI::isKindOf<Tube>(*object))  	 				renderTube_(*(const   									Tube*) object);
			else if (RTTI::isKindOf<Box>(*object))  					renderBox_(*(const   		 			 					 Box*) object);
			else if (RTTI::isKindOf<SimpleBox>(*object))  		renderSimpleBox_(*(const   		 		 SimpleBox*) object);
			else if (RTTI::isKindOf<Label>(*object))  	 			renderLabel_(*(const   				 				 Label*) object);
			else if (RTTI::isKindOf<MultiLine>(*object)) renderMultiLine_(*(const MultiLine*) object);
			else if (RTTI::isKindOf<GridVisualisation>(*object)) 	renderGridVisualisation_(*(const  GridVisualisation*) object);
			else if (RTTI::isKindOf<QuadMesh>(*object))   		 renderQuadMesh_(*(const   						QuadMesh*) object);
			// ... add more types of GeometricObjects here
			else
			{
				// unknown type of GeometricObject
				Log.error() << "unknown type of GeometricObject in Renderer::render_: " 
										<< typeid(object).name() << "  " << object << std::endl;
				return;
			}
		}


		bool Renderer::hasStage() const
		{ 
			return stage_ != &RTTI::getDefault<Stage>();
		}

		bool Renderer::init(Scene& scene)
		{
			scene_ = &scene;

			Stage* stage = scene.getStage();
			if (stage == 0)
			{
				init(Stage(), scene.width(), scene.height());
			}
			else
			{
				init(*stage, scene.width(), scene.height());
			}

			return true;
		}

		bool Renderer::init(const Stage& stage, float width, float  height)
		{
			stage_ = &stage;
			width_ = width;
			height_ = height;
			return true;
		}

		void Renderer::setLights(bool /*reset_all*/)
		{
			Log.error() << "Renderer::setLights() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::updateCamera(const Camera* /*camera*/)
		{
			Log.error() << "Renderer::updateCamera() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::updateBackgroundColor()
		{
			Log.error() << "Renderer::updateBackgroundColor() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::setupEnvironmentMap(const QImage& image)
		{
			Log.error() << "Renderer::setupEnvironmentMap() not implemented for this kind of renderer yet!" << std::endl;
		}

		void Renderer::setupStereo(float /*eye_separation*/, float /*focal_length*/)
		{
			Log.error() << "Renderer::setupStereo() not implemented for this kind of renderer yet!" << std::endl;
		}

		Vector3 Renderer::mapViewportTo3D(Position /*x*/, Position /*y*/)
		{
			Log.error() << "Renderer::mapViewportTo3D() not implemented for this kind of renderer yet!" << std::endl;
			return Vector3();
		}

		Vector2 Renderer::map3DToViewport(const Vector3& /*vec*/)
		{
			Log.error() << "Renderer::map3DToViewport() not implemented for this kind of renderer yet!" << std::endl;
			return Vector2();
		}

		void Renderer::renderRuler()
		{
			Log.error() << "Renderer::renderRuler() not implemented for this kind of renderer yet!" << std::endl;
		}

		bool Renderer::finish()
		{
			return true;
		}


} } // namespaces
