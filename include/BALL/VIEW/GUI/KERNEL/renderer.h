// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: renderer.h,v 1.2 2003/08/26 08:05:02 oliver Exp $

#ifndef BALL_VIEW_GUI_KERNEL_RENDERER_H
#define BALL_VIEW_GUI_KERNEL_RENDERER_H

#ifndef BALL_VIEW_GUI_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/GUI/KERNEL/representation.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class Line;
		class Tube;
		class Mesh;
		class Label;
		class Point;
		class Sphere;
		class Disc;
		class Box;
		class SimpleBox;
		class TwoColoredLine;
		class TwoColoredTube;
		class Stage;

		/** Renderer is just a generic base class.
		 		Derived classes are GLRenderer and POVRenderer.
				Every renderer has a pointer to a Stage object, which contains the viewpoint,
				the LightSource, etc. The renderer also knows the width and height, of the display, it shall
				render.
		*/
		class Renderer
			:	public Object
		{
			public:

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			Renderer()
				throw();
			
			/**	Copy constructor.
			*/
			Renderer(const Renderer& renderer)
				throw();

			/** Destructor
			*/
			virtual ~Renderer()
				throw() {}

			/** Explicit default initialization.
			*/
			virtual void clear()
				throw() {}

			//@}
			/**	@name	Accessors
			*/
			//@{
			
			/** Update the display of a Representation.
			*/
			virtual void update(Representation& /*representation*/)
				throw() {}

			/** Get the stage for the renderer (const)
			 */
			virtual const Stage& getStage() const
				throw() { return *stage_;}
			
			/** Set the stage for the renderer
			 */
			virtual void setStage(const Stage& stage)
				throw() { stage_ = &stage;}

			/// Test if a Stage was assigned to the Renderer
			bool hasStage() const
				throw();

			/// Set the size of the display
			virtual void setSize(float width, float height)
				throw() { width_ = width; height_ = height;}

			///
			virtual float getWidth() const
				throw() { return width_;}

			///
			virtual float getHeight() const
				throw() { return height_;}

			//@}
			/**	@name Predicates
			*/
			//@{
			
			///
			bool operator == (const Renderer& /*renderer*/) const
				throw() { return true; }

			/// Wrapper for the renderering of special GeometricObjects
			virtual void render_(const GeometricObject* object)
				throw();

			////
			protected:

			// Render a text label
			virtual void renderLabel_(const Label& /*label*/)
				throw() {Log.error() << "renderLabel_ not implemented in derived Renderer class" << std::endl;}

			/// Render a line
			virtual void renderLine_(const Line& /*line*/)
				throw() {Log.error() << "renderLine_ not implemented in derived Renderer class" << std::endl;}

			/// Render a surface mesh
			virtual void renderMesh_(const Mesh& /*mesh*/)
				throw() {Log.error() << "renderMesh_ not implemented in derived Renderer class" << std::endl;}
 
			/// Render a single point
 			virtual void renderPoint_(const Point& /*point*/)
				throw() {Log.error() << "renderPoint_ not implemented in derived Renderer class" << std::endl;}

			/// Render a box
			virtual void renderBox_(const Box& /*box*/)
				throw() {Log.error() << "renderBox_ not implemented in derived Renderer class" << std::endl;}

			/// Render a simple box (parallel to the axes)
			virtual void renderSimpleBox_(const SimpleBox& /*box*/)
				throw() {Log.error() << "renderSimpleBox_ not implemented in derived Renderer class" << std::endl;}

			/// Render a sphere
			virtual void renderSphere_(const Sphere& /*sphere*/)
				throw() {Log.error() << "renderSphere_ not implemented in derived Renderer class" << std::endl;}

			/// Render a disc
			virtual void renderDisc_(const Disc& /*disc*/)
				throw() {Log.error() << "renderDisc_ not implemented in derived Renderer class" << std::endl;}

			/// Render a tube
			virtual void renderTube_(const Tube& /*tube*/)
				throw() {Log.error() << "renderTube_ not implemented in derived Renderer class" << std::endl;}

			/// Render a line with two colors
			virtual void renderTwoColoredLine_(const TwoColoredLine& /*two_colored_line*/)
				throw() {Log.error() << "renderTwoColoredLine_ not implemented in derived Renderer class" << std::endl;}

			/// Render a tube with two colors
			virtual void renderTwoColoredTube_(const TwoColoredTube& /*two_colored_tube*/)
				throw() {Log.error() << "renderTwoColoredTube_ not implemented in derived Renderer class" << std::endl;}

			//@}
			
			//_
			const Stage*		stage_;

			//_
			float 					width_;

			//_
			float 					height_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_RENDERER_H
