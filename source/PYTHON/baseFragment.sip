// $Id: baseFragment.sip,v 1.2 2000/07/03 11:11:04 oliver Exp $

class BaseFragment
  : Composite, 
		PropertyManager
{
%HeaderCode
	#include <BALL/KERNEL/baseFragment.h>
	using namespace BALL;
%End

  public:
  BaseFragment();
  BaseFragment(const BaseFragment&, bool = true);
  BaseFragment(const String&);
	~BaseFragment();
  virtual void clear();
  virtual void destroy();
	//  virtual void persistentWrite(PersistenceManager& pm, const char* = 0) const;
	//  virtual void persistentRead(PersistenceManager& pm);
  void set(const BaseFragment&, bool = true);
	//  BaseFragment& operator = (const BaseFragment&);
  void get(BaseFragment&, bool = true) const;
  void swap(BaseFragment&);
  void setName(const String&);
  String& getName();
  const String& getName() const;
  Molecule* getMolecule();
  const Molecule* getMolecule() const;
  BaseFragment* getSuperBaseFragment();
  const BaseFragment* getSuperBaseFragment() const;
  BaseFragment* getBaseFragment(Position);
  const BaseFragment* getBaseFragment(Position) const;
  Atom* getAtom(Position);
  const Atom* getAtom(Position) const;
  Atom* getAtom(const String&);
  const Atom* getAtom(const String&) const;
  Size countBaseFragments() const;
  Size countAtoms() const;
  Size countBonds() const;
  Size countInterBonds() const;
  Size countIntraBonds() const;
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
  void destroyBonds();
  bool isSubBaseFragmentOf(const BaseFragment&) const;
  bool isSuperBaseFragmentOf(const BaseFragment&) const;
  virtual bool isValid() const;
	//  virtual void dump(ostream& s = cout,unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;

	// workaround for template method apply()...
	bool apply(AtomProcessor& /processor/);

	
	operator const char* ();
%MemberCode
  BaseFragment* ptr;
  if ((ptr = (BaseFragment*)sipGetCppPtr(sipThis,sipClass_BaseFragment)) == NULL)
    return NULL;

  return PyString_FromString(String(String("BaseFragment ") + ptr->getName() 
						+ " { " + String(ptr->countAtoms()) + " atoms }").c_str());
%End
};
