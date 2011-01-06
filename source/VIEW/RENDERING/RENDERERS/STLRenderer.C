//STL Renderer: v 1.0 (2008.09.12) Annette Treichel

#include <BALL/VIEW/RENDERING/RENDERERS/STLRenderer.h>
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

//default constructor
STLRenderer::STLRenderer()
	: Renderer(),
		outfile_(),
		current_indent_(0)
{
}

//constructor containing the filename: this is the usally used constructor
//the filename has to be saved as the stl file has to end with the same 
//line as it starts just with endsolid instead "solid"
STLRenderer::STLRenderer(const String& name)
	throw(Exception::FileNotFound)
	: Renderer(),
		width(600),
		height(600),
		current_indent_(0)
{
	outfile_.open(name, std::ios::out);

	out_("solid " + name + " created by BALL");
	endingName_ = "endsolid " + name + " created by BALL";
}

//destructor
STLRenderer::~STLRenderer()
{
	#ifdef BALL_VIEW_DEBUG
		Log.info() << "Destructing object " << (void *)this 
			<< " of class " << RTTI::getName<STLRenderer>() << std::endl;
	#endif
}

void STLRenderer::clear()
{
	outfile_.clear();
	current_indent_ = 0;
}

void STLRenderer::setFileName(const String& name)
	throw(Exception::FileNotFound)
{
	outfile_.open(name, std::ios::out);
	current_indent_ = 0;

	outheader_("solid " + name + " created by BALL");
}

//as stl contains neither colors nor structured surfaces there is no color definition part

//The VRML Vewctor interpretation is reused
String STLRenderer::VRMLVector3(Vector3 input)
{
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
bool STLRenderer::init(const Stage& stage)
{
	#ifdef BALL_VIEW_DEBUG_PROCESSORS
		Log.info() << "Start the STLRender output..." << std::endl;
	#endif

	stage_ = &stage;
	return true;
}

//finishing means to make the last line the first with "endsolid" instead of "solid"
bool STLRenderer::finish()
{
	out_(endingName_);
	outfile_.close();
	current_indent_ = 0;

	return true;
}



void STLRenderer::footer_()
{
//isn't used
}


void STLRenderer::renderSphere_(const Sphere& sphere)
{
//Spere represented by cube with 6 sides or 12 triangles 
//core is sphere._getPosition()

	Vector3 core;
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 temp;
	Vector3 n;
	Vector3 a;
	Vector3 b;

	//the concept is to create a cube whose sides are parallel to the axes
	//the edges of the cube are used as points of the triangles

	float radius = sphere.getRadius();
	core = sphere.getPosition();
	//bottom plane
	temp.set((-1)*radius,(-1)*radius,(-1)*radius);
	v1 = core + temp ;
	temp.set(radius,(-1)*radius,(-1)*radius);
	v2 = core + temp;
	temp.set((-1)*radius,radius,(-1)*radius);
	v3 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = b % a;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//a square consists of 2 triangles. now we switch to sides and take the fourth point instead the first
	
	temp = v2;
	v2 = v3;
	v3 = temp;
	temp.set(radius,radius,(-1)*radius);
	v1 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = b % a;
	n.normalize();

	//print the next triangle

	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//top plane:
	//now we do exactly the same but for another square, meaning to triangles
	temp.set((-1)*radius,(-1)*radius,radius);
	v1 = core + temp;
	temp.set(radius,(-1)*radius,radius);
	v2 = core + temp;
	temp.set((-1)*radius,radius,radius);
	v3 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	temp.set(radius,radius,radius);
	v1 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//back plane:
	temp.set((-1)*radius,(-1)*radius,(-1)*radius);
	v1 = core + temp;
	temp.set((-1)*radius,(-1)*radius,radius);
	v2 = core + temp;
	temp.set((-1)*radius,radius,(-1)*radius);
	v3 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	temp.set((-1)*radius,radius,radius);
	v1 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//front plane:
	temp.set(radius,(-1)*radius,(-1)*radius);
	v1 = core + temp;
	temp.set(radius,(-1)*radius,radius);
	v2 = core + temp;
	temp.set(radius,radius,(-1)*radius);
	v3 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = b % a;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	temp.set(radius,radius,radius);
	v1 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = b % a;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//left plane:
	temp.set((-1)*radius,(-1)*radius,(-1)*radius);
	v1 = core + temp;
	temp.set(radius,(-1)*radius,(-1)*radius);
	v2 = core + temp;
	temp.set((-1)*radius,(-1)*radius,radius);
	v3 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	temp.set(radius,(-1)*radius,radius);
	v1 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//right plane:
	temp.set((-1)*radius,radius,(-1)*radius);
	v1 = core + temp;
	temp.set(radius,radius,(-1)*radius);
	v2 = core + temp;
	temp.set((-1)*radius,radius,radius);
	v3 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = b % a;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	temp.set(radius,radius,radius);
	v1 = core + temp;
	a = v2 - v1;
	b = v3 - v1;
	n = b % a;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");
}


void STLRenderer::renderTube_(const Tube& tube)
{
//	Tube is represented by a cuboid with 4 sides or 8 triangles 
//	the middle of the cuboid is the core

//what we do:
//at first we search for the first radial vector by taking the normal ( by crossproduct)
//between the y axe and the length vector
//if our lengthvector of the tube is parallel to the y axe we just take another axe

	Vector3 l = (tube.getVertex2() - tube.getVertex1()) / 2.0;
	Vector3 core = tube.getVertex1() + l;
	//search for the first radians equivalent vector
	Vector3 d = l % Vector3(0,1,0);
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(1,0,0);
	}
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(0,0,1);
	}
	d.normalize();
	d = d * sqrt(tube.getRadius()) / 2.0;

	//2. radius vector:
