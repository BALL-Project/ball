// $Id: atom.sip,v 1.1 2000/07/18 17:02:08 oliver Exp $

class Atom
  : Composite,
    PropertyManager
{
%HeaderCode 
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
	typedef Atom::Type AtomType;
%End
  public:
  // friend class Bond;
  // BALL_CREATE(Atom)
  // typedef short Type; -> translated to AtomType (see below)
  enum
  {
	  UNKNOWN_TYPE = -1,
  	ANY_TYPE = 0,
    MAX_NUMBER_OF_BONDS = 8
  };
  enum Property
  {
  	NUMBER_OF_PROPERTIES
  };
  enum FullNameType 
  {
	   NO_VARIANT_EXTENSIONS,
  	 ADD_VARIANT_EXTENSIONS
  };
  Atom();
  Atom(const Atom& /atom/, bool /deep/ = true);
  Atom
    (Element& /element/,
     const String& /name/, const String& /type_name/ = "UNK" ,
     int /atom_type/ = UNKNOWN_TYPE ,
     const Vector3& /position/ = Vector3(0,0,0 ),
     const Vector3& /velocity/ = Vector3(0,0,0 ),
     const Vector3& /force/ = Vector3(0,0,0 ),
     float /charge/ = 0 ,
     float /radius/ = 0 );
   ~Atom();
   virtual void clear();
   virtual void destroy();
   // virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
   // virtual void persistentRead(PersistenceManager& pm);
   void set(const Atom& /atom/, bool /deep/ = true);
   // Atom& operator = (const Atom& /atom/);
   void get(Atom& /atom/, bool /deep/ = true) const;
   void swap(Atom& /atom/);
   void setElement(Element& /element/);
   const Element& getElement() const;
   void setCharge(float /charge/);
   float getCharge() const;
   Molecule *getMolecule();
   const Molecule *getMolecule() const;
   Fragment* getFragment();
   const Fragment* getFragment() const;
   void setName(const String& /name/);
   String& getName();
   const String& getName() const;
   String getFullName(FullNameType /type/ = ADD_VARIANT_EXTENSIONS) const;
   void setPosition(const Vector3& /position/);
   Vector3& getPosition();
   const Vector3& getPosition() const;
   void setRadius(float /radius/);
   float getRadius() const;
   void setType(AtomType /atom_type/);
   AtomType getType() const;
   String getTypeName() const;
   void setTypeName(const String& /name/);
   void setVelocity(const Vector3& /velocity/);
   const Vector3& getVelocity() const;
   void setForce(const Vector3& /force/);
   Vector3& getForce();
   const Vector3& getForce() const;
   Size countBonds() const;
   Bond* getBond(Position /index/);
   const Bond* getBond(Position /index/) const;
   Bond* getBond(const Atom& /atom/);
   const Bond *getBond(const Atom& /atom/) const;
   Bond *createBond(Atom& /atom/);
   Bond *createBond(Bond& /bond/, Atom& /atom/);
   Bond *cloneBond(Bond& /bond/, Atom& /atom/);
   bool destroyBond(const Atom& /atom/);
   void destroyBonds();
   bool hasBond(const Bond& /bond/) const;
   bool isBondedTo(const Atom& /atom/) const;
   bool isBonded() const;
   bool isGeminal(const Atom& /atom/) const;
   bool isVicinal(const Atom& /atom/) const;
   virtual bool isValid() const;
   // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
   // virtual void read(std::istream& s);
   // virtual void write(std::ostream& s) const;
   // bool applyBonds(UnaryProcessor<Bond>& processor);
	
	// convert the atom to a string representation
	operator const char* ();
%MemberCode
  Atom* ptr;
  if ((ptr = (Atom*)sipGetCppPtr(sipThis,sipClass_Atom)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Atom ") + ptr->getName() 
				+ " { " + ptr->getElement().getSymbol() + " @ (" 
				+ String(ptr->getPosition().x) + " " + String(ptr->getPosition().y) + " " 
				+ String(ptr->getPosition().z) + " }").c_str());
%End
};
 
class AtomType
{
%HeaderCode
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
	typedef Atom::Type AtomType;
%End
	public:
	operator const char* ();
%MemberCode
	AtomType* ptr;
  if ((ptr = (AtomType*)sipGetCppPtr(sipThis,sipClass_AtomType)) == NULL)
    return NULL;

  return PyString_FromString(String(*ptr).c_str());
%End
%CanConvertToClassCode
	// automatic conversion of Py integers to Position 
	return (PyInt_Check(sipPy) || sipIsSubClassInstance(sipPy, sipClass_AtomType));
%End
%ConvertToClassCode
	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new AtomType(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = (AtomType*)sipConvertToCpp(sipPy, sipClass_AtomType, sipIsErr);

	return 0;
%End
};

