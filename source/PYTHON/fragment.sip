// $Id: fragment.sip,v 1.2 2000/07/03 11:11:06 oliver Exp $

class Fragment
 : BaseFragment
{
%HeaderCode
	#include <BALL/KERNEL/fragment.h>
	using namespace BALL;
%End
	public:
  Fragment();
  Fragment(const Fragment& /fragment/, bool = true);
  Fragment(const String& /name/);
	~Fragment();
  virtual void clear();
  virtual void destroy();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
	//  Fragment& operator = (const Fragment& fragment);
  virtual bool isValid() const;
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;

	operator const char* ();
%MemberCode
  Fragment* ptr;
  if ((ptr = (Fragment*)sipGetCppPtr(sipThis,sipClass_Fragment)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Fragment ") + ptr->getName() 
				+ " { " + String(ptr->countAtoms()) + " atoms }").c_str());
%End
};  
