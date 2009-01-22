// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/VIEW/RENDERING/raytracingRenderer.h>
#include <BALL/VIEW/WIDGETS/scene.h>

//RTfact mandatory includes
#include <RTfact/Config/ExternalIncludes.hpp>
#include <RTfact/Config/Init.inc.cpp>

//Error logging with format string
#define RTFACT_LOG_ERROR_F(aFormat, aParams)                      \
    RTFACT_LOG_ERROR((boost::format(aFormat) % aParams).str())

//Name of the initialization file
#define CONFIG_FILE "RTfact.ini"

namespace BALL
{
	namespace VIEW
	{

	#ifdef CLASSIC_SHADERS			
		RaytracingRenderer::RaytracingRenderer()
			throw()
			: Renderer(),
				scene_(0),
				rtfactScene(new t_Scene()),
				context(*rtfactScene, tree, intersector),
				integrator(context)

	#else
		RaytracingRenderer::RaytracingRenderer()
			throw()
			: Renderer(),
				scene_(0),
				rtfactScene(new t_Scene())
	#endif
		{
			//
		}

		RaytracingRenderer::~RaytracingRenderer()
			throw()
		{			
			//
		}

		bool RaytracingRenderer::init(Scene& scene, t_Window& aWindow)
			throw()
		{
			scene_ = &scene;
			window = &window;

			Stage* stage = scene.getStage();
			if (stage == 0)
			{				
				Renderer::init(Stage(), scene.height(), scene.width());
			}
			else
			{				
				Renderer::init(*stage, scene.height(), scene.width());
			}
			
			#ifdef CLASSIC_SHADERS
    
				IO::Loader<t_Scene>::registerLoader(".obj", new IO::TriangleSceneShaderObjLoader<SceneConfig>);

			#else

				IO::Loader<t_Scene>::registerLoader(".obj", new IO::TriangleSceneObjLoader<t_Scene>);

			#endif

			IO::Loader<BasicImage2D<3, float> >::registerLoader(".tga", new IO::Float3Image2DTGALoader<BasicImage2D<3, float> >);

			return initialize();
		}				

		String  RaytracingRenderer::getRenderer()
			throw()
		{
			return (char*)"BALLView CUDA ray caster v1";	
		}		
		
		void RaytracingRenderer::raytraceAllRepresentations()
			throw()
		{
			cameraController.applyChanges();
			
			BallViewFrameBufferPtr framebuffer = window->getFramebuffer();

			ImageClipRegion clip(0, 0, 
									framebuffer->getImage().getResX(),
									framebuffer->getImage().getResY());

            #ifdef USE_TBB

                tbb::parallel_for(tbb::blocked_range2d<uint>(
                    clip.top, clip.bottom, 64,
                    clip.left, clip.right, 64),
                    RenderParallel(renderer, camera, *rtfactScene, tree, intersector,
                                   pixelSampler, integrator, window));

            #else

                renderer.render<PRIMARY_RAY_PACKET_SIZE>(
                    camera, *rtfactScene, tree, intersector,
                    pixelSampler, integrator, *framebuffer, clip);                

            #endif                       
		}

/////////////////////////////////////
/////////Private methods ////////////
/////////////////////////////////////

		bool RaytracingRenderer::initialize()
		{			
			if(!findConfig(CONFIG_FILE))
			{
				RTFACT_LOG_ERROR_F("Failed to load RTfact configuration file: %s", CONFIG_FILE);
				return false;
			}
			
			if(!loadVolume())
			{
				RTFACT_LOG_ERROR("Failed to load volume into RTfact");
				return false;
			}			
			
			intersector.init(*rtfactScene);
			renderer.init(*rtfactScene, tree, intersector, camera);

			if(!initAccelStruct())
			{
				RTFACT_LOG_ERROR("Failed to initialize RTfact acceleration structure");
				return false;
			}									

			cameraController = new t_CameraController(camera, *window, tree.getBounds(), config);			
			cameraController.applyChanges();
			
			return true;
		}

