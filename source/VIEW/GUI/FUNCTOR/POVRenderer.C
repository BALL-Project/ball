// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: POVRenderer.C,v 1.4 2002/12/20 14:58:27 anhi Exp $

#include <BALL/VIEW/GUI/FUNCTOR/POVRenderer.h>

#include <BALL/SYSTEM/file.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/VIEW/PRIMITIV/point.h>
#include <BALL/VIEW/PRIMITIV/sphere.h>
#include <BALL/VIEW/PRIMITIV/mesh.h>
#include <BALL/VIEW/PRIMITIV/tube.h>
#include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>


using std::endl;

namespace BALL
{
	namespace VIEW
	{

		POVRenderer::POVRenderer()
			throw()
			: ExternalRenderer(),
				outfile_()
		{
		}

		POVRenderer::POVRenderer(const String& name)
			throw(Exception::FileNotFound)
			: ExternalRenderer()
		{
			outfile_.open(name, std::ios::out);
		}

		POVRenderer::~POVRenderer()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<POVRenderer>() << std::endl;
			#endif

			destroy();
		}

		void POVRenderer::clear()
			throw()
		{
			outfile_.clear();
		}

		void POVRenderer::destroy()
			throw()
		{
		}

		void POVRenderer::setFileName(const String& name)
			throw(Exception::FileNotFound)
		{
			outfile_.open(name, std::ios::out);
		}

		String POVRenderer::POVColorRGBA(const ColorRGBA& input)
			throw()
		{
			float colorVal;
			
			String output = "";
			output+="color red ";
			input.getRed().get(colorVal);
			output+=String(colorVal/255.);
			output+=" green ";
			input.getGreen().get(colorVal);
			output+=String(colorVal/255.);
			output+=" blue ";
			input.getBlue().get(colorVal);
			output+=String(colorVal/255.);
			output+=" transmit ";
			input.getAlpha().get(colorVal);
			output+=String(1.-colorVal/255.);

			return output;
		}

		String POVRenderer::POVVector3(const Vector3& input)
			throw()
		{
			String output = "";
			output+="<";
			output+=String(input.x);
			output+=", ";
			output+=String(input.y);
			output+=", ";
			output+=String(input.z);
			output+=">";

			return output;
		}

		bool POVRenderer::start()
			throw()
		{
			#ifdef BALL_VIEW_DEBUG_PROCESSORS
			Log.info() << "Start the POVRender output..." << std::endl;
			#endif

			outfile_	<< "// POVRay file created by the BALL POVRenderer" 
								<< std::endl 
								<< std::endl;

			// Find out the camera position. This is stored in the Scene.
			Vector3 view_point = scene_->getViewPointPosition();
			Vector3 look_at    = scene_->getLookAtPosition();
			Vector3 scaling;
			scene_->getScalingFactor(scaling);
			outfile_ << "camera {" << endl;
			outfile_ << "\tlocation " << POVVector3(view_point) << endl;
			outfile_ << "\tup       " << POVVector3(scene_->getUpVector()) << endl;
			outfile_ << "\tright    " << POVVector3(scene_->getRightVector()) << endl;
			outfile_ << "\tlook_at  " << POVVector3(look_at) << endl;
			outfile_ << "}" << endl << endl;

			// Set the light sources
			// TODO: get them from the Scene!
			outfile_ << "light_source { <-50, 70, 100> color red 1.0 green 1.0 blue 1.0 }" << endl << endl;
			
			// Set black background
			outfile_ << "background { color red 0.0 green 0.0 blue 0.0 }" << endl << endl;

			// Define the finish we will use for our molecular objects (defining the molecular
			// "material properties"
			outfile_ << "#declare BALLFinish = finish { specular 0.4 diffuse 1.0 ambient 0.0 }" << endl << endl; 
			
			// now begin the CSG union containing all the geometric objects
			outfile_ << "union {" << endl;

			return true;
		}

		bool POVRenderer::finish()
			throw()
		{
			outfile_ << "}" << endl;
			outfile_.close();

			return true;
		}

