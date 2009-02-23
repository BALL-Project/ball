// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: renderer.h,v 1.11.16.1 2007/03/25 21:26:16 oliver Exp $

#ifndef BALL_VIEW_RENDERING_RENDERER_H
#define BALL_VIEW_RENDERING_RENDERER_H

#ifndef BALL_VIEW_KERNEL_REPRESENTATION_H
#	include <BALL/VIEW/KERNEL/representation.h>
#endif

#ifndef BALL_CONCEPT_OBJECT_H
#	include <BALL/CONCEPT/object.h>
#endif

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		class Scene;
		class Line;
		class MultiLine;
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
		class ClippingPlane;
		class GridVisualisation;
		class QuadMesh;

		/** Renderer is just a generic base class.
		 		Derived classes are GLRenderer and POVRenderer.
				Every renderer has a pointer to a Stage object, which contains the viewpoint,
				the LightSource, etc. The renderer also knows the width and height, of the display, it shall
				render.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT Renderer
			:	public Object
		{
			public:

			BALL_CREATE(Renderer)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			Renderer();
			
			/**	Copy constructor.
			*/
			Renderer(const Renderer& renderer);

			/** Destructor
			*/
			virtual ~Renderer() throw() {}

			/** Explicit default initialization.
			*/
			virtual void clear()
				throw() {}

			/** Initialization routines.
			 		This method is called by Scene::initializeGL.
			*/
			virtual bool init(Scene& scene);

			/** Initialisation with the Stage.
			 		Called by Scene::exportScene().
			 */
			virtual bool init(const Stage& stage, float height, float width);

			/// Set the light sources according to the stage
			virtual void setLights(bool reset_all = false);

			/** Update the camera position either from a given Camera, or from the default Stage.
			 */
			virtual void updateCamera(const Camera* camera = 0);

			/// Update the background color from the stage
			virtual void updateBackgroundColor();

			/** Sets this renderer as a part of a stereo setup.
			 *
			 *  eye_separation denotes the distance along the right vector used
			 *  by this "eye".
			 */
			virtual void setupStereo(float eye_separation, float focal_length);

			/** Decide between event based and (threaded) continuous loop rendering.
			*/
			virtual void useContinuousLoop(bool use_loop) 
			{ 
				// TODO: mutex for use_continuous_loop_ just to be on the safe side
				use_continuous_loop_ = use_loop;
			};

			/** Returns the mode of the render loop.
			*/
			bool isContinuous() { return use_continuous_loop_; }

			///
			virtual bool finish()
				throw();

			//@}
			/**	@name	Accessors
			*/
			//@{
			
			/** Render a Representation.
			*/
			virtual bool renderOneRepresentation(const Representation& representation)
				throw();


			/** Buffer a Representation for later rendering.
			 */
			virtual void bufferRepresentation(const Representation& rep) {};

			/** Remove a representation from the buffer.
			 */
			virtual void removeRepresentation(const Representation& rep) {};

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
			virtual void setSize(float width, float height) { width_ = width; height_ = height;}

			///
			virtual float getWidth() const { return width_;}

			///
			virtual float getHeight() const { return height_;}

			/** Decides whether the renderer should use a faster preview mode.
			 *  Please note that not all renderers support previews and may choose
			 *  just to ignore this setting.
			 */
			virtual void setPreviewMode(bool show_preview) { show_preview_ = show_preview; }

			/** Decides whether to draw light sources explicitly.
			 *  Please note that not all renderers support this feature and may choose
			 *  just to ignore this setting.
			 */
			virtual void showLightSources(bool show_light_sources) { show_light_sources_ = show_light_sources; }
			
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

			protected:

			///
			virtual void renderClippingPlane_(const ClippingPlane&)
				throw() {Log.error() << "renderClippingPlane_ not implemented in derived Renderer class" << std::endl;}

			// Render a text label
			virtual void renderLabel_(const Label& /*label*/)
				throw() {Log.error() << "renderLabel_ not implemented in derived Renderer class" << std::endl;}

			/// Render a line
			virtual void renderLine_(const Line& /*line*/)
				throw() {Log.error() << "renderLine_ not implemented in derived Renderer class" << std::endl;}
			
			/// Render an illuminated line
			virtual void renderMultiLine_(const MultiLine& /*line*/)
				throw() {Log.error() << "renderMultiLine_ not implemented in derived Renderer class" << std::endl;}

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
			
			/// Render a grid
			virtual void renderGridVisualisation_(const GridVisualisation&)
				throw() {Log.error() << "renderGridVisualisation_ not implemented in derived Renderer class" << std::endl;}
			/// Render a quad mesh
			virtual void renderQuadMesh_(const QuadMesh&)
				throw() {Log.error() << "renderQuadMesh_ not implemented in derived Renderer class" << std::endl;}
				//@}
			
			//_
			Scene* scene_;

			//_
			const Stage*		stage_;

			//_
			float 					width_;

			//_
			float 					height_;

			//_
			bool						show_preview_;

			//_
			float						volume_width_;

			//_
			bool						show_light_sources_;

			//_
			Camera					camera_;

			// An offset added to camera position and look at
			Vector3					camera_offset_;

			bool						use_continuous_loop_;
		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RENDERER_H
