// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Fragment
  : AtomContainer
{
%TypeHeaderCode
	#include <BALL/KERNEL/fragment.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(Fragment)
  enum Property
  {
   NUMBER_OF_PROPERTIES = AtomContainer::Property::NUMBER_OF_PROPERTIES
  };
  Fragment();
  Fragment(const Fragment&, bool deep = true);
  Fragment(const String&);
  ~Fragment();
  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  // Fragment& operator = (const Fragment& fragment);
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;


  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Fragment ") + sipCpp->getName()
    			    + " { " + String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
};
