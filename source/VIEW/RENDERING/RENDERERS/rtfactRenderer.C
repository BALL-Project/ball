#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>

#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>

#include <RTfact/Config/Init.hpp>
#include <RTfact/Utils/Transform.hpp>

using RTfact::Vec3f;
using RTfact::Remote::GroupHandle;
using RTfact::Remote::GeoHandle;
using RTfact::Transform;
using RTfact::Matrix4f;
using RTfact::Remote::float3;
using RTfact::Remote::RTAppearanceHandle;

namespace BALL
{
	namespace VIEW
	{
		bool RTfactRenderer::init(Scene& scene)
		{		     
			Renderer::init(scene);

			GroupHandle root = m_renderer.getRoot();
			
			// prepare the sphere template
			TriangulatedSphere sphere_template;
			sphere_template.pentakisDodecaeder();
			sphere_template.refine(1, true);

			sphere_template.exportSurface(sphere_template_);

			// prepare the tube template
			TriangulatedSurface* tube_template = TriangulatedSurface::createTube(18, 0, false, true);
			tube_template->exportSurface(tube_template_);
			delete (tube_template);
				
			objects_.clear();
			rtfact_needs_update_ = false;

			return true;
		}

		void RTfactRenderer::setLights(bool reset_all)
		{
			if (reset_all)
			{
				for (Position i=0; i<lights_.size(); ++i)
					m_renderer.removeLight(lights_[i]);
				lights_.clear();
			}

			Vector3 direction, light_position, attenuation;

			Size current_light=0; 
			Size num_lights=lights_.size();

			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();

			for (; it != stage_->getLightSources().end(); ++it, ++current_light)
			{
				if (current_light >= num_lights)
				{
					RTfact::Remote::RTLightHandle light; 
					switch (it->getType())
					{
						case LightSource::DIRECTIONAL:
							light = m_renderer.createLight("DirectionalLight"); 
							break;
						case LightSource::POSITIONAL:
							light = m_renderer.createLight("PointLight"); 
							break;
						default:
							std::cerr << "Light source type not supported!" << std::endl;
							break;
					}
					lights_.push_back(light);
				}

				switch (it->getType())
				{
					case LightSource::DIRECTIONAL:
						direction = it->getDirection();
						if (it->isRelativeToCamera())
							direction = stage_->calculateAbsoluteCoordinates(direction);

						lights_[current_light]->setParam("direction", float3(direction.x, direction.y, direction.z));
						break;
					case LightSource::POSITIONAL:
						light_position = it->getPosition();
						if (it->isRelativeToCamera())
						{
							light_position = stage_->calculateAbsoluteCoordinates(it->getPosition())+stage_->getCamera().getViewPoint();
						}
						lights_[current_light]->setParam("position", float3(light_position.x, light_position.y, light_position.z));

						attenuation = it->getAttenuation();
						lights_[current_light]->setParam("attenuation", float3(attenuation.x, attenuation.y, attenuation.z));
						break;
					default:
						std::cerr << "Light source type not supported!" << std::endl;
						break;
				}
				float intensity = it->getIntensity()*100;
				ColorRGBA const& color = it->getColor();

				lights_[current_light]->setParam("intensity", float3((float)color.getRed()*intensity,(float)color.getGreen()*intensity,(float)color.getBlue()*intensity));
			}
		}

		void RTfactRenderer::updateCamera(const Camera* camera)
		{
			// the renderer should be paused whenever the camera has been updated
			if (camera == 0) camera = &(stage_->getCamera());

			Vector3 const& position = camera->getViewPoint();
			Vector3 const& view_vector = camera->getViewVector();
			Vector3 const& look_up = camera->getLookUpVector();

			if (use_continuous_loop_)
			{
				if (   ((last_camera_position - position   ).getSquareLength() > 1e-5)
						 ||((last_camera_view_vec - view_vector).getSquareLength() > 1e-5)
						 ||((last_camera_lookup   - look_up    ).getSquareLength() > 1e-5))
						m_renderer.pauseAnimation(false);
			}

			m_renderer.setCameraPosition(float3(position.x, position.y, position.z),
																	 float3(view_vector.x, view_vector.y, view_vector.z),
																	 float3(look_up.x, look_up.y, look_up.z));

			// lights that are relative to the camera need to have their position updated
			if (lights_.size() == 0) return; // TEST
			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
			Size current_light=0;
			Vector3 light_position, direction;
			for (; it != stage_->getLightSources().end(); ++it, ++current_light)
			{
				switch (it->getType())
				{
					case LightSource::DIRECTIONAL:
						direction = it->getDirection();
						if (it->isRelativeToCamera())
							direction = stage_->calculateAbsoluteCoordinates(direction);

						lights_[current_light]->setParam("direction", float3(direction.x, direction.y, direction.z));
					case LightSource::POSITIONAL:
						light_position = it->getPosition();
						if (it->isRelativeToCamera())
						{
							light_position = stage_->calculateAbsoluteCoordinates(it->getPosition())+stage_->getCamera().getViewPoint();
						}
						lights_[current_light]->setParam("position", float3(light_position.x, light_position.y, light_position.z));
						break;
					default:
						break;
				}
			}
			last_camera_position  = position;
			last_camera_view_vec  = view_vector; 
			last_camera_lookup    = look_up;
		}

