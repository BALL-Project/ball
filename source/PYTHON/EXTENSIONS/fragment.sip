// $Id: fragment.sip,v 1.2 2001/12/22 16:39:15 oliver Exp $

class Fragment
  : AtomContainer
{
%HeaderCode
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
  Fragment(const Fragment& /fragment/, bool /deep/ = true);
  Fragment(const String& /name/);
  ~Fragment() throw();
  // void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  // void persistentRead(PersistenceManager& pm);
  // Fragment& operator = (const Fragment& fragment);
  // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // virtual void read(std::istream& s);
  // virtual void write(std::ostream& s) const;
	PyMethod __str__
%MemberCode
  Fragment* ptr;
  if ((ptr = (Fragment*)sipGetCppPtr((sipThisType*)a0,sipClass_Fragment)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Fragment ") + ptr->getName()
        + " { " + String(ptr->countAtoms()) + " atoms }").c_str());
%End
};
