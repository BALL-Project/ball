 class BaseFragment
  : Composite, PropertyManager
 {
%HeaderCode
#include <BALL/KERNEL/baseFragment.h>
using namespace BALL;
%End

  public:
  BaseFragment();
  BaseFragment(const BaseFragment&, bool = true);
  BaseFragment(const String&);
//  virtual ~BaseFragment();
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
  BaseFragment* getBaseFragment(int);
  const BaseFragment* getBaseFragment(int) const;
  Atom* getAtom(int);
  const Atom* getAtom(int) const;
  Atom* getAtom(const String&);
  const Atom* getAtom(const String&) const;
  int countBaseFragments() const;
  int countAtoms() const;
  int countBonds() const;
  int countInterBonds() const;
  int countIntraBonds() const;
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
 };
