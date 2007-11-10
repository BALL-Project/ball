// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class GridVisualisation 
  : GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	enum 
	{
		PLANE,
		SLICES,
		DOTS
	};

//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const GridVisualisation& operator = (const GridVisualisation& line) throw();
  GridVisualisation() throw();
  GridVisualisation(const GridVisualisation&) throw();
  ~GridVisualisation() throw();
//const GridVisualisation& operator = (const GridVisualisation& plane) throw();
	const Vector3& getNormal() const;
	void setNormal(const Vector3&);
	const Vector3& getPoint() const;
	void setPoint(const Vector3&);
	void setGrid(const RegularData3D*);
	const RegularData3D* getGrid() const;
	Position getTexture() const;
	void setTexture(Position );
	void setDotSize(Size);
	Size getDotSize() const;
	
	Vector3 origin; 
	Vector3 x;
	Vector3 y;
	Vector3 z;
	float max_dim;
	Size slices;
//	vector<Vector3> points;
//	Type type;
	bool draw_box;

	// convert to a string representation
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("GridVisualisation ") 
						+ " { "
						+ "   Grid: " + String((long int)sipCpp->getGrid())
						+ "   Texture: " + String(sipCpp->getTexture())
						+ " }").c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("GridVisualisation ") 
						+ " { "
						+ "   Grid: " + String((long int)sipCpp->getGrid())
						+ "   Texture: " + String(sipCpp->getTexture())
						+ " }").c_str());
%End

};