		Processor::Result POVRenderer::operator() (Composite& composite)
			throw()
		{
			if (RTTI::isKindOf<Atom>(composite))
			{
				Atom *a = (Atom*)RTTI::castTo<Atom>(composite);
				// We have found an atom. This may contain bonds, and
				// these might know if we should render them...
				Atom::BondIterator b;
			//Log.info() << "Atom found!" << endl;	
				for (b=a->beginBond(); +b; ++b)
				{
					//Log.info() << "Bond found!" <<  " " << b << endl;
					// now we should try to get the Tube, which is appended as
					// a child to this Composite Bond
			
					Composite *c;
					Position j=0;
					
					do {
						c=b->getChild(j);
						j++;
						
						if (c!=0)  
						{
							GeometricObject *geometric_object = (GeometricObject*)RTTI::castTo<GeometricObject>(*c);
							
							if ((geometric_object!=0) && !(geometric_object->hasProperty(GeometricObject::PROPERTY__OBJECT_DYNAMIC)))
							{
								if (RTTI::isKindOf<MOLVIEW::TwoColoredTube>(*c))
								{
									// we have found a two colored tube
									MOLVIEW::TwoColoredTube *t = (MOLVIEW::TwoColoredTube*)RTTI::castTo<MOLVIEW::TwoColoredTube>(*geometric_object);

									ColorRGBA color1, color2;
									
									// first, find out its color
									if (!(t->isSelected()))
									{
										color1 = t->getColor1();
										color2 = t->getColor2();
									}
									else
									{
										color1 = BALL_SELECTED_COLOR;
										color2 = BALL_SELECTED_COLOR;
									}

									// then, find out its radius
									float radius = t->getRadius();

									// and finally, the base and the cap
									Vector3 base_point = t->getVertex1();
									Vector3  cap_point = t->getVertex2();
									Vector3  mid_point = t->getMiddleVertex();

									// now write the information into the outfile_
									outfile_ << "\tcylinder {" << endl;
									outfile_ << "\t\t" << POVVector3(base_point) << ", ";
									outfile_           << POVVector3( mid_point) << ", ";
									outfile_           <<                 radius << endl;
									outfile_ <<"\t\ttexture {" << endl;
									outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color1);
									outfile_ << " } " << endl;
									outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
									outfile_ << "\t\t}" << endl;
									outfile_ << "\t}" << endl << endl;
						
									outfile_ << "\tcylinder {" << endl;
									outfile_ << "\t\t" << POVVector3(mid_point) << ", ";
									outfile_           << POVVector3(cap_point) << ", ";
									outfile_           <<                 radius << endl;
									outfile_ <<"\t\ttexture {" << endl;
									outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color2);
									outfile_ << " } " << endl;
									outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
									outfile_ << "\t\t}" << endl;
									outfile_ << "\t}" << endl << endl;
								}
								else if (RTTI::isKindOf<Tube>(*c))
								{
									// we have found a tube
									Tube *t = (Tube*)RTTI::castTo<Tube>(*geometric_object);

									ColorRGBA color;
									// first, find out its color
									if (!(t->isSelected()))
									{
										color = t->getColor();
									}
									else
									{
										color = BALL_SELECTED_COLOR;
									}

									// then, find out its radius
									float radius = t->getRadius();

									// and finally, the base and the cap
									Vector3 base_point = t->getVertex1();
									Vector3  cap_point = t->getVertex2();

									// now write the information into the outfile_
									outfile_ << "\tcylinder {" << endl;
									outfile_ << "\t\t" << POVVector3(base_point) << ", ";
									outfile_           << POVVector3( cap_point) << ", ";
									outfile_           <<                 radius << endl;
									outfile_ <<"\t\ttexture {" << endl;
									outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color);
									outfile_ << " } " << endl;
									outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
									outfile_ << "\t\t}" << endl;
									outfile_ << "\t}" << endl << endl;
								} 
							}
						}

					} while (c!=b->getLastChild());

				} 
			}
	
			if (RTTI::isKindOf<GLObject>(composite) == false)
			{
				return Processor::CONTINUE;
			}

			GeometricObject *geometric_object = (GeometricObject*)RTTI::castTo<GeometricObject>(composite);

			if (geometric_object == 0)
			{
				return Processor::CONTINUE;
			}

			GLObject *GL_object = (GLObject*)RTTI::castTo<GLObject>(composite);

			if (GL_object == 0)
			{
				return Processor::CONTINUE;
			}

			// now find out the type of the geometric object and export it accordingly
			if (RTTI::isKindOf<Point>(*geometric_object))
			{
		//		Log.error() << "Point encountered! These are ignored during export!" << endl;
				return Processor::CONTINUE;
			}
			if (RTTI::isKindOf<Sphere>(*geometric_object))
			{
		//		Log.info() << "Extracting a sphere..." << endl;
				// we have found a sphere
				Sphere *s = (Sphere*)RTTI::castTo<Sphere>(*geometric_object);
				
				ColorRGBA color;
				// first find out its color
				if (!(s->isSelected()))
				{
					color = s->getColor();
				}
				else
				{
					color = BALL_SELECTED_COLOR;
				}

				// then, find out its radius
				float radius = s->getRadius();

				// and finally, its position
				Vector3 position = s->getVertex();

				// now write the information into the outfile_
				outfile_ << "\tsphere {" << endl << "\t\t";
				outfile_ << POVVector3(position) << ", ";
				outfile_ << radius << endl;
				outfile_ <<"\t\ttexture {" << endl;
				outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color);
				outfile_ << " } " << endl;
				outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
				outfile_ << "\t\t}" << endl;
				outfile_ << "\t}" << endl << endl;
			}
			if (RTTI::isKindOf<MOLVIEW::TwoColoredTube>(*geometric_object))
			{
				// we have found a two colored tube
				MOLVIEW::TwoColoredTube *t = (MOLVIEW::TwoColoredTube*)RTTI::castTo<MOLVIEW::TwoColoredTube>(*geometric_object);

				ColorRGBA color1, color2;

				// first, find out its color
				if (!(t->isSelected()))
				{
					color1 = t->getColor1();
					color2 = t->getColor2();
				}
				else
				{
					color1 = BALL_SELECTED_COLOR;
					color2 = BALL_SELECTED_COLOR;
				}

				// then, find out its radius
				float radius = t->getRadius();

				// and finally, the base and the cap
				Vector3 base_point = t->getVertex1();
				Vector3  cap_point = t->getVertex2();
				Vector3  mid_point = t->getMiddleVertex();

				// now write the information into the outfile_
				outfile_ << "\tcylinder {" << endl;
				outfile_ << "\t\t" << POVVector3(base_point) << ", ";
				outfile_           << POVVector3( mid_point) << ", ";
				outfile_           <<                 radius << endl;
				outfile_ <<"\t\ttexture {" << endl;
				outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color1);
				outfile_ << " } " << endl;
				outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
				outfile_ << "\t\t}" << endl;
				outfile_ << "\t}" << endl << endl;

				outfile_ << "\tcylinder {" << endl;
				outfile_ << "\t\t" << POVVector3(mid_point) << ", ";
				outfile_           << POVVector3(cap_point) << ", ";
				outfile_           <<                 radius << endl;
				outfile_ <<"\t\ttexture {" << endl;
				outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color2);
				outfile_ << " } " << endl;
				outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
				outfile_ << "\t\t}" << endl;
				outfile_ << "\t}" << endl << endl;
			}
			if (RTTI::isKindOf<Tube>(*geometric_object))
			{
			//	Log.info() << "Extracting a tube..." << endl;
				// we have found a tube
				Tube *t = (Tube*)RTTI::castTo<Tube>(*geometric_object);

				ColorRGBA color;
				// first, find out its color
				if (!(t->isSelected()))
				{
					color = t->getColor();
				}
				else
				{
					color = BALL_SELECTED_COLOR;
				}

				// then, find out its radius
				float radius = t->getRadius();

				// and finally, the base and the cap
				Vector3 base_point = t->getVertex1();
				Vector3  cap_point = t->getVertex2();

				// now write the information into the outfile_
				outfile_ << "\tcylinder {" << endl;
				outfile_ << "\t\t" << POVVector3(base_point) << ", ";
				outfile_           << POVVector3( cap_point) << ", ";
				outfile_           <<                 radius << endl;
				outfile_ <<"\t\ttexture {" << endl;
				outfile_ << "\t\t\t\tpigment { " << POVColorRGBA(color);
				outfile_ << " } " << endl;
				outfile_ << "\t\t\t\tfinish  { BALLFinish } " << endl;
				outfile_ << "\t\t}" << endl;
				outfile_ << "\t}" << endl << endl;
			}	
			if (RTTI::isKindOf<Mesh>(*geometric_object))
			{
				//Log.info() << "Extracting a mesh..." << endl;
				// we have found a mesh
				Mesh *m = (Mesh*)RTTI::castTo<Mesh>(*geometric_object);

				// so we should let POVRay know...
				outfile_ << "\tmesh {" << endl;
				
				ColorRGBA c1, c2, c3;
				Surface::Triangle t;
				Vector3 v1, v2, v3;
				Vector3 n1, n2, n3;
				
				// first, find out if we have a different color for each
				// point
				if (m->colorList.size()<m->vertex.size())
				{
					// nope. we don't...
					if (m->colorList.size()==0)
					{
						c1 = ColorRGBA(255,255,255,255);
					}
					else
					{
						c1 = m->colorList[0];
					}
					
					for (Position i=0; i<m->triangle.size(); i++)
					{
						t = m->triangle[i];

						v1 = m->vertex[t.v1];
						n1 = m->normal[t.v1];
						v2 = m->vertex[t.v2];
						n2 = m->normal[t.v2];
						v3 = m->vertex[t.v3];
						n3 = m->normal[t.v3];
				
						outfile_ << "\t\tsmooth_triangle {" << endl;
						outfile_ << "\t\t\t#local BALLColor = texture { pigment { ";
						outfile_ << POVColorRGBA(c1) << " } finish { BALLFinish } }" << endl << endl;
						outfile_ << "\t\t\t" << POVVector3(v1) << ", ";
						outfile_             << POVVector3(n1) << ", " << endl;
						outfile_ << "\t\t\t" << POVVector3(v2) << ", ";
						outfile_             << POVVector3(n2) << ", " << endl;
						outfile_ << "\t\t\t" << POVVector3(v3) << ", ";
						outfile_             << POVVector3(n3) << endl << endl;

						// And now the color. This is easy here, because we
						// only have one color.
						outfile_ <<"\t\t\ttexture {" << endl;
						outfile_ << "\t\t\t\t\tBALLColor" << endl;
						outfile_ << "\t\t\t}" << endl;
						outfile_ << "\t\t}" << endl << endl;
					}
				}
				else
				{
					// we have a color for each of the vertices
					for (Position i=0; i<m->triangle.size(); i++)
					{
						t = m->triangle[i];

						v1 = m->vertex[t.v1];
						n1 = m->normal[t.v1];
						c1 = m->colorList[t.v1];
						v2 = m->vertex[t.v2];
						n2 = m->normal[t.v2];
						c2 = m->colorList[t.v2];
						v3 = m->vertex[t.v3];
						n3 = m->normal[t.v3];
						c3 = m->colorList[t.v3];
				
						outfile_ << "\t\tsmooth_triangle {" << endl;
						outfile_ << "\t\t\t#local BALLColor1 = texture { pigment { ";
						outfile_ << POVColorRGBA(c1) << " } finish { BALLFinish } }" << endl;
						outfile_ << "\t\t\t#local BALLColor2 = texture { pigment { ";
						outfile_ << POVColorRGBA(c2) << " } finish { BALLFinish } }" << endl;
						outfile_ << "\t\t\t#local BALLColor3 = texture { pigment { ";
						outfile_ << POVColorRGBA(c3) << " } finish { BALLFinish } }" << endl << endl;
						outfile_ << "\t\t\t" << POVVector3(v1) << ", ";
						outfile_             << POVVector3(n1) << ", " << endl;
						outfile_ << "\t\t\t" << POVVector3(v2) << ", ";
						outfile_             << POVVector3(n2) << ", " << endl;
						outfile_ << "\t\t\t" << POVVector3(v3) << ", ";
						outfile_             << POVVector3(n3) << endl << endl;

						// And now the colors.
						outfile_ <<"\t\t\ttexture_list {" << endl;
						outfile_ << "\t\t\t\t\tBALLColor1 BALLColor2 BALLColor3" << endl;
						outfile_ << "\t\t\t}" << endl;
						outfile_ << "\t\t}" << endl << endl;
					}
				}

				// now close the mesh
				outfile_ << "\t}" << endl << endl;
			}
			else
			{
//				Log.error() << "Error! Export of " << streamClassName(typeid(composite)) << " " <<streamClassName(typeid(geometric_object)) << " is not yet implemented!!!" << endl;
			}

			return Processor::CONTINUE;
		}
	}
}
