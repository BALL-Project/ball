// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class AtomType
{
%TypeHeaderCode
	#include <BALL/KERNEL/atom.h>
	using namespace BALL;
	typedef Atom::Type AtomType;
%End
	public:
%ConvertToTypeCode
	if (sipIsErr == NULL)
		return PyInt_Check(sipPy) || BALL_CAN_CONVERT_TO_INSTANCE(sipPy, Atom);

	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new AtomType(PyInt_AS_LONG(sipPy));
	
		return 1;
	}
	
	*sipCppPtr = BALL_CONVERT_TO_CPP(AtomType);

	return 0;
%End

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(*sipCpp).c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(*sipCpp).c_str());
%End
};

class Atom
  : Composite,
    PropertyManager
{
%TypeHeaderCode 
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
    MAX_NUMBER_OF_BONDS = 12
  };
  enum Property
  {
  	NUMBER_OF_PROPERTIES = 0
  };
  enum FullNameType 
  {
	   NO_VARIANT_EXTENSIONS,
  	 ADD_VARIANT_EXTENSIONS,
	   ADD_RESIDUE_ID,
		 ADD_VARIANT_EXTENSIONS_AND_ID
  };
  Atom() throw();
  Atom(const Atom& /atom/, bool /deep/ = true) throw();
  Atom
    (Element& /element/,
     const String& /name/, const String& /type_name/ = "UNK" ,
     int /atom_type/ = UNKNOWN_TYPE ,
     const Vector3& /position/ = Vector3(0,0,0 ),
     const Vector3& /velocity/ = Vector3(0,0,0 ),
     const Vector3& /force/ = Vector3(0,0,0 ),
     float /charge/ = 0,
     float /radius/ = 0,
		 Index /formal_charge/ = 0);
   ~Atom() throw();
   virtual void clear() throw();
   virtual void destroy() throw();
   // void persistentWrite(PersistenceManager& /pm/, const char* /name/ = 0) const throw(GeneralException);
   // void persistentRead(PersistenceManager& /pm/) throw(GeneralException);
   void set(const Atom& /atom/, bool /deep/ = true) throw();
   void get(Atom& /atom/, bool /deep/ = true) const throw();
   void swap(Atom& /atom/) throw();
   // Atom& operator = (const Atom& /atom/);

	 // Predicates
	 bool operator == (const Atom& /atom/) const throw();
	 bool operator != (const Atom& /atom/) const throw();

	 // Accessors
   void setElement(Element& /element/) throw();
   const Element& getElement() const throw();

   void setCharge(float /charge/) throw();
   float getCharge() const throw();

	 void setFormalCharge(Index /formal_charge/) throw();
   Index getFormalCharge() const throw();

   // const Fragment* getFragment() const throw();
   Fragment* getFragment() throw();

	 // const Residue* getResidue() const throw();
	 Residue* getResidue() throw();

	 // const Chain* getChain() const throw();
   Chain* getChain() throw();

	 // const SecondaryStructure* getSecondaryStructure() const throw();
	 SecondaryStructure* getSecondaryStructure() throw();

   // const Molecule* getMolecule() const throw();
   Molecule* getMolecule() throw();

	 Position getIndex() const;

   void setName(const String& /name/) throw();
   const String& getName() const throw();
   String getFullName(FullNameType /type/ = ADD_VARIANT_EXTENSIONS) const throw();

   void setPosition(const Vector3& /position/) throw();
   const Vector3& getPosition() const throw();
   float getDistance(const Atom& a) const;
   //Vector3& getPosition() throw();

   void setRadius(float /radius/) throw();
   float getRadius() const throw();

   void setType(AtomType /atom_type/);
   AtomType getType() const;

   String getTypeName() const throw();
   void setTypeName(const String& /name/) throw();

   void setVelocity(const Vector3& /velocity/) throw();
   const Vector3& getVelocity() const throw();
	 // Vector3& getForce() throw();
	  
   void setForce(const Vector3& /force/) throw();
   const Vector3& getForce() const throw();
   // Vector3& getForce() throw();

   Size countBonds() const;

   Bond* getBond(Position /index/) throw(IndexOverflow);
   //const Bond* getBond(Position /index/) const throw(IndexOverflow);

   Bond* getBond(const Atom& /atom/) throw();
   //const Bond *getBond(const Atom& /atom/) const throw();

   Bond* createBond(Atom& /atom/) throw(TooManyBonds);
   Bond* createBond(Bond& /bond/, Atom& /atom/) throw(TooManyBonds);

   Bond* cloneBond(Bond& /bond/, Atom& /atom/) throw();
   bool destroyBond(const Atom& /atom/) throw();
   void destroyBonds() throw();

   bool hasBond(const Bond& /bond/) const throw();
   bool isBoundTo(const Atom& /atom/) const throw();
   bool isBound() const throw();
   bool isGeminal(const Atom& /atom/) const throw();
   bool isVicinal(const Atom& /atom/) const throw();
   virtual bool isValid() const throw();

   // virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
   // virtual void read(std::istream& s);
   // virtual void write(std::ostream& s) const;
   bool applyBonds(BondProcessor& /processor/) throw();
	
	// convert the atom to a string representation
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Atom ") + sipCpp->getName() 
						+ " { " + sipCpp->getElement().getSymbol() + " @ (" 
						+ String(sipCpp->getPosition().x) + " " + String(sipCpp->getPosition().y) + " " 
						+ String(sipCpp->getPosition().z) + ") }").c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("Atom ") + sipCpp->getName() 
						+ " { " + sipCpp->getElement().getSymbol() + " @ (" 
						+ String(sipCpp->getPosition().x) + " " + String(sipCpp->getPosition().y) + " " 
						+ String(sipCpp->getPosition().z) + " }").c_str());
%End


};
 

