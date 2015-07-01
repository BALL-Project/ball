// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Vector3
 : PersistentObject
{
%TypeHeaderCode
	#include <BALL/MATHS/vector3.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  public:
  // BALL_CREATE_NODEEP(Vector3)
  Vector3();
  // Vector3(const T* ptr);
  Vector3(float);
  Vector3(float, float, float);
  Vector3(const Vector3&);
  Vector3(float, const Angle&, const Angle&);
  ~Vector3();
  // virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // virtual void persistentRead(PersistenceManager& pm);
  // void set(const float* ptr);
  void set(float);
  void set(float, float, float);
  void set(const Vector3&);
  void set(float, const Angle&, const Angle&);
  // Vector3& operator = (const Vector3&);
  // Vector3& operator = (const float*);
  // void get(float*) const;
  // void get(float&, float&, float&) const;
  void get(Vector3&) const;
  void get(float, Angle&, Angle&) const;
  void swap(Vector3&);
  float getLength() const;
  float getSquareLength() const;
  Vector3& normalize();
  Vector3& negate();
  static const Vector3& getZero();
  static const Vector3& getUnit();

  float operator [] (Index);
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

  // float operator [] (Index) const;
  // ???? const Vector3& operator + () const;
  Vector3 operator - () const;
  Vector3& operator += (const Vector3&);
  Vector3& operator -= (const Vector3&);
 //???? Vector3& operator *= (float);
  Vector3& operator /= (float) throw(DivisionByZero);
  // Vector3& operator %= (const Vector3&);
  Vector3 operator * (float);
  Vector3 operator / (float) throw(DivisionByZero);
  float operator * (const Vector3&) const;
  Vector3 operator % (const Vector3&) const;
  float getDistance(const Vector3&) const;
  float getSquareDistance(const Vector3&) const;
  Angle getAngle(const Vector3&) const;
  Vector3 getOrthogonalProjection(const Vector3&) const;
  static Vector3 getPerpendicularNormalization
   (const Vector3&, const Vector3&, const Vector3&);
  static float getTripleProduct (const Vector3&, const Vector3&, const Vector3&);
  bool operator == (const Vector3&) const;
  bool operator != (const Vector3&) const;
  bool isZero() const;
  bool isOrthogonalTo(Vector3&) const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  bool isValid() const;

	Vector3 operator + (const Vector3&) const;
	Vector3 operator - (const Vector3&) const;

  float x;
  float y;
  float z;


  SIP_PYOBJECT __str__();
%MethodCode
  static String tmp;
  tmp = "(";
  tmp += String(sipCpp->x) + " ";
  tmp += String(sipCpp->y) + " ";
  tmp += String(sipCpp->z) + ")";
 	sipRes = PyString_FromString(tmp.c_str());
%End

};

// Vector3 operator * (float scalar, const Vector3& vector)
// Vector3 operator * (const Vector3& vector, float scalar)
// std::istream& operator >> (std::istream& s, Vector3& v)
// std::ostream& operator << (std::ostream& s, const Vector3& v)
