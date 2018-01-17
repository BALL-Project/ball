// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/atom.h>
#include <BALL/MATHS/vector2.h>
#include <BALL/MATHS/plane3.h>
#include <BALL/MATHS/analyticalGeometry.h>
#include <BALL/SYSTEM/timer.h>

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>
#include <BALL/VIEW/DATATYPE/colorMap.h>
#include <BALL/VIEW/KERNEL/mainControl.h>

#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/quadMesh.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>

#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QOpenGLContext>

#include <glRenderer.h>
#include <glRenderSetup.h>
#include <vertexBuffer.h>

using namespace std;

//   #define BALL_VIEW_DEBUG
//   #define BALL_BENCHMARKING

#define BYTES_PER_TEXEL 4
#define TEXTURE_SIZE 128  // including border
#define NUM_CHANNELS 4

namespace BALL
{
	namespace VIEW
	{

#ifdef BALL_VIEW_DEBUG
#define CHECK_GL_ERROR \
		{\
			GLenum e = glGetError(); \
			if (e != GL_NO_ERROR)\
			{\
				Log.error() << "GL Error occurred in " << __FILE__ << " " << __LINE__ << std::endl;\
				switch (e)\
				{\
					case GL_INVALID_VALUE: Log.error() << " GL_INVALID_VALUE" << std::endl;break;\
					case GL_INVALID_ENUM: Log.error() << " GL_INVALID_ENUM" << std::endl;break;\
					case GL_INVALID_OPERATION: Log.error() << " GL_INVALID_OPERATION" << std::endl;break;\
					case GL_STACK_OVERFLOW: Log.error() << " GL_STACK_OVERFLOW" << std::endl;break;\
					case GL_STACK_UNDERFLOW: Log.error() << " GL_STACK_UNDERFLOW" << std::endl;break;\
					case GL_TABLE_TOO_LARGE: Log.error() << " GL_TABLE_TOO_LARGE" << std::endl;break;\
					default: Log.error() << " UNKNOWN ERROR" << std::endl;\
				}\
			}\
		}
#else
#define CHECK_GL_ERROR
#endif

		GLRenderer::GLRenderer()
			: Renderer("GLRenderer"),
				drawing_mode_(DRAWING_MODE_SOLID),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				near_(1.5),
				far_(2000.f),
				left_(-1),
				right_(1),
				top_(1),
				bottom_(-1),
				GL_spheres_list_(0),
				GL_tubes_list_(0),
				GL_boxes_list_(0),
				name_to_object_(),
				object_to_name_(),
				all_names_(1),
				render_mode_(RENDER_MODE_UNDEFINED),
				use_vertex_buffer_(false),
				smooth_lines_(false),
				picking_mode_(false),
				model_type_(MODEL_LINES),
				drawed_other_object_(false),
				drawed_mesh_(false),
				GLU_quadric_obj_(0),
				orthographic_zoom_(10.f),
				line_tex_(new GLubyte[TEXTURE_SIZE * TEXTURE_SIZE * NUM_CHANNELS]),
				object_buffer_(new GLuint[BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS])
		{
		}

		GLRenderer::~GLRenderer()
		{
			clear();
			delete[] line_tex_;
			delete[] object_buffer_;
		}

		void GLRenderer::clear()
		{
			glDisable(GL_BLEND);
			glDisable(GL_LIGHTING);
			glDisable(GL_RESCALE_NORMAL);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_NORMALIZE);

			name_to_object_.clear();
			object_to_name_.clear();
			all_names_ = 1;

			if (GL_spheres_list_) delete[] GL_spheres_list_;
			if (GL_boxes_list_)   delete[] GL_boxes_list_;
			if (GL_tubes_list_)   delete[] GL_tubes_list_;

			DisplayListHashMap::Iterator it = display_lists_.begin();
			for (; it != display_lists_.end(); it++)
			{
				delete it->second;
			}
			display_lists_.clear();

			orthographic_zoom_ = 10.f;
		}

		boost::shared_ptr<RenderSetup> GLRenderer::createRenderSetup(RenderTarget* target, Scene* scene)
		{
			return boost::shared_ptr<RenderSetup>(new GLRenderSetup(this, target, scene));
		}

		void GLRenderer::setAntialiasing(bool state)
		{
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
			if (state)
			{
				glEnable(GL_MULTISAMPLE);

				if(smooth_lines_) {
					// smooth line drawing
					glEnable(GL_LINE_SMOOTH);
				}
				// slower, but better results:
				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

			}
			else
			{
				glDisable(GL_MULTISAMPLE);
				glDisable(GL_LINE_SMOOTH);
				glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
			}
		}

		void GLRenderer::setSmoothLines(bool smooth_lines)
		{
			smooth_lines_ = smooth_lines;
		}

		bool GLRenderer::getSmoothLines()
		{
			return smooth_lines_;
		}

