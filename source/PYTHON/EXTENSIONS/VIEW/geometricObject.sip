// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/geometricObject.h>
	#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>
	#include <BALL/VIEW/PRIMITIVES/box.h>
	#include <BALL/VIEW/PRIMITIVES/disc.h>
	#include <BALL/VIEW/PRIMITIVES/label.h>
	#include <BALL/VIEW/PRIMITIVES/line.h>
	#include <BALL/VIEW/PRIMITIVES/mesh.h>
	#include <BALL/VIEW/PRIMITIVES/point.h>
	#include <BALL/VIEW/PRIMITIVES/simpleBox.h>
	#include <BALL/VIEW/PRIMITIVES/sphere.h>
	#include <BALL/VIEW/PRIMITIVES/tube.h>
	#include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
	#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
	#include <BALL/VIEW/PRIMITIVES/multiLine.h>
	#include <BALL/VIEW/PRIMITIVES/quadMesh.h>

	using namespace BALL;
	using namespace BALL::VIEW;
%End

%ConvertToSubClassCode
/////////////////////////////////////////////////
/////////////////////////////////////////////////

	//BALL_TO_SIP_MAP(Point)
	//BALL_TO_SIP_MAP(MultiLine)
	//BALL_TO_SIP_MAP(QuadMesh)
	
	BALL_TO_SIP_MAP(Sphere)
	BALL_TO_SIP_MAP(TwoColoredTube)
	BALL_TO_SIP_MAP(Tube)
	BALL_TO_SIP_MAP(TwoColoredLine)
	BALL_TO_SIP_MAP(Line)
	BALL_TO_SIP_MAP(Mesh)
	BALL_TO_SIP_MAP(GridVisualisation)
	BALL_TO_SIP_MAP(Box)
	BALL_TO_SIP_MAP(Disc)
	BALL_TO_SIP_MAP(SimpleBox)
	{
	}
%End

  public:

  // BALL_CREATE(GeometricObject);
  GeometricObject() throw();
  GeometricObject(const GeometricObject&) throw();
  ~GeometricObject() throw();
  virtual void clear() throw();
  void set(const GeometricObject&) throw();
  // GeometricObject& operator = (const GeometricObject& object) throw();
  void swap(GeometricObject&) throw();
  void setComposite(const Composite*) throw();
  const Composite* getComposite() const throw();
  virtual bool isValid() const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void setColor(const ColorRGBA&) throw();
  void setColor(const ColorUnit&, const ColorUnit&, const ColorUnit&, const ColorUnit& alpha = 1.0) throw();

	ColorRGBA& getColor() throw();
  // const ColorRGBA& getColor() const throw();

  void getColor(ColorUnit&, ColorUnit&, ColorUnit&, ColorUnit&) const throw();
	//bool operator == (const GeometricObject& object) const throw();
	//bool operator < (const GeometricObject& object) const throw();

	// convert to a string representation
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("GeometricObject") 
						+ " { "
						+ " Composite: " + String((long int)sipCpp->getComposite())
						+ " }").c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("GeometricObject") 
						+ " { "
						+ " Composite: " + String((long int)sipCpp->getComposite())
						+ " }").c_str());
%End

};
