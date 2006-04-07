// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glRenderer.C,v 1.71.2.17 2006/04/07 09:26:01 amoll Exp $
//

#include <BALL/VIEW/RENDERING/glRenderer.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/clippingPlane.h>

#include <BALL/VIEW/WIDGETS/scene.h>
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
#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>

#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/atom.h>

#include <QtGui/qpixmap.h>
#include <QtGui/qpainter.h>
#include <QtGui/qimage.h>

#ifdef BALL_ENABLE_VERTEX_BUFFER
 #include <BALL/VIEW/RENDERING/vertexBuffer.h>
#endif


using namespace std;

//   #define BALL_VIEW_DEBUG
//   #define BALL_BENCHMARKING
//   #define BALL_ENABLE_VERTEX_BUFFER

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
			throw()
			: Renderer(),
				scene_(0),
				drawing_mode_(DRAWING_MODE_SOLID),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				GL_spheres_list_(0),
				GL_tubes_list_(0),
				GL_boxes_list_(0),
				name_to_object_(),
				object_to_name_(),
				all_names_(1),
				stereo_(NO_STEREO),
				render_mode_(RENDER_MODE_UNDEFINED),
				use_vertex_buffer_(false),
				picking_mode_(false),
				model_type_(MODEL_LINES),
				drawed_other_object_(false),
				drawed_mesh_(false),
				GLU_quadric_obj_(0)
		{
		}

		GLRenderer::~GLRenderer()
			throw()
		{
			clear();
		}

		void GLRenderer::clear()
			throw()
		{
			name_to_object_.clear();
			object_to_name_.clear();
			all_names_ = 1;

			if (GL_spheres_list_ != 0) delete[] GL_spheres_list_;
			if (GL_boxes_list_   != 0) delete[] GL_boxes_list_;
			if (GL_tubes_list_   != 0) delete[] GL_tubes_list_;
			
			DisplayListHashMap::Iterator it = display_lists_.begin();
			for (; it != display_lists_.end(); it++)
			{
				delete it->second;
			}
			display_lists_.clear();
		}

		bool GLRenderer::init(Scene& scene)
			throw()
		{
			scene_ = &scene;

			Stage* stage = scene.getStage();
			if (stage == 0)
			{
				init(Stage(), scene.width(), scene.height());
			}
			else
			{
				init(*stage, scene.width(), scene.height());
			}

			return true;
		}


		bool GLRenderer::init(const Stage& stage, float height, float width)
			throw()
		{
			Renderer::init(stage, height, width);

			glFrontFace(GL_CCW);     // selects counterclockwise polygons as front-facing
			glCullFace(GL_BACK);		 // specify whether front- or back-facing facets can be culled

			// Force OpenGL to normalize transformed normals to be of unit 
			// length before using the normals in OpenGL's lighting equations
			// While this corrects potential lighting problems introduced by scaling, 
			// it also slows OpenGL's vertex processing speed since normalization requires extra operations.
			glEnable(GL_NORMALIZE);   

			glDisable(GL_FOG);
			
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, false);

			// Specifies the depth comparison function:
			// Passes if the incoming z value is greater than  or equal to the stored z value
			glDepthFunc(GL_LEQUAL);

			// specify the clear value for the depth buffer 
			glClearDepth(200.0);

			// select smooth shading 
			glShadeModel(GL_SMOOTH);

			// smooth line drawing
			glEnable(GL_LINE_SMOOTH);
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			
			// is problematic on some machines and should not be used:
			// glEnable(GL_POLYGON_SMOOTH);
			// glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

 			glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);

			// glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
			// slower, but better results:
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

			// setup all in the stage given lightsources
			setLights();
			glEnable(GL_LIGHTING);

			// set the background color according to the stage
			updateBackgroundColor();

			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			
			GLfloat diff[] = {0.4, 0.4, 0.4, 1.0};
			GLfloat shin[] = {76.8};
			GLfloat spec[] = {0.774597, 0.774597, 0.774597, 1.0};
			GLfloat ambient[] = {0.25, 0.25, 0.25, 1.0};

			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  spec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin );
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diff);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);

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

			return true;
		}


		void GLRenderer::updateBackgroundColor() 
			throw()
		{
			glClearColor((float) stage_->getBackgroundColor().getRed(),
									 (float) stage_->getBackgroundColor().getGreen(),
									 (float) stage_->getBackgroundColor().getBlue(),
									 (float) stage_->getBackgroundColor().getAlpha());
		}
			

		void GLRenderer::setLights(bool reset_all)
			throw()
		{
			GLenum light_nr = GL_LIGHT0;

			if (reset_all)
			{
				GLenum LIGHTS_MAX = GL_MAX_LIGHTS;
				if (LIGHTS_MAX > 10) LIGHTS_MAX = 10;
				for (; light_nr < GL_LIGHT0 + LIGHTS_MAX; light_nr++)
				{
					glDisable(light_nr);
				}
				
				light_nr = GL_LIGHT0;
			}

			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
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
				if (it->getAngle() <= 90)
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

				glEnable(light_nr);
				light_nr++;
			}
		}


		void GLRenderer::removeRepresentation(const Representation& rep)
			throw()
		{
			if (rep.getGeometricObjects().size() == 0) return;

		#ifdef BALL_ENABLE_VERTEX_BUFFER
			if (vertexBuffersEnabled())
			{
				clearVertexBuffersFor(*(Representation*)&rep);
			}
		#endif
			

			DisplayListHashMap::Iterator hit = display_lists_.find(&rep);
			if (hit == display_lists_.end()) return;

			delete hit->second;
			display_lists_.erase(hit);
		}

		void GLRenderer::bufferRepresentation(const Representation& rep)
			throw()
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

		#ifdef BALL_ENABLE_VERTEX_BUFFER
			clearVertexBuffersFor(*(Representation*)&rep);
			
			if (use_vertex_buffer_ && drawing_mode_ != DRAWING_MODE_WIREFRAME)
			{
				// prevent copying the pointers of the buffers later...
				rep_to_buffers_[&rep] = vector<MeshBuffer*>();
				
				vector<MeshBuffer*>& buffers = rep_to_buffers_.find(&rep)->second;

				const List<GeometricObject*>& geometric_objects = rep.getGeometricObjects();
				List<GeometricObject*>::ConstIterator git = geometric_objects.begin();
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


		bool GLRenderer::render(const Representation& representation, bool for_display_list)
			throw()
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
				// prevent artifacts:
				// first run to fill depth buffer
				initTransparent();
				glDepthMask(GL_TRUE);
 				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
				renderRepresentation_(representation, for_display_list);

				// options for second run 
 				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
				initTransparent();
			}

			renderRepresentation_(representation, for_display_list);

			return true;
		}

		void GLRenderer::renderRepresentation_(const Representation& representation, bool for_display_list)
		{
			model_type_ = representation.getModelType();

			// accelerate things a little by calling getGeometricObjects() only once
			const List<GeometricObject*>& geometric_objects = representation.getGeometricObjects();
			List<GeometricObject*>::ConstIterator it = geometric_objects.begin();
			if (for_display_list)
			{
				#ifdef BALL_ENABLE_VERTEX_BUFFER
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
				#endif
					// render everything
					for (; it != geometric_objects.end(); it++)
					{
						render_(*it);
					}
			#ifdef BALL_ENABLE_VERTEX_BUFFER
				}
			#endif
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
			throw()
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

		void GLRenderer::renderSphere_(const Sphere& sphere)
			throw() 
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
			throw()
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(disc);
			translateVector3_(disc.getCircle().p);
			const Vector3 rotation_axis(-disc.getCircle().n.y, disc.getCircle().n.x, 0.0);
			// angle between z-axis-vector and result
			const float angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(disc.getCircle().n.z / disc.getCircle().n.getLength()));
			rotateVector3Angle_(rotation_axis, angle);

			static Position slices[4] = {6, 14, 24, 40};
			static Position rings[4]  = {4, 8, 16, 32};

			initGLU_(drawing_mode_);

			gluDisk(GLU_quadric_obj_, 0, disc.getCircle().radius, slices[drawing_precision_], rings[drawing_precision_]);

			glPopMatrix();
		}


		void GLRenderer::renderLine_(const Line& line)
			throw()
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

		void GLRenderer::renderIlluminatedLine_(const IlluminatedLine& line)
			throw()
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
			const std::vector<Vector3>& tangents = line.tangents;
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
			throw()
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
			QPixmap pm(r.size() + QSize(border * 2, border * 2));

			QPainter p;
			p.begin(&pm);
				p.setFont(label.getFont());
 				pm.fill(scene_->getStage()->getBackgroundColor().getQColor());
				p.setPen(label.getColor().getQColor());
				p.drawText(-r.x() + border, -r.y() + border, text);
			p.end();

			QImage data = pm.toImage();
			QImage gldata = QGLWidget::convertToGLFormat(data);

			glDrawPixels(data.width(), data.height(), GL_RGBA, GL_UNSIGNED_BYTE, gldata.bits());

      glPopMatrix();
      glEnable(GL_LIGHTING);
		}


		void GLRenderer::renderPoint_(const Point& point)
			throw()
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
			throw()
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
			throw()
		{
			initDrawingOthers_();

			glPushMatrix();
			setColor4ub_(box);
			
			translateVector3_(box.getPoint());

			Vector3 v1(box.getRightVector());
			if (v1.getSquareLength() != 0) v1.normalize();
			
			Vector3 v2(box.getHeightVector());
			if (v2.getSquareLength() != 0) v2.normalize();
			
			Vector3 v3(box.getRightVector() % box.getHeightVector());
			if (v3.getSquareLength() != 0) v3.normalize();

			float m[16] = { v1.x, v1.y, v1.z, 0,
											v2.x, v2.y, v2.z, 0,
											v3.x, v3.y, v3.z, 0,
											0,0,0,1};
			glMultMatrixf(m);
			
			glScalef(box.getRightVector().getLength(),
							 box.getHeightVector().getLength(),
							 box.getDepth());

			GL_boxes_list_[display_lists_index_].draw();

			glPopMatrix();
		}

		void GLRenderer::renderTube_(const Tube& tube)
			throw()
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
			const float angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(result.z / length));

			translateVector3_(tube.getVertex1());

			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength());

			GL_tubes_list_[display_lists_index_].draw();

			glPopMatrix();
		}


		void GLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
			throw()
		{
			initDrawingOthers_();

			const Vector3 result(tube.getVertex2() - tube.getVertex1());
			const float length = result.getLength();

			if (Maths::isZero(length)) return;

			// cross product with z-axis-vector and result
			const Vector3 rotation_axis(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			const float angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(result.z / length));

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
			throw()
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
			throw()
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

			///////////////////////////////////////////////////////////////////
			if (drawing_mode_ == DRAWING_MODE_DOTS)
			{
				glBegin(GL_POINTS);

				if (!multiple_colors)
				{
					for (Size index = 0; index < mesh.vertex.size(); ++index)
					{
						vertexVector3_(mesh.vertex[index]);
					}
				}
				else
				{
					for (Size index = 0; index < mesh.vertex.size(); ++index)
					{
						setColorRGBA_(mesh.colors[index]);
						vertexVector3_(mesh.vertex[index]);
					}
				}

				glEnd();
			}
			///////////////////////////////////////////////////////////////////
			else if (drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				Size nr_triangles = mesh.triangle.size();
				
				if (!multiple_colors)
				{
					for (Size index = 0; index < nr_triangles; ++index)
					{
						glBegin(GL_LINE_STRIP);
						normalVector3_(normal_vector_);

						vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
						glEnd();
					}
				}
				else
				{
					for (Size index = 0; index < nr_triangles; ++index)
					{
						glBegin(GL_LINE_STRIP);
						
						normalVector3_(normal_vector_);

						setColorRGBA_(mesh.colors[mesh.triangle[index].v1]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

						setColorRGBA_(mesh.colors[mesh.triangle[index].v2]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

						setColorRGBA_(mesh.colors[mesh.triangle[index].v3]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
						
						glEnd();
					}
				}
			}
			///////////////////////////////////////////////////////////////////
			else 				// draw the triangles solid
			{
				glBegin(GL_TRIANGLES);

				Size nr_triangles = mesh.triangle.size();

				if (!multiple_colors)
				{
					for (Size index = 0; index < nr_triangles; ++index)
					{
						normalVector3_(mesh.normal[mesh.triangle[index].v1]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

						normalVector3_(mesh.normal[mesh.triangle[index].v2]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

						normalVector3_(mesh.normal[mesh.triangle[index].v3]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
					}
				}
				else
				{
					for (Size index = 0; index < nr_triangles; ++index)
					{
						Position p = mesh.triangle[index].v1;
						setColorRGBA_(mesh.colors[p]);
						normalVector3_(  mesh.normal[p]);
						vertexVector3_(  mesh.vertex[p]);

						p = mesh.triangle[index].v2;
						setColorRGBA_(mesh.colors[p]);
						normalVector3_(  mesh.normal[p]);
						vertexVector3_(  mesh.vertex[p]);

						p = mesh.triangle[index].v3;
						setColorRGBA_(mesh.colors[p]);
						normalVector3_(  mesh.normal[p]);
						vertexVector3_(  mesh.vertex[p]);
					}
				}

				glEnd();
			}
		}


		void GLRenderer::initGLU_(DrawingMode mode)
		{
			if (mode == DRAWING_MODE_WIREFRAME)
			{
				gluQuadricDrawStyle(GLU_quadric_obj_, GLU_LINE);
			}
			else if (mode == DRAWING_MODE_SOLID)
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
			throw()
		{
			glPushMatrix();

			Position slices[4] = {6, 14, 24, 64};
			Position stacks[4] = {4, 8, 16, 64};

			for (Position mode = DRAWING_MODE_DOTS; mode <= DRAWING_MODE_SOLID; mode++)
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
			throw()
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
			throw()
		{
			glPushMatrix();

			Position slices[4] = {6, 10, 20, 64};

			for (Position mode = DRAWING_MODE_DOTS; mode <= DRAWING_MODE_SOLID; mode++)
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
			throw()
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
			throw()
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
			throw()
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
			throw()
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
		void GLRenderer::pickObjects1(Position x1, Position y1, Position x2, Position y2)
			throw()
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
			Size width  = BALL_ABS((Index)x2 - (Index)x1);
			Size height = BALL_ABS((Index)y2 - (Index)y1);
			
			Position center_x = BALL_MIN(x2, x1) + width / 2;
			Position center_y = BALL_MIN(y2, y1) + height / 2;
			
			if (width == 0)	width = 1;
			if (height == 0) height = 1;
			
			single_pick_ = (width <= 3 && height <= 3);
			clearNames_();
			
			// calculate picking matrix
			gluPickMatrix(center_x, viewport[3] - center_y, width, height, viewport);

			// prepare camera
			initPerspective();
			
			glMatrixMode(GL_MODELVIEW);
			updateCamera();
		}			


		void GLRenderer::pickObjects2(List<GeometricObject*>& objects)
			throw()
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


		// ############################ MOVEMENT/SIZE ###################################		
		void GLRenderer::setSize(float width, float height)
			throw()
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

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			initPerspective();
			glMatrixMode(GL_MODELVIEW);
		}


		void GLRenderer::updateCamera(const Camera* camera)
			throw()
		{
			if (camera == 0) camera = &stage_->getCamera();

			if (Maths::isZero(camera->getViewVector().getSquareLength()))
			{
				Log.error() << "Unvalid camera settings: View point = LookAt point" << std::endl;
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
		}

		bool GLRenderer::hasDisplayListFor(const Representation& rep) const
			throw()
		{
			return display_lists_.has(&rep);
		}

		bool GLRenderer::isExtensionSupported(const String& extension) const
			throw()
		{
			// Extension names should not have spaces
			if (extension == "" || extension.hasSubstring(" ")) return false;

			// Get Extensions String
			if (glGetString(GL_EXTENSIONS) == 0) 
			{
				return false;
			}

			String supported_extensions = (const char*) glGetString(GL_EXTENSIONS);
			if (supported_extensions.hasSubstring(extension)) return true;

			return false;
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
			if (glGetString(GL_VERSION) == 0) return "";
			return (char*)glGetString(GL_VERSION);
		}

		vector<String> GLRenderer::getExtensions()
		{
			vector<String> string_vector;
			if (glGetString(GL_EXTENSIONS) == 0) return string_vector;
			String exts = (char*)glGetString(GL_EXTENSIONS);
			exts.split(string_vector);
			return string_vector;
		}
		
		bool GLRenderer::enableVertexBuffers(bool state)
			throw()
		{
		
		#ifdef BALL_ENABLE_VERTEX_BUFFER
			if (!isExtensionSupported("GL_ARB_vertex_buffer_object")) 
			{
				use_vertex_buffer_ = false;
				return false;
			}

			if (state != use_vertex_buffer_)
			{
				if (state) Log.info() << "Enabling Vertex Buffer" << std::endl;
				else       Log.info() << "Disabling Vertex Buffer" << std::endl;
			}
			use_vertex_buffer_ = state;
		#else
			use_vertex_buffer_ = false;
		#endif

		#ifdef BALL_ENABLE_VERTEX_BUFFER
			if (use_vertex_buffer_) MeshBuffer::initGL();
		#endif

			return true;
		}

		void GLRenderer::clearVertexBuffersFor(Representation& rep)
			throw()
		{
		#ifdef BALL_ENABLE_VERTEX_BUFFER
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
		#endif
		}

		void GLRenderer::drawBuffered(const Representation& rep)
			throw()
		{
			if (rep.isHidden()) return;

		#ifdef BALL_ENABLE_VERTEX_BUFFER
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
		#endif
		
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
			throw()
		{
		#ifdef BALL_ENABLE_VERTEX_BUFFER
			return isExtensionSupported("GL_ARB_vertex_buffer_object");
		#else
			return false;
		#endif
		}

		void GLRenderer::renderClippingPlane_(const ClippingPlane& plane)
			throw()
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
			const float angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(n.z / n.getLength()));
			rotateVector3Angle_(rotation_axis, angle);

			initGLU_(DRAWING_MODE_SOLID);
			gluDisk(GLU_quadric_obj_, 0, 20 , 140, 80);

			glPopMatrix();
			glEnable(GL_CULL_FACE);

			glPopAttrib();
		}

		void GLRenderer::initPerspective()
		{
			if (getStereoMode() != GLRenderer::NO_STEREO)
			{
				Log.error() << "Dont call GLRenderer::initPerspective() in Stereo mode! " << std::endl;
				return;
			}

			glFrustum(-2.0 * x_scale_, 2.0 * x_scale_, -2.0 * y_scale_, 2.0 * y_scale_, 1.5, 600);
		}

		void GLRenderer::generateIlluminationTexture_(float ka, float kd, float kr, float shininess)
		{
			const int TEXTURE_SIZE = 128;  // including border
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
					float x = ( (float) j + 1. ) / (TEXTURE_SIZE + 1.);
					float y = ( (float) i + 1. ) / (TEXTURE_SIZE + 1.);

					float LT = 2. * x - 1.;
					float VT = 2. * y - 1.;

					float intensity = 0.;
					intensity += ka;
					intensity += kd * sqrt( 1. - LT*LT );
					intensity += kr * pow( fabs( LT*VT - sqrt( 1. - LT*LT ) * sqrt( 1. - VT*VT ) ), shininess * 255. );

					if( intensity < 0. || 1. < intensity )
					{
						if( intensity < 0. ) intensity = 0.;
						if( 1. < intensity ) intensity = 1.;
					}
//   					assert( 0. <= intensity && intensity <= 1. );

					Index c = (Index)(intensity * 255);
//   					assert( 0 <= c && c <= 255 );

					line_tex_[i][j][R] = (GLubyte) c;
					line_tex_[i][j][G] = (GLubyte) c;
					line_tex_[i][j][B] = (GLubyte) c;
					line_tex_[i][j][A] = 127;
				}
			}
		}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/RENDERING/glRenderer.iC>
#	endif

	} // namespace VIEW
} // namespace BALL