//herefore we now take the crossproduct between the length vector and the first 
//radial vector.
//now we get a vector normal to both
//there to vectors are added to the extrempoints of the ends of the tube
//voila we get the endigs of four rectangles or eight triangles
	Vector3 f = d % l;
	f.normalize();
	f = f * sqrt(tube.getRadius()) / 2.0;

	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 n;
	Vector3 a;
	Vector3 b;
	Vector3 temp;

	//back plane:
	v1 = core + l + d;
	v2 = core - l + d;
	v3 = core + l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core -l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//front plane:
	v1 = core + l + f;
	v2 = core - l + f;
	v3 = core + l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//left plane:
	v1 = core + l - d;
	v2 = core - l - d;
	v3 = core + l - f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - f - l;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//right plane:
	v1 = core + l - f;
	v2 = core - l - f;
	v3 = core + l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

}


void STLRenderer::renderTwoColoredTube_(const TwoColoredTube& tube)
{
//	Tube is represented by a cuboid with 4 sides or 8 triangles 
//	the middle of the cuboid is the core

	Vector3 l = (tube.getVertex2() - tube.getVertex1()) / 2.0;
	Vector3 core = tube.getVertex1() + l;
	//search for the first radians equivalent vector
	Vector3 d = l % Vector3(0,1,0);
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(1,0,0);
	}
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(0,0,1);
	}
	d.normalize();
	d = d * sqrt(tube.getRadius()) / 2.0;

	//2. radius vector:
	Vector3 f = d % l;
	f.normalize();
	f = f * sqrt(tube.getRadius()) / 2.0;

	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 n;
	Vector3 a;
	Vector3 b;
	Vector3 temp;

	//back plane:
	v1 = core + l + d;
	v2 = core - l + d;
	v3 = core + l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core -l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//front plane:
	v1 = core + l + f;
	v2 = core - l + f;
	v3 = core + l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//left plane:
	v1 = core + l - d;
	v2 = core - l - d;
	v3 = core + l - f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - f - l;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//right plane:
	v1 = core + l - f;
	v2 = core - l - f;
	v3 = core + l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");
}

