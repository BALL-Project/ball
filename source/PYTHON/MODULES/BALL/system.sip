// $Id: system.sip,v 1.1 2000/07/18 17:05:02 oliver Exp $

class System
  : BaseFragment,
  	PropertyManager
{
%HeaderCode
	#include <BALL/KERNEL/system.h>
	using namespace BALL;
%End
  public:
  System();
  System(const System& /system/, bool /deep/ = true);
  System(const String& /name/);
  ~System();
  virtual void clear();
  virtual void destroy();
//  void persistentWrite(PersistenceManager& pm, const char* = 0) const;
//  void persistentRead(PersistenceManager& pm);
  void set(const System&, bool = true);
//  System& operator = (const System&);
  void get(System&, bool = true) const;
  void swap(System&);
  void setName(const String&);
  String& getName();
  const String& getName() const;
  Size countMolecules() const;
  Size countFragments() const;
  Size countAtoms() const;
  void prepend(Molecule&);
  void append(Molecule&);
  void insert(Molecule&);
  void insertBefore(Molecule&, Composite&);
  void insertAfter(Molecule&, Composite&);
  bool remove(Molecule&);
  void spliceBefore(System&);
  void spliceAfter(System&);
  void splice(System&);  
  void destroyBonds();
  virtual bool isValid() const;
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;

	//  template <typename T>
	//  bool apply(UnaryProcessor<T>& processor)

	operator const char* ();
%MemberCode
  System* ptr;
  if ((ptr = (System*)sipGetCppPtr(sipThis,sipClass_System)) == NULL)
    return NULL;

  return PyString_FromString(String(String("System ") + ptr->getName() 
					+ " { " + String(ptr->countMolecules()) + " molecules,  " 
					+ String(ptr->countAtoms()) + " atoms }").c_str());
%End

};
