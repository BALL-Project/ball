 class Atom
  : Composite,
    PropertyManager
 {
%HeaderCode
#include <BALL/KERNEL/atom.h>
using namespace BALL;
%End
  public:
   Atom();
   Atom(const Atom& , bool = true);
   // virtual ~Atom();
   virtual void clear();
   virtual void destroy();
   //virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
   //virtual void persistentRead(PersistenceManager& pm);
   void set(const Atom&, bool = true);
   //Atom& operator = (const Atom& atom);
   void get(Atom&, bool = true) const;
   void swap(Atom&);
   void setElement(Element&);
   const Element& getElement() const;
   void setCharge(float);
   float getCharge() const;
   Molecule *getMolecule();
   const Molecule *getMolecule() const;
   Fragment *getFragment();
   const Fragment *getFragment() const;
   void setName(const String&);
   String& getName();
   String& getFullName();
   const String& getName() const;
   void setPosition(const Vector3&);
   Vector3& getPosition();
   const Vector3& getPosition() const;
   void setRadius(float);
   float getRadius() const;
   void setType(int);
   int getType() const;
   String getTypeName() const;
   void setTypeName(const String&);
   void setVelocity(const Vector3&);
   const Vector3& getVelocity() const;
   void setForce(const Vector3&);
   Vector3& getForce();
   const Vector3& getForce() const;
   int countBonds() const;
   Bond* getBond(int);
   const Bond* getBond(int) const;
   Bond* getBond(const Atom&);
   const Bond *getBond(const Atom&) const;
   Bond *createBond(Atom&);
   Bond *createBond(Bond&, Atom& );
   Bond *cloneBond(Bond&, Atom&);
   bool destroyBond(const Atom&);
   void destroyBonds();
   bool hasBond(const Bond&) const;
   bool isBondedTo(const Atom&) const;
   bool isBonded() const;
   bool isGeminal(const Atom&) const;
   bool isVicinal(const Atom&) const;
   virtual bool isValid() const;
   //virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
   //virtual void read(istream& s);
   //virtual void write(ostream& s) const;
   //bool apply(UnaryProcessor<Bond>& processor);
   //bool apply(UnaryProcessor<Object>& processor);
   //typedef Index BondIteratorPosition_;
 };