void STLRenderer::renderLine_(const Line& tube)
{
//	Line is represented as a tube but with very small radius 
//	the middle of the cuboid is the core

	Vector3 l = (tube.getVertex2() - tube.getVertex1()) / 2.0;
	Vector3 core = tube.getVertex1() + l;
	//search for the first radians equivalent vector
	Vector3 d = l % Vector3(0,1,0);
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(1,0,0);
	}
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(0,0,1);
	}
	d.normalize();
	d = d / sqrt((l / 0.5).getSquareLength());

	//2. radius vector:
	Vector3 f = d % l;
	f.normalize();
	f = f / sqrt((l / 0.5).getSquareLength());

	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 n;
	Vector3 a;
	Vector3 b;
	Vector3 temp;

	//back plane:
	v1 = core + l + d;
	v2 = core - l + d;
	v3 = core + l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core -l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//front plane:
	v1 = core + l + f;
	v2 = core - l + f;
	v3 = core + l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//left plane:
	v1 = core + l - d;
	v2 = core - l - d;
	v3 = core + l - f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - f - l;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//right plane:
	v1 = core + l - f;
	v2 = core - l - f;
	v3 = core + l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

}

void STLRenderer::renderTwoColoredLine_(const TwoColoredLine& tube)
{
//	Line is represented as a tube but with very small radius 
//	the middle of the cuboid is the core

	Vector3 l = (tube.getVertex2() - tube.getVertex1()) / 2.0;
	Vector3 core = tube.getVertex1() + l;
	//search for the first radians equivalent vector
	Vector3 d = l % Vector3(0,1,0);
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(1,0,0);
	}
	if(d.getSquareLength()==0)
	{
		d = l % Vector3(0,0,1);
	}
	d.normalize();
	d = d / sqrt((l / 0.5).getSquareLength());

	//2. radius vector:
	Vector3 f = d % l;
	f.normalize();
	f = f / sqrt((l / 0.5).getSquareLength());

	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 n;
	Vector3 a;
	Vector3 b;
	Vector3 temp;

	//back plane:
	v1 = core + l + d;
	v2 = core - l + d;
	v3 = core + l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core -l + f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//front plane:
	v1 = core + l + f;
	v2 = core - l + f;
	v3 = core + l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l - d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//left plane:
	v1 = core + l - d;
	v2 = core - l - d;
	v3 = core + l - f;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - f - l;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	//right plane:
	v1 = core + l - f;
	v2 = core - l - f;
	v3 = core + l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

	temp = v2;
	v2 = v3;
	v3 = temp;
	v1 = core - l + d;
	a = v2 - v1;
	b = v3 - v1;
	n = a % b;
	n.normalize();


	out_(" facet normal " + (String)n.x + " " + (String)n.y  + " " + (String)n.z);
	out_("  outer loop");
	out_("    vertex " + String(v1.x) + " " + String(v1.y) + " " + String(v1.z));
	out_("    vertex " + String(v2.x) + " " + String(v2.y) + " " + String(v2.z));
	out_("    vertex " + String(v3.x) + " " + String(v3.y) + " " + String(v3.z));
 	out_("  endloop");
 	out_(" endfacet");

}





void STLRenderer::renderMesh_(const Mesh& mesh)
{

//	current_indent_ --; 
	
	// print triangles =====================================
	Position count=0;
	vector<Surface::Triangle>::const_iterator itt = mesh.triangle.begin(); 
	for (; itt != mesh.triangle.end(); itt++)
	{
		//header für jedes Dreieck
		outheader_("facet normal " + (VRMLVector3(mesh.normal[count])));
		outheader_("outer loop");

		String out1 = ("vertex " + VRMLVector3(mesh.vertex[(*itt).v1]));
		String out2 = ("vertex " + VRMLVector3(mesh.vertex[(*itt).v2]));
		String out3 = ("vertex " + VRMLVector3(mesh.vertex[(*itt).v3]));
		out_(out1);
		out_(out2);
		out_(out3);
	
		outfinish_("endloop");
		outfinish_("endfacet");
		count++;
	}
}

void STLRenderer::out_(const String& data)
{
	if (current_indent_ < 0)
	{
//   		BALLVIEW_DEBUG
	}
	String out;
	for (Index p=0; p< current_indent_; p++)
	{
		out += " ";
	}
	out += data;
	outfile_ << out << std::endl;
}

} } // namespaces

