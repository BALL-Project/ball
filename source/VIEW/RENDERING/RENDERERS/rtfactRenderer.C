#include <BALL/STRUCTURE/triangulatedSurface.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>

#include <BALL/VIEW/RENDERING/RENDERERS/rtfactRenderer.h>

#include <RTfact/Config/Init.hpp>

#include <RTpie/Utils/TransformTools.hpp>

#include <QtGui/QImage>

#include <list>

static const float LINE_RADIUS = 0.02;

using RTfact::Vec3f;
using RTpieCpp::GeometryHandle;
using RTpieCpp::MeshHandle;
using RTpieCpp::PickTaskHandle;
using RTpieCpp::IntersectTaskHandle;
using RTpieCpp::LightHandle;
using RTpieCpp::Image2DHandle;
using RTpieCpp::BufferHandle;
using RTpieCpp::float3;
using RTpieCpp::InstanceHandle;
using RTpieCpp::FrameBufferHandle;
using RTpieCpp::SceneHandle;
using RTpieCpp::AppearanceHandle;

namespace BALL
{
	namespace VIEW
	{
		bool RTfactRenderer::init(Scene& scene)
		{
			Renderer::init(scene);

			// create rtpie handles
			rayTracer = RTpieCpp::CreateRayTracerHandle();
			sceneHandle = RTpieCpp::CreateSceneHandle();

			rayTracer.attachScene(sceneHandle);

			renderTask = rayTracer.createRenderTask();
			renderTask.setOverSamplingRate(1);
			renderTask.setAccumulatePixels(true);
			renderTask.setMaxDepth(20);

			pickTask = rayTracer.createPickTask();

			cameraHandle = RTpieCpp::CreateCameraHandle();

			renderTask.setCamera(cameraHandle);

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

		GeometricObject* RTfactRenderer::pickObject(Position x, Position y)
		{
			PickTaskHandle pickTask = rayTracer.createPickTask();
			pickTask.setCamera( cameraHandle );
			pickTask.pick(
				static_cast<int>(x),
				static_cast<int>(y),
				renderBuffer.getWidth(), renderBuffer.getHeight());

			//returns true if some object was picked
			if(pickTask.hasResult())
			{
				//for all scene object
				HashMap<RTpieCpp::InstanceHandle, GeometricObject*>::iterator geo = geometric_objects_.begin();
				for (; geo != geometric_objects_.end(); ++geo)
				{
					if (pickTask.getInstance() == geo->first)
					{
						return geo->second;
					}
				}
			}

			return NULL;
		}

		// TODO: this should be done in parallel, or directly in the ray tracer
		void RTfactRenderer::pickObjects(Position x1, Position y1, Position x2, Position y2,
		                                 std::list<GeometricObject*>& objects)
		{
			if (x1 > x2) std::swap(x1, x2);
			if (y1 > y2) std::swap(y1, y2);

			for (Position x=x1; x<=x2; ++x)
			{
				for (Position y=y1; y<=y2; ++y)
				{
					GeometricObject* result = pickObject(x, y);
					if (result)
					{
						objects.push_back(result);
					}
				}
			}
		}

		void RTfactRenderer::setSize(float width, float height)
		{
			width_ = width;
			height_ = height;

			if (width > height)
			{
				x_scale_ = width / (height * 2);
				y_scale_ = 0.5;
			}
			else
			{
				x_scale_ = 0.5;
				y_scale_ = height / (width * 2);
			}

			setFrustum(1.5f, RTfact::Packet<1,float>::C_INFINITY, -2.f * x_scale_, 2.f * x_scale_, 2.f * y_scale_, -2.f * y_scale_);
		}

		void RTfactRenderer::setupStereo(float eye_separation, float focal_length)
		{
			// TODO: - make near and far clip configurable!!!
			//       - keep the same frustrum until either the size or the stereo settings change

			float nearf    = 1.5f;
			float ndfl    = nearf / focal_length;
			float left    = -2.0 * stereo_frustum_conversion_width_  * x_scale_;
			float right   =  2.0 * stereo_frustum_conversion_width_  * x_scale_;
			float bottom 	= -2.0 * stereo_frustum_conversion_height_ * y_scale_;
			float top    	=  2.0 * stereo_frustum_conversion_height_ * y_scale_;

			float new_left   = 2*left  - eye_separation * ndfl;
			float new_right  = 2*right - eye_separation * ndfl;

			if (stage_->getCamera().getProjectionMode() == Camera::PERSPECTIVE)
				setFrustum(nearf, RTfact::Packet<1,float>::C_INFINITY, left, right, top, bottom);
			else
			{
				//glOrtho(new_left * orthographic_zoom_, new_right * orthographic_zoom_,
				//         bottom_ * orthographic_zoom_,      top_ * orthographic_zoom_, near_, far_);
			}
		}

		void RTfactRenderer::getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f)
		{
		  cameraHandle.getFrustum(&left_f, &right_f, &top_f, &bottom_f, &near_f, &far_f);

			left_f   *= 2.f*x_scale_;
			right_f  *= 2.f*x_scale_;
			top_f    *= 2.f*y_scale_;
			bottom_f *= 2.f*y_scale_;
		}

