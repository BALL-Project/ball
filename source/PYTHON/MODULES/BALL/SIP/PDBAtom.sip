// $Id: PDBAtom.sip,v 1.2 2001/07/25 11:32:31 oliver Exp $

class PDBAtom
  : Atom
{
%HeaderCode
	#include <BALL/KERNEL/PDBAtom.h>
	using namespace BALL;
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
  const Protein* getProtein() const;
  Chain* getChain();
  const Chain* getChain() const;
  Residue* getResidue();
  const Residue* getResidue() const;
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
	PyMethods:
	__str__
%MemberCode
  PDBAtom* ptr;
  if ((ptr = (PDBAtom*)sipGetCppPtr((sipThisType*)a0,sipClass_PDBAtom)) == NULL)
    return NULL;

  return PyString_FromString(String(String("PDBAtom ") + ptr->getName()
        + " { " + ptr->getElement().getSymbol() + " @ ("
        + String(ptr->getPosition().x) + " " + String(ptr->getPosition().y) + " "
        + String(ptr->getPosition().z) + " }").c_str());
%End
};
