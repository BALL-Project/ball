#include <BALL/VIEW/WIDGETS/stereoHalfImage.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/line.h>

#include <QtGui/QDesktopWidget>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/DIALOGS/stageSettings.h>

#include <GL/glu.h>

#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QImage>

namespace BALL
{
	namespace VIEW
	{
		StereoHalfImage::StereoHalfImage(Index display_number, bool left_eye)
			: QGLWidget(Scene::gl_format_, QApplication::desktop()->screen(display_number)),
				gl_renderer_(NULL),
				stage_(NULL),
				scene_(NULL),
				mode_(Scene::DIRECT_RENDERING)
		{
			prefactor_ = left_eye ? 1. : -1.;
			setAutoBufferSwap(false);
		}

		StereoHalfImage::StereoHalfImage(Stage* stage, Scene* scene, Index display_number, bool left_eye)
			: QGLWidget(Scene::gl_format_, QApplication::desktop()->screen(display_number)),
				stage_(stage),
				scene_(scene),
				mode_(Scene::DIRECT_RENDERING)
		{
			makeCurrent();
			gl_renderer_ = new GLRenderer;
			gl_renderer_->init(*scene_);
			gl_renderer_->init(*stage_, width(), height());
			prefactor_ = left_eye ? 1. : -1.;
			setAutoBufferSwap(false);
		}

		void StereoHalfImage::init(Stage* stage, Scene* scene)
		{
			stage_ = stage;
			scene_ = scene;

			makeCurrent();
			delete(gl_renderer_);
			gl_renderer_ = new GLRenderer;
			gl_renderer_->init(*scene_);
			gl_renderer_->init(*stage_, width(), height());
		}

		QSize StereoHalfImage::minimumSizeHint() const
		{
			return QSize(50, 50);
		}

		QSize StereoHalfImage::sizeHint() const
		{
			return QSize(400, 400);
		}

		void StereoHalfImage::initializeGL()
		{
		}

		void StereoHalfImage::paintGL()
		{
			if (gl_renderer_ && stage_ && scene_)
			{
				gl_renderer_->init(*scene_);

				gl_renderer_->setSize(width(), height());
				gl_renderer_->initSolid();
				if (stage_->getLightSources().size() == 0) scene_->setDefaultLighting(false);
				gl_renderer_->updateCamera();
				//			gl_renderer_->enableVertexBuffers(want_to_use_vertex_buffer_);
				scene_->stage_settings_->getGLSettings();


				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				Camera camera = stage_->getCamera();

				Vector3 old_view_point = camera.getViewPoint();
				Vector3 old_look_at = camera.getLookAtPosition();
				Vector3	diff = prefactor_ * camera.getRightVector() * (stage_->getEyeDistance() / 2.0);  

				float nearf = 1.5; 
				float farf = 600;

				float ndfl    = nearf / stage_->getFocalDistance();

				float left  = -2.0 *gl_renderer_->getXScale() - prefactor_* 0.5 * stage_->getEyeDistance() * ndfl;
				float right =  2.0 *gl_renderer_->getXScale() - prefactor_* 0.5 * stage_->getEyeDistance() * ndfl;

				//================== draw first buffer =============
				glMatrixMode(GL_PROJECTION);

				glLoadIdentity();

				glFrustum(left, right, -2.0 * gl_renderer_->getXScale(), 2.0 * gl_renderer_->getYScale(), nearf,farf);
				glViewport(0, 0, width(), height());

				if (stage_->swapSideBySideStereo())
					diff *= -1;

				// draw models
				glMatrixMode(GL_MODELVIEW);

				glPushMatrix();
				camera.setViewPoint(old_view_point + diff);
				camera.setLookAtPosition(old_look_at + diff);
				gl_renderer_->updateCamera(&camera);
				gl_renderer_->setLights(true);
				//scene_->renderRepresentations_((Scene::RenderMode)mode_);
				renderRepresentations_(Scene::DIRECT_RENDERING);
				glPopMatrix();
			}
			}

