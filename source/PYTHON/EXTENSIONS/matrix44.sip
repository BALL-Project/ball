// $Id: matrix44.sip,v 1.3 2001/12/27 11:47:18 oliver Exp $

class Matrix4x4
{
%HeaderCode
	#include <BALL/MATHS/matrix44.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Matrix4x4)
  Matrix4x4();
  // Matrix4x4(float* ptr);
  // Matrix4x4(float ptr[4][4]);
  Matrix4x4(const Matrix4x4& /m/);
  Matrix4x4
   (const Vector4& /col1/, const Vector4& /col2/,
    const Vector4& /col3/, const Vector4& /col4/);
  Matrix4x4
   (float /m11/, float /m12/, float /m13/, float /m14/, 
    float /m21/, float /m22/, float /m23/, float /m24/, 
    float /m31/, float /m32/, float /m33/, float /m34/, 
    float /m41/, float /m42/, float /m43/, float /m44/);
  ~Matrix4x4();
  // void set(float* ptr);
  // void set(float ptr[4][4]);
  void set(const Matrix4x4& /m/);
  void set
   (const Vector4& /col1/, const Vector4& /col2/,
    const Vector4& /col3/, const Vector4& /col4/);
  void set
   (float /m11/, float /m12/, float /m13/, float /m14/, 
    float /m21/, float /m22/, float /m23/, float /m24/, 
    float /m31/, float /m32/, float /m33/, float /m34/, 
    float /m41/, float /m42/, float /m43/, float /m44/);
  // Matrix4x4& operator = ( float* ptr);
  // Matrix4x4& operator = (float ptr[4][4]);
  // Matrix4x4& operator = (const Matrix4x4& m);
  // void get(float* ptr) const;
  // void get(float ptr[4][4]) const;
  void get(Matrix4x4& /m/) const;
  void get
   (Vector4& /col1/, Vector4& /col2/,
    Vector4& /col3/, Vector4& /col4/) const;
  void get
   (float /m11/, float /m12/, float /m13/, float /m14/, 
    float /m21/, float /m22/, float /m23/, float /m24/, 
    float /m31/, float /m32/, float /m33/, float /m34/, 
    float /m41/, float /m42/, float /m43/, float /m44/) const;
  void swap(Matrix4x4& /m/);
  float getTrace() const;
  static const Matrix4x4& getZero();
  static const Matrix4x4& getIdentity();
  void setIdentity();
  void set(float /t/ = 1);
  void transpose();
  Vector4 getRow(Position /row/) const;
  Vector4 getColumn(Position /col/) const;
  void setRow(Position /row/, const Vector4& /row_value/);
  void setColumn(Position /col/, const Vector4& /col_value/);
  bool isEqual(const Matrix4x4& /m/) const;
  Vector4 getDiagonal() const;
  float operator () (Position /row/, Position /col/);
  float operator () (Position /row/, Position /col/) const;
  float operator [] (Position /index/) const;
  float operator [] (Position /index/);
  Matrix4x4 operator + () const;
  Matrix4x4 operator - () const;
  Matrix4x4 operator + (const Matrix4x4& /m/) const;
  Matrix4x4& operator += (const Matrix4x4& /m/);
  Matrix4x4 operator - (const Matrix4x4& /m/) const;
  Matrix4x4& operator -= (const Matrix4x4& /m/);
  Matrix4x4 operator * (float /scalar/) const;
  Matrix4x4& operator *= (float /scalar/);
  Matrix4x4 operator / (float /scalar/) const;
  Matrix4x4& operator /= (float /scalar/);
  Matrix4x4 operator * (const Matrix4x4& /m/) const;
  Matrix4x4& operator *= (const Matrix4x4& /m/);
  Vector4 operator * (const Vector4& /vector/) const;
  bool invert(Matrix4x4& /inverse/) const;
  bool invert();
  float getDeterminant() const;
  void translate(float /x/, float /y/, float /z/);
  void translate(const Vector3& /v/);
  void setTranslation(float /x/, float /y/, float /z/);
  void setTranslation(const Vector3& /v/);
  void scale(float /x_scale/, float /y_scale/, float /z_scale/);
  void scale(float /scale/);
  void scale(const Vector3& /v/);
  void setScale(float /x_scale/, float /y_scale/, float /z_scale/);
  void setScale(float /scale/);
  void setScale(const Vector3& /v/);
  void rotateX(const Angle& /phi/);
  void setRotationX(const Angle& /phi/);
  void rotateY(const Angle& /phi/);
  void setRotationY(const Angle& /phi/);
  void rotateZ(const Angle &/phi/);
  void setRotationZ(const Angle &/phi/);
  void rotate(const Angle& /phi/, float /axis_x/, float /axis_y/, float /axis_z/);
  void rotate(const Angle& /phi/, const Vector3& /axis/);
  void rotate(const Angle& /phi/, const Vector4& /axis/);
  void setRotation(const Angle& /phi/, float /axis_x/, float /axis_y/, float /axis_z/); 
  void setRotation(const Angle& /phi/, const Vector3& /axis/);
  void setRotation(const Angle& /phi/, const Vector4& /axis/);
  bool operator == (const Matrix4x4& /m/) const; 
  bool operator != (const Matrix4x4& /m/) const;
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