		void GLRenderer::setMaterial_(const Stage::Material& material)
		{
			GLfloat shin[] = { material.shininess };
			GLfloat spec[] = {
				material.specular_intensity * (float)material.specular_color.getRed(),
				material.specular_intensity * (float)material.specular_color.getGreen(),
				material.specular_intensity * (float)material.specular_color.getBlue(),
				material.specular_intensity * (float)material.specular_color.getAlpha()
			};
			GLfloat ambient[] = {
				material.ambient_intensity * (float)material.ambient_color.getRed(),
				material.ambient_intensity * (float)material.ambient_color.getGreen(),
				material.ambient_intensity * (float)material.ambient_color.getBlue(),
				material.ambient_intensity * (float)material.ambient_color.getAlpha()
			};

			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  spec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin );
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   ambient);
		}

		bool GLRenderer::init(const Stage& stage, float width, float height)
		{
			Renderer::init(stage, width, height);

			enableVertexBuffers(true);

			// Force OpenGL to normalize transformed normals to be of unit
			// length before using the normals in OpenGL's lighting equations
			// While this corrects potential lighting problems introduced by scaling,
			// it also slows OpenGL's vertex processing speed since normalization requires extra operations.
			glEnable(GL_NORMALIZE);

			glFrontFace(GL_CCW);     // selects counterclockwise polygons as front-facing
			glCullFace(GL_BACK);		 // specify whether front- or back-facing facets can be culled

			glDisable(GL_FOG);
			
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, false);

			// Specifies the depth comparison function:
			// Passes if the incoming z value is greater than  or equal to the stored z value
			glDepthFunc(GL_LEQUAL);

			// specify the clear value for the depth buffer 
			glClearDepth(200.0);

			setAntialiasing(true);

			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			// select smooth shading 
			glShadeModel(GL_SMOOTH);

			// is problematic on some machines and should not be used:
			glDisable(GL_POLYGON_SMOOTH);
			// glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

			glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);

			// setup all in the stage given lightsources
			setSize(width, height);
			setLights();
			glEnable(GL_LIGHTING);

			// set the background color according to the stage
			updateBackgroundColor();

			glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);

			// if displaylists were already calculated, return
			if (GL_spheres_list_ != 0) return true;

			initSolid();

			GL_spheres_list_ = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();
			GL_tubes_list_   = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();
			GL_boxes_list_   = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

			GLU_quadric_obj_ = gluNewQuadric();
			gluQuadricOrientation(GLU_quadric_obj_, GLU_OUTSIDE);
			gluQuadricNormals(GLU_quadric_obj_, GLU_SMOOTH); //GLU_FLAT GLU_SMOOTH
			gluQuadricTexture(GLU_quadric_obj_, GL_FALSE);
			gluQuadricDrawStyle(GLU_quadric_obj_, GLU_FILL);

			createSpheres_();
			createTubes_();
			createBoxes_();

			//////////////////////////////////////////////////////
			// toon shader:
			// first entries: greatest angle between normal and light vector
			float shader[32] = { 0.2, 0.21, 0.23, 0.25, 0.27, 0.29, 0.31, 0.33, 0.36, 0.39, 0.41, 0.4, 0.44, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

			float cel_shader_data[32][3];

			for (Position p = 0; p < 32; p++)
			{
				cel_shader_data[p][0] = 
				cel_shader_data[p][1] = 
				cel_shader_data[p][2] = shader[p];
			}

			glEnable(GL_TEXTURE_1D);
			glGenTextures(1, &cel_texture_);		
			glBindTexture(GL_TEXTURE_1D, cel_texture_);	
			glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 32, 0, GL_RGB , GL_FLOAT, cel_shader_data);
			glBindTexture(GL_TEXTURE_1D, 0);	

			//////////////////////////////////////////////////////

			// display list for illuminated lines
			generateIlluminationTexture_(0.1, 0.3, 0.599, 0.5);
			line_list_.clear();
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
			glGenTextures(1, &line_texture_bind_);
			glBindTexture(GL_TEXTURE_2D, line_texture_bind_); 
			glTexImage2D(GL_TEXTURE_2D, 0, 4, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, line_tex_);

			line_list_.useCompileMode();
			line_list_.startDefinition();

			glBindTexture(GL_TEXTURE_2D, line_texture_bind_);
			
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
			glEnable( GL_TEXTURE_2D );

			// glEnable( GL_LINE_SMOOTH );
			glEnable( GL_BLEND );
		//         glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );    
			// glHint( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );

			line_list_.endDefinition();

			initSolid();
			updateCamera();

			return true;
		}


		void GLRenderer::updateBackgroundColor() 
		{
			glClearColor((float) stage_->getBackgroundColor().getRed(),
									 (float) stage_->getBackgroundColor().getGreen(),
									 (float) stage_->getBackgroundColor().getBlue(),
									 (float) stage_->getBackgroundColor().getAlpha());
		}
			
		void GLRenderer::setLights(bool reset_all)
		{
			GLenum light_nr = GL_LIGHT0;

			if (reset_all)
			{
				GLint lights_max;
				glGetIntegerv(GL_MAX_LIGHTS, &lights_max);
				for (; (GLint)light_nr < GL_LIGHT0 + lights_max; light_nr++)
				{
					glDisable(light_nr);
				}
				
				light_nr = GL_LIGHT0;
			}

			list<LightSource>::const_iterator it = stage_->getLightSources().begin();
			for (; it != stage_->getLightSources().end(); it++)
			{
				if (reset_all && !it->isRelativeToCamera())
				{
					continue;
				}
				
				// setup the light intensity
				GLfloat intensity[] = {((float) it->getColor().getRed()) 		* it->getIntensity(),
															( (float) it->getColor().getGreen()) 	* it->getIntensity(),
															( (float) it->getColor().getBlue())		* it->getIntensity(),
															( (float) it->getColor().getAlpha())};

				GLfloat zero[] = {0, 0, 0, 0};

				if (it->getType() == LightSource::AMBIENT)
				{
					glLightfv(light_nr, GL_AMBIENT, intensity);
					glLightfv(light_nr, GL_DIFFUSE, zero);
					glLightfv(light_nr, GL_SPECULAR, zero);
					glEnable(light_nr);
					light_nr++;
					continue;
				}
				
				glLightfv(light_nr, GL_AMBIENT, zero);
				glLightfv(light_nr, GL_DIFFUSE, intensity);
				glLightfv(light_nr, GL_SPECULAR, intensity);

				Vector3 light_dir;

				if (it->getType() == LightSource::DIRECTIONAL)
				{
					// directional light sources dont have a position!
					// but they get their direction with GL_POSITION!
					light_dir = -it->getDirection();
					if (it->isRelativeToCamera())
					{
						light_dir = stage_->calculateAbsoluteCoordinates(light_dir);
					}

					GLfloat pos[]  = { light_dir.x,
														 light_dir.y,
														 light_dir.z,
														 0.0};  // the 1 is for positional lights

					glLightfv(light_nr, GL_POSITION, pos);	
					glEnable(light_nr);
					light_nr++;
					continue;
				}

				Vector3 light_pos;

				if (it->isRelativeToCamera())
				{
					light_pos = stage_->calculateAbsoluteCoordinates(it->getPosition()) + stage_->getCamera().getViewPoint();
					light_dir = stage_->calculateAbsoluteCoordinates(it->getDirection());
				}
				else
				{
					light_pos = it->getPosition();
					light_dir = it->getDirection() - light_pos;
				}
			
				// setup the direction of the light
				GLfloat dir[] = { light_dir.x,
													light_dir.y,
													light_dir.z};
				glLightfv(light_nr, GL_SPOT_DIRECTION, dir);

				// setup the angle of the light cone
				GLfloat angle = 180;
				if (it->getAngle().toDegree() <= 90)
				{
					angle = it->getAngle().toDegree();
				}
				
				glLightfv(light_nr, GL_SPOT_CUTOFF, &angle);
				glLightf(light_nr, GL_SPOT_EXPONENT, (GLfloat) 100);

				// setup the position of the lightsource
				GLfloat pos[]  = { light_pos.x,
													 light_pos.y,
													 light_pos.z,
													 1.0};  // the 1 is for positional lights

				glLightfv(light_nr, GL_POSITION, pos);
			
				Vector3 att =  it->getAttenuation();
				glLightf(light_nr, GL_CONSTANT_ATTENUATION, att.x);
				glLightf(light_nr, GL_LINEAR_ATTENUATION, att.y);
				glLightf(light_nr, GL_QUADRATIC_ATTENUATION, att.z);

				glEnable(light_nr);
				light_nr++;
			}
		}

		void GLRenderer::removeRepresentation(const Representation& rep)
		{
			if (!getMainControl()->getRepresentationManager().has(rep)) return;

			if (rep.getGeometricObjects().size() == 0) return;

			if (vertexBuffersEnabled())
			{
				clearVertexBuffersFor(*(Representation*)&rep);
			}

			DisplayListHashMap::Iterator hit = display_lists_.find(&rep);
			if (hit == display_lists_.end()) return;

			delete hit->second;
			display_lists_.erase(hit);
		}

		void GLRenderer::bufferRepresentation(const Representation& rep)
		{
#ifdef BALL_BENCHMARKING
			Timer t;
			t.start();
#endif
			GLDisplayList* display_list;
			if (display_lists_.has(&rep))
			{
				display_list = display_lists_[&rep];
				display_list->clear();
			}
			else
			{
				display_list = new GLDisplayList;
				display_lists_[&rep] = display_list;
			}

			display_list->useCompileMode();
			display_list->startDefinition();
			
			render(rep, true);
			
			display_list->endDefinition();

#ifdef BALL_HAS_GLEW
			clearVertexBuffersFor(*(Representation*)&rep);
			
			if (use_vertex_buffer_ && drawing_mode_ != DRAWING_MODE_WIREFRAME)
			{
				// prevent copying the pointers of the buffers later...
				rep_to_buffers_[&rep] = vector<MeshBuffer*>();
				
				vector<MeshBuffer*>& buffers = rep_to_buffers_.find(&rep)->second;

				const list<GeometricObject*>& geometric_objects = rep.getGeometricObjects();
				list<GeometricObject*>::const_iterator git = geometric_objects.begin();
				for (; git != geometric_objects.end(); git++)
				{
					const Mesh* const mesh = dynamic_cast<Mesh*>(*git);
					if (mesh != 0) 
					{
						MeshBuffer* buffer = new MeshBuffer;
						buffer->setMesh(*mesh);
						buffer->initialize();
						buffers.push_back(buffer);
					}
				}
			}
#endif

#ifdef BALL_BENCHMARKING
		t.stop();
		logString("OpenGL rendering time: " + String(t.getCPUTime()));
#endif
		}

		// TODO: do we need the mode???
		void GLRenderer::renderToBuffer(RenderTarget* /*renderTarget*/, BufferMode mode)
		{
			if (show_preview_)
				setAntialiasing(false);

			glDepthMask(GL_TRUE);

			glDrawBuffer(GL_BACK);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!getMainControl())
				return;

			RepresentationManager& pm = getMainControl()->getRepresentationManager();

			// ============== enable active Clipping planes ==============================
			GLint current_clipping_plane = GL_CLIP_PLANE0;

			vector<ClippingPlane*> active_planes;
			vector<ClippingPlane*> inactive_planes;

			bool move_mode = scene_->inMoveMode();
			
			const vector<ClippingPlane*>& vc = pm.getClippingPlanes();
			vector<ClippingPlane*>::const_iterator plane_it = vc.begin();
			for (;plane_it != vc.end(); plane_it++)
			{
				ClippingPlane& plane = **plane_it;

				if (plane.isHidden()) continue;

				if (!plane.isActive()) 
				{
					inactive_planes.push_back(*plane_it);
					if (!move_mode) continue;
				}

				active_planes.push_back(*plane_it);

				const Vector3& n(plane.getNormal());
				const GLdouble planef[] ={n.x, n.y, n.z, plane.getDistance()};
				glClipPlane(current_clipping_plane, planef);
				current_clipping_plane++;
			}

			// -------------------------------------------------------------------
			// show light sources
			if (show_light_sources_)
			{
				list<LightSource>::const_iterator lit = stage_->getLightSources().begin();
				for (; lit != stage_->getLightSources().end(); lit++)
				{
					Vector3 pos = (*lit).getPosition();
					Vector3 dir = (*lit).getDirection();

					if ((*lit).isRelativeToCamera())
					{
						pos = stage_->getCamera().getViewPoint() + stage_->calculateAbsoluteCoordinates(pos);
						dir = pos + stage_->calculateAbsoluteCoordinates(dir);
					}

					Vector3 diff = dir - pos;
					if (!Maths::isZero(diff.getSquareLength())) diff.normalize();
					diff *= 100.0;

					Sphere s;
					s.setPosition(pos);
					s.setRadius(5);
					s.setColor(ColorRGBA(255,255,255));
					renderSphere_(s);

					Tube t;
					t.setVertex1(pos);
					t.setVertex2(pos + diff);
					t.setColor(ColorRGBA(255,255,255));
					renderTube_(t);
				}
			}
			// -------------------------------------------------------------------
			
			
			// we draw all the representations in different runs, 
			// 1. normal reps
			// 2. transparent reps
			// 3. allways front
			for (Position run = 0; run < 3; run++)
			{
				if (run == 1)
				{
					// render inactive clipping planes
					for (plane_it = inactive_planes.begin(); plane_it != inactive_planes.end(); plane_it++)
					{
						renderClippingPlane_(**plane_it);
					}
				}

				RepresentationList::const_iterator it = pm.getRepresentations().begin();
				for(; it != pm.getRepresentations().end(); it++)
				{
					Representation& rep = **it;
					if (rep.isHidden()) continue;

					if (run == 0)
					{
						// render all "normal" (non always front and non transparent models)
						if (!rep.hasProperty("DONT_CLIP") && (
								rep.getTransparency() != 0 ||
								rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT)))
						{
							continue;
						}
					}
					else if (run == 1)
					{
						// render all transparent models
						if (rep.hasProperty("DONT_CLIP") ||
								rep.getTransparency() == 0 ||
								rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
						{
							continue;
						}
					}
					else
					{
						// render all always front models
						if (!rep.hasProperty(Representation::PROPERTY__ALWAYS_FRONT)) continue;
					}
					
					vector<Position> rep_active_planes; // clipping planes

					Index cap_nr = -1;
					for (Position plane_nr = 0; plane_nr < active_planes.size(); plane_nr++)
					{
						if (!active_planes[plane_nr]->getRepresentations().has(*it)) continue;
						
						const ClippingPlane& plane = *active_planes[plane_nr];
						rep_active_planes.push_back(plane_nr);
						if (!plane.cappingEnabled())
						{
							glEnable(plane_nr + GL_CLIP_PLANE0);
							continue;
						}

						cap_nr = plane_nr;
					}

					// no capping? then we are done: disable again all clipping planes
					if (cap_nr == -1)
					{
						bufferingDependentRender_(rep, mode);

						for (Position p = 0; p < rep_active_planes.size(); p++)
						{
							glDisable(rep_active_planes[p] + GL_CLIP_PLANE0);
						}
						continue;
					}

					// draw a capping plane
 					const ClippingPlane& plane = *active_planes[cap_nr];
					Vector3 p, x, y, n;
					p = plane.getPoint();
					x = getNormal(plane.getNormal()) * 400;
					y = x % plane.getNormal() * 400;
 					n = plane.getNormal();
					n.normalize();
					
					// disable all clipping planes
					for (Position p = 0; p < rep_active_planes.size(); p++)
					{
						glDisable(rep_active_planes[p] + GL_CLIP_PLANE0);
					}

					// fill the stencil buffer
					glEnable(cap_nr + GL_CLIP_PLANE0);
					glEnable(GL_STENCIL_TEST);
					glClearStencil(0);
					glClear(GL_STENCIL_BUFFER_BIT);
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					glStencilFunc(GL_ALWAYS, 0x0, 0xff);
					glStencilOp(GL_KEEP, GL_INVERT, GL_INVERT);
					bufferingDependentRender_(rep, mode);
					
					// disable all clipping planes
					for (Position p = 0; p < rep_active_planes.size(); p++)
					{
						glEnable(rep_active_planes[p] + GL_CLIP_PLANE0);
					}

					// render the Representation once again, this time with colors
					setColorRGBA_(ColorRGBA(0,1.0,0));
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					glDisable(GL_STENCIL_TEST);
					bufferingDependentRender_(rep, mode);

					// render the capping plane
					glEnable(GL_STENCIL_TEST);
					glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);

					ColorRGBA color = ClippingPlane::getCappingColor();
					bool transparent = (int)color.getAlpha() != 255;
					if (transparent) initTransparent();
					else 						 initSolid();

					glDisable(cap_nr + GL_CLIP_PLANE0);
					Disc d(Circle3(p, n, 400));
					d.setColor(color);
					render_(&d);
					
					glDisable(GL_STENCIL_TEST);
				}
			}

			if (show_preview_) 
				setAntialiasing(true);

			return;
		}

		void GLRenderer::bufferingDependentRender_(const Representation& repr, BufferMode mode)
		{
			RepresentationManager& pm = getMainControl()->getRepresentationManager();
			Representation* rep = (Representation*)& repr;

			// preview mode:
			// if we have a model with only sticks and cylinders, decrease the rendering detail
			// and draw it directly instead of using display lists.
			// BUT: if the representation is currently rebuild, we can only draw buffered!
			DrawingPrecision pbak = repr.getDrawingPrecision();

			if (show_preview_)
			{
				if (repr.getModelType() >= MODEL_STICK &&
						repr.getModelType() <= MODEL_VDW &&
						pm.startRendering(rep))
				{
					rep->setDrawingPrecision(DRAWING_PRECISION_LOW);
					mode = DIRECT_RENDERING;
				}
			}

			if (mode == DISPLAY_LISTS_RENDERING &&
					!repr.hasProperty("RENDER_DIRECT") && 
					repr.getDrawingMode() != DRAWING_MODE_TOON)
			{
				drawBuffered(repr);
				return;
			}

			if (!show_preview_) 
			{
				// if we use previewing mode, the RepresentationManager was already notified above
				if (!pm.startRendering(rep))
				{
					// if Representation is to be rebuilded, but it is currently recalculated,
					// we can only draw it from the DisplayList
					drawBuffered(repr);
					return;
				}
			}

			if (mode == REBUILD_DISPLAY_LISTS)
			{
				bufferRepresentation(repr);
			}
			else //	DIRECT_RENDERING:
			{
				render(repr);
			}

			if (show_preview_)
			{
				if (repr.getDrawingPrecision() != pbak)
				{
					// if previewing mode was used: reset the drawing precision
					(*(Representation*)&repr).setDrawingPrecision(pbak);
				}
			}

			pm.finishedRendering(rep);
		}

		bool GLRenderer::render(const Representation& representation, bool for_display_list)
		{
			if (representation.isHidden()) return true;

			if (!representation.isValid())
			{
				BALLVIEW_DEBUG;
				representation.dump(std::cout, 0);
				return false;
			}

			drawing_precision_  = representation.getDrawingPrecision();
			drawing_mode_ 		  = representation.getDrawingMode();

			drawed_mesh_ = false;
			drawed_other_object_ = false;

			display_lists_index_ = drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + drawing_precision_;

			if (representation.getDrawingMode() == DRAWING_MODE_DOTS)
			{
				glDisable(GL_LIGHTING);
			}
			else
			{
				glEnable(GL_LIGHTING);
			}

			if (representation.hasProperty(Representation::PROPERTY__ALWAYS_FRONT))
			{
				initAlwaysFront();
			}
			else if (representation.getTransparency() == 0)
			{
				initSolid();
			}
			else
			{
				if (!representation.hasProperty("DONT_CLIP"))
				{
					// prevent artifacts:
					// first run to fill depth buffer
					initTransparent();
					glDepthMask(GL_TRUE);
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
					renderRepresentation_(representation, for_display_list);

					// options for second run 
					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				}
				initTransparent();
			}

			renderRepresentation_(representation, for_display_list);
			glFlush();

			return true;
		}

		void GLRenderer::renderRepresentation_(const Representation& representation, bool for_display_list)
		{
			model_type_ = representation.getModelType();

			if (representation.hasProperty("Rendering::Material"))
			{
				NamedProperty rt_mat_property = representation.getProperty("Rendering::Material");
				boost::shared_ptr<PersistentObject> mat_ptr = rt_mat_property.getSmartObject();
				setMaterial_(*dynamic_cast<Stage::Material*>(mat_ptr.get()));
			}
			else
			{
				setMaterial_(stage_->getMaterial());
			}

			// accelerate things a little by calling getGeometricObjects() only once
			const list<GeometricObject*>& geometric_objects = representation.getGeometricObjects();
			if (geometric_objects.size() == 0) return;
			list<GeometricObject*>::const_iterator it = geometric_objects.begin();
			if (for_display_list)
			{
				if (use_vertex_buffer_ && drawing_mode_ != DRAWING_MODE_WIREFRAME)
				{
					// draw everything except of meshes, these are put into vertex buffer objects in bufferRepresentation()
					for (; it != geometric_objects.end(); it++)
					{
						if (dynamic_cast<Mesh*>(*it) == 0) render_(*it);
					}
				}
				else
				{
					// render everything
					for (; it != geometric_objects.end(); it++)
					{
						render_(*it);
					}
				}
			}
			else // drawing for picking directly
			{
				for (; it != geometric_objects.end(); it++)
				{
					// render everything with names from glLoadName
					glLoadName(getName(**it));
					render_(*it);
				}
			}

			glFlush();
		}

		void GLRenderer::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "Drawing Precision: " 	<< drawing_precision_ 	<< std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Drawing Mode: " 		 	<< drawing_mode_  << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Width: " << width_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "Height: " << height_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "XScale: " << x_scale_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "YScale: " << y_scale_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

