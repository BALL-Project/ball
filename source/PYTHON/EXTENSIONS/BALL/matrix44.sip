// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Matrix4x4
{
%TypeHeaderCode
	#include <BALL/MATHS/matrix44.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Matrix4x4)
  Matrix4x4();
  // Matrix4x4(float* ptr);
  // Matrix4x4(float ptr[4][4]);
  Matrix4x4(const Matrix4x4&);
  Matrix4x4
   (const Vector4&, const Vector4&,
    const Vector4&, const Vector4&);
  Matrix4x4
   (float, float, float, float, 
    float, float, float, float, 
    float, float, float, float, 
    float, float, float, float);
  ~Matrix4x4();
  // void set(float* ptr);
  // void set(float ptr[4][4]);
  void set(const Matrix4x4&);
  void set
   (const Vector4&, const Vector4&,
    const Vector4&, const Vector4&);
  void set
   (float, float, float, float, 
    float, float, float, float, 
    float, float, float, float, 
    float, float, float, float);
  // Matrix4x4& operator = ( float* ptr);
  // Matrix4x4& operator = (float ptr[4][4]);
  // Matrix4x4& operator = (const Matrix4x4& m);
  // void get(float* ptr) const;
  // void get(float ptr[4][4]) const;
  void get(Matrix4x4&) const;
  void get
   (Vector4&, Vector4&,
    Vector4&, Vector4&) const;
  void get
   (float, float, float, float, 
    float, float, float, float, 
    float, float, float, float, 
    float, float, float, float) const;
  void swap(Matrix4x4&);
  float getTrace() const;
  static const Matrix4x4& getZero();
  static const Matrix4x4& getIdentity();
  void setIdentity();
  void set(float t = 1);
  void transpose();
  Vector4 getRow(Position) const;
  Vector4 getColumn(Position) const;
  void setRow(Position, const Vector4&);
  void setColumn(Position, const Vector4&);
  bool isEqual(const Matrix4x4&) const;
  Vector4 getDiagonal() const;
  float operator () (Position, Position);
%MethodCode
	sipRes = sipCpp->operator () (*a0, *a1);
%End

  // float operator () (Position, Position) const;
  // float operator [] (Position) const;
 float operator [] (Position) /Numeric/;
%MethodCode
	try
	{
		sipRes = sipCpp->operator [] (*a0);
	}
	catch (IndexOverflow& e)
	{
		PyErr_SetString(PyExc_IndexError, "index overflow in Matrix4x4. Max. index is 15!");
		sipIsErr = 1;
	}
%End

	int __len__ ();
%MethodCode
  sipRes = 16;
%End

  // ???? Matrix4x4 operator + () const;
  // ???? Matrix4x4 operator - () const;
  Matrix4x4 operator + (const Matrix4x4&) const;
  Matrix4x4& operator += (const Matrix4x4&);
  Matrix4x4 operator - (const Matrix4x4&) const;
  Matrix4x4& operator -= (const Matrix4x4&);
  Matrix4x4 operator * (float) const;
  Matrix4x4& operator *= (float);
  Matrix4x4 operator / (float) const;
  Matrix4x4& operator /= (float);
  Matrix4x4 operator * (const Matrix4x4&) const;
  Matrix4x4& operator *= (const Matrix4x4&);
  Vector4 operator * (const Vector4&) const;
  bool invert(Matrix4x4&) const;
  bool invert();
  float getDeterminant() const;
  void translate(float, float, float);
  void translate(const Vector3&);
  void setTranslation(float, float, float);
  void setTranslation(const Vector3&);
  void scale(float, float, float);
  void scale(float);
  void scale(const Vector3&);
  void setScale(float, float, float);
  void setScale(float);
  void setScale(const Vector3&);
  void rotateX(const Angle&);
  void setRotationX(const Angle&);
  void rotateY(const Angle&);
  void setRotationY(const Angle&);
  void rotateZ(const Angle &);
  void setRotationZ(const Angle &);
  void rotate(const Angle&, float, float, float);
  void rotate(const Angle&, const Vector3&);
  void rotate(const Angle&, const Vector4&);
  void setRotation(const Angle&, float, float, float); 
  void setRotation(const Angle&, const Vector3&);
  void setRotation(const Angle&, const Vector4&);
  bool operator == (const Matrix4x4&) const; 
  bool operator != (const Matrix4x4&) const;
  bool isIdentity() const;
  bool isRegular() const;
  bool isSingular() const;
  bool isSymmetric() const;
  bool isLowerTriangular() const;
  bool isUpperTriangular() const;
  bool isDiagonal() const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;

  float m11;
  float m12;
  float m13;
  float m14;
  float m21;
  float m22;
  float m23;
  float m24;
  float m31;
  float m32;
  float m33;
  float m34;
  float m41;
  float m42;
  float m43;
  float m44;
};

