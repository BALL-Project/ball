 class Bond
  : Composite,
    PropertyManager
 {
%HeaderCode
#include <BALL/KERNEL/bond.h>
using namespace BALL;
%End
   public:
  Bond();
	 Bond(const Bond&, bool = true);
//	Bond(const String&, Atom&, Atom&, int = ORDER__UNKNOWN , int = TYPE__UNKNOWN );
	  static Bond* createBond(Bond&, Atom&, Atom&);
//	virtual ~Bond();
  virtual void clear();
  virtual void destroy();
//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
//	void persistentRead(PersistenceManager& pm);
//  void finalize();
  void set(const Bond&, bool = true);
//  Bond& operator = (const Bond&);
  void get(Bond&, bool = true) const;
   void swap(Bond&);
   void setFirstAtom(Atom*);
   Atom* getFirstAtom();
   const Atom* getFirstAtom() const;
   Atom* getPartner(const Atom&) const;
   void setSecondAtom(Atom*);
   Atom* getSecondAtom();
   const Atom* getSecondAtom() const;
   void setName(const String&);
   const String& getName() const;
   void setOrder(int);
   int getOrder() const;
   void setType(int);
   int getType() const;
   float getLength() const;
   static Bond* getBond(Atom&, Atom&);
   Atom* getBondedAtomOf(const Atom&);
   const Atom* getBondedAtomOf(const Atom&) const;
   bool isBondOf(const Atom&) const;
   bool isBonded() const;
   bool isInterBond() const;
   bool isInterBondOf(const BaseFragment&) const;
   bool isInterBondOf(const System&) const;
   bool isIntraBond() const;
   bool isIntraBondOf(const BaseFragment&) const;
   bool isIntraBondOf(const System&) const;
   virtual bool isValid() const;
//   virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
 };
