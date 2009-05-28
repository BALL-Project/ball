// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class PDBAtom
  : Atom
{
%TypeHeaderCode
	#include <BALL/KERNEL/PDBAtom.h>
	using namespace BALL;
  typedef Atom::Type AtomType;
%End
  public:
  PDBAtom();
  PDBAtom(const PDBAtom&, bool = true);
  PDBAtom
   (Element&,
    const String&,
    const String&,
    AtomType,
    const Vector3&,
    const Vector3&,
    const Vector3&,
    float,
    float,
    char,
    char,
    char,
    float,
    float);
	~PDBAtom() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
  void set(const PDBAtom&, bool = true);
	//  PDBAtom& operator = (const PDBAtom&);
  void get(PDBAtom&, bool = true) const;
  void swap(PDBAtom&);

  Protein* getProtein();
  // const Protein* getProtein() const;

  Chain* getChain();
  // const Chain* getChain() const;

  Residue* getResidue();
  // const Residue* getResidue() const;

  void setBranchDesignator(char);
  char getBranchDesignator() const;
  void setRemotenessIndicator(char);
  char getRemotenessIndicator() const;
  void setAlternateLocationIndicator(char);
  char getAlternateLocationIndicator() const;
  void setOccupancy(float);
  float getOccupancy() const;
  void setTemperatureFactor(float);
  float getTemperatureFactor() const;
  virtual bool isValid() const throw();
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;
  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("PDBAtom ") + sipCpp->getName()
        + " { " + sipCpp->getElement().getSymbol() + " @ ("
        + String(sipCpp->getPosition().x) + " " + String(sipCpp->getPosition().y) + " "
        + String(sipCpp->getPosition().z) + " }").c_str());
%End
};
