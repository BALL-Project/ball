// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glRenderer.C,v 1.40 2004/07/26 13:45:10 amoll Exp $
//

#include <BALL/VIEW/RENDERING/glRenderer.h>
#include <BALL/VIEW/RENDERING/glDisplayList.h>
#include <BALL/VIEW/KERNEL/common.h>

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
#include <BALL/VIEW/PRIMITIVES/mesh.h>

#include <qfont.h>
#include <qpainter.h>
#include <qbitmap.h>
#include <qimage.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		GLRenderer::GLRenderer()
			throw()
			: Renderer(),
				drawing_mode_(DRAWING_MODE_SOLID),
				drawing_precision_(DRAWING_PRECISION_HIGH),
				GL_spheres_list_(0),
				GL_tubes_list_(0),
				GL_boxes_list_(0),
				name_to_object_(),
				object_to_name_(),
				all_names_(0),
				stereo_(NO_STEREO),
				render_mode_(RENDER_MODE_UNDEFINED)
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
			all_names_ = 0;
		}

		bool GLRenderer::init(const Stage& stage, float width, float height)
			throw()
		{
			Renderer::init(stage, width, height);

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
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			glEnable(GL_LINE_SMOOTH);

			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

			glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);

			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);

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

			glMaterialfv(GL_FRONT, GL_SPECULAR,  spec);
			glMaterialfv(GL_FRONT, GL_SHININESS, shin );
			glMaterialfv(GL_FRONT, GL_DIFFUSE,  diff);
			glMaterialfv(GL_FRONT, GL_AMBIENT,  ambient);

			// BACKSIDE MATERIAL PROPERTIES
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  spec);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shin );
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diff);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  ambient);
			GLfloat diff2[] = {0.3, 0.3, 0.3, 1.0};
			GLfloat shin2[] = {76.8};
			GLfloat spec2[] = {0.674597, 0.674597, 0.674597, 1.0};
			GLfloat ambient2[] = {0.20, 0.20, 0.20, 1.0};
		
			glMaterialfv(GL_BACK, GL_SPECULAR,  spec2);
			glMaterialfv(GL_BACK, GL_SHININESS, shin2);
			glMaterialfv(GL_BACK, GL_DIFFUSE,  diff2);
			glMaterialfv(GL_BACK, GL_AMBIENT,  ambient2);

			// if displaylists were already calculated, return
			if (GL_spheres_list_ != 0) return true;

			initSolid();

			GL_spheres_list_ = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();
			GL_tubes_list_   = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();
			GL_boxes_list_   = new GLDisplayList[BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE]();

			createSpheres_();
			createTubes_();
			createBoxes_();

			return true;
		}


		void GLRenderer::initTransparent()
			throw()
		{
			if (render_mode_ == RENDER_MODE_TRANSPARENT) return;
			render_mode_ = RENDER_MODE_TRANSPARENT;
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDepthMask(GL_FALSE);
		}

		void GLRenderer::initSolid()
			throw()
		{
			if (render_mode_ == RENDER_MODE_SOLID) return;
			render_mode_ = RENDER_MODE_SOLID;
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glDepthMask(GL_TRUE);
		}

		void GLRenderer::initAlwaysFront()
			throw()
		{
			if (render_mode_ == RENDER_MODE_ALWAYS_FRONT) return;
			render_mode_ = RENDER_MODE_ALWAYS_FRONT;
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
			glDepthMask(GL_TRUE);
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
				if (LIGHTS_MAX > GL_LIGHT0 + 10) LIGHTS_MAX = GL_LIGHT0 + 10;
				for (; light_nr < LIGHTS_MAX; light_nr++)
				{
					glDisable(light_nr);
				}
				
				light_nr = GL_LIGHT0;
			}

			List<LightSource>::ConstIterator it = stage_->getLightSources().begin();
			for (; it != stage_->getLightSources().end(); it++)
			{
				if (reset_all &&
						!it->isRelativeToCamera())
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
				// setup the direction of the light
				GLfloat dir[] = { it->getDirection().x,
													it->getDirection().y,
													it->getDirection().z};
				glLightfv(light_nr, GL_SPOT_DIRECTION, dir);
				// setup the angle of the light cone
				GLfloat angle = it->getAngle().toDegree();
				glLightfv(light_nr, GL_SPOT_CUTOFF, &angle);
				// ---------------------------------------------------------------
				if (it->getType() == LightSource::POSITIONAL)
				{
					// setup the position of the lightsource
					GLfloat pos[]  = {it->getPosition().x, 
														it->getPosition().y, 
														it->getPosition().z, 
														1.0};  // the 1 is important
					glLightfv(light_nr, GL_POSITION, pos);
				}
				// ---------------------------------------------------------------
				else if (it->getType() == LightSource::DIRECTIONAL)
				{
					GLfloat pos[]  = {it->getPosition().x, 
														it->getPosition().y, 
														it->getPosition().z, 
														0.0};
					glLightfv(light_nr, GL_POSITION, pos);
				}
				// ---------------------------------------------------------------
				else
				{
					Log.error() << "Unknown type of light in " << __FILE__ << "  " << __LINE__ << " : " 
											<< it->getType() << std::endl;
					return;
				}

				glEnable(light_nr);
				light_nr++;
			}
		}


		void GLRenderer::removeDisplayListFor(const Representation& rep)
			throw()
		{
			if (!display_lists_.has(&rep)) return;
			delete display_lists_[&rep];
			display_lists_.erase(&rep);
		}

		void GLRenderer::rebuildDisplayListFor(const Representation& rep)
			throw()
		{
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
			render(rep);
			display_list->endDefinition();
		}

		void GLRenderer::drawFromDisplayList(const Representation& rep)
			throw()
		{
			if (!display_lists_.has(&rep) ||
					rep.isHidden()) 
			{
				return;
			}

			display_lists_[&rep]->draw();
		}


		bool GLRenderer::render(const Representation& representation)
			throw()
		{
			if (representation.isHidden()) return true;

			if (!representation.isValid())
			{
				Log.error() << "Representation " << &representation 
										<< "not valid, so aborting." << std::endl;
				return false;
			}

			drawing_precision_  = representation.getDrawingPrecision();
			drawing_mode_ 		  = representation.getDrawingMode();

			if (representation.getDrawingMode() == DRAWING_MODE_DOTS)
			{
				glDisable(GL_LIGHTING);
			}
			else
			{
				glEnable(GL_LIGHTING);
			}

			List<GeometricObject*>::ConstIterator it;
			for (it =  representation.getGeometricObjects().begin();
					 it != representation.getGeometricObjects().end();
					 it++)
			{
				render_(*it);
			}

			glFlush();

			return true;
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
			s << "Drawing Mode: " 		 		<< drawing_mode_  << std::endl;

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


		// --------------------------render methods-----------------------------------------
		void GLRenderer::render_(const GeometricObject* object)
			throw()
		{
		 	glLoadName(getName(*object));
			Renderer::render_(object);
		}


		void GLRenderer::renderSphere_(const Sphere& sphere)
			throw() 
		{
	 		glPushMatrix();
			setColor4ub_(sphere);
			translateVector3_(sphere.getPosition());
			scale_(sphere.getRadius());

			GL_spheres_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 
											 drawing_precision_].draw();

 			glPopMatrix();
		}

		void GLRenderer::renderDisc_(const Disc& disc)
			throw()
		{
			glPushMatrix();
			setColor4ub_(disc);
			translateVector3_(disc.getCircle().p);
			Vector3 rotation_axis(-disc.getCircle().n.y, disc.getCircle().n.x, 0.0);
			// angle between z-axis-vector and result
			Real angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(disc.getCircle().n.z / disc.getCircle().n.getLength()));
			rotateVector3Angle_(rotation_axis, angle);

			if (drawing_precision_ == 0)
				GL_quadric_object_.drawDisk(0, disc.getCircle().radius, 6, 4);
			else if (drawing_precision_ == 1)
				GL_quadric_object_.drawDisk(0, disc.getCircle().radius, 14, 8);
			else
				GL_quadric_object_.drawDisk(0, disc.getCircle().radius, 24, 16);
			glPopMatrix();
		}


		void GLRenderer::renderLine_(const Line& line)
			throw()
		{
			glDisable(GL_LIGHTING);
			setColor4ub_(line);

			// BAUSTELLE
			// drawing mode dots must be implemented

			glBegin(GL_LINES);
				vertexVector3_(line.getVertex1());
				vertexVector3_(line.getVertex2());
			glEnd();

			glEnable(GL_LIGHTING);
		}


		void GLRenderer::renderLabel_(const Label& label)
			throw()
		{
			glPushMatrix();
			glDisable(GL_LIGHTING);
			setColor4ub_(label);

			// build bitmap
			int width, height;
			GLubyte* text_array = generateBitmapFromText_(label.getText(), width, height);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glRasterPos3f((GLfloat)label.getVertex().x, 
										(GLfloat)label.getVertex().y, 
										(GLfloat)label.getVertex().z);
			// draw bitmap
			glBitmap(width, height, width/2, height/2.0, 0, 0, text_array);

			glPopMatrix();
			glEnable(GL_LIGHTING);
		}


		void GLRenderer::renderPoint_(const Point& point)
			throw()
		{
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
			glPushMatrix();
			setColor4ub_(box);
			translateVector3_(box.a);
			scaleVector3_(box.b - box.a);
			GL_boxes_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION 
										 + drawing_precision_].draw();
			glPopMatrix();
		}


		void GLRenderer::renderBox_(const Box& box)
			throw()
		{
			glPushMatrix();
			setColor4ub_(box);
			
			translateVector3_(box.getPoint());
			scaleVector3_(box.getDiagonalVector());
			Real angle = Vector3(1,0,0).getAngle(box.getRightVector()).toRadian();
			rotateVector3Angle_(Vector3(1,0,0), angle);

			angle = Vector3(0,-1,0).getAngle(box.getHeightVector()).toRadian();
			rotateVector3Angle_(Vector3(0, -1, 0), angle);

			Vector3 depth_vector = box.getRightVector() % box.getHeightVector();
			angle = Vector3(0, 0, -1).getAngle(depth_vector).toRadian();
			rotateVector3Angle_(depth_vector, angle);

			GL_boxes_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION 
										 + drawing_precision_].draw();

			glPopMatrix();
		}


		void GLRenderer::renderTube_(const Tube& tube)
			throw()
		{
			glPushMatrix();
			setColor4ub_(tube);

			Vector3 result = tube.getVertex2() - tube.getVertex1();

			// cross product with z-axis-vector and result
			Vector3 rotation_axis(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			Real angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(result.z / result.getLength()));

			translateVector3_(tube.getVertex1());
			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength());

			GL_tubes_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION 
										 + drawing_precision_].draw();

			glPopMatrix();
		}


		void GLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
			throw()
		{
			Vector3 result = tube.getVertex2() - tube.getVertex1();

			// cross product with z-axis-vector and result
			Vector3 rotation_axis(-result.y, result.x, 0.0);
			// angle between z-axis-vector and result
			Real angle = BALL_ANGLE_RADIAN_TO_DEGREE(acos(result.z / result.getLength()));

			glPushMatrix();
			setColor4ub_(tube);

			translateVector3_(tube.getVertex1());
			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength() / (Real)2);

			GL_tubes_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION 
										 + drawing_precision_].draw();

			glPopMatrix();
			glPushMatrix();
			translateVector3_(tube.getMiddleVertex());

			rotateVector3Angle_(rotation_axis, angle);

			glScalef((GLfloat)tube.getRadius(),
							 (GLfloat)tube.getRadius(),
							 (GLfloat)tube.getLength() / (Real)2);

			// draw second half
			if (tube.getComposite() == 0 ||
					!tube.getComposite()->isSelected())
			{
				setColorRGBA_(tube.getColor2());
			}

			GL_tubes_list_[drawing_mode_ * BALL_VIEW_MAXIMAL_DRAWING_PRECISION 
										 + drawing_precision_].draw();

			glPopMatrix();
		}

		void GLRenderer::renderTwoColoredLine_(const TwoColoredLine& line)
			throw()
		{
			setColor4ub_(line);

			glDisable(GL_LIGHTING);
			glBegin(GL_LINE_STRIP);
			
			vertexVector3_(line.getVertex1());
			vertexVector3_(line.getMiddleVertex());

			if (line.getComposite() == 0 || 
					!line.getComposite()->isSelected())
			{
				setColorRGBA_(line.getColor2());
			}

			vertexVector3_(line.getMiddleVertex());
			vertexVector3_(line.getVertex2());

			glEnd();
			glEnable(GL_LIGHTING);
		}


		void GLRenderer::renderMesh_(const Mesh& mesh)
			throw()
		{
			if (drawing_mode_ == DRAWING_MODE_DOTS ||
					drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				glDisable(GL_LIGHTING);
			}
			// If we have only one color for the whole mesh, this can
			// be assigned efficiently
			if (mesh.colorList.size() < mesh.vertex.size())
			{	
				if (mesh.colorList.size() == 0)
				{
					glColor4ub(255,255,255,255);
				}
				else
				{
					setColorRGBA_(mesh.colorList[0]);
				}

				if (drawing_mode_ == DRAWING_MODE_DOTS)
				{
					glBegin(GL_POINTS);

					normalVector3_(normal_vector_);
					// draw the triangles with lines
					for (Size index = 0; index < mesh.vertex.size(); ++index)
					{
						vertexVector3_(mesh.vertex[index]);
					}

					glEnd();
				}
				else if (drawing_mode_ == DRAWING_MODE_WIREFRAME)
				{
					glBegin(GL_LINE_STRIP);
					// draw the triangles with lines
					for (Size index = 0; index < mesh.triangle.size(); ++index)
					{
						normalVector3_(normal_vector_);

						vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
					}
					glEnd();
				}
				else
				{
					// draw the triangles solid
					if (render_mode_ == RENDER_MODE_SOLID)
					{
		 				glDisable(GL_CULL_FACE);
					}
					
					glBegin(GL_TRIANGLES);
					for (Size index = 0; index < mesh.triangle.size(); ++index)
					{
						normalVector3_(mesh.normal[mesh.triangle[index].v1]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

						normalVector3_(mesh.normal[mesh.triangle[index].v2]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

						normalVector3_(mesh.normal[mesh.triangle[index].v3]);
						vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
					}
					glEnd();

					if (render_mode_ == RENDER_MODE_SOLID)
					{
						glEnable(GL_CULL_FACE);
					}
				}
				
				if (drawing_mode_ == DRAWING_MODE_DOTS ||
						drawing_mode_ == DRAWING_MODE_WIREFRAME)
				{
					glEnable(GL_LIGHTING);
				}
				
				return;
			}

			// we have a different color for each vertex
			if (drawing_mode_ == DRAWING_MODE_DOTS)
			{
				glBegin(GL_POINTS);
				normalVector3_(normal_vector_);

				for (Size index = 0; index < mesh.vertex.size(); ++index)
				{
					setColorRGBA_(mesh.colorList[index]);
					vertexVector3_(mesh.vertex[index]);
				}

				glEnd();
				// ------------------
			}
			else if (drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				glBegin(GL_LINE_STRIP);
				// draw the triangles with lines
				for (Size index = 0; index < mesh.triangle.size(); ++index)
				{
					normalVector3_(normal_vector_);

					setColorRGBA_(mesh.colorList[mesh.triangle[index].v1]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

					setColorRGBA_(mesh.colorList[mesh.triangle[index].v2]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

					setColorRGBA_(mesh.colorList[mesh.triangle[index].v3]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
				}
				glEnd();
				// ------------------
			}
			else
			{
				// draw the triangles solid
				if (render_mode_ == RENDER_MODE_SOLID)
				{
		 			glDisable(GL_CULL_FACE);
				}
					
 			  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, true);
				glBegin(GL_TRIANGLES);
				for (Size index = 0; index < mesh.triangle.size(); ++index)
				{
					setColorRGBA_(mesh.colorList[mesh.triangle[index].v1]);
					normalVector3_(mesh.normal[mesh.triangle[index].v1]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v1]);

					setColorRGBA_(mesh.colorList[mesh.triangle[index].v2]);
					normalVector3_(mesh.normal[mesh.triangle[index].v2]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v2]);

					setColorRGBA_(mesh.colorList[mesh.triangle[index].v3]);
					normalVector3_(mesh.normal[mesh.triangle[index].v3]);
					vertexVector3_(mesh.vertex[mesh.triangle[index].v3]);
				}
				glEnd();
 			  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, false);

				if (render_mode_ == RENDER_MODE_SOLID)
				{
		 			glEnable(GL_CULL_FACE);
				}
					
				// ------------------
			}

			if (drawing_mode_ == DRAWING_MODE_DOTS ||
					drawing_mode_ == DRAWING_MODE_WIREFRAME)
			{
				glEnable(GL_LIGHTING);
			}
		}

		GLubyte* GLRenderer::generateBitmapFromText_(const String& text, int& width, int& height) const
			throw()
		{
			QColor c1(0,0,0);
			QColor c2(255,255,255);
			int border = 2;
			
			QPainter p;
			QPixmap pm(1,1,1);
			p.begin(&pm);
				p.setFont(QFont("helvetica"));
				QRect r = p.fontMetrics().boundingRect(text.c_str());
			p.end();
			pm.resize(r.size() + QSize(border * 2, border * 2));
			
			p.begin(&pm);
				pm.fill(c1);
				p.setPen(c2);
				p.drawText(-r.x() + border, -r.y() + border, text.c_str());
			p.end();

			// convert to image (acces to single pixel is allowed here)
			QImage image = pm.convertToImage();
			int pixel_width = image.width();
			width = (pixel_width + 7) / 8;
			height = image.height();
			// convert to opengl usable bitmap
			int array_size = width * height;
			
			GLubyte* text_array = new GLubyte[array_size];
			
			// clear char array
			for (int i = 0; i < array_size; ++i)
			{
				*(text_array + i) = 0;
			}
			
			// copy image to char array
			int offset = (height - 1) * width;

			for (int i = 0; i < height; ++i, offset -= width)
			{
				for (int j = 0; j < pixel_width; ++j)
				{
					if (qGray(image.pixel(j,i)) != 255)
					{
						*(text_array + (j>>3) + offset) |= (128 >> (j&7)); 
					}
				}
			}

			width = pixel_width;
			return text_array;
		}


		void GLRenderer::createSpheres_()
			throw()
		{
			glPushMatrix();
			// building point display list
			GL_spheres_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].startDefinition();
			createDottedSphere_(1); // Precision 0 is far too evil here
			GL_spheres_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].endDefinition();

			GL_spheres_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].startDefinition();
			createDottedSphere_(2);
			GL_spheres_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].endDefinition();

			GL_spheres_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].startDefinition();
			createDottedSphere_(3);
			GL_spheres_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].endDefinition();

			// create quadric object
			GLQuadricObject GL_quadric_object;

			GL_quadric_object.setDrawStyle(GLU_LINE);
			GL_quadric_object.setNormals(GLU_SMOOTH);
			GL_quadric_object.setOrientation(GLU_OUTSIDE);

			// building wireframe display list
			GL_spheres_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].startDefinition();
			GL_quadric_object.drawSphere(1, 6, 4);
			GL_spheres_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].endDefinition();
			
			GL_spheres_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].startDefinition();
			GL_quadric_object.drawSphere(1, 14, 8);
			GL_spheres_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].endDefinition();
			
			GL_spheres_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].startDefinition();
			GL_quadric_object.drawSphere(1, 24, 16);
			GL_spheres_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].endDefinition();
			
			GL_quadric_object.setDrawStyle(GLU_FILL);

			// building solid display list
			GL_spheres_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].startDefinition();
			GL_quadric_object.drawSphere(1, 6, 4);
			GL_spheres_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].endDefinition();
			
			GL_spheres_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].startDefinition();
			GL_quadric_object.drawSphere(1, 14, 8);
			GL_spheres_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].endDefinition();
			
			GL_spheres_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].startDefinition();
			GL_quadric_object.drawSphere(1, 24, 16);
			GL_spheres_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].endDefinition();
			glPopMatrix();
		}


		// ======================== data for dotted spheres =======================
		#		define BALL_OPENGL_SPHERE_X .525731112119133606
		#		define BALL_OPENGL_SPHERE_Z .850650808352039932

		const float GLRenderer::sphere_vertices_[12][3] =
		{
			{-BALL_OPENGL_SPHERE_X, 0.0, BALL_OPENGL_SPHERE_Z},
			{ BALL_OPENGL_SPHERE_X, 0.0, BALL_OPENGL_SPHERE_Z},
			{-BALL_OPENGL_SPHERE_X, 0.0,-BALL_OPENGL_SPHERE_Z},
			{ BALL_OPENGL_SPHERE_X, 0.0,-BALL_OPENGL_SPHERE_Z},
			{ 0.0, BALL_OPENGL_SPHERE_Z, BALL_OPENGL_SPHERE_X},
			{ 0.0, BALL_OPENGL_SPHERE_Z,-BALL_OPENGL_SPHERE_X},
			{ 0.0,-BALL_OPENGL_SPHERE_Z, BALL_OPENGL_SPHERE_X},
			{ 0.0,-BALL_OPENGL_SPHERE_Z,-BALL_OPENGL_SPHERE_X},
			{ BALL_OPENGL_SPHERE_Z, BALL_OPENGL_SPHERE_X, 0.0},
			{-BALL_OPENGL_SPHERE_Z, BALL_OPENGL_SPHERE_X, 0.0},
			{ BALL_OPENGL_SPHERE_Z,-BALL_OPENGL_SPHERE_X, 0.0},
			{-BALL_OPENGL_SPHERE_Z,-BALL_OPENGL_SPHERE_X, 0.0}
		};

		const int GLRenderer::sphere_indices_[20][3] =
		{
			{ 0, 4, 1}, { 0, 9, 4}, { 9, 5, 4}, { 4, 5, 8}, { 4, 8, 1},
			{ 8,10, 1}, { 8, 3,10}, { 5, 3, 8}, { 5, 2, 3}, { 2, 7, 3},
			{ 7,10, 3}, { 7, 6,10}, { 7,11, 6}, {11, 0, 6}, { 0, 1, 6},
			{ 6, 1,10}, { 9, 0,11}, { 9,11, 2}, { 9, 2, 5}, { 7, 2,11}
		};


		void GLRenderer::createDottedSphere_(int precision)
			throw()
		{
			glBegin(GL_POINTS);

			for (int i = 0; i < 20; ++i)
			{
				Vector3 v1(sphere_vertices_[sphere_indices_[i][0]][0],
									 sphere_vertices_[sphere_indices_[i][0]][1],
									 sphere_vertices_[sphere_indices_[i][0]][2]);
				
				Vector3 v2(sphere_vertices_[sphere_indices_[i][1]][0],
									 sphere_vertices_[sphere_indices_[i][1]][1],
									 sphere_vertices_[sphere_indices_[i][1]][2]);
				
				Vector3 v3(sphere_vertices_[sphere_indices_[i][2]][0],
									 sphere_vertices_[sphere_indices_[i][2]][1],
									 sphere_vertices_[sphere_indices_[i][2]][2]);
				
				subdivideTriangle_(v1, v2, v3, precision);
			}

			glEnd();
		}

		void GLRenderer::subdivideTriangle_(Vector3& v1, Vector3& v2, Vector3& v3, int precision)
			throw()
		{
			if (precision == 0)
			{
				Vector3 result = v1 + v2 + v3;
				result.normalize();
				vertexVector3_(result);
				return;
			}

			Vector3 v12 = v1 + v2;
			Vector3 v23 = v2 + v3;
			Vector3 v31 = v3 + v1;
			
			v12.normalize();
			v23.normalize();
			v31.normalize();

			subdivideTriangle_(v1, v12, v31, precision - 1);
			subdivideTriangle_(v2, v23, v12, precision - 1);
			subdivideTriangle_(v3, v31, v23, precision - 1);
			subdivideTriangle_(v12, v23, v31, precision - 1);
		}


		void GLRenderer::createTubes_()
			throw()
		{
			// create quadric object
			GLQuadricObject GL_quadric_object;

			GL_quadric_object.setDrawStyle(GLU_POINT);
			GL_quadric_object.setNormals(GLU_SMOOTH);
			GL_quadric_object.setOrientation(GLU_OUTSIDE);

			// building point display list
			GL_tubes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 6, 1);
			GL_tubes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].endDefinition();

			GL_tubes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 10, 1);
			GL_tubes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].endDefinition();

			GL_tubes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 20, 1);
			GL_tubes_list_[0 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].endDefinition();

			// building wireframe display list
			GL_quadric_object.setDrawStyle(GLU_LINE);

			GL_tubes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 6, 1);
			GL_tubes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].endDefinition();
			
			GL_tubes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 10, 1);
			GL_tubes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].endDefinition();
			
			GL_tubes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 20, 1);
			GL_tubes_list_[1 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].endDefinition();
			
			// building solid display list
			GL_quadric_object.setDrawStyle(GLU_FILL);

			GL_tubes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 6, 1);
			GL_tubes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 0].endDefinition();
			
			GL_tubes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 10, 1);
			GL_tubes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 1].endDefinition();
			
			GL_tubes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].startDefinition();
			GL_quadric_object.drawCylinder(1, 1, 1, 20, 1);
			GL_tubes_list_[2 * BALL_VIEW_MAXIMAL_DRAWING_PRECISION + 2].endDefinition();
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

		// ############################ NAMING ###################################
		GLRenderer::Name GLRenderer::getName(const GeometricObject& object)
			throw()
		{
			NameHashMap::Iterator name_iterator = object_to_name_.find(&object);

			if (name_iterator != object_to_name_.end())
			{
				return name_iterator->second;
			}

			Name name = ++all_names_;

			object_to_name_.insert(NameHashMap::ValueType(&object, name));
			name_to_object_.insert(GeometricObjectHashMap::ValueType(name, &object));

			return name;
		}


		GeometricObject* GLRenderer::getObject(GLRenderer::Name name) const
			throw()
		{
			if (name == 0) return 0;
			if (!name_to_object_.has(name))
			{
				return 0;
			}

			GeometricObjectHashMap::ConstIterator it = name_to_object_.find(name);

			return (GeometricObject*) it->second;
		}


		void GLRenderer::clearNames_()
			throw()
		{
			name_to_object_.clear();
			object_to_name_.clear();

			all_names_ = 0;
		}


		// ############################ PICKING ###################################		
		void GLRenderer::pickObjects1(float x1, float y1, float x2, float y2)
			throw()
		{
			glFlush();
			GLint viewport[4];
			// init name stack for 32000 objects
			glGetIntegerv(GL_VIEWPORT, viewport);
			glSelectBuffer(BALL_GLRENDERER_PICKING_NUMBER_OF_MAX_OBJECTS, object_buffer_);
			glRenderMode(GL_SELECT);
			glInitNames();
			glPushName(0);
			glMatrixMode(GL_PROJECTION);
			
			glPushMatrix();
			glLoadIdentity();
			
			// calculate picking rectangle
			int width  = BALL_ABS((int)x2 - (int)x1);
			int height = BALL_ABS((int)y2 - (int)y1);
			
			int center_x = BALL_MIN((int)x2, (int)x1) + width / 2;
			int center_y = BALL_MIN((int)y2, (int)y1) + height / 2;
			
			if (width == 0)	width = 1;
			if (height == 0) height = 1;
			
			clearNames_();
			
			// calculate picking matrix
			gluPickMatrix((double)center_x, 
										(double)(viewport[3] - center_y),
										(double)width,
										(double)height,
										viewport);

			// prepare camera
			glFrustum(-2.0 * x_scale_, 2.0 * x_scale_, -2.0 * y_scale_, 2.0 * y_scale_, 1.5, 300);
			
			glMatrixMode(GL_MODELVIEW);
			updateCamera();
		}			


		void GLRenderer::pickObjects2(List<GeometricObject*>& objects, int width, int height)
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

			// collect only the nearest Object
			if (width <= 3 && height <= 3) 
			{
				Position z_coord;
				
				// find minimum z-coord
				for (int index = 0; index < number_of_hits; ++index)
				{
					names = *object_buffer_ptr;
					++object_buffer_ptr;
					z_coord = *object_buffer_ptr;
					
					++object_buffer_ptr;
					++object_buffer_ptr;
					
					if (z_coord <= minimum_z_coord)
					{
						minimum_z_coord = z_coord;
						nearest_name = (Name)*object_buffer_ptr;
					}
					
					object_buffer_ptr += names;
				}    
				
				GeometricObject* i = getObject(nearest_name);
				if (i != 0) objects.push_back(i);
			}
			else // collect all objects that are in the picking area
			{
				for (int index = 0; index < number_of_hits; ++index)
				{
					names = *object_buffer_ptr;
					
					++object_buffer_ptr;
					++object_buffer_ptr;
					++object_buffer_ptr;
					
					nearest_name = (Name)*object_buffer_ptr;
					
					object_buffer_ptr += names;

					GeometricObject* i = getObject(nearest_name);
					if (i != 0) objects.push_back(i);
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

			glFrustum (-2.0 * x_scale_, 
									2.0 * x_scale_, 
								 -2.0 * y_scale_, 
									2.0 * y_scale_, 
									1.5, 300);

			glMatrixMode(GL_MODELVIEW);
		}


		void GLRenderer::updateCamera(const Camera* camera)
			throw()
		{
			if (camera == 0) camera = &stage_->getCamera();

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


		void GLRenderer::setStereoMode(StereoMode state)
			throw()
		{
			if (state == stereo_) return;
			stereo_ = state;
		}

} } // namespaces