		void RTfactRenderer::setFrustum(float near_f, float far_f, float left_f, float right_f, float top_f, float bottom_f)
		{
			cameraHandle.setFrustum(
				left_f/ (2.f*x_scale_), right_f/ (2.f*x_scale_),
				top_f/ (2.f*y_scale_), bottom_f/ (2.f*y_scale_),
				near_f, far_f);
		}

		void RTfactRenderer::setLights(bool reset_all)
		{
			if (reset_all)
			{
				for (Position i=0; i<lights_.size(); ++i)
				  lights_[i].clear();
				lights_.clear();
			}

			Vector3 direction, light_position, attenuation;

			Size current_light=0;
			Size num_lights=lights_.size();

			std::list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			for (; it != stage_->getLightSources().end(); ++it, ++current_light)
			{
				if (current_light >= num_lights)
				{
					LightHandle light;
					switch (it->getType())
					{
						case LightSource::DIRECTIONAL:
						  light = sceneHandle.createLight("DirectionalLight");
							break;
						case LightSource::POSITIONAL:
						  light = sceneHandle.createLight("PointLight");
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

						lights_[current_light].setParam3f("direction", float3(direction.x, direction.y, direction.z));
						break;
					case LightSource::POSITIONAL:
						light_position = it->getPosition();
						if (it->isRelativeToCamera())
						{
							light_position = stage_->calculateAbsoluteCoordinates(it->getPosition())+stage_->getCamera().getViewPoint();
						}
						lights_[current_light].setParam3f("position", float3(light_position.x, light_position.y, light_position.z));

						attenuation = it->getAttenuation();
						lights_[current_light].setParam3f("attenuation", float3(attenuation.x, attenuation.y, attenuation.z));
						break;
					default:
						std::cerr << "Light source type not supported!" << std::endl;
						break;
				}

				float intensity = it->getIntensity();
				ColorRGBA const& color = it->getColor();

				lights_[current_light].setParam3f("intensity", float3((float)color.getRed()*intensity,(float)color.getGreen()*intensity,(float)color.getBlue()*intensity));
			}
		}

		void RTfactRenderer::updateCamera(const Camera* camera)
		{

			// the renderer should be paused whenever the camera has been updated
			if (camera == 0) camera = &(stage_->getCamera());

			Vector3 const& position = camera->getViewPoint();
			// RTfact relies on a normalized view vector, so we have to normalize it prior to handing it to RTfact
			// TODO: Store a normalized view vector in our Camera, mind project files
			Vector3  view_vector = camera->getViewVector();
			view_vector.normalize();
			Vector3 const& look_up = camera->getLookUpVector();

			if (use_continuous_loop_)
			{
				if (   ((last_camera_position - position   ).getSquareLength() > 1e-5)
						 ||((last_camera_view_vec - view_vector).getSquareLength() > 1e-5)
						 ||((last_camera_lookup   - look_up    ).getSquareLength() > 1e-5))
						renderTask.setAccumulatePixels(true);
			}

			cameraHandle.setPosition( float3(position.x, position.y, position.z) );
			cameraHandle.setDirection( float3(view_vector.x, view_vector.y, view_vector.z) );
			cameraHandle.setUpVector( float3(look_up.x, look_up.y, look_up.z) );

			if (lights_.size() != stage_->getLightSources().size()) return;

			// lights that are relative to the camera need to have their position updated
			if (lights_.size() == 0) return; // TEST
			std::list<LightSource>::const_iterator it = stage_->getLightSources().begin();
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

						if(!(lights_[current_light].getParam3f("direction") == float3(direction.x, direction.y, direction.z)))
							lights_[current_light].setParam3f("direction", float3(direction.x, direction.y, direction.z));
					case LightSource::POSITIONAL:
						light_position = it->getPosition();
						if (it->isRelativeToCamera())
						{
							light_position = stage_->calculateAbsoluteCoordinates(it->getPosition())+stage_->getCamera().getViewPoint();
						}

						if(!(lights_[current_light].getParam3f("position") == float3(light_position.x, light_position.y, light_position.z)))
							lights_[current_light].setParam3f("position", float3(light_position.x, light_position.y, light_position.z));
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
			sceneHandle.setEnvironmentColor(stage_->getBackgroundColor().getRed(),
															  stage_->getBackgroundColor().getGreen(),
																stage_->getBackgroundColor().getBlue(), 1);
		}

		void RTfactRenderer::setupEnvironmentMap(const QImage& image)
		{
			// convert the image into an RTfact - compatible array
			Size num_pixels = image.width()*image.height();

			float* rtfact_env_map = new float[3*num_pixels];

			Position current_pixel = 0;
			for (Position i=0; i<image.width(); ++i)
			{
				for (Position j=0; j<image.height(); ++j, ++current_pixel)
				{
					QRgb pixel = image.pixel(i,j);

					rtfact_env_map[3*current_pixel]   = qRed(pixel)/255.;
					rtfact_env_map[3*current_pixel+1] = qGreen(pixel)/255.;
					rtfact_env_map[3*current_pixel+2] = qBlue(pixel)/255.;
				}
			}

			RTfact::uint colorBufferLength = image.width() * image.height() * 3 * sizeof(float);
			BufferHandle colorBuffer = RTpieCpp::CreateBufferHandleUseData(colorBufferLength, rtfact_env_map);
			Image2DHandle imageHandle = RTpieCpp::CreateImage2DHandleUseData(
								RTfact::RTpie::IImage2D::COMPONENT_FLOAT,
								3,
								image.width(), image.height(),
								colorBuffer
						);

			sceneHandle.setEnvironmentTexture(imageHandle);

			delete[] (rtfact_env_map);
		}

		void RTfactRenderer::prepareBufferedRendering(const Stage& stage)
		{
			// this function is not needed for this kind of raytracer
		}

		void RTfactRenderer::bufferRepresentation(const Representation& rep)
		{
			if (rep.getGeometricObjects().empty())
				return;

			if (rep.isHidden())
			{
				if (rep.needsUpdate())
				{
					// if the representation has been changed while it was hidden, we need
					// to recreate it from scratch the next time it is enabled
					//
					// NOTE: it is safe to call removeRepresentation even if the representation
					//       has not yet been added
					removeRepresentation(rep);
				}
				return;
			}

			if (objects_.find(&rep) != objects_.end())
			{
				// was the representation previously disabled and now just needs enabling?
				if (objects_[&rep].has_been_disabled)
				{
					RTfactData& rt_data = objects_[&rep];

					// iterate over all top group handles and add them to the root again to make them visible
					for (Position i=0; i<rt_data.object_handles.size(); ++i)
					{
						rt_data.instance_handles.push_back(rt_data.object_handles[i].createInstance());
					}

					objects_[&rep].has_been_disabled = false;
					rtfact_needs_update_ = true;

					return;
				}
				else
				{
					// TODO: handle the update more gracefully!
					removeRepresentation(rep);
				}
			}

			RTfactData rt_data;
			rt_data.has_been_disabled = false;

			Stage::RaytracingMaterial rt_material = scene_->getStage()->getRTMaterial();
			if (rep.hasProperty("RTFact::Material"))
			{
				NamedProperty rt_mat_property = rep.getProperty("RTFact::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
				rt_material = *dynamic_cast<Stage::RaytracingMaterial*>(mat_ptr.get());
			}

			std::list<GeometricObject*>::const_iterator it;
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

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());

					//
					float const* colors = 0;
					if (mesh.colors.size() > 1)
					{
						colors  = reinterpret_cast<float const*>(&(mesh.colors[0]));
						rt_data.material_handles.back().setParamb("useVertexColor", true);

						//colors conversion (rtfact accepts RGB)
						float* rgbcolors = new float[3*mesh.colors.size()];
						for(int i =0; i < mesh.colors.size(); i++)
						{
							rgbcolors[i * 3 + 0] = colors[i * 4 + 0];
							rgbcolors[i * 3 + 1] = colors[i * 4 + 1];
							rgbcolors[i * 3 + 2] = colors[i * 4 + 2];
						}

						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setPrimitives(
												(unsigned int)mesh.triangle.size(),
												indices,
												vertices,
												normals,
												rgbcolors, 0);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						delete[] rgbcolors;
					}
					else
					{
						ColorRGBA const &c = (mesh.colors.size() == 1) ? mesh.colors[0] : ColorRGBA(1., 1., 1., 1.);

						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(c.getRed(), c.getGreen(), c.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setPrimitives(
												(unsigned int)mesh.triangle.size(),
												indices,
												vertices,
												normals,
												0, 0);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());
					}

					if (!rep.isHidden()) {
						rt_data.instance_handles.push_back(
						    rt_data.object_handles.back().createInstance());
					}

					geometric_objects_[rt_data.instance_handles.back()] = *it;

					rtfact_needs_update_ = true;
				}

				if (RTTI::isKindOf<Sphere>(**it))
				{
					Sphere const& sphere = *(const Sphere*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(sphere_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(sphere_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(sphere_template_.triangle[0]));

					ColorRGBA const& color = sphere.getColor();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color.getRed(), color.getGreen(), color.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.object_handles.push_back(
							sceneHandle.createGeometry());
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					rt_data.mesh_handles.back().setPrimitives(
											(unsigned int)sphere_template_.triangle.size(),
											indices,
											vertices,
											normals,
											0, 0);
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					//
					Vector3 const& sphere_pos = sphere.getPosition();
					float radius = sphere.getRadius();

					float mat[16] = {
						radius, 0, 0, 0,
						0, radius, 0, 0,
						0, 0, radius, 0,
						sphere_pos.x, sphere_pos.y, sphere_pos.z, 1};

					if (!rep.isHidden()) {
						rt_data.instance_handles.push_back(
						    rt_data.object_handles.back().createInstance());
						rt_data.instance_handles.back().setTransform(mat, NULL);
					}

					geometric_objects_[rt_data.instance_handles.back()] = *it;

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

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					convertMaterial(rt_material, rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.object_handles.push_back(
							sceneHandle.createGeometry());
					geometric_objects_[rt_data.instance_handles.back()] = *it;
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					rt_data.mesh_handles.back().setPrimitives(
											(unsigned int)tube_template_.triangle.size(),
											indices,
											vertices,
											normals,
											0, 0);
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					if (color1 == color2)
					{

						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());

							transformTube(old_tube, rt_data.instance_handles.back());
						}

					}
					else
					{
						//
						rt_data.material_handles.push_back(
								sceneHandle.createAppearance("PhongShader"));
						convertMaterial(rt_material, rt_data.material_handles.back());
						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						//
						RTpieCpp::GeometryHandle& lastGeom = rt_data.object_handles.back();
						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						geometric_objects_[rt_data.instance_handles.back()] = *it;
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setPrimitives(
												(unsigned int)tube_template_.triangle.size(),
												indices,
												vertices,
												normals,
												0, 0);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						// NOTE: Just copying tube would be highly dangerous; vertex2 can store pointers
						//       to the vertices instead of using its own, and these are copied as well!
						TwoColoredTube new_tube_1, new_tube_2;

						new_tube_1.setVertex1(old_tube.getVertex1());
						new_tube_1.setVertex2(old_tube.getMiddleVertex());
						new_tube_1.setRadius(old_tube.getRadius());

						new_tube_2.setVertex1(old_tube.getMiddleVertex());
						new_tube_2.setVertex2(old_tube.getVertex2());
						new_tube_2.setRadius(old_tube.getRadius());

						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									lastGeom.createInstance());
              transformTube(new_tube_1, rt_data.instance_handles.back());

							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());
              transformTube(new_tube_2, rt_data.instance_handles.back());
						}
					}

