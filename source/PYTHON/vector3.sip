 class Vector3
  : PersistentObject
 {
%HeaderCode
#include <BALL/MATHS/vector3.h>
using namespace BALL;
%End
  public:
  Vector3();
//  Vector3(float*);
//	Vector3(const TVector_<T>&);
  Vector3(float);
//  Vector3(float, float, float)
  Vector3(Vector3&);
  Vector3(float, Angle&, Angle&);
//  virtual ~Vector3();
//  virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
//  virtual void persistentRead(PersistenceManager& pm);
  void set(float);
//  void set(float);
//  void set(const TVector_<T>& vector);
//  void set(float, float, float);
  void set(Vector3&);
//  void set(float, const Angle&, const Angle&);
//  Vector3& operator = (const Vector3& v);
//  Vector3& operator = (float* ptr);
//  Vector3& operator = (const TVector_<T>& vector);
//  void get(float*) const;
//  void get(TVector_<T>& vector) const;
//  void get(float, float, float) const;
//  void get(Vector3&, bool = true) const;
//  void get(float r, Angle&, Angle&) const;
  void swap(Vector3&);
  float getLength() const;
  float getSquareLength() const;
  Vector3& normalize();
  static const Vector3& getZero();
  static const Vector3& getUnit();
//  float operator [] (int);
//  float operator [] (Index index) const;
//  operator TVector_<T> () const;
//  Vector3 operator + () const;
//  Vector3 operator - () const;
//  template <typename C>
//  friend Vector3<C> operator + (const Vector3<C>& a, const Vector3<C>& b);
// translation to SIP code: non-friend op
		Vector3 operator + (const Vector3& /vector/);

//  Vector3& operator += (const Vector3& vector);
//  template <typename C>
//  friend Vector3<C> operator - (const Vector3<C>& a, const Vector3<C>& b);
//  Vector3 &operator -= (const Vector3& vector);
//  Vector3 operator * (float scalar);
//  Vector3& operator *= (float scalar);
//  Vector3 operator / (float lambda);
// Vector3& operator /= (float lambda);
//  float operator * (const Vector3& vector) const;
//  Vector3 operator % (const Vector3& vector) const;
//  Vector3& operator %= (const Vector3& vector);
  float getDistance(const Vector3&) const;
  float getSquareDistance(const Vector3&) const;
  Angle getAngle(const Vector3&) const;
  Vector3 getOrthogonalProjection(const Vector3&) const;
  static Vector3 getPerpendicularNormalization
   (const Vector3&, const Vector3&, const Vector3&);
//  bool operator == (const Vector3&) const;
//  bool operator != (const Vector3&) const;
  bool isZero() const;
  bool isOrthogonalTo(Vector3&) const;
//  void dump(ostream& s = cout, unsigned long depth = 0) const;
  bool isValid() const;
	
	// conversion to printable string:
	operator const char* ();
%MemberCode
  Vector3* ptr;
  if ((ptr = (Vector3*)sipGetCppPtr(sipThis,sipClass_Vector3)) == NULL)
    return NULL;

	static String tmp;
	tmp = "(";
	tmp += String(ptr->x) + " ";
	tmp += String(ptr->y) + " ";
	tmp += String(ptr->z) + ")";
  return PyString_FromString(tmp.c_str());	
%End
};
