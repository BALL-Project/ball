// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: renderer.C,v 1.5 2004/02/12 16:17:26 amoll Exp $

#include <BALL/VIEW/RENDERING/renderer.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
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


namespace BALL
{
	namespace VIEW
	{

		Renderer::Renderer()
			throw() 
			: Object(),
				stage_(0),
				width_(0),
				height_(0)
		{
			stage_ = &RTTI::getDefault<Stage>();
		}

		Renderer::Renderer(const Renderer& renderer)
			throw() 
		: Object(renderer),
			stage_(renderer.stage_),
			width_(renderer.width_),
			height_(renderer.height_)
		{}


		void Renderer::render_(const GeometricObject* object)
			throw()
		{
			if 			(RTTI::isKindOf<Sphere>(*object))  					renderSphere_(*(const 									Sphere*) object);
			else if (RTTI::isKindOf<TwoColoredLine>(*object))   renderTwoColoredLine_(*(const   TwoColoredLine*) object);
			else if (RTTI::isKindOf<TwoColoredTube>(*object))   renderTwoColoredTube_(*(const   TwoColoredTube*) object);
			else if (RTTI::isKindOf<Point>(*object))  	 				renderPoint_(*(const  					 				 Point*) object);
			else if (RTTI::isKindOf<Box>(*object))  						renderBox_(*(const   		 			 						 Box*) object);
			else if (RTTI::isKindOf<SimpleBox>(*object))  			renderSimpleBox_(*(const   		 			 SimpleBox*) object);
			else if (RTTI::isKindOf<Label>(*object))  	 				renderLabel_(*(const   				 					 Label*) object);
			else if (RTTI::isKindOf<Mesh>(*object))   					renderMesh_(*(const   										Mesh*) object);
			else if (RTTI::isKindOf<Disc>(*object))   					renderDisc_(*(const   										Disc*) object);
			else if (RTTI::isKindOf<Line>(*object))  	 					renderLine_(*(const   										Line*) object);
			else if (RTTI::isKindOf<Tube>(*object))  	 					renderTube_(*(const   										Tube*) object);
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
			throw() 
		{ 
			return stage_ != &RTTI::getDefault<Stage>();
		}


		bool Renderer::init(const Stage& stage, float width, float  height)
			throw()
		{
			stage_ = &stage;
			width_ = width;
			height_ = height;
			return true;
		}

		bool Renderer::finish()
			throw()
		{
			return true;
		}

} } // namespaces