// =================================================================================
// --------------------------render methods-----------------------------------------
// =================================================================================

		void GLRenderer::renderRuler()
		{
			const Camera& s = stage_->getCamera();
			Vector3 v = s.getViewVector();
			v.normalize();
			const Vector3 x = s.getRightVector();
			const Vector3 y = s.getLookUpVector();
			float delta = 0.001;
			float size = 50;

			initTransparent();

			Vector3 p = mapViewportTo3D((Index)(width_ / 2.0), (Index)(height_ / 2.0)) - x * size / 2.0 - y * size / 2.0;

			Box xp(p, x * size, y * size, delta);
			xp.setColor(ColorRGBA(0,255,190,90));
			render_(&xp);

			ColorRGBA color1(255,255,255,255);
			ColorRGBA color2(0,0,0,230);
			Line line;
			p -= v * delta;

			for (Position i = 0; i <= size; i+=1)
			{
				if (i % 10 == 0) line.setColor(color2);
				else  					 line.setColor(color1);

				line.setVertex1(p + x * i);
				line.setVertex2(p + x * i + y * size);
				render_(&line);

				line.setVertex1(p + y * i);
				line.setVertex2(p + y * i + x * size);
				render_(&line);
			}

			initSolid();
		}

		void GLRenderer::renderSphere_(const Sphere& sphere)
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(sphere);
			translateVector3_(sphere.getPosition());
			scale_(sphere.getRadius());

			// render spheres of stick models with less precision for atoms with
			// more than 2 bonds:
			if (model_type_ == MODEL_STICK &&
					drawing_precision_ > DRAWING_PRECISION_LOW)
			{
				Index precision = drawing_precision_;
				const Atom* const atom = dynamic_cast<const Atom*>(sphere.getComposite());

				if (atom != 0 && atom->countBonds() > 2)
				{
					precision--;
				}

				GL_spheres_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION +
												 precision].draw();
			}
			else
			{
				GL_spheres_list_[display_lists_index_].draw();
			}

			glPopMatrix();
		}

		void GLRenderer::renderDisc_(const Disc& disc)
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(disc);
			translateVector3_(disc.getCircle().p);
			const Vector3 rotation_axis(-disc.getCircle().n.y, disc.getCircle().n.x, 0.0);
			// angle between z-axis-vector and result
			const float angle = Angle::toDegree(acos(disc.getCircle().n.z / disc.getCircle().n.getLength()));
			rotateVector3Angle_(rotation_axis, angle);

			static Position slices[4] = {6, 14, 24, 40};
			static Position rings[4]  = {4, 8, 16, 32};

			initGLU_(drawing_mode_);

			gluDisk(GLU_quadric_obj_, 0, disc.getCircle().radius, slices[drawing_precision_], rings[drawing_precision_]);

			glPopMatrix();
		}


		void GLRenderer::renderLine_(const Line& line)
		{
			initDrawingOthers_();

			glDisable(GL_LIGHTING);
			setColor4ub_(line);

			glBegin(GL_LINES);
				vertexVector3_(line.getVertex1());
				vertexVector3_(line.getVertex2());
			glEnd();

			glEnable(GL_LIGHTING);
		}

		void GLRenderer::renderMultiLine_(const MultiLine& line)
		{
			initDrawingOthers_();

			glDisable(GL_LIGHTING);
			/*

			// TEST!!! this should use a light source. currently we use a headlight
			Vector3 view_vector = stage_->getCamera().getViewVector();
			view_vector.normalize();
			Vector3 light_source = view_vector - stage_->getCamera().getLookUpVector();;
			light_source.normalize();
		//    			Vector3 light_source = stage_->getCamera().getRightVector();

			// This should be definitely done somewhere else...
			GLint current_matrix_mode;
			glGetIntegerv(GL_MATRIX_MODE, &current_matrix_mode);

			glMatrixMode(GL_TEXTURE);
			GLfloat matrix[16];

			for (Size i=0; i<16; i++)
				matrix[i] = 0.;

		//			matrix[0] = 0.5 * light_source.x; matrix[1] = 0.5 * light_source.y; matrix[2] = 0.5 * light_source.z; matrix[3] = 1.;
		//			matrix[4] = 0.5 * view_vector.x;  matrix[5] = 0.5 * view_vector.y;  matrix[6] = 0.5 * view_vector.z;  matrix[7] = 1.;
			matrix[0] = 0.5 * light_source.x; matrix[4] = 0.5 * light_source.y; matrix[8] = 0.5 * light_source.z; matrix[12] = 1.;
			matrix[1] = 0.5 * view_vector.x;  matrix[5] = 0.5 * view_vector.y;  matrix[9] = 0.5 * view_vector.z;  matrix[13] = 1.;

			matrix[15] = 2.;

			glLoadMatrixf(matrix);
			glMatrixMode(current_matrix_mode);

			line_list_.draw();
			*/

			const std::vector<Vector3>& vertices = line.vertices;
		//   			const std::vector<Vector3>& tangents = line.tangents;
			const std::vector<ColorRGBA>& colors = line.colors;

			glBegin(GL_LINE_STRIP);

			if (colors.size() > 0) setColorRGBA_(colors[0]);

			for (Position i = 0; i < vertices.size(); i++)
			{
				if (colors.size() > 1) setColorRGBA_(colors[i]);
		//    				glTexCoord3f(tangents[i].x, tangents[i].y, tangents[i].z);
				vertexVector3_(vertices[i]);
			}

			glEnd();

		//   			glDisable(GL_TEXTURE_2D);
		}

		void GLRenderer::renderLabel_(const Label& label)
		{
			initDrawingOthers_();

			glPushMatrix();
			glDisable(GL_LIGHTING);

			glRasterPos3f((GLfloat)label.getVertex().x,
										(GLfloat)label.getVertex().y,
										(GLfloat)label.getVertex().z);

			QFontMetrics fm(label.getFont());
			QString text = label.getExpandedText().c_str();
			QRect r = fm.boundingRect(text);

			int border = 2;
			QImage pm(r.size() + QSize(border * 2, border * 2), QImage::Format_ARGB32_Premultiplied);

			QPainter p;
			p.begin(&pm);
				p.setFont(label.getFont());
				p.setCompositionMode(QPainter::CompositionMode_Source);
				p.fillRect(pm.rect(), Qt::transparent);
				p.setCompositionMode(QPainter::CompositionMode_SourceOver);
				p.setPen(label.getColor().getQColor());
				p.drawText(-r.x() + border, -r.y() + border, text);
			p.end();

			QImage gldata = QGLWidget::convertToGLFormat(pm);

			glPushAttrib(GL_BLEND);
			glEnable(GL_BLEND);
			glDrawPixels(pm.width(), pm.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());
			glPopAttrib();

			glPopMatrix();
			glEnable(GL_LIGHTING);
		}


		void GLRenderer::renderPoint_(const Point& point)
		{
			initDrawingOthers_();

			glDisable(GL_LIGHTING);
			setColor4ub_(point);
			glBegin(GL_POINTS);
			normalVector3_(normal_vector_);
			vertexVector3_(point.getVertex());
			glEnd();
			glEnable(GL_LIGHTING);
		}


		void GLRenderer::renderSimpleBox_(const SimpleBox& box)
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(box);
			translateVector3_(box.a);
			scaleVector3_(box.b - box.a);
			GL_boxes_list_[display_lists_index_].draw();
			glPopMatrix();
		}


		void GLRenderer::renderBox_(const Box& box)
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(box);

			translateVector3_(box.getPoint());

			Vector3 v1(box.getRightVector());
			if (v1.getSquareLength() != 0) v1.normalize();

			Vector3 v2(box.getHeightVector());
			if (v2.getSquareLength() != 0) v2.normalize();

			Vector3 v3(box.getDepthVector());
			if (v3.getSquareLength() != 0) v3.normalize();

			float m[16] = { v1.x, v1.y, v1.z, 0,
											v2.x, v2.y, v2.z, 0,
											v3.x, v3.y, v3.z, 0,
											0,0,0,1};
			glMultMatrixf(m);

			glScalef(box.getRightVector().getLength(),
							 box.getHeightVector().getLength(),
							 box.getDepthVector().getLength());

			GL_boxes_list_[display_lists_index_].draw();

			glPopMatrix();
		}

		void GLRenderer::renderTube_(const Tube& tube)
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(tube);

			const Vector3 result = tube.getVertex2() - tube.getVertex1();
			const float length = result.getLength();

			if (Maths::isZero(length)) return;

			// cross product with z-axis-vector and result
			const Vector3 rotation_axis(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			const float angle = Angle::toDegree(acos(result.z / length));

			translateVector3_(tube.getVertex1());

			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength());

			GL_tubes_list_[display_lists_index_].draw();

			glPopMatrix();
		}


		void GLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
		{
			initDrawingOthers_();

			const Vector3 result(tube.getVertex2() - tube.getVertex1());
			const float length = result.getLength();

			if (Maths::isZero(length)) return;

			// cross product with z-axis-vector and result
			const Vector3 rotation_axis(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			const float angle = Angle::toDegree(acos(result.z / length));

			glPushMatrix();
			setColor4ub_(tube);

			translateVector3_(tube.getVertex1());

			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength() / (float)2);

			GL_tubes_list_[display_lists_index_].draw();

			glPopMatrix();
			glPushMatrix();
			translateVector3_(tube.getMiddleVertex());

			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength() / (float)2);

			setColorRGBA_(tube.getColor2());

			GL_tubes_list_[display_lists_index_].draw();

			glPopMatrix();
		}

		void GLRenderer::renderTwoColoredLine_(const TwoColoredLine& line)
		{
			initDrawingOthers_();

			setColor4ub_(line);
			glDisable(GL_LIGHTING);

			glBegin(GL_LINES);
			vertexVector3_(line.getVertex1());
			vertexVector3_(line.getMiddleVertex());
			glEnd();

			glBegin(GL_LINES);
			setColorRGBA_(line.getColor2());
			vertexVector3_(line.getMiddleVertex());
			vertexVector3_(line.getVertex2());
			glEnd();

			glEnable(GL_LIGHTING);
		}


		void GLRenderer::initDrawingMeshes_()
		{
			if (drawed_mesh_) return;

			if (drawing_mode_ == DRAWING_MODE_DOTS)
			{
				glDisable(GL_LIGHTING);
				normalVector3_(normal_vector_);
			}
			else if (drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				glDisable(GL_LIGHTING);
			}
			else // draw the triangles solid
			{
				if (model_type_  == MODEL_CARTOON)
				{
					glEnable(GL_CULL_FACE);
				}
				else
				{
					glDisable(GL_CULL_FACE);
					glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);
				}
			}

			drawed_mesh_ = true;
			drawed_other_object_ = false;
		}


		void GLRenderer::initDrawingOthers_()
		{
			if (drawed_other_object_) return;

			if (drawing_mode_ == DRAWING_MODE_DOTS ||
					drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				glEnable(GL_LIGHTING);
			}
			else // solid
			{
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, false);
			}

			glEnable(GL_CULL_FACE);

			drawed_other_object_ = true;
			drawed_mesh_ = false;
		}


		void GLRenderer::renderMesh_(const Mesh& mesh)
		{
			if (mesh.normal.size() != mesh.vertex.size())
			{
				BALLVIEW_DEBUG;
				return;
			}

			/*
			// debugging for normals:
			initDrawingOthers_();
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);

			for (Size index = 0; index < mesh.vertex.size(); ++index)
			{
				vertexVector3_(mesh.vertex[index]);
				vertexVector3_(mesh.vertex[index] + mesh.normal[index]);
			}
			glEnd();
			glEnable(GL_LIGHTING);
			*/

			///////////////////////////////////////////////
			// here starts the normal mesh rendering code:
			initDrawingMeshes_();

			// If we have only one color for the whole mesh, this can
			// be assigned efficiently
			bool multiple_colors = true;
			if (mesh.colors.size() < mesh.vertex.size())
			{
				if (mesh.colors.size() > 0)
				{
					setColorRGBA_(mesh.colors[0]);
				}
				multiple_colors = false;
			}

			Size nr_triangles = mesh.triangle.size();

			///////////////////////////////////////////////////////////////////
			if (drawing_mode_ == DRAWING_MODE_DOTS)
			{
				glBegin(GL_POINTS);
				for (Size index = 0; index < mesh.vertex.size(); ++index)
				{
					if (multiple_colors) setColorRGBA_(mesh.colors[index]);
					vertexVector3_(mesh.vertex[index]);
				}
				glEnd();
			}
			///////////////////////////////////////////////////////////////////
			else if (drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				if (model_type_ == MODEL_CONTOUR_SURFACE)
				{
					
					Position masking_bit = 0;
					Position mask_12 = 1;
					Position mask_23 = 2;
					Position mask_31 = 4;
					Vector3 v1, v2, v3;
					vector<Position> triangle_masks;

					for (Size index = 0; index < nr_triangles; ++index)
					{
						v1 = mesh.vertex[mesh.triangle[index].v1];
						v2 = mesh.vertex[mesh.triangle[index].v2];
						v3 = mesh.vertex[mesh.triangle[index].v3];
						
						float trs = 0.02;
						if	(	((fabs(v1.x - v2.x) <= trs) && v3.x != v1.x)
								||((fabs(v1.y - v2.y) <= trs) && v3.y != v1.y)
								||((fabs(v1.z - v2.z) <= trs) && v3.z != v1.z))
							masking_bit |= mask_12;
						if	(	((fabs(v2.x - v3.x) <= trs) && v1.x != v2.x)
								||((fabs(v2.y - v3.y) <= trs) && v1.y != v2.y)
								||((fabs(v2.z - v3.z) <= trs) && v1.z != v2.z))
							masking_bit |= mask_23;		
						if	(	((fabs(v3.x - v1.x) <= trs) && v2.x != v3.x)
								||((fabs(v3.y - v1.y) <= trs) && v2.y != v3.y)
								||((fabs(v3.z - v1.z) <= trs) && v2.z != v3.z))
							masking_bit |= mask_31;
						
						masking_bit ^= 7;
						triangle_masks.push_back(masking_bit);
						masking_bit = 0;
					}

					for (Size index = 0; index < nr_triangles; ++index)
					{
						if (triangle_masks[index] == 0)
						{
							glBegin(GL_LINE_LOOP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

							glEnd();
						}
						else if (triangle_masks[index] == 1)
						{
							glBegin(GL_LINE_STRIP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

							glEnd();
						}
						else if (triangle_masks[index] == 2)
						{
							glBegin(GL_LINE_STRIP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

							glEnd();
						}
						else if (triangle_masks[index] == 3)
						{
							glBegin(GL_LINE_STRIP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

							glEnd();
						}
						else if (triangle_masks[index] == 4)
						{
							glBegin(GL_LINE_STRIP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

							glEnd();
						}
						else if (triangle_masks[index] == 5)
						{
							glBegin(GL_LINE_STRIP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

							glEnd();
						}
						else if (triangle_masks[index] == 6)
						{
							glBegin(GL_LINE_STRIP);

							normalVector3_(normal_vector_);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

							if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
							vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

							glEnd();
						}
					}
				}
				else
				{
					for (Size index = 0; index < nr_triangles; ++index)
					{
						glBegin(GL_LINE_LOOP);

						normalVector3_(normal_vector_);

						if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

						if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

						if (multiple_colors) setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);

						glEnd();
					}
				}
			}
			///////////////////////////////////////////////////////////////////
			else if (drawing_mode_ == DRAWING_MODE_SOLID)				// draw the triangles solid
			{
				glBegin(GL_TRIANGLES);
				for (Size index = 0; index < nr_triangles; ++index)
				{
					Position p = mesh.triangle[index].v1;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.triangle[index].v2;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.triangle[index].v3;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);
				}
				glEnd();
			}
			else 		// draw the triangles per cel shading
			{
				// a part of this code stems from http://nehe.gamedev.net lesson 37
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_BACK, GL_LINE);										// Draw Backfacing Polygons As Wireframes
				glLineWidth(3);																			// Set The Line Width
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);																// Don't Draw Any Front-Facing Polygons
				glDepthFunc(GL_LEQUAL);															// Change The Depth Mode
				setColorRGBA_(ColorRGBA(0.,0.,0.,1.));
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);							// Use The Good Calculations
				glEnable(GL_LINE_SMOOTH);														// Enable Anti-Aliasing

				glBegin(GL_TRIANGLES);															// Tell OpenGL What We Want To Draw
				for (Size index = 0; index < nr_triangles; ++index)
				{
					vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
				}
				glEnd ();													// Tell OpenGL We've Finished

				// reset to normal:
				glCullFace(GL_BACK);							// Reset The Face To Be Culled
				glPolygonMode (GL_BACK, GL_FILL);	// Reset Back-Facing Polygon Drawing Mode
				glLineWidth(1);										// Set The Line Width

				// map the texture so it simulates shadows:
				glEnable(GL_TEXTURE_1D);
				glBindTexture(GL_TEXTURE_1D, cel_texture_);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

				// some artefacts when using GL_LINEAR:
		//   				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//   				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				vector<float> tex_values;
				tex_values.reserve(mesh.normal.size());
				Vector3 vv = -scene_->getStage()->getCamera().getViewVector() ;
				vv.normalize();
				// other light positions possible:
		//    				vv += scene_->getStage()->getCamera().getLookUpVector() / 2.0;
		//    				vv += scene_->getStage()->getCamera().getRightVector() / 2.0;
		//    				vv.normalize();

				float v;
				for (Position p = 0; p < mesh.normal.size(); p++)
				{
					v = mesh.normal[p] * vv;
					if (v < 0) v = 0;
					tex_values.push_back(std::min(1.f, v));
				}

				// prevent problems with single colored meshes:
				if (mesh.colors.size() > 0) setColorRGBA_(mesh.colors[0]);

				glBegin(GL_TRIANGLES);
				for (Size index = 0; index < nr_triangles; ++index)
				{
					Position p = mesh.triangle[index].v1;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					glTexCoord1f (tex_values[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.triangle[index].v2;
					glTexCoord1f (tex_values[p]);
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.triangle[index].v3;
					glTexCoord1f (tex_values[p]);
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					vertexVector3_(mesh.vertex[p]);
				}

				glEnd();
				glEnable(GL_LIGHTING);
				glDisable(GL_TEXTURE_1D);
			}
		}


		void GLRenderer::initGLU_(DrawingMode mode)
		{
			if (mode == DRAWING_MODE_WIREFRAME)
			{
				gluQuadricDrawStyle(GLU_quadric_obj_, GLU_LINE);
			}
			else if (mode >= DRAWING_MODE_SOLID)
			{
				gluQuadricDrawStyle(GLU_quadric_obj_, GLU_FILL);
			}
			else if (mode == DRAWING_MODE_DOTS)
			{
				gluQuadricDrawStyle(GLU_quadric_obj_, GLU_POINT);
			}
			else
			{
				BALLVIEW_DEBUG;
			}
		}

		void GLRenderer::createSpheres_()
		{
			glPushMatrix();

			Position slices[4] = {14, 24, 64, 100};
			Position stacks[4] = {8, 16, 44, 80};

			for (Position mode = DRAWING_MODE_DOTS; mode < BALL_VIEW_MAXIMAL_DRAWING_MODE; mode++)
			{
				initGLU_((DrawingMode)mode);

				for (Position dp = DRAWING_PRECISION_LOW; dp <= DRAWING_PRECISION_ULTRA; dp++)
				{
					GL_spheres_list_[mode * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + dp].startDefinition();
					gluSphere(GLU_quadric_obj_, 1, slices[dp], stacks[dp]);
					GL_spheres_list_[mode * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + dp].endDefinition();
				}
			}

			glPopMatrix();
		}


		void GLRenderer::createDottedSphere_(int precision)
		{
			glBegin(GL_POINTS);

			vector<Vector3> results = createSphere((Size)precision);
			for (Position p = 0; p < results.size(); p++)
			{
				vertexVector3_(results[p]);
			}

			glEnd();
		}


		void GLRenderer::createTubes_()
		{
			glPushMatrix();

			Position slices[4] = {6, 10, 20, 64};

			for (Position mode = DRAWING_MODE_DOTS; mode < BALL_VIEW_MAXIMAL_DRAWING_MODE; mode++)
			{
				initGLU_((DrawingMode)mode);

				for (Position dp = DRAWING_PRECISION_LOW; dp <= DRAWING_PRECISION_ULTRA; dp++)
				{
					GL_tubes_list_[mode * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + dp].startDefinition();
					gluCylinder(GLU_quadric_obj_, 1, 1, 1, slices[dp], 1);
					GL_tubes_list_[mode * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + dp].endDefinition();
				}
			}

			glPopMatrix();
		}


		void GLRenderer::createBoxes_()
		{
			// building point display list
			for (Position pos = 0; pos < BALL_VIEW_MAXIMAL_DRAWING_PRECISION; pos++)
			{
				GL_boxes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + pos].startDefinition();
				createDotBox_();
				GL_boxes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + pos].endDefinition();
			}

			// building wireframe display list
			for (Position pos = 0; pos < BALL_VIEW_MAXIMAL_DRAWING_PRECISION; pos++)
			{
				GL_boxes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + pos].startDefinition();
				createLineBox_();
				GL_boxes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + pos].endDefinition();
			}

			// building solid display list
			for (Position pos = 0; pos < BALL_VIEW_MAXIMAL_DRAWING_PRECISION; pos++)
			{
				GL_boxes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + pos].startDefinition();
				createSolidBox_();
				GL_boxes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + pos].endDefinition();
			}
		}


		void GLRenderer::createLineBox_()
		{
			glBegin(GL_LINES);

			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);

			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);

			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);

			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);

			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);

			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);

			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);

			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);

			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);

			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);

			glEnd();
		}


		void GLRenderer::createDotBox_()
		{
			glBegin(GL_POINTS);

			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);

			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			glEnd();
		}


		void GLRenderer::createSolidBox_()
		{
			glBegin(GL_QUADS);

			// back
			glNormal3f((GLfloat)0, (GLfloat)0, (GLfloat)-1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);

			// above
			glNormal3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);

			// front
			glNormal3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			// under
			glNormal3f((GLfloat)0, (GLfloat)-1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);

			// left
			glNormal3f((GLfloat)-1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)0, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)0, (GLfloat)1, (GLfloat)1);

			// right
			glNormal3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)1);
			glVertex3f((GLfloat)1, (GLfloat)0, (GLfloat)0);
			glVertex3f((GLfloat)1, (GLfloat)1, (GLfloat)0);

			glEnd();
		}


		// ############################ PICKING ###################################
		void GLRenderer::pickObjects(Position x1, Position y1, Position x2, Position y2,
		                             list<GeometricObject*>& objects)
		{
			pickObjects1(x1, y1, x2, y2);
			renderToBuffer(NULL, GLRenderer::DIRECT_RENDERING);
			pickObjects2(objects);
		}

		void GLRenderer::pickObjects1(Position x1, Position y1, Position x2, Position y2)
		{
			glFlush();
			GLint viewport[4];
			// init name stack for 32000 objects
			glGetIntegerv(GL_VIEWPORT, viewport);
			glSelectBuffer(BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS, object_buffer_);
			// uncoment this for debuging:
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			glMatrixMode(GL_PROJECTION);

			glPushMatrix();
			glLoadIdentity();

			// calculate picking rectangle
			auto width  = std::abs((Index)x2 - (Index)x1);
			auto height = std::abs((Index)y2 - (Index)y1);

			Position center_x = std::min(x2, x1) + width / 2;
			Position center_y = std::min(y2, y1) + height / 2;

			if (width == 0)	width = 1;
			if (height == 0) height = 1;

			single_pick_ = (width <= 3 && height <= 3);
			clearNames_();

			// calculate picking matrix
			gluPickMatrix(center_x, viewport[3] - center_y, width, height, viewport);

			// prepare camera
			setProjection();

			glMatrixMode(GL_MODELVIEW);

			updateCamera();
		}

		void GLRenderer::pickObjects2(list<GeometricObject*>& objects)
		{
			glFlush();

			glMatrixMode(GL_PROJECTION);
			// get number of hits
			int number_of_hits = glRenderMode(GL_RENDER);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			// return if no objects are picked
			if (number_of_hits == 0)
			{
				updateCamera();
				return;
			}

			Position minimum_z_coord = UINT_MAX;
			Position names;
			Name nearest_name = 0;
			Position* object_buffer_ptr = (Position*) object_buffer_;
			GeometricObject* go = 0;

			// collect only the nearest Object
			if (single_pick_)
			{
				Position z_coord;

				// find minimum z-coord
				for (Index index = 0; index < number_of_hits; ++index)
				{
					names = *object_buffer_ptr;
					++object_buffer_ptr;
					z_coord = *object_buffer_ptr;

					object_buffer_ptr += 2;

					if (z_coord <= minimum_z_coord)
					{
						minimum_z_coord = z_coord;
						nearest_name = *object_buffer_ptr;
					}

					object_buffer_ptr += names;
				}

				go = getObject(nearest_name);
				if (go != 0) objects.push_back(go);
			}
			else // collect all objects that are in the picking area
			{
				for (Index index = 0; index < number_of_hits; ++index)
				{
					names = *object_buffer_ptr;
					object_buffer_ptr += 3;
					nearest_name = *object_buffer_ptr;
					object_buffer_ptr += names;

					go = getObject(nearest_name);
					if (go != 0) objects.push_back(go);
				}
			}

			updateCamera();
		}

		void GLRenderer::setFogIntensity(float intensity)
		{
			if (intensity == 0)
			{
				glDisable(GL_FOG);
			}
			else
			{
				glEnable(GL_FOG);

				ColorRGBA co = stage_->getBackgroundColor();
				GLfloat color[4] = {(float) co.getRed(), (float) co.getGreen(), (float) co.getBlue(), 1.0};
				glFogfv(GL_FOG_COLOR, color);

				glFogf(GL_FOG_START, 10.0);
				float end = stage_->getFogIntensity();
				end = 400 - end;
				end += 20;
				glFogf(GL_FOG_END, end);
				glFogi(GL_FOG_MODE, GL_LINEAR);

				// doesnt work as expected:
				// glFogf(GL_FOG_END, 400);
				// glFogf(GL_FOG_DENSITY, ((float) stage_->getFogIntensity()) / 40.0);
			}
		}

		// ############################ MOVEMENT/SIZE ###################################
		// TODO: shouldn't we use a camera aperture angle?
		void GLRenderer::setSize(float width, float height)
		{
			width_ 	= width;
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

			glViewport(0, 0, (int)width_, (int)height_);

			initPerspective();
		}


		void GLRenderer::updateCamera(const Camera* camera)
		{
			if (camera == 0) camera = &(stage_->getCamera());

			if (Maths::isZero(camera->getViewVector().getSquareLength()))
			{
				Log.error() << (String)(qApp->translate("BALL::VIEW::GLRenderer", "Invalid camera settings: View point = LookAt point")) << std::endl;
				return;
			}

			glLoadIdentity();

			gluLookAt(camera->getViewPoint().x,
								camera->getViewPoint().y,
								camera->getViewPoint().z,
								camera->getLookAtPosition().x,
								camera->getLookAtPosition().y,
								camera->getLookAtPosition().z,
								camera->getLookUpVector().x,
								camera->getLookUpVector().y,
								camera->getLookUpVector().z);

			normal_vector_ = (-camera->getViewVector().normalize());

			setLights(false);
		}

		void GLRenderer::setupStereo(float eye_separation, float focal_length)
		{
			// TODO: - make near and far clip configurable!!!
			//       - keep the same frustrum until either the size or the stereo settings change

			float ndfl    = near_ / focal_length;

			// TODO: is this correct?
			float new_left   = 2*stereo_frustum_conversion_width_*left_  - eye_separation * ndfl;
			float new_right  = 2*stereo_frustum_conversion_width_*right_ - eye_separation * ndfl;

			float new_top    = top_    * stereo_frustum_conversion_height_;
			float new_bottom = bottom_ * stereo_frustum_conversion_height_;

			glMatrixMode(GL_PROJECTION);

			glLoadIdentity();

			if (stage_->getCamera().getProjectionMode() == Camera::PERSPECTIVE)
				glFrustum(new_left, new_right, new_bottom, new_top, near_, far_);
			else
				glOrtho(new_left   * orthographic_zoom_, new_right * orthographic_zoom_, 
				        new_bottom * orthographic_zoom_, new_top   * orthographic_zoom_, near_, far_);

			glViewport(0, 0, width_, height_);

			glMatrixMode(GL_MODELVIEW);
		}

		bool GLRenderer::hasDisplayListFor(const Representation& rep) const
		{
			return display_lists_.has(&rep);
		}

		bool GLRenderer::isExtensionSupported(const char* extension) const
		{
			return QOpenGLContext::currentContext()->hasExtension(extension);
		}

		String GLRenderer::getVendor()
		{
			if (glGetString(GL_VENDOR) == 0) return "";
			return (char*)glGetString(GL_VENDOR);
		}

		String GLRenderer::getRenderer()
		{
			if (glGetString(GL_RENDERER) == 0) return "";
			return (char*)glGetString(GL_RENDERER);
		}

		String GLRenderer::getOpenGLVersion()
		{
			if (runningOnVirtualBox())
			{
				return (String)qApp->translate("BALL::VIEW::GLRenderer","<Unable to get OpenGL version on Virtual Box probably due to driver bug>");
			}

			if (glGetString(GL_VERSION) == 0) return "";
			return (char*)glGetString(GL_VERSION);
		}

		vector<String> GLRenderer::getExtensions()
		{
			vector<String> string_vector;
			char* extensions = (char*)glGetString(GL_EXTENSIONS);

			if (!extensions) 
				return string_vector;

			String exts(extensions);

			exts.split(string_vector);
			return string_vector;
		}

		bool GLRenderer::runningOnVirtualBox()
		{
			return (getVendor() == "Humper") && (getRenderer() == "Chromium");
		}

		bool GLRenderer::enableVertexBuffers(bool state)
		{
			if (!isExtensionSupported("GL_ARB_vertex_buffer_object"))
			{
				use_vertex_buffer_ = false;
				return false;
			}

			if (state != use_vertex_buffer_)
			{
				if (state) Log.info() << (String)qApp->translate("BALL::VIEW::GLRenderer", "Enabling Vertex Buffer") << std::endl;
				else       Log.info() << (String)qApp->translate("BALL::VIEW::GLRenderer", "Disabling Vertex Buffer") << std::endl;
			}
			use_vertex_buffer_ = state;

			return true;
		}

		void GLRenderer::clearVertexBuffersFor(Representation& rep)
		{
			MeshBufferHashMap::Iterator vit = rep_to_buffers_.find(&rep);
			if (vit == rep_to_buffers_.end()) return;

			vector<MeshBuffer*>& meshes = vit->second;
			vector<MeshBuffer*>::iterator bit = meshes.begin();
			for (; bit != meshes.end(); bit++)
			{
				delete *bit;
			}

			meshes.clear();
			rep_to_buffers_.erase(vit);
		}

		void GLRenderer::drawBuffered(const Representation& rep)
		{
			if (rep.isHidden()) return;

			// if we have vertex buffers for this Representation, draw them
			if (use_vertex_buffer_ && drawing_mode_ != DRAWING_MODE_WIREFRAME)
			{
				MeshBufferHashMap::Iterator vit = rep_to_buffers_.find(&rep);
				if (vit != rep_to_buffers_.end())
				{
					initDrawingMeshes_();
					MeshBuffer::setGLRenderer(this);
					vector<MeshBuffer*>& buffers = vit->second;

					vector<MeshBuffer*>::iterator bit = buffers.begin();
					for (; bit != buffers.end(); bit++)
					{
						(*bit)->draw();
					}
				}
			}

			// if we have a displaylist for this Representation, draw it
			DisplayListHashMap::Iterator dit = display_lists_.find(&rep);
			if (dit != display_lists_.end())
			{
				dit->second->draw();
			}
		}

		void GLRenderer::enterPickingMode()
		{
			picking_mode_ = true;
		}

		void GLRenderer::exitPickingMode()
		{
			picking_mode_ = false;
			object_to_name_.clear();
			name_to_object_.clear();
			all_names_ = 1;
		}


		bool GLRenderer::vertexBuffersSupported() const
		{
			return isExtensionSupported("GL_ARB_vertex_buffer_object");
		}

		void GLRenderer::renderClippingPlane_(const ClippingPlane& plane)
		{
			display_lists_index_ = DRAWING_MODE_SOLID * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + DRAWING_PRECISION_HIGH;

			glPushAttrib(GL_LIGHTING_BIT | GL_BLEND);
			const Vector3& point(plane.getPoint());
			const Vector3& n(plane.getNormal());

			Tube tube;
			tube.setVertex1(point);
			tube.setVertex2(point - (plane.getNormal() * 10.0));
			tube.setColor(ColorRGBA(0,255,255));
			tube.setRadius(1);

			initDrawingOthers_();
			initTransparent();
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);
			glDisable(GL_CULL_FACE);
			renderTube_(tube);

			initDrawingOthers_();
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDepthMask(GL_FALSE);

			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);
			glDisable(GL_CULL_FACE);

			glPushMatrix();
			setColorRGBA_(ColorRGBA(0,0,255, 190));;
			translateVector3_(point);
			const Vector3 rotation_axis(-n.y, n.x, 0.0);
			// angle between z-axis-vector and result
			const float angle = Angle::toDegree(acos(n.z / n.getLength()));
			rotateVector3Angle_(rotation_axis, angle);

			initGLU_(DRAWING_MODE_SOLID);
			gluDisk(GLU_quadric_obj_, 0, 20 , 140, 80);

			glPopMatrix();
			glEnable(GL_CULL_FACE);

			glPopAttrib();
		}

		void GLRenderer::initPerspective()
		{
			if (getStereoMode() == GLRenderer::ACTIVE_STEREO)
			{
				//  "Dont call GLRenderer::initPerspective() in Stereo mode! "
				return;
			}

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			setProjection();

			glMatrixMode(GL_MODELVIEW);
		}

		void GLRenderer::setProjection()
		{
			left_   = -2 * x_scale_;
			right_  =  2 * x_scale_;
			bottom_ = -2 * y_scale_;
			top_    =  2 * y_scale_;

			if (stage_->getCamera().getProjectionMode() == Camera::PERSPECTIVE)
			{
				glFrustum(left_, right_, bottom_, top_, near_, far_);
			}
			else
			{
				glOrtho(left_   * orthographic_zoom_, right_ * orthographic_zoom_, 
				        bottom_ * orthographic_zoom_,   top_ * orthographic_zoom_, near_, far_);
			}
		}

		void GLRenderer::setOrthographicZoom(float orthographic_zoom)
		{
			orthographic_zoom_ = orthographic_zoom;
		}

		float GLRenderer::getOrthographicZoom()
		{
			return orthographic_zoom_;
		}

		void GLRenderer::generateIlluminationTexture_(float ka, float kd, float kr, float shininess)
		{
			enum { R = 0, G = 1, B = 2, A = 3 };

		//   			assert( 0. <= ka && ka <= 1. );
		//   			assert( 0. <= kd && kd <= 1. );
		//   			assert( 0. <= kr && kr <= 1. );
		//   			float k = ka + kd + kr;
		//   			assert( 0. <= k  &&  k <= 1. );

			Index i = 0;
			Index j = 0;
			for( i = 0; i < TEXTURE_SIZE; ++i )
			{
				for( j = 0; j < TEXTURE_SIZE; ++j )
				{
					float x = ( (float) j + 1 ) / (TEXTURE_SIZE + 1);
					float y = ( (float) i + 1 ) / (TEXTURE_SIZE + 1);

					float LT = 2 * x - 1;
					float VT = 2 * y - 1;

					float intensity = 0;
					intensity += ka;
					intensity += kd * sqrt( 1. - LT*LT );
					intensity += kr * pow( fabs( LT*VT - sqrt( 1. - LT*LT ) * sqrt( 1. - VT*VT ) ), shininess * 255. );

					if( intensity < 0. || 1. < intensity )
					{
						if( intensity < 0. ) intensity = 0;
						if( 1. < intensity ) intensity = 1;
					}
		//   					assert( 0. <= intensity && intensity <= 1. );

					Index c = (Index)(intensity * 255);
		//   					assert( 0 <= c && c <= 255 );

					unsigned int lineTexIndex = i * TEXTURE_SIZE * NUM_CHANNELS  + j * NUM_CHANNELS;
					line_tex_[lineTexIndex + R] = (GLubyte) c;
					line_tex_[lineTexIndex + G] = (GLubyte) c;
					line_tex_[lineTexIndex + B] = (GLubyte) c;
					line_tex_[lineTexIndex + A] = 127;
				}
			}
		}

		Position GLRenderer::getTextureIndex_(Position x, Position y, Position z, Size width, Size height)
		{
			return BYTES_PER_TEXEL * (x * width + y) +
						 BYTES_PER_TEXEL * width * height * z;
		}

		Position GLRenderer::createTextureFromGrid(const GridVisualisation& vis)
		{
			const RegularData3D& grid = *vis.getGrid();
			const ColorMap& map = *vis.getColorMap().get();

			// prevent warning and error if not using GLEW:
			Position texname = 0;
#ifdef BALL_HAS_GLEW
			removeTextureFor_(grid);
			RegularData3D::IndexType tex_size = grid.getSize();

			// Generate The Texture
			Position i = 0;
			GLubyte* texels = new GLubyte[tex_size.x * tex_size.y * tex_size.z * BYTES_PER_TEXEL];
			for (Position z = 0; z < tex_size.z; z++)
			{
				for (Position y = 0; y < tex_size.y; y++)
				{
					for (Position x = 0; x < tex_size.x; x++)
					{
						const ColorRGBA& c = map.map(grid.getData(RegularData3D::IndexType(x,y,z)));
						texels[i + 0] = (unsigned char)c.getRed();
						texels[i + 1] = (unsigned char)c.getGreen();
						texels[i + 2] = (unsigned char)c.getBlue();
						texels[i + 3] = (unsigned char)c.getAlpha();
						i += 4;
					}
				}
			}

			glGenTextures(1, (GLuint*)&texname);
			glBindTexture(GL_TEXTURE_3D, texname);
			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, tex_size.x, tex_size.y, tex_size.z, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
			glBindTexture(GL_TEXTURE_3D, 0);
			grid_to_texture_[&grid] = texname;
			delete[] texels;
#endif
			return texname;
		}

		void GLRenderer::removeTextureFor_(const RegularData3D& grid)
		{
			if (!grid_to_texture_.has(&grid)) return;
			glDeleteTextures(1, (GLuint*)&grid_to_texture_[&grid]);
			grid_to_texture_.erase(&grid);
		}

		void GLRenderer::setupGridClipPlanes_(const GridVisualisation& slice)
		{
			double planes[6][4];

			Vector3 x,y,z;
			x = slice.x;
			x.normalize();
			y = slice.y;
			y.normalize();
			z = slice.z;
			z.normalize();


			Vector3 origin = slice.origin;
			Vector3 dv = slice.x + slice.y + slice.z;
			dv *= 0.001;
			Vector3 e = origin + slice.x + slice.y + slice.z + dv ;
			origin -= dv;

			float d = x * (-origin);
			planes[0][0] = x.x; planes[0][1] = x.y; planes[0][2] = x.z; planes[0][3] = d;
			d = y * (-origin);
			planes[1][0] = y.x; planes[1][1] = y.y; planes[1][2] = y.z; planes[1][3] = d;
			d = z * (-origin);
			planes[2][0] = z.x; planes[2][1] = z.y; planes[2][2] = z.z; planes[2][3] = d;

			d = -x * (-e);
			planes[3][0] = -x.x; planes[3][1] = -x.y; planes[3][2] = -x.z; planes[3][3] = d;
			d = -y * (-e);
			planes[4][0] = -y.x; planes[4][1] = -y.y; planes[4][2] = -y.z; planes[4][3] = d;
			d = -z * (-e);
			planes[5][0] = -z.x; planes[5][1] = -z.y; planes[5][2] = -z.z; planes[5][3] = d;

			for (Position plane = GL_CLIP_PLANE0 + 0; plane < GL_CLIP_PLANE0 + 6; plane++)
			{
				glClipPlane(plane, &planes[plane - GL_CLIP_PLANE0][0]);
				glEnable(plane);
			}
		}


		void GLRenderer::renderGridVisualisation_(const GridVisualisation& vol)
		{
			GLuint texname;
			if (!grid_to_texture_.has(vol.getGrid()))
			{
				texname = createTextureFromGrid(vol);
				grid_to_texture_[vol.getGrid()] = texname;
			} else {
				texname = grid_to_texture_[vol.getGrid()];
			}

			if (texname == 0)
			{
				scene_->setStatusbarText((String)qApp->translate("BALL::VIEW::GLRenderer", "Graphics card does not support 3D textures. Abort"), true);
				return;
			}

			const Vector3 origin = vol.origin;

			if (vol.draw_box)
			{
				Box box(origin, vol.x, vol.y, vol.z);
				box.setColor(stage_->getBackgroundColor().getInverseColor());
				Position dli = display_lists_index_;
				display_lists_index_ = DRAWING_MODE_WIREFRAME * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + drawing_precision_;
				renderBox_(box);
				display_lists_index_ = dli;
			}

			initDrawingOthers_();
			glDisable(GL_LIGHTING);
			glDisable(GL_CULL_FACE);

			////////////////////////////////////////////////////////////////////////////////
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_3D, texname);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

			///// init the texture: automated texture coordinate generation
			// normalized vectors in grids directions:
			Vector3 xd = vol.x;
			xd.normalize();
			Vector3 yd = vol.y;
			yd.normalize();
			Vector3 zd = vol.z;
			zd.normalize();

			// plane vectors and distance for texture coordinates:
			float xp[4], yp[4], zp[4], d;
			Vector3 n;

			n = zd;
			d = n * (-origin);
			xp[0] = n.x; xp[1] = n.y; xp[2] = n.z; xp[3] = d;

			n = xd;
			d = n * (-origin);
			yp[0] = n.x; yp[1] = n.y; yp[2] = n.z; yp[3] = d;

			n = yd;
			d = n * (-origin);
			zp[0] = n.x; zp[1] = n.y; zp[2] = n.z; zp[3] = d;

			glEnable(GL_TEXTURE_3D);
			glTexGenf(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGenf(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGenf(GL_R,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
			glTexGenfv(GL_S,GL_OBJECT_PLANE, yp);
			glTexGenfv(GL_T,GL_OBJECT_PLANE, zp);
			glTexGenfv(GL_R,GL_OBJECT_PLANE, xp);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);

			// stretch the texture accordingly
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			const Vector3 dim = vol.x + vol.y + vol.z;
			glScaled((double)1.0 / (double) dim.x,
							 (double)1.0 / (double) dim.y,
							 (double)1.0 / (double) dim.z);
			glMatrixMode(GL_MODELVIEW);

			// render this as one slice
			if (vol.type == GridVisualisation::PLANE)
			{
				setupGridClipPlanes_(vol);
				Vector3 n = vol.getNormal();
				Vector3 v1 = getNormal(n);
				Vector3 v2 = v1 % n;

				v1.normalize();
				v2.normalize();
				v1 *= vol.max_dim * 2.0;
				v2 *= vol.max_dim * 2.0;

				Vector3 o = vol.getPoint() - v1 - v2;

				v1 *= 2.0;
				v2 *= 2.0;

				glBegin(GL_QUADS);
				normalVector3_(-vol.getNormal());
				vertexVector3_(o);
				vertexVector3_(o + v1);
				vertexVector3_(o + v1 + v2);
				vertexVector3_(o + v2);
				glEnd();

				for (Position plane = GL_CLIP_PLANE0; plane < GL_CLIP_PLANE0 + 6; plane++)
				{
					glDisable(plane);
				}
			}
			else if (vol.type == GridVisualisation::SLICES)
			{
				// volume rendering
				const Vector3& vv = scene_->getStage()->getCamera().getViewVector();

				Vector3 normals[3];
				normals[0] = vol.x;
				normals[1] = vol.y;
				normals[2] = vol.z;

				// calculate the angles between vv and the axes of the grid
				Angle angles[3], aangles[3];

				for (Position i = 0; i < 3; i++)
				{
					angles[i] = normals[i].getAngle(vv);
					aangles[i] = (-normals[i]).getAngle(vv);
				}

				Angle min_angle(angles[0]);
				Position min = 0;
				bool anti = false;
				for (Position i = 0; i < 3; i++)
				{
					if (angles[i] < min_angle)
					{
						min = i;
						anti = false;
						min_angle = angles[i];
					}

					if (aangles[i] < min_angle)
					{
						min = i;
						anti = true;
						min_angle = aangles[i];
					}
				}

				// order the axes: 3. vector is the axis nearest to view vector direction
				Vector3 vectors[2];
				Position v = 0;
				Vector3 normal;
				for (Position i = 0; i < 3; i++)
				{
					if (i == min)
					{
						normal = normals[i];
						continue;
					}

					vectors[v] = normals[i];
					v++;
				}

				// order of layers: depends on direction of normal to view vector direction
				Vector3 offset;
				Vector3 diff = normal;
				if (!anti)
				{
					offset = normal;
					diff *= -1;
				}

				// normal and start points
				diff /= ((float)vol.slices - 1.);
				Vector3 o  = origin + offset;
				Vector3 x  = o + vectors[0];
				Vector3 xy = x + vectors[1];
				Vector3 y  = o + vectors[1];

				glBegin(GL_QUADS);
				for (Position i = 0; i < vol.slices; ++i)
				{
					vertexVector3_(y);
					vertexVector3_(xy);
					vertexVector3_(x);
					vertexVector3_(o);

					o  += diff;
					x  += diff;
					xy += diff;
					y  += diff;
				}
				glEnd();
			}
			else
			{
				glPointSize(vol.getDotSize());
				glBegin(GL_POINTS);
				for (Position p = 0; p < vol.points.size(); p++)
				{
					vertexVector3_(vol.points[p]);
				}
				glEnd();
				glEnable(GL_LIGHTING);
				glPointSize(1);
			}

			glEnable(GL_LIGHTING);
			glEnable(GL_CULL_FACE);

			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glBindTexture(GL_TEXTURE_3D, 0);
		}

		void GLRenderer::renderQuadMesh_(const QuadMesh& mesh)
		{
			if (mesh.normal.size() != mesh.vertex.size())
			{
				BALLVIEW_DEBUG;
				return;
			}

			/*
			// debugging for normals:
			initDrawingOthers_();
			glDisable(GL_LIGHTING);
			glBegin(GL_LINES);

			for (Size index = 0; index < mesh.vertex.size(); ++index)
			{
				vertexVector3_(mesh.vertex[index]);
				vertexVector3_(mesh.vertex[index] + mesh.normal[index]);
			}
			glEnd();
			glEnable(GL_LIGHTING);
			*/

			///////////////////////////////////////////////
			// here starts the normal mesh rendering code:
			initDrawingMeshes_();

			// If we have only one color for the whole mesh, this can
			// be assigned efficiently
			bool multiple_colors = true;
			if (mesh.colors.size() < mesh.vertex.size())
			{
				if (mesh.colors.size() > 0)
				{
					setColorRGBA_(mesh.colors[0]);
				}
				multiple_colors = false;
			}

			Size nr_triangles = mesh.quad.size();

			///////////////////////////////////////////////////////////////////
			if (drawing_mode_ == DRAWING_MODE_DOTS)
			{
				glBegin(GL_POINTS);
				for (Size index = 0; index < mesh.vertex.size(); ++index)
				{
					if (multiple_colors) setColorRGBA_(mesh.colors[index]);
					vertexVector3_(mesh.vertex[index]);
				}
				glEnd();
			}
			///////////////////////////////////////////////////////////////////
			else if (drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				for (Size index = 0; index < nr_triangles; ++index)
				{
					glBegin(GL_LINE_STRIP);

					normalVector3_(normal_vector_);

					if (multiple_colors) setColorRGBA_(mesh.colors[mesh.quad[index].q1]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q1]);

					if (multiple_colors) setColorRGBA_(mesh.colors[mesh.quad[index].q2]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q2]);

					if (multiple_colors) setColorRGBA_(mesh.colors[mesh.quad[index].q3]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q3]);

					if (multiple_colors) setColorRGBA_(mesh.colors[mesh.quad[index].q4]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q4]);

					glEnd();
				}
			}
			///////////////////////////////////////////////////////////////////
			else if (drawing_mode_ == DRAWING_MODE_SOLID)				// draw the triangles solid
			{
				glBegin(GL_QUADS);
				for (Size index = 0; index < nr_triangles; ++index)
				{
					Position p = mesh.quad[index].q1;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.quad[index].q2;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.quad[index].q3;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.quad[index].q4;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					normalVector3_(mesh.normal[p]);
					vertexVector3_(mesh.vertex[p]);
				}
				glEnd();
			}
			else 		// draw the triangles per cel shading
			{
				// a part of this code stems from http://nehe.gamedev.net lesson 37
				glDisable(GL_LIGHTING);
				glPolygonMode(GL_BACK, GL_LINE);										// Draw Backfacing Polygons As Wireframes
				glLineWidth(3);																			// Set The Line Width
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);																// Don't Draw Any Front-Facing Polygons
				glDepthFunc(GL_LEQUAL);															// Change The Depth Mode
				setColorRGBA_(ColorRGBA(0.,0.,0.,1.));
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);							// Use The Good Calculations
				glEnable(GL_LINE_SMOOTH);														// Enable Anti-Aliasing

				glBegin(GL_TRIANGLES);															// Tell OpenGL What We Want To Draw
				for (Size index = 0; index < nr_triangles; ++index)
				{
					vertexVector3_(mesh.vertex[mesh.quad[index].q1]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q2]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q3]);
					vertexVector3_(mesh.vertex[mesh.quad[index].q3]);
				}
				glEnd ();													// Tell OpenGL We've Finished

				// reset to normal:
				glCullFace(GL_BACK);							// Reset The Face To Be Culled
				glPolygonMode (GL_BACK, GL_FILL);	// Reset Back-Facing Polygon Drawing Mode
				glLineWidth(1);										// Set The Line Width

				// map the texture so it simulates shadows:
				glEnable(GL_TEXTURE_1D);
				glBindTexture(GL_TEXTURE_1D, cel_texture_);
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
				glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

				// some artefacts when using GL_LINEAR:
		//   				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//   				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				vector<float> tex_values;
				tex_values.reserve(mesh.normal.size());
				Vector3 vv = -scene_->getStage()->getCamera().getViewVector() ;
				vv.normalize();
				// other light positions possible:
		//    				vv += scene_->getStage()->getCamera().getLookUpVector() / 2.0;
		//    				vv += scene_->getStage()->getCamera().getRightVector() / 2.0;
		//    				vv.normalize();

				float v;
				for (Position p = 0; p < mesh.normal.size(); p++)
				{
					v = mesh.normal[p] * vv;
					if (v < 0) v = 0;
					tex_values.push_back(std::min(1.f, v));
				}

				// prevent problems with single colored meshes:
				if (mesh.colors.size() > 0) setColorRGBA_(mesh.colors[0]);

				glBegin(GL_QUADS);
				for (Size index = 0; index < nr_triangles; ++index)
				{
					Position p = mesh.quad[index].q1;
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					glTexCoord1f (tex_values[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.quad[index].q2;
					glTexCoord1f (tex_values[p]);
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.quad[index].q3;
					glTexCoord1f (tex_values[p]);
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					vertexVector3_(mesh.vertex[p]);

					p = mesh.quad[index].q4;
					glTexCoord1f (tex_values[p]);
					if (multiple_colors) setColorRGBA_(mesh.colors[p]);
					vertexVector3_(mesh.vertex[p]);
				}

				glEnd();
				glEnable(GL_LIGHTING);
				glDisable(GL_TEXTURE_1D);
			}
		}

		void GLRenderer::getFrustum(float& near_f, float& far_f, float& left_f, float& right_f, float& top_f, float& bottom_f)
		{
			near_f = near_;
			far_f  = far_;

			left_f  = left_;
			right_f = right_;

			top_f    = top_;
			bottom_f = bottom_;
		}


#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <glRenderer.iC>
#	endif

	} // namespace VIEW
} // namespace BALL
