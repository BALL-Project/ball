 class Object
 {
%HeaderCode
#include <BALL/CONCEPT/object.h>
using namespace BALL;
%End
  public:
  Object() throw();
  Object(const Object&) throw();
  ~Object() throw();
  virtual void clear() throw();
  Size getHandle() const throw();
  static Size getNextHandle() throw();
  static Size getNewHandle() throw();
  virtual bool operator == (const Object& /object/) const;
  virtual bool operator != (const Object& /object/) const;
  virtual bool operator < (const Object& /object/) const;
  virtual bool operator <= (const Object& /object/) const;
 	virtual bool operator >= (const Object& /object/) const;
  virtual bool operator > (const Object& /object/) const;
  int compare(const Object&) const;
  // virtual void read(::std::istream& s);
  // virtual void write(::std::ostream& s) const;
  // friend ::std::istream& operator >> (::std::istream& s, Object& /object/);
  // friend ::std::ostream& operator << (::std::ostream& s, const Object& /object/);
  virtual bool isValid() const throw();
  //virtual void dump(::std::ostream& s = std::cout, unsigned long depth = 0) const;
 };
