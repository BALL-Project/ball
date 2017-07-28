class Surface
{
%TypeHeaderCode
	#include <BALL/MATHS/surface.h>
%End
	public:

	class Triangle
	{
		public:
		Index v1;
		Index v2;
		Index v3;
	};

	Surface();
	Surface(const Surface&);
	~Surface();

	void clear();
	void set(const Surface&);
	void get(Surface&) const;
	void readMSMSFile(const String&, const String&);
	float getArea() const;
	bool operator == (const Surface&) const;
	bool operator != (const Surface&) const;
	Size getNumberOfTriangles() const throw();
	Size getNumberOfVertices() const throw();
	Size getNumberOfNormals() const throw();
	Triangle& getTriangle(Position) throw();
	Vector3& getVertex(Position) throw();
	Vector3& getNormal(Position) throw();
	void clearVertices();
	void clearTriangles();
	void clearNormals();
	void pushBackTriangle(const Triangle&) throw();
	void pushBackVertex(const Vector3&) throw();
	void pushBackNormal(const Vector3&) throw();
	void resizeTriangles(Size);
	void resizeNormals(Size);
	void resizeVertices(Size);
};
