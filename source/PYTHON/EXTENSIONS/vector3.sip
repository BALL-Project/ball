// $Id: vector3.sip,v 1.2 2001/12/22 16:39:16 oliver Exp $

class Vector3
 : PersistentObject
{
  public:
  // BALL_CREATE_NODEEP(Vector3)
  Vector3();
  // Vector3(const T* ptr);
  Vector3(float /value/);
  Vector3(float /vx/, float /vy/, float /vz/);
  Vector3(const Vector3& /vector/);
  Vector3(float /r/, const Angle& /phi/, const Angle& /theta/);
  ~Vector3();
  // virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // virtual void persistentRead(PersistenceManager& pm);
  // void set(const float* ptr);
  void set(float /value/);
  void set(float /vx/, float /vy/, float /vz/);
  void set(const Vector3& /vector/);
  void set(float /r/, const Angle& /phi/, const Angle& /theta/);
  // Vector3& operator = (const Vector3& /v/);
  // Vector3& operator = (const float* /ptr/);
  // void get(float* /ptr/) const;
  // void get(float& /x/, float& /y/, float& /z/) const;
  void get(Vector3& /vector/) const;
  void get(float /r/, Angle& /phi/, Angle& /theta/) const;
  void swap(Vector3& /vector/);
  float getLength() const;
  float getSquareLength() const;
  Vector3& normalize();
  Vector3& negate();
  static const Vector3& getZero();
  static const Vector3& getUnit();
  float operator [] (Index /index/);
  float operator [] (Index /index/) const;
  const Vector3& operator + () const;
  Vector3 operator - () const;
  Vector3& operator += (const Vector3& /vector/);
  Vector3& operator -= (const Vector3& /vector/);
  Vector3& operator *= (float /scalar/);
  Vector3& operator /= (float /lambda/) throw(DivisionByZero);
  // Vector3& operator %= (const Vector3& /vector/);
  Vector3 operator * (float /scalar/);
  Vector3 operator / (float /lambda/) throw(DivisionByZero);
  float operator * (const Vector3& /vector/) const;
  Vector3 operator % (const Vector3& /vector/) const;
  float getDistance(const Vector3& /vector/) const;
  float getSquareDistance(const Vector3& /vector/) const;
  Angle getAngle(const Vector3& /vector/) const;
  Vector3 getOrthogonalProjection(const Vector3& /direction/) const;
  static Vector3 getPerpendicularNormalization
   (const Vector3& /a/, const Vector3& /b/, const Vector3& /c/);
  static float getTripleProduct (const Vector3& /a/, const Vector3& /b/, const Vector3& /c/);
  bool operator == (const Vector3& /vector/) const;
  // bool operator != (const Vector3& /vector/) const;
  bool isZero() const;
  bool isOrthogonalTo(Vector3& /vector/) const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  bool isValid() const;

	Vector3 operator + (const Vector3&) const;
	Vector3 operator - (const Vector3&) const;

  float x;
  float y;
  float z;
	PyMethod __str__
%MemberCode
  Vector3* ptr;
  if ((ptr = (Vector3*)sipGetCppPtr((sipThisType*)a0,sipClass_Vector3)) == NULL)
    return NULL;

  static String tmp;
  tmp = "(";
  tmp += String(ptr->x) + " ";
  tmp += String(ptr->y) + " ";
  tmp += String(ptr->z) + ")";
  return PyString_FromString(tmp.c_str());
%End

};

// Vector3 operator * (float scalar, const Vector3& vector)
// Vector3 operator * (const Vector3& vector, float scalar)
// std::istream& operator >> (std::istream& s, Vector3& v)
// std::ostream& operator << (std::ostream& s, const Vector3& v)
