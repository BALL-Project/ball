class GridVisualisation
	: GeometricObject
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/gridVisualisation.h>
%End
	public:

	enum
	{
		PLANE,
		SLICES,
		DOTS
	};

	GridVisualisation() throw();
	GridVisualisation(const GridVisualisation&) throw();
	~GridVisualisation() throw();
	const Vector3& getNormal() const;
	void setNormal(const Vector3&);
	const Vector3& getPoint() const;
	void setPoint(const Vector3&);
	void setGrid(const RegularData3D*);
	const RegularData3D* getGrid() const;
	void setDotSize(Size);
	Size getDotSize() const;

	Vector3 origin;
	Vector3 x;
	Vector3 y;
	Vector3 z;
	float max_dim;
	Size slices;
	bool draw_box;

	// convert to a string representation
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("GridVisualisation ")
		+ " { "
		+ "   Grid: " + String((long int)sipCpp->getGrid())
		+ " }").c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("GridVisualisation ")
		+ " { "
		+ "   Grid: " + String((long int)sipCpp->getGrid())
		+ " }").c_str());
%End
};
