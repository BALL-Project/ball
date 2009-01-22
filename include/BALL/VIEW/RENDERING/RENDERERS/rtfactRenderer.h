// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_RENDERING_RTFACTRENDERER_H
#define BALL_VIEW_RENDERING_RTFACTRENDERER_H

#define BALLVIEW_RTFACTRENDERER_THROW(exceptionName, message) (throw BALL::Exception::##exceptionName##(__FILE__, __LINE__, message))
#include <BALL/VIEW/RENDERING/raytracingRenderer.h>

//RTRemote proxy to RTfact
#include <RTremote/Renderer.hpp>

//#include <RTfact/Config/Init.inc.cpp>
#include <RTfact/Utils/Packets/Detail/Constants.inc.cpp>

namespace BALL
{
	namespace VIEW
    {
		/** RTfactRenderer
		 		Provides ray tracing through RTfact-RTRemote
				\ingroup ViewRendering
		*/
		class BALL_VIEW_EXPORT RTfactRenderer
			: public RaytracingRenderer
		{			
			public:			

			/// Default Constructor.
			RTfactRenderer()
            {

            }

			/// Destructor
			virtual ~RTfactRenderer()
            {

            }
            
            /************************************************************************/
            /* RaytracingRenderer methods                                          */
            /************************************************************************/
			virtual bool init(const Scene& scene)
            {                
                using RTfact::Triangle;
                using RTfact::Vec3f;
                using RTfact::Remote::GroupHandle;
                using RTfact::Remote::GeoHandle;
                using RTfact::Transform;
                using RTfact::Remote::float3;

                GroupHandle root = m_renderer.getRoot();
                TriangleVector faces;                               
                Triangle t(Vec3f<1>(0,0,0),Vec3f<1>(1,0,0),Vec3f<1>(1,1,0));
                t.texCoords[0] = std::make_pair(0.f,0.f);
                t.texCoords[1] = std::make_pair(0.f,512.f); 
                t.texCoords[2] = std::make_pair(512.f,0.f);
                faces.push_back(t);                

                for(TriangleVector::iterator it = faces.begin(); it != faces.end(); it++)
                {
                    it->normals[0]=it->normal;
                    it->normals[1]=it->normal;
                    it->normals[2]=it->normal;
                }        
                
                GeoHandle geo1 = m_renderer.createGeometry(faces);                

                GroupHandle topGroup = m_renderer.createGroup(Transform::identity());
                topGroup->add(geo1);                
                for(int i = 0; i < 6; i++) 
                {                    
                    GroupHandle subgroup=m_renderer.createGroup(Transform::translation(2.5*pow((double)2,i/2)*((i+1)%2),0,2.5*pow((double)2,i/2)*(i%2)));
                    GroupHandle group=m_renderer.createGroup(Transform::identity());

                    subgroup->add(topGroup);
                    group->add(topGroup);
                    group->add(subgroup);
                    topGroup = group; 
                }
                root->add(topGroup);

                RTfact::Remote::RTAppearanceHandle mat1 = m_renderer.createAppearance("PhongShader");                
                mat1->setParam("diffuseColor", float3(1.0, 1.0, 0));
                mat1->setParam("ambientIntensity", float3(.7, .7, .7));
                
                geo1->setAppearance(mat1);                                

                m_renderer.setCameraPosition(
                    float3(-1.48978, 4.14466, 11.4965),
                    float3(0.864694, -0.481748, 0.142207),
                    float3(0, 1, 0));            

                //root->setTransform(Transform::rotationY(0.2f));
                //root->add(geo2);
                m_renderer.createAccelStruct();

                RTfact::Remote::RTLightHandle light = m_renderer.createLight("PointLight"); 

                light->setParam("position", float3(0,10,0));
                light->setParam("intensity", float3(100,100,100));

                return true;
            }
													
            virtual String getRenderer()	
                throw()
            {
                return "RTfact-RTRemote Ray Tracer";
            }

            virtual void formatUpdated()
            {

            }

            virtual void prepareBufferedRendering(const Stage& stage)
            {

            }

            virtual void renderToBufferImpl(FrameBufferPtr buffer)
            {               
                FrameBufferFormat fmt = buffer->getFormat();                
                if(fmt.getPixelFormat() == PixelFormat::RGBF_96)
                {
                    renderImpl<float>(reinterpret_cast<float*>(buffer->getData()), fmt.getWidth(), fmt.getHeight(), fmt.getPitch());                    
                }
            }
        private:

            RTfact::Remote::Renderer m_renderer;

            template<typename taPixelType>
            void renderImpl(taPixelType* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch);

            template<>
            void renderImpl(float* buffer, const unsigned int width, const unsigned int height, const unsigned int pitch)
            {
                //m_renderer.attachFrameBuffer(width, height, pitch, buffer);                    
                //m_renderer.renderToBuffer();             

                
                for (unsigned int j = 0; j < height; j++)
                {
                    for (unsigned int i = 0; i < width; i++)
                    {                        
                        if ( (((i / 64) % 2) == 0) && (((j / 64) % 2) == 0))
                        {                                                        
                            buffer[3 * (j*width + i) + 0] = 1;
                            buffer[3 * (j*width + i) + 1] = 0;
                            buffer[3 * (j*width + i) + 2] = 0;                            
                        }
                        else
                        {                            
                            buffer[3 * (j*width + i) + 0] = 0;
                            buffer[3 * (j*width + i) + 1] = 1;
                            buffer[3 * (j*width + i) + 2] = 1;                            
                        }
                    }
                }                 
            }

		};


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_RENDERING_RTFACTRENDERER_H
