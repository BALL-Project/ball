// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
#define BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H

#ifndef BALL_VIEW_RENDERING_RENDERER_H
#	include <BALL/VIEW/RENDERING/renderer.h>
#endif

// TODO: do we need these includes?
/*#ifndef BALL_MATHS_QUATERNION_H
# include <BALL/MATHS/quaternion.h>
#endif 

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif
*/

#ifndef BALL_VIEW_KERNEL_STAGE_H
# include <BALL/VIEW/KERNEL/stage.h>
#endif

#include <BALL/VIEW/DATATYPE/raytraceableGrid.h>
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>

#include <RTfact/Config/Init.inc.cpp>

#include <RTfact/Utils/IO/SerializationUtils.hpp>
#include <RTfact/Utils/IO/ConfigurationFile.hpp>
#include <RTfact/Utils/StaticInitializer.hpp>

#include <RTfact/Model/Scene/BasicScene.hpp>
#include <RTfact/Model/Texture/Float3ConstantTexture.hpp>
#include <RTfact/Model/Texture/BasicFloat3Texture2D.hpp>

#include <RTfact/Model/Renderer/BasicSurfaceRayTracer.hpp>

#include <RTfact/Model/Camera/PinholeCamera.hpp>
#include <RTfact/Model/PixelSampler/PixelCenterSampler.hpp>

#include <RTfact/Model/Material/LambertianMaterial.hpp>

#include <RTfact/Concept/SurfaceShaderContext.hpp>
#include <RTfact/Model/Integrator/SurfaceShaderIntegrator.hpp>
#include <RTfact/Model/SurfaceShader/DiffuseShader.hpp>

#include <RTfact/Model/Light/PointLight.hpp>

#include <RTfact/Model/Primitive/BVH.hpp>

#include <RTfact/Model/Builder/SAHBVHBuilder.hpp>

#include <RTfact/Model/Intersector/BVHIntersector.hpp>
#include <RTfact/Model/Intersector/PlueckerTriangleIntersector.hpp>
#include <RTfact/Model/Intersector/PlueckerTriangleIntersectorOld.hpp>

#include <RTfact/Model/Integrator/SimpleIntegrator.hpp>
#include <RTfact/Model/Integrator/EyeLightIntegrator.hpp>
#include <RTfact/Model/Integrator/DirectIlluminationIntegrator.hpp>

#include <RTfact/Utils/UI/Controllers/CameraController.hpp>

#include <RTfact/Utils/IO/Loader.hpp>

#include <RTfact/Utils/IO/Loaders/TriangleSceneObjLoader.hpp>
#include <RTfact/Utils/IO/Loaders/TriangleSceneShaderObjLoader.hpp>
#include <RTfact/Utils/IO/Loaders/Float3Image2DTGALoader.hpp>

#include <RTfact/Utils/IO/ConfigurationFile.hpp>


#include <BALL/VIEW/RENDERING/raytracingGLWindow.h>


/* 
 * 
 * RTfact configuration 
 *
 */

#define CLASSIC_SHADERS
//#define USE_TBB

//#ifndef APIENTRY
//#define APIENTRY
//#endif

class QFont;

namespace BALL
{
	namespace VIEW
	{
		typedef RTfact::PixelCenterSampler t_PixelSampler;
		typedef RTfact::BasicSurfaceRayTracer t_Renderer;

		typedef RTfact::BVH<Triangle> t_Tree;	
		typedef RTfact::SAHBVHBuilder t_Builder;  
		typedef RTfact::BVHIntersector<RTfact::PlueckerTriangleIntersector> t_Intersector;

		#ifdef CLASSIC_SHADERS

			struct SceneConfig
			{
				typedef SceneConfig This;

				typedef RTfact::BasicScene<This, Light, Triangle> Scene;
				typedef RTfact::SurfaceShaderContext<Scene, t_Tree, t_Intersector> Context;
				typedef RTfact::SurfaceShader<Context> MaterialBase;
			};

			typedef SceneConfig::Scene t_Scene;
			typedef SceneConfig::Context t_Context;
			typedef RTfact::SurfaceShaderIntegrator<t_Context> t_Integrator;

		#else

			typedef RTfact::BasicScene<Material, Light, Triangle> t_Scene;
			typedef RTfact::EyeLightIntegrator<t_Scene, t_Tree, t_Intersector> t_Integrator;
			//typedef SimpleIntegrator t_Integrator;

		#endif

		typedef RTfact::PinholeCamera t_Camera;
		typedef RaytracingGLWindow t_Window;
		typedef RTfact::UI::CameraController<t_Camera> t_CameraController;

		class Scene;
		//class ColorMap;		

		/** RaytracingRenderer
		 		Renderer which provides raytracing instead of regular rendering.
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT RaytracingRenderer
			: public Renderer
		{			
			public:			

			/// Default Constructor.
			RaytracingRenderer()
				throw();

			/// Destructor
			virtual ~RaytracingRenderer()
				throw();			
						
			/** Initialise the renderer, by calling the init method below
			 		This method is called by Scene::initializeRaytracing.
			*/
			virtual bool init(Scene& scene, t_Window& aWindow)
				throw();

			
			virtual void prepareBufferedRendering() = 0;
			virtual void renderToBufferImpl(FrameBufferPtr buffer) = 0;
						
			
			///
			//void setSize(float width, float height)
			//	throw();
			
			/** Update the camera position either from a given Camera, or from the default Stage.
			*/
			//void updateCamera(const Camera* camera = 0)
			//	throw();			
			
			/// Enable or disable antialiasing
			//void setAntialiasing(bool state);
			
			/// Remove all references and auxilliary data for the given Representation
			//void removeRepresentation(const Representation& rep)
			//	throw();

			/// Initialize the renderer for a new representation
			//void addRepresentation(const Representation& rep)
			//	throw();
		
			///
			//void setStereoMode(StereoMode state)
			//	throw();

			///
			//StereoMode getStereoMode() const
			//	throw();

			///
			//RenderMode getRenderMode() const
			//	throw();

			///
			//void setRenderMode(RenderMode mode) { render_mode_ = mode;}
			
			///
			//virtual void deactivateAllRepresentations()
			//	throw();

			///
			//virtual bool activateRepresentation(const Representation& representation)
			//	throw();

			///
			String getRenderer()	
				throw();

			///
			//DrawingMode getDrawingMode() const {return drawing_mode_;};
			
			///
			//void initPerspective();

			/** The main raytracing function.
			 *  This function is called by the scene as soon as all active
			 *  representations have been registered using activateRepresentation()
			 */
			void raytraceAllRepresentations()
				throw();

			//void setupGridClipPlanes_(const GridVisualisation& slice);

			///
			Scene* scene_;

			///
			//DrawingMode drawing_mode_;
			//StereoMode stereo_;
			//RenderMode render_mode_;
			//bool picking_mode_;

		private:

			bool initialize();
			bool loadVolume(RTfact::IO::ConfigurationFile& aConfig);
			bool findConfig(std::string configFilename);

			RTfact::IO::ConfigurationFile config;
			boost::intrusive_ptr<t_Scene> rtfactScene;
			boost::intrusive_ptr<t_CameraController> cameraController;
			boost::intrusive_ptr<t_Window> window;
			t_Camera camera;
			t_Renderer renderer;
			t_Tree tree;
			t_Builder builder;
			t_Intersector intersector;
			t_PixelSampler pixelSampler;
		#ifdef CLASSIC_SHADERS
			t_Context context;
		#endif
			t_Integrator integrator;

		#ifdef USE_TBB
			tbb::task_scheduler_init init;
		#endif

		};


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RAYTRACINGRENDERER_H