					rtfact_needs_update_ = true;
				}

				if (RTTI::isKindOf<TwoColoredLine>(**it))
				{
					TwoColoredLine const& old_line = *(const TwoColoredLine*)*it;

					float const* vertices = reinterpret_cast<float const*>(&(tube_template_.vertex[0]));
					float const* normals  = reinterpret_cast<float const*>(&(tube_template_.normal[0]));
					Index const* indices  = reinterpret_cast<Index const*>(&(tube_template_.triangle[0]));

					// we will produce two tubes using the same vertex/normal/color values, just with the correct offsets
					ColorRGBA const& color1 = old_line.getColor();
					ColorRGBA const& color2 = old_line.getColor2();

					//
					rt_data.material_handles.push_back(
					    sceneHandle.createAppearance("PhongShader"));
					updateMaterialFromStage(rt_data.material_handles.back());
					rt_data.material_handles.back().setParam3f(
							"diffuseColor", float3(color1.getRed(), color1.getGreen(), color1.getBlue()));
					rt_data.material_handles.back().setParamb("useVertexColor", false);

					//
					rt_data.object_handles.push_back(
							sceneHandle.createGeometry());
					geometric_objects_[rt_data.instance_handles.back()] = *it;
					rt_data.mesh_handles.push_back(
							rt_data.object_handles.back().createMesh());
					rt_data.mesh_handles.back().setPrimitives(
											(unsigned int)tube_template_.triangle.size(),
											indices,
											vertices,
											normals,
											0, 0);
					rt_data.mesh_handles.back().setAppearance(
							rt_data.material_handles.back());

					if (color1 == color2)
					{
						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());

							transformLine(old_line, rt_data.instance_handles.back());
						}
					}
					else
					{
						//
						rt_data.material_handles.push_back(
								sceneHandle.createAppearance("PhongShader"));
						convertMaterial(rt_material, rt_data.material_handles.back());
						rt_data.material_handles.back().setParam3f(
								"diffuseColor", float3(color2.getRed(), color2.getGreen(), color2.getBlue()));
						rt_data.material_handles.back().setParamb("useVertexColor", false);

						//
						RTpieCpp::GeometryHandle& lastGeom = rt_data.object_handles.back();
						rt_data.object_handles.push_back(
								sceneHandle.createGeometry());
						geometric_objects_[rt_data.instance_handles.back()] = *it;
						rt_data.mesh_handles.push_back(
								rt_data.object_handles.back().createMesh());
						rt_data.mesh_handles.back().setPrimitives(
												(unsigned int)tube_template_.triangle.size(),
												indices,
												vertices,
												normals,
												0, 0);
						rt_data.mesh_handles.back().setAppearance(
								rt_data.material_handles.back());

						// NOTE: Just copying tube would be highly dangerous; vertex2 can store pointers
						//       to the vertices instead of using its own, and these are copied as well!
						TwoColoredLine new_line_1, new_line_2;

						new_line_1.setVertex1(old_line.getVertex1());
						new_line_1.setVertex2(old_line.getMiddleVertex());
						//new_line_1.setRadius(LINE_RADIUS);//old_line.getRadius());

						new_line_2.setVertex1(old_line.getMiddleVertex());
						new_line_2.setVertex2(old_line.getVertex2());
						//new_line_2.setRadius(LINE_RADIUS);//old_line.getRadius());

						if (!rep.isHidden()) {
							rt_data.instance_handles.push_back(
									lastGeom.createInstance());
              transformLine(new_line_1, rt_data.instance_handles.back());

							rt_data.instance_handles.push_back(
									rt_data.object_handles.back().createInstance());
              transformLine(new_line_2, rt_data.instance_handles.back());
						}
					}

					rtfact_needs_update_ = true;

				}
			}

			if (rtfact_needs_update_ && use_continuous_loop_)
			  renderTask.setAccumulatePixels(true);

			objects_[&rep] = rt_data;
		}

		void RTfactRenderer::removeRepresentation(Representation const& rep)
		{
			if (objects_.find(&rep) != objects_.end())
			{
				// TODO: find out if this also deletes the geometries and materials
				RTfactData& rt_data = objects_[&rep];

				for (Position i=0; i<rt_data.instance_handles.size(); ++i)
				{
					geometric_objects_.erase(rt_data.instance_handles[i]);
					rt_data.instance_handles[i].clear();
				}
				rt_data.instance_handles.clear();

				rt_data.material_handles.clear();

				rt_data.mesh_handles.clear();

				rtfact_needs_update_ = true;

				if (use_continuous_loop_ && !rep.isHidden())
					renderTask.setAccumulatePixels(true);

				objects_.erase(&rep);
			}
		}

		void RTfactRenderer::useContinuousLoop(bool use_loop)
		{
			Renderer::useContinuousLoop(use_loop);
			renderTask.setAccumulatePixels(use_loop);
		}

		void RTfactRenderer::renderToBufferImpl(FrameBufferPtr buffer)
		{
			Stage const& stage = *(scene_->getStage());

			// deactivate hidden representations (we need no reactivation code,
			// since reactivated representations will simply be buffered again)
			for (HashMap<Representation const*, RTfactData>::iterator it = objects_.begin(); it != objects_.end(); ++it)
			{
				if (it->first->isHidden() && !it->second.has_been_disabled)
				{
					// It is safe to remove a group multiple times from RTfact. It may not be
					// *fast*, but it should be safe.
					RTfactData& rt_data = it->second;

					for (Position i=0; i<rt_data.instance_handles.size(); ++i)
					{
					  rt_data.instance_handles[i].clear();
					}
					rt_data.instance_handles.clear();

					it->second.has_been_disabled = true;

					rtfact_needs_update_ = true;
				}
			}

			if (rtfact_needs_update_)
			{
				rayTracer.syncStructures();
				rtfact_needs_update_ = false;
			}

			FrameBufferFormat fmt = buffer->getFormat();
			if (objects_.size() != 0)
			{

				Image2DHandle distanceBufferHandle;
				Image2DHandle colorBufferHandle;

				if (fmt.getPixelFormat() == PixelFormat::RGBF_96)
				{
					//color buffer (use given memory)
					t_ColorImage* colorImage = new t_ColorImage((float*)buffer->getData(), 3, width_, height_, width_+stride_, false);
					RTfact::uint colorBufferLength = colorImage->getResX() * colorImage->getResY()* colorImage->getComponentCount() * sizeof(t_ColorImage::Component);
					BufferHandle colorBuffer = RTpieCpp::CreateBufferHandleUseData(colorBufferLength, colorImage->getFirstComponent(0,0));
					colorBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_FLOAT, 3, width_, height_, colorBuffer);

					//distance buffer
					t_DistanceImage* distanceImage = new t_DistanceImage(1, width_, height_);
					RTfact::uint distanceBufferLength = distanceImage->getResX() * distanceImage->getResY()* distanceImage->getComponentCount() * sizeof(t_DistanceImage::Component);
					BufferHandle distanceBuffer = RTpieCpp::CreateBufferHandleUseData(distanceBufferLength, distanceImage->getFirstComponent(0,0));
					distanceBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_FLOAT, 1, width_, height_, distanceBuffer);

					//set framebuffer buffers
					framebuffer.setColorImage(colorImage);
					framebuffer.setDistanceImage(distanceImage);
				}
				else if (fmt.getPixelFormat() == PixelFormat::RGBA_32)
				{
					//color buffer (use given memory)
					t_ByteColorImage* colorImage = new t_ByteColorImage((unsigned char*)buffer->getData(), 4, width_, height_, width_+stride_, false);
					RTfact::uint colorBufferLength = colorImage->getResX() * colorImage->getResY()* colorImage->getComponentCount() * sizeof(t_ByteColorImage::Component);
					BufferHandle colorBuffer = RTpieCpp::CreateBufferHandleUseData(colorBufferLength, colorImage->getFirstComponent(0,0));
					colorBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_BYTE, 4, width_, height_, colorBuffer);

					//distance buffer
					t_ByteDistanceImage* distanceImage = new t_ByteDistanceImage(1, width_, height_);
					RTfact::uint distanceBufferLength = distanceImage->getResX() * distanceImage->getResY()* distanceImage->getComponentCount() * sizeof(t_ByteDistanceImage::Component);
					BufferHandle distanceBuffer = RTpieCpp::CreateBufferHandleUseData(distanceBufferLength, distanceImage->getFirstComponent(0,0));
					distanceBufferHandle = RTpieCpp::CreateImage2DHandleUseData(RTfact::RTpie::IImage2D::COMPONENT_BYTE, 1, width_, height_, distanceBuffer);

					//set framebuffer buffers
					byteFramebuffer.setColorImage(colorImage);
					byteFramebuffer.setDistanceImage(distanceImage);
				}
				else
				{
					return;
				}

				//set framebuffer to rendertask
				renderBuffer = RTpieCpp::CreateFrameBufferHandle();
				renderBuffer.setColorBuffer(colorBufferHandle);
				renderBuffer.setDistanceBuffer(distanceBufferHandle);
				renderTask.setFrameBuffer(renderBuffer);

				//
				framebuffer.prePaint();

				rayTracer.syncStructures();

				renderTask.run();

				framebuffer.postPaint();

				if (use_continuous_loop_)
				{
					renderTask.setAccumulatePixels(true);
				}

			}
		}

		void RTfactRenderer::updateMaterialForRepresentation(Representation const* rep)
		{

			if (objects_.find(rep) != objects_.end())
			{
				RTfactData& rt_data = objects_[rep];

				for (Position i=0; i<rt_data.material_handles.size(); ++i)
				{
					if (rep->hasProperty("RTFact::Material"))
					{
						NamedProperty rt_mat_property = rep->getProperty("RTFact::Material");
						boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
						convertMaterial(*dynamic_cast<Stage::RaytracingMaterial*>(mat_ptr.get()), rt_data.material_handles[i]);
					}
					else
						updateMaterialFromStage(rt_data.material_handles[i]);
				}
			}
		}

		void RTfactRenderer::transformTube(const TwoColoredTube& tube, InstanceHandle& instance)
		{
			Vector3 vec = tube.getVertex2() - tube.getVertex1();
			const double len = vec.getLength();
			const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
			const float radius = tube.getRadius();

			Vector3 const& midpoint = tube.getVertex1();
			//Rotate the vector around the normal
			vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

			Matrix4x4 matrix = Matrix4x4::getIdentity();
			matrix.rotate(Angle(-angle), vec.y, -vec.x, 0);

			Matrix4x4 temp;
			temp.setScale(radius, radius, len);
			matrix*=temp;

			temp.setTranslation(midpoint);
			matrix = temp*matrix;

			float trafo[16];
			for (Position i=0; i<4; ++i)
				for (Position j=0; j<4; ++j)
					trafo[i*4+j] = matrix(j, i);

			instance.setTransform(trafo, 0);
		}

		void RTfactRenderer::transformLine(const TwoColoredLine& line, InstanceHandle& instance)
		{
			Vector3 vec = line.getVertex2() - line.getVertex1();
			const double len = vec.getLength();
			const double angle = acos(vec.z / len); // the denominator accounts for the non-normalized rotation axis
			const float radius = LINE_RADIUS;//line.getRadius();

			Vector3 const& midpoint = line.getVertex1();
			//Rotate the vector around the normal
			vec /= sqrt(vec.x*vec.x + vec.y*vec.y);

			Matrix4x4 matrix = Matrix4x4::getIdentity();
			matrix.rotate(Angle(-angle), vec.y, -vec.x, 0);

			Matrix4x4 temp;
			temp.setScale(radius, radius, len);
			matrix*=temp;

			temp.setTranslation(midpoint);
			matrix = temp*matrix;

			float trafo[16];
			for (Position i=0; i<4; ++i)
				for (Position j=0; j<4; ++j)
					trafo[i*4+j] = matrix(i, j);

			instance.setTransform(trafo, 0);
		}

		void RTfactRenderer::updateMaterialFromStage(AppearanceHandle& material)
		{

			Stage::RaytracingMaterial const& rt_material = scene_->getStage()->getRTMaterial();
			convertMaterial(rt_material, material);

		}

		void RTfactRenderer::convertMaterial(Stage::RaytracingMaterial const& rt_material, AppearanceHandle& material)
		{
			// ambience
			float red   = (float)rt_material.ambient_color.getRed()   * rt_material.ambient_intensity;
			float blue  = (float)rt_material.ambient_color.getBlue()  * rt_material.ambient_intensity;
			float green = (float)rt_material.ambient_color.getGreen() * rt_material.ambient_intensity;

			material.setParam3f("ambientIntensity", float3(red, blue, green));

			// specularity
			red   = (float)rt_material.specular_color.getRed()   * rt_material.specular_intensity;
			blue  = (float)rt_material.specular_color.getBlue()  * rt_material.specular_intensity;
			green = (float)rt_material.specular_color.getGreen() * rt_material.specular_intensity;

			material.setParam3f("specularColor", float3(red, blue, green));

			// reflectiveness
			red   = (float)rt_material.reflective_color.getRed()   * rt_material.reflective_intensity;
			blue  = (float)rt_material.reflective_color.getBlue()  * rt_material.reflective_intensity;
			green = (float)rt_material.reflective_color.getGreen() * rt_material.reflective_intensity;

			material.setParam3f("reflective", float3(red, blue, green));

			// shininess
			material.setParamf("shininess", rt_material.shininess);

			// transparency
			material.setParamf("alpha", (100.f - rt_material.transparency) * 0.01f);
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

			IntersectTaskHandle intersectTask = rayTracer.createIntersectTask();
			intersectTask.intersectRays(
				reinterpret_cast<const float*>(&origins[0]),
				reinterpret_cast<const float*>(&directions[0]),
				origins.size(),
				reinterpret_cast<float*>(&results[0]));

			return results;
		}

	}
}