		void RTfactRenderer::updateBackgroundColor() 
		{
			m_renderer.setEnvironmentColor(stage_->getBackgroundColor().getRed(),
																		 stage_->getBackgroundColor().getGreen(),
																		 stage_->getBackgroundColor().getBlue());
		}
			
		void RTfactRenderer::prepareBufferedRendering(const Stage& stage)
		{
			// this function is not needed for this kind of raytracer
		}

		void RTfactRenderer::bufferRepresentation(const Representation& rep)
		{
			if (objects_.find(&rep) != objects_.end())
			{
				// TODO: handle the update more gracefully!
				removeRepresentation(rep);
			}

			RTfactData rt_data;

			List<GeometricObject*>::ConstIterator it;
			for (it =  rep.getGeometricObjects().begin();
					 it != rep.getGeometricObjects().end();
					 it++)
			{
				if (RTTI::isKindOf<Mesh>(**it))
				{
					Mesh const& mesh = *(const Mesh*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(mesh.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(mesh.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(mesh.triangle[0]));


					RTAppearanceHandle material = m_renderer.createAppearance("PhongShader");
					updateMaterialFromStage(material);

					GeoHandle handle;

					float const* colors = 0;
					if (mesh.colors.size() > 1)
					{
						colors  = reinterpret_cast<float const*>(&(mesh.colors[0]));
						material->setParam("useVertexColor", true);

						handle = m_renderer.createGeometry(vertices, normals, colors, (const unsigned int*)indices, (unsigned int)mesh.triangle.size(), material);
					} 
					else 
					{
						ColorRGBA const &c = (mesh.colors.size() == 1) ? mesh.colors[0] : ColorRGBA(1., 1., 1., 1.);

						material->setParam("diffuseColor", float3(c.getRed(), c.getGreen(), c.getBlue()));
						material->setParam("useVertexColor", false);

						handle = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)mesh.triangle.size(), material);
					}

					GroupHandle meshGroup = m_renderer.createGroup(Transform::identity());
					meshGroup->add(handle);                
					m_renderer.getRoot()->add(meshGroup);

					rt_data.top_group_handles.push_back(meshGroup);
					rt_data.object_handles.push_back(handle);
					rt_data.material_handles.push_back(material);

					rtfact_needs_update_ = true;
				} 

				if (RTTI::isKindOf<Sphere>(**it))
				{
					Sphere const& sphere = *(const Sphere*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(sphere_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(sphere_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(sphere_template_.triangle[0]));

					ColorRGBA const& color = sphere.getColor();

					RTAppearanceHandle material = m_renderer.createAppearance("PhongShader");
					updateMaterialFromStage(material);

					material->setParam("diffuseColor", float3(color.getRed(), color.getGreen(), color.getBlue()));
					material->setParam("useVertexColor", false);

					GeoHandle handle   = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)sphere_template_.triangle.size(), material);

					Vector3 const& sphere_pos = sphere.getPosition();
					float radius = sphere.getRadius();

					GroupHandle sphereGroup = m_renderer.createGroup(  Transform::translation(sphere_pos.x, sphere_pos.y, sphere_pos.z)
							*Transform::scale(Vec3f<1>(radius, radius, radius)));
					sphereGroup->add(handle);
					m_renderer.getRoot()->add(sphereGroup);

					rt_data.top_group_handles.push_back(sphereGroup);
					rt_data.object_handles.push_back(handle);
					rt_data.material_handles.push_back(material);

					rtfact_needs_update_ = true;
				}

				if (RTTI::isKindOf<TwoColoredTube>(**it))
				{
					TwoColoredTube const& old_tube = *(const TwoColoredTube*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

					// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
					ColorRGBA const& color1 = old_tube.getColor();
					ColorRGBA const& color2 = old_tube.getColor2();

					RTAppearanceHandle material_1 = m_renderer.createAppearance("PhongShader");
					updateMaterialFromStage(material_1);

					material_1->setParam("diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					material_1->setParam("useVertexColor", false);

					GeoHandle handle_1 = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)tube_template_.triangle.size(), material_1);

					if (color1 == color2)
					{
						GroupHandle tubeGroup = transformTube(old_tube);
						tubeGroup->add(handle_1);

						m_renderer.getRoot()->add(tubeGroup);

						rt_data.top_group_handles.push_back(tubeGroup);
						rt_data.object_handles.push_back(handle_1);
						rt_data.material_handles.push_back(material_1);
					} 
					else 
					{
						RTAppearanceHandle material_2 = m_renderer.createAppearance("PhongShader");
						updateMaterialFromStage(material_2);

						material_2->setParam("diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						material_2->setParam("useVertexColor", false);

						GeoHandle handle_2 = m_renderer.createGeometry(vertices, normals, (const unsigned int*)indices, (unsigned int)tube_template_.triangle.size(), material_2);

						// NOTE: Just copying tube would be highly dangerous; vertex2 can store pointers
						//       to the vertices instead of using its own, and these are copied as well!
						TwoColoredTube new_tube_1, new_tube_2;

						new_tube_1.setVertex1(old_tube.getVertex1());
						new_tube_1.setVertex2(old_tube.getMiddleVertex());
						new_tube_1.setRadius(old_tube.getRadius());

						new_tube_2.setVertex1(old_tube.getMiddleVertex());
						new_tube_2.setVertex2(old_tube.getVertex2());
						new_tube_2.setRadius(old_tube.getRadius());

						GroupHandle all_group = m_renderer.createGroup(Transform::identity());

						GroupHandle tubeGroup_1 = transformTube(new_tube_1);
						tubeGroup_1->add(handle_1);

						GroupHandle tubeGroup_2 = transformTube(new_tube_2);
						tubeGroup_2->add(handle_2);

						all_group->add(tubeGroup_1);
						all_group->add(tubeGroup_2);

						m_renderer.getRoot()->add(all_group);

						rt_data.top_group_handles.push_back(all_group);
						rt_data.object_handles.push_back(handle_1);
						rt_data.object_handles.push_back(handle_2);
						rt_data.material_handles.push_back(material_1);
						rt_data.material_handles.push_back(material_2);
					}

					rtfact_needs_update_ = true;
				}
			}

			if (rtfact_needs_update_ && use_continuous_loop_)
				m_renderer.pauseAnimation(false);

			objects_[&rep] = rt_data;
		}

		void RTfactRenderer::removeRepresentation(Representation const& rep)
		{
			if (objects_.find(&rep) != objects_.end())
			{
				// 	     - find out if this also deletes the geometries and materials
				RTfactData& rt_data = objects_[&rep];
				GroupHandle root = m_renderer.getRoot();

				for (Position i=0; i<rt_data.top_group_handles.size(); ++i)
				{
					root->remove(rt_data.top_group_handles[i]);
				}

				objects_.erase(&rep);

				if (use_continuous_loop_)
					m_renderer.pauseAnimation(false);
			}
		}

		void RTfactRenderer::useContinuousLoop(bool use_loop)
		{
			Renderer::useContinuousLoop(use_loop);

			m_renderer.pauseAnimation(use_loop);
		}

		void RTfactRenderer::renderToBufferImpl(FrameBufferPtr buffer)
		{		    
			Stage const& stage = *(scene_->getStage());

			// deactivate hidden representations
			for (HashMap<Representation const*, RTfactData>::iterator it = objects_.begin(); it != objects_.end(); ++it)
			{
				if (it->first->isHidden())
					continue; // TODO: code for deactivation
				else
					continue; // TODO: code for reactivation
			}

			if (rtfact_needs_update_)
			{
				m_renderer.createAccelStruct();
				rtfact_needs_update_ = false;
			}

			FrameBufferFormat fmt = buffer->getFormat();		    
			if ((fmt.getPixelFormat() == PixelFormat::RGBF_96) && (objects_.size() != 0))
			{
				m_renderer.attachFrameBuffer((float*)buffer->getData(), 3, fmt.getWidth(), fmt.getHeight());
				m_renderer.renderToBuffer();

				if (use_continuous_loop_)
					m_renderer.pauseAnimation(true);
			}
		}

		void RTfactRenderer::updateMaterialForRepresentation(Representation const* rep)
		{
			if (objects_.find(rep) != objects_.end())
			{
				RTfactData& rt_data = objects_[rep];

				for (Position i=0; i<rt_data.material_handles.size(); ++i)
				{
					updateMaterialFromStage(rt_data.material_handles[i]);
				}
			}
		}

		void RTfactRenderer::updateMaterialForRepresentation(Representation const* rep, const Stage::RaytracingMaterial& new_material)
		{
			if (objects_.find(rep) != objects_.end())
			{
				RTfactData& rt_data = objects_[rep];

				for (Position i=0; i<rt_data.material_handles.size(); ++i)
				{
					convertMaterial(new_material, rt_data.material_handles[i]);
				}
			}
		}


		GroupHandle RTfactRenderer::transformTube(const TwoColoredTube& tube) 
		{
			Vector3 vec = tube.getVertex2() - tube.getVertex1();
			const double len = vec.getLength();
			const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
			const float radius = tube.getRadius();

			Matrix4x4 trafo = Matrix4x4::getIdentity();
			//Rotate the vector around the normal
			vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

			trafo.rotate(Angle(-angle), vec.y, -vec.x, 0);

			Matrix4f matrix(trafo.m11, trafo.m12, trafo.m13, trafo.m14,
											trafo.m21, trafo.m22, trafo.m23, trafo.m24,
											trafo.m31, trafo.m32, trafo.m33, trafo.m34,
											trafo.m41, trafo.m42, trafo.m43, trafo.m44);	

			Vector3 const& midpoint = tube.getVertex1();

			return m_renderer.createGroup(	Transform::translation(midpoint.x, midpoint.y, midpoint.z)
																		 *matrix*Transform::scale(Vec3f<1>(radius, radius, len)));
		}

		void RTfactRenderer::updateMaterialFromStage(RTAppearanceHandle& material)
		{
			Stage::RaytracingMaterial const& rt_material = scene_->getStage()->getRTMaterial();
			convertMaterial(rt_material, material);
		}

		void RTfactRenderer::convertMaterial(Stage::RaytracingMaterial const& rt_material, RTAppearanceHandle& material)
		{
			// ambience
			float red   = (float)rt_material.ambient_color.getRed()   * rt_material.ambient_intensity;
			float blue  = (float)rt_material.ambient_color.getBlue()  * rt_material.ambient_intensity;
			float green = (float)rt_material.ambient_color.getGreen() * rt_material.ambient_intensity;

			material->setParam("ambientIntensity", float3(red, blue, green));

			// specularity
			red   = (float)rt_material.specular_color.getRed()   * rt_material.specular_intensity;
			blue  = (float)rt_material.specular_color.getBlue()  * rt_material.specular_intensity;
			green = (float)rt_material.specular_color.getGreen() * rt_material.specular_intensity;

			material->setParam("specularColor", float3(red, blue, green));

			// reflectiveness
			red   = (float)rt_material.reflective_color.getRed()   * rt_material.reflective_intensity;
			blue  = (float)rt_material.reflective_color.getBlue()  * rt_material.reflective_intensity;
			green = (float)rt_material.reflective_color.getGreen() * rt_material.reflective_intensity;

			material->setParam("reflective", float3(red, blue, green));

			// shininess
			material->setParam("shininess", rt_material.shininess);

			// transparency
			material->setParam("alpha", (100.f - rt_material.transparency) * 0.01f);
		}

		std::vector<float> RTfactRenderer::intersectRaysWithGeometry(const std::vector<Vector3>& origins,
		                                                             const std::vector<Vector3>& directions)
		{
			if (origins.size() != directions.size())
			{
				Log.error() << "RTfactRenderer::intersectRaysWithGeometry: sizes of origins/directions vectors disagree!" << std::endl;

				return std::vector<float>();
			}

			std::vector<float> results(origins.size());

			m_renderer.intersectRays(reinterpret_cast<const float*>(&origins[0]), 
			                         reinterpret_cast<const float*>(&directions[0]), 
															 origins.size(),
															 reinterpret_cast<float*>(&results[0]));

			return results;
		}

	}
}
