// $Id: molecule.sip,v 1.1 2000/07/18 21:27:47 oliver Exp $

class Molecule
  : BaseFragment
{
%HeaderCode
	#include <BALL/KERNEL/molecule.h>
	using namespace BALL;
%End
  public:
  Molecule();
  Molecule(const Molecule&, bool = true);
  Molecule(const String&);
	//  virtual ~Molecule();
  virtual void clear();
  virtual void destroy();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
  void set(const Molecule&, bool = true);
	//  Molecule& operator = (const Molecule&);
  void get(Molecule&, bool = true) const;
  void swap(Molecule&);
  System* getSystem();
  const System* getSystem() const;
  void prepend(Atom&);
  void append(Atom&);
  void insert(Atom&);
  void insertBefore(Atom&, Composite&);
  void insertAfter(Atom&, Composite&);
  bool remove(Atom&);
  void prepend(BaseFragment&);
  void append(BaseFragment&);
  void insert(BaseFragment&);
  void insertBefore(BaseFragment&, Composite&);
  void insertAfter(BaseFragment&, Composite&);
  void spliceBefore(BaseFragment&);
  void spliceAfter(BaseFragment&);
  void splice(BaseFragment&);
  bool remove(BaseFragment&);
  virtual bool isValid() const;
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;
	operator const char* ();
%MemberCode
  Molecule* ptr;
  if ((ptr = (Molecule*)sipGetCppPtr(sipThis,sipClass_Molecule)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Molecule ") + ptr->getName() 
				+ " { " + String(ptr->countAtoms()) + " atoms }").c_str());
%End
};
