// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Surface
{
%TypeHeaderCode
	#include <BALL/MATHS/surface.h>
	using namespace BALL;
%End
  public:
  class Triangle
 	{
		public:
	  Index v1;
	  Index v2;
	  Index v3;
  };

	public:
  Surface();
  Surface(const Surface&);
  ~Surface();
  void clear();
  void set(const Surface&);
	//  const Surface& operator = (const Surface& box);
  void get(Surface&) const;
  void readMSMSFile(const String&, const String&);
  float getArea() const;
  bool operator == (const Surface&) const;
  bool operator != (const Surface&) const;
  // vector<Vector3>  vertex;
  // vector<Vector3>  normal;
  // vector<Triangle> triangle;

	Size getNumberOfTriangles() const throw();
	Size getNumberOfVertices() const throw();
	Size getNumberOfNormals() const throw();

	Triangle& getTriangle(Position) throw();
	// const Triangle& getTriangle(Position) const throw();

	Vector3& getVertex(Position) throw();
	// const Vector3& getVertex(Position) const throw();

	Vector3& getNormal(Position) throw();
	// const Vector3& getNormal(Position) const throw();
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
