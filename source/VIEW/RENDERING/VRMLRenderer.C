// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: VRMLRenderer.C,v 1.3 2004/02/18 16:06:43 amoll Exp $
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
		outfile_(),
		current_intend_(0)
{
}

VRMLRenderer::VRMLRenderer(const String& name)
	throw(Exception::FileNotFound)
	: Renderer(),
		width(600),
		height(600),
		current_intend_(0)
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
	current_intend_ = 0;
}

void VRMLRenderer::setFileName(const String& name)
	throw(Exception::FileNotFound)
{
	outfile_.open(name, std::ios::out);
	current_intend_ = 0;
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

	out_("#VRML V2.0 utf8");
	out_("");
/*
	// Find out the position of the camera.
	const Camera& camera = stage_->getCamera();

	view_point           = camera.getViewPoint();
	Vector3 up_vector    = camera.getLookUpVector();
	Vector3 right_vector = camera.getRightVector(); 
	Vector3 look_at      = camera.getLookAtPosition();// camera.getLookAtPosition();
*/

	return true;
}

bool VRMLRenderer::finish()
	throw()
{
	outfile_.close();
	current_intend_ = 0;

	return true;
}

void VRMLRenderer::renderSphere_(const Sphere& sphere)
	throw()
{
	outheader_("Transform {");
	outheader_("translation " + VRMLVector3(sphere.getPosition()));
	outheader_("children [");

	outheader_("Shape {");
	VRMLobjectColor(sphere);

	outheader_("geometry Sphere {");
	out_("radius " + String(sphere.getRadius()));
	current_intend_ --;
	outfinish_("}");
	outfinish_("}");
	outfinish_("]");
	out_("}");
}

void VRMLRenderer::VRMLobjectColor(const GeometricObject& object)
	throw()
{
	outheader_("appearance Appearance {");
	outheader_("material Material {");
	out_("diffuseColor " 
					 + String ((float)object.getColor().getRed()) + " "
					 + String ((float)object.getColor().getBlue()) + " "
					 + String ((float)object.getColor().getGreen()));

	out_("shininess 0.5");
	current_intend_ --;
	outfinish_("}");
	outfinish_("}");
}

void VRMLRenderer::renderMesh_(const Mesh& mesh)
	throw()
{
	// so we should let VRMLRay know...
	outheader_("Shape {");
	outheader_("geometry IndexedFaceSet {");
	outheader_("coord Coordinate {");
	outheader_("point [");

	// print vertices
	vector<Vector3>::const_iterator itv = mesh.vertex.begin(); 
	for (; itv != mesh.vertex.end(); itv++)
	{
		String out = VRMLVector3(*itv);
		if (itv != mesh.vertex.end()) 
		{
			out += ",";
		}
		out_(out);
	}
	current_intend_ --;
	outfinish_("]");
	out_("}"); // correct
	
	// print triangles =====================================
	outheader_("coordIndex [");
	
	vector<Surface::Triangle>::const_iterator itt = mesh.triangle.begin(); 
	for (; itt != mesh.triangle.end(); itt++)
	{
		String out = (String((*itt).v1) + " " 
						 + String((*itt).v2) + " " 
						 + String((*itt).v3) + " " 
						 + " -1"); 

		if (itt != mesh.triangle.end()) 
		{
			out +=  ",";
		}
		out_(out);
	}
	current_intend_ --;
	outfinish_("]");

// print colors ========================================
	outheader_("color Color {");
	outheader_("color [");
	if (mesh.colorList.size() == 0)
	{
		out_(VRMLColorRGBA(ColorRGBA(1.,1,1)));
	}
	else
	{
		vector<ColorRGBA>::const_iterator itc = mesh.colorList.begin(); 
		for (; itc != mesh.colorList.end(); itc++)
		{
			String out = VRMLColorRGBA(*itc);
			if (itc != mesh.colorList.end()) 
			{
				out += ",";
			}
			out_(out);
		}
	}
	current_intend_ --;
	outfinish_("]");
	out_("}");
	outfinish_("colorPerVertex FALSE");
	outfinish_("}");
	outfinish_("}");
}

void VRMLRenderer::out_(const String& data)
	throw()
{
	if (current_intend_ < 0)
	{
		Log.error() << "Problem in " << __FILE__ << __LINE__ << std::endl;
	}
	String out;
	for (Index p=0; p< current_intend_; p++)
	{
		out += " ";
	}
	out += data;
	outfile_ << out << std::endl;
}

} } // namespaces
