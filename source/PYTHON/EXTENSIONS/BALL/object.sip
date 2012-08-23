// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Object
{
%TypeHeaderCode
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
  bool operator == (const Object&) const;
  bool operator != (const Object&) const;
  bool operator < (const Object&) const;
  bool operator <= (const Object&) const;
 	bool operator >= (const Object&) const;
  bool operator > (const Object&) const;
  int compare(const Object&) const;
  // virtual void read(::std::istream& s);
  // virtual void write(::std::ostream& s) const;
  // friend ::std::istream& operator >> (::std::istream& s, Object&);
  // friend ::std::ostream& operator << (::std::ostream& s, const Object&);
  virtual bool isValid() const throw();
  //virtual void dump(::std::ostream& s = std::cout, unsigned long depth = 0) const;
};