		bool RayTracingRenderer::loadVolume(RTfact::IO::ConfigurationFile& aConfig)
		{
			RTFACT_LOG("Loading scene data... ");

			if(aConfig.exists("sceneDumpInput"))
			{				
				const std::string filePath = aConfig["sceneDumpInput"];
				std::ifstream in(filePath.c_str(), std::ios_base::binary);
				if(!in)
				{
					RTFACT_LOG_ERROR_F("Could not open file: %s", filePath);					
					return false;
				}				
				IO::Serializer serializer(in);
				serializer >> *rtfactScene;
			}
			else
			{
				#ifdef CLASSIC_SHADERS
					TriangleSceneShaderObjLoader<SceneConfig> loader;
					scene = loader.load(aConfig["scene"], aConfig);
				#else
					rtfactScene = IO::Loader<t_Scene>::load(aConfig["scene"], aConfig);
				#endif
			}

			if(rtfactScene->materials.size() == 0)
			{
				RTFACT_USER_WARNING("No materials loaded. Setting default.");

				#ifdef CLASSIC_SHADERS
					 //DiffuseShader<typename t_Scene::MaterialBase::Context> *p = new DiffuseShader<typename t_Scene::MaterialBase::Context>();

					scene->materials.push_back(
						new DiffuseShader<t_Context, Float3ConstantTexture>(
							Float3ConstantTexture(1.f, 1.f, 1.f)));
				#else
	                
					Float3ConstantTexture diffTex(1, 1, 1);
					Float3ConstantTexture ambTex(0.3, 0.3, 0.3);

					rtfactScene->materials.push_back(
						new LambertianMaterial<Float3ConstantTexture, Float3ConstantTexture>(diffTex, ambTex));

				#endif

				for(RTfact::uint i = 0; i < scene->primitives.size(); ++i)
				{
					rtfactScene->primitives[i].materialID = 0;
				}
			}

			if(rtfactScene->lights.size() == 0)
			{
				RTFACT_USER_WARNING("No lights loaded.");				
			}

			RTFACT_LOG_LINE("done.");			

			if(aConfig.exists("sceneDumpOutput"))
			{
				RTFACT_LOG("Dumping scene data... ");

				const std::string filePath = aConfig["sceneDumpOutput"];
				std::ofstream out(filePath.c_str(), std::ios_base::binary);

				if(!out)
				{
					RTFACT_LOG_ERROR_F("Could not open file: %s", filePath);
					RTFACT_LOG_LINE("failed");
				}				
				else
				{
					IO::Serializer serializer(out);
					serializer << *rtfactScene;
					RTFACT_LOG_LINE("done.");
				}				
			}
			return true;
		}

		bool RaytracingRenderer::findConfig(std::string configFilename)
		{
			try
			{
				RTfact::StaticInitializer::performInitialization();
				config.loadFromFile(configFilename);
				return true;
			}
			catch(RTfact::ProgramException& e)
			{
				RTFACT_LOG_LINE(e.getMessage());	
				return false;
			}
		}

		bool RaytracingRenderer::initAccelStruct()
		{
			if(config.exists("accelStructDumpInput"))
			{
				RTFACT_LOG("Loading acceleration structure data... ");

				const std::string filePath = aConfig["accelStructDumpInput"];
				std::ifstream in(filePath.c_str(), std::ios_base::binary);
				if(!in)
				{
					RTFACT_LOG_ERROR_F("Could not open file: %s", filePath);
					return false;
				}				

				IO::Serializer serializer(in);
				serializer >> tree;				

				RTFACT_LOG_LINE("done.");
			}
			else
			{
				RTFACT_LOG("Building acceleration structure... ");

				TimeMeasurer m;
				m.start();

				builder.build(tree, rtfactScene->primitives.begin(),
								    rtfactScene->primitives.end(),
								    boost::lambda::bind(&Triangle::getBounds, boost::lambda::_1));				

				m.end();

				RTFACT_LOG_LINE_F("done (%1% seconds).", (m.getTotalTime() / 1000.0));
			}

			if(config.exists("accelStructDumpOutput"))
			{
				RTFACT_LOG("Dumping acceleration structure data... ");

				const std::string filePath = aConfig["accelStructDumpOutput"];
				std::ofstream out(filePath.c_str(), std::ios_base::binary);
				
				if(!out)
				{
					RTFACT_LOG_ERROR_F("Could not open file: %s", filePath);
					RTFACT_LOG_LINE("failed");
				}				
				else
				{
					IO::Serializer serializer(out);
					serializer << tree;

					RTFACT_LOG_LINE("done.");
				}				
			}

			return true;
		}
	}
}