			void StereoHalfImage::renderRepresentations_(int mode)
			{
				RepresentationManager& pm = getMainControl()->getRepresentationManager();

				if (scene_->volume_width_ != FLT_MAX)
				{
					const Camera& camera = stage_->getCamera();
					Vector3 n(-camera.getViewVector());
					n.normalize();
					Vector3 nr(camera.getRightVector());
					nr.normalize();
					Vector3 nu(camera.getLookUpVector());
					nu.normalize();
					ClippingPlane plane;

					for (Position p = 1; p < 6; p++)
					{
						if (p == 1)
						{
							plane.setPoint(camera.getLookAtPosition() - n * scene_->volume_width_);
							plane.setNormal(n);
						}
						else if (p == 2)
						{
							plane.setPoint(camera.getLookAtPosition() - nr * scene_->volume_width_);
							plane.setNormal(nr);
						}
						else if (p == 3)
						{
							plane.setPoint(camera.getLookAtPosition() + nr * scene_->volume_width_);
							plane.setNormal(-nr);
						}
						else if (p == 4)
						{
							plane.setPoint(camera.getLookAtPosition() - nu * scene_->volume_width_);
							plane.setNormal(nu);
						}
						else if (p == 5)
						{
							plane.setPoint(camera.getLookAtPosition() + nu * scene_->volume_width_);
							plane.setNormal(-nu);
						}

						Vector3 v = plane.getNormal();
						GLdouble planef[] ={v.x, v.y, v.z, plane.getDistance()};
						glClipPlane(GL_CLIP_PLANE0 + p, planef);
						glEnable(GL_CLIP_PLANE0 + p);
					}
				}
			
				// ============== enable active Clipping planes ==============================
				GLint current_clipping_plane = GL_CLIP_PLANE0;

				vector<ClippingPlane*> active_planes;
				vector<ClippingPlane*> inactive_planes;

				bool move_mode = (scene_->mouse_button_is_pressed_ && scene_->getMode() == Scene::MOVE__MODE);
				
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
				if (scene_->show_light_sources_)
				{
					List<LightSource>::ConstIterator lit = stage_->getLightSources().begin();
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
						gl_renderer_->renderSphere_(s);

						Tube t;
						t.setVertex1(pos);
						t.setVertex2(pos + diff);
						t.setColor(ColorRGBA(255,255,255));
						gl_renderer_->renderTube_(t);
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
						for (Position p = 0; p < 20; p++)
						{
							if (scene_->volume_width_ != FLT_MAX) glDisable(GL_CLIP_PLANE0 + p);
						}

						// render inactive clipping planes
						for (plane_it = inactive_planes.begin(); plane_it != inactive_planes.end(); plane_it++)
						{
							gl_renderer_->renderClippingPlane_(**plane_it);
						}
					}

					RepresentationList::ConstIterator it = pm.getRepresentations().begin();
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
							render_(rep, mode);

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
						render_(rep, mode);
						
						// disable all clipping planes
						for (Position p = 0; p < rep_active_planes.size(); p++)
						{
							glEnable(rep_active_planes[p] + GL_CLIP_PLANE0);
						}

						// render the Representation once again, this time with colors
						gl_renderer_->setColorRGBA_(ColorRGBA(0,1.0,0));
						glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
						glDisable(GL_STENCIL_TEST);
						render_(rep, mode);

						// render the capping plane
						glEnable(GL_STENCIL_TEST);
						glStencilFunc(GL_NOTEQUAL, 0x0, 0xff);

						ColorRGBA color = ClippingPlane::getCappingColor();
						bool transparent = (int)color.getAlpha() != 255;
						if (transparent) gl_renderer_->initTransparent();
						else 						 gl_renderer_->initSolid();

						glDisable(cap_nr + GL_CLIP_PLANE0);
						Disc d(Circle3(p, n, 400));
						d.setColor(color);
						gl_renderer_->render_(&d);
						
						glDisable(GL_STENCIL_TEST);
					}
				}

				if (scene_->text_ != "")
				{
					ColorRGBA c = stage_->getBackgroundColor().getInverseColor();
					QFont font;
					font.setPixelSize(scene_->font_size_);
					font.setBold(true);
					glDisable(GL_LIGHTING);
					gl_renderer_->setColorRGBA_(c);
					QFontMetrics fm(font);
					QRect r = fm.boundingRect(scene_->text_.c_str());
					renderText(width() -  (20 + r.width()), 
										 height() - (r.height() - 5),
										 scene_->text_.c_str(), font);
					glEnable(GL_LIGHTING);
				}
			}

			void StereoHalfImage::render_(const Representation& repr, int mode)
			{
				RepresentationManager& pm = getMainControl()->getRepresentationManager();
				Representation* rep = (Representation*)& repr;

				// preview mode:
				// if we have a model with only sticks and cylinders, decrease the rendering detail
				// and draw it directly instead of using display lists.
				// BUT: if the representation is currently rebuild, we can only draw buffered!
				bool used_preview = false;
				DrawingPrecision pbak = repr.getDrawingPrecision();
				if (scene_->use_preview_ && scene_->preview_)
				{
					if (repr.getModelType() >= MODEL_STICK &&
							repr.getModelType() <= MODEL_VDW &&
							pm.startRendering(rep))
					{
						rep->setDrawingPrecision(DRAWING_PRECISION_LOW);
						mode = Scene::DIRECT_RENDERING;
						used_preview = true;
					}
				}

				if (mode == Scene::DISPLAY_LISTS_RENDERING &&
						!repr.hasProperty("RENDER_DIRECT") && 
						repr.getDrawingMode() != DRAWING_MODE_TOON)
				{
					gl_renderer_->drawBuffered(repr);
					return;
				}

				if (!used_preview) 
				{
					// if we use previewing mode, the RepresentationManager was already notified above
					if (!pm.startRendering(rep))
					{
						// if Representation is to be rebuilded, but it is currently recalculated,
						// we can only draw it from the DisplayList
						gl_renderer_->drawBuffered(repr);
						return;
					}
				}

				if (mode == Scene::REBUILD_DISPLAY_LISTS)
				{
					gl_renderer_->bufferRepresentation(repr);
				}
				else //	DIRECT_RENDERING:
			{
				gl_renderer_->render(repr);
			}

			if (used_preview)
			{
				if (repr.getDrawingPrecision() != pbak)
				{
					// if previewing mode was used: reset the drawing precision
					(*(Representation*)&repr).setDrawingPrecision(pbak);
				}
			}

			pm.finishedRendering(rep);
		}


	}
}
