// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: VRMLRenderer.C,v 1.1 2003/10/10 16:05:51 amoll Exp $
//

#include <BALL/VIEW/RENDERING/VRMLRenderer.h>
#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <BALL/VIEW/PRIMITIVES/label.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>

#include <BALL/MATHS/surface.h>

using std::endl;

namespace BALL
{
	namespace VIEW
	{

VRMLRenderer::VRMLRenderer()
	throw()
	: Renderer(),
		outfile_()
{
}

VRMLRenderer::VRMLRenderer(const String& name)
	throw(Exception::FileNotFound)
	: Renderer(),
		width(600),
		height(600)
{
	outfile_.open(name, std::ios::out);
}

VRMLRenderer::~VRMLRenderer()
	throw()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
			<< " of class " << RTTI::getName<VRMLRenderer>() << std::endl;
	#endif
}

void VRMLRenderer::clear()
	throw()
{
	outfile_.clear();
}

void VRMLRenderer::setFileName(const String& name)
	throw(Exception::FileNotFound)
{
	outfile_.open(name, std::ios::out);
}

String VRMLRenderer::VRMLColorRGBA(const ColorRGBA& input)
	throw()
{
	return String((float) input.getRed()) + " " + String((float) input.getGreen()) + String((float) input.getBlue());
}

String VRMLRenderer::VRMLVector3(Vector3 input)
	throw()
{
	// we convert into the new coordinate system: the camera looks
	// down the negative z axis and sits at (0,0,0)
	/*
	input += origin_;
	input  = rotation_ * input;
	*/
	String output = "";
	output+=String(input.x);
	output+=" ";
	output+=String(input.y);
	output+=" ";
	output+=String(input.z);

	return output;
}

// init must be called right before the rendering starts, since
// we need to fix the camera, light sources, etc...
bool VRMLRenderer::init(const Stage& stage)
	throw()
{
	#ifdef BALL_VIEW_DEBUG_PROCESSORS
		Log.info() << "Start the VRMLRender output..." << std::endl;
	#endif

	stage_ = &stage;

	outfile_	<< "# VRML file created by the BALL VRMLRenderer" 
						<< std::endl 
						<< std::endl;
/*
	// Find out the position of the camera.
	const Camera& camera = stage_->getCamera();

	view_point           = camera.getViewPoint();
	Vector3 up_vector    = camera.getLookUpVector();
	Vector3 right_vector = camera.getRightVector(); 
	Vector3 look_at      = camera.getLookAtPosition();// camera.getLookAtPosition();
*/
	outfile_ << "Transform {" << endl;
	outfile_ << "  children [" << endl;
	outfile_ << "    shape {" << endl;

	return true;
}

bool VRMLRenderer::finish()
	throw()
{
	outfile_ << "    }" << endl;
	outfile_ << "  ]" << endl;
	outfile_ << "}" << endl;
	outfile_.close();

	return true;
}

void VRMLRenderer::renderSphere_(const Sphere& sphere)
	throw()
{
	outfile_ << "Transform {" << std::endl;
	outfile_ << "translation_ " << VRMLVector3(sphere.getPosition()) << std::endl;
	outfile_ << "children [" << std::endl;

		VRMLobjectColor(sphere);

		outfile_ << " Sphere {" << endl;
		outfile_ << "  radius " << sphere.getRadius() << std::endl;
		outfile_ << " }" << endl << endl;
	outfile_ << "]" << std::endl;
}

void VRMLRenderer::VRMLobjectColor(const GeometricObject& object)
	throw()
{
	outfile_ << "appearence Appearance {" << std::endl;
	outfile_ << "material Material {" << std::endl;
	outfile_ << "diffuseColor " 
					 << String ((float)object.getColor().getRed()) << " "
					 << String ((float)object.getColor().getBlue()) << " "
					 << String ((float)object.getColor().getGreen()) << std::endl;

	outfile_ << "shininess 0.5" << std::endl;
	outfile_ << "  }" << std::endl;
	outfile_ << "}" << std::endl;
}

void VRMLRenderer::renderDisc_(const Disc& disc)
	throw()
{
return;
	ColorRGBA color;
	// first find out its color
	if ((disc.getComposite() && (disc.getComposite()->isSelected())))
	{
		color = BALL_SELECTED_COLOR;
	}
	else
	{
		color = disc.getColor();
	}

	// then, find out its radius
	float radius;

	// its normal
	Vector3 normal;
	// and finally, its position
	Vector3 position;

	disc.getCircle().get(position, normal, radius);
//Log.error() << normal << std::endl;
	normal -= origin_;

	// now write the information into the outfile_
	outfile_ << "\tdisc {" << endl << "\t\t";
	outfile_ << VRMLVector3(position) << ", ";
	outfile_ << VRMLVector3(normal) << ", ";
	outfile_ << radius << endl;
	//outfile_ <<"\t\ttexture {" << endl;
	outfile_ << "\tpigment { " << VRMLColorRGBA(color) << " } " << endl;
	outfile_ << "\t} " << endl << endl;
}

void VRMLRenderer::renderTube_(const Tube& tube)
	throw()
{
return;
	ColorRGBA color;
	// first, find out its color
	if ((tube.getComposite()) && (tube.getComposite()->isSelected()))
	{
		color = BALL_SELECTED_COLOR;
	}
	else
	{
		color = tube.getColor();
	}

	// then, find out its radius
	float radius = tube.getRadius();

	// and finally, the base and the cap
	Vector3 base_point = tube.getVertex1();
	Vector3  cap_point = tube.getVertex2();

	// now write the information into the outfile_
	outfile_ << "\tcylinder {" << endl;
	outfile_ << "\t\t" << VRMLVector3(base_point) << ", ";
	outfile_           << VRMLVector3( cap_point) << ", ";
	outfile_           <<                 radius << endl;
	//outfile_ <<"\t\ttexture {" << endl;
	//outfile_ << "\t\t\t\tpigment { " << VRMLColorRGBA(color);
	outfile_ << "\tpigment { " << VRMLColorRGBA(color) << " } " << endl;
	outfile_ << "\t} " << endl;
}	

void VRMLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
	throw()
{
return;
	// we have found a two colored tube
	ColorRGBA color1, color2;

	// first, find out its color
	if (tube.getComposite() && (tube.getComposite()->isSelected()))
	{
		color1 = BALL_SELECTED_COLOR;
		color2 = BALL_SELECTED_COLOR;
	}
	else
	{
		color1 = tube.getColor();
		color2 = tube.getColor2();
	}

	// then, find out its radius
	float radius = tube.getRadius();

	// and finally, the base and the cap
	Vector3 base_point = tube.getVertex1();
	Vector3  cap_point = tube.getVertex2();
	Vector3  mid_point = tube.getMiddleVertex();

	// now write the information into the outfile_
	outfile_ << "\tcylinder {" << endl;
	outfile_ << "\t\t" << VRMLVector3(base_point) << ", ";
	outfile_           << VRMLVector3( mid_point) << ", ";
	outfile_           <<                 radius << endl;
	outfile_ << "\tpigment { " << VRMLColorRGBA(color1) << " } " << endl;
	outfile_ << "\t}" << endl << endl;
	
	outfile_ << "\tcylinder {" << endl;
	outfile_ << "\t\t" << VRMLVector3(mid_point) << ", ";
	outfile_           << VRMLVector3(cap_point) << ", ";
	outfile_           <<                 radius << endl;
	outfile_ << "\tpigment { " << VRMLColorRGBA(color2) << " } " << endl;
	outfile_ << "\t}" << endl << endl;
}

void VRMLRenderer::renderMesh_(const Mesh& mesh)
	throw()
{
	// so we should let VRMLRay know...
	outfile_ << "geometry IndexedFaceSet {" << endl;
	outfile_ << "coord Coordinate {" << endl;
	outfile_ << "point [" << endl;

	// print vertices
	vector<Vector3>::const_iterator itv = mesh.vertex.begin(); 
	for (; itv != mesh.vertex.end(); itv++)
	{
		outfile_ << VRMLVector3(*itv);
		if (itv != mesh.vertex.end()) 
		{
			outfile_ << "," << std::endl;
		}
	}
	outfile_ << std::endl << "     ]" << std::endl;
	outfile_ << "    }" << std::endl;
	
	// print triangles =====================================
	outfile_ << "coordIndex [" << std::endl;
	
	vector<Surface::Triangle>::const_iterator itt = mesh.triangle.begin(); 
	for (; itt != mesh.triangle.end(); itt++)
	{
		outfile_ << String((*itt).v1) << " " 
						 << String((*itt).v2) << " " 
						 << String((*itt).v3) << " " 
						 << " -1"; 

		if (itt != mesh.triangle.end()) 
		{
			outfile_ << "," << std::endl;
		}
	}
	outfile_ << std::endl << "     ]" << std::endl;

// print colors ========================================
	outfile_ << "color Color {" << std::endl;
	outfile_ << "color [" << std::endl;
	vector<ColorRGBA>::const_iterator itc = mesh.colorList.begin(); 
	for (; itc != mesh.colorList.end(); itc++)
	{
		outfile_ << VRMLColorRGBA(*itc) << std::endl;

		if (itc != mesh.colorList.end()) 
		{
			outfile_ << "," << std::endl;
		}
	}
	outfile_ << std::endl << "     ]" << std::endl;
	outfile_ << "    }" << std::endl;
	outfile_ << "    colorPerVertex FALSE" << std::endl;
	outfile_ << "  }" << std::endl;
}

} } // namespaces
