// $Id: secondaryStructure.sip,v 1.2 2000/08/30 19:59:11 oliver Exp $

class SecondaryStructure
  : AtomContainer
{
%HeaderCode
	#include <BALL/KERNEL/secondaryStructure.h>
	using namespace BALL;
%End
  public:
  SecondaryStructure();
  SecondaryStructure(const SecondaryStructure&,bool = true);
  SecondaryStructure(const String&);
  ~SecondaryStructure();
  virtual void clear();
  virtual void destroy();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
  void set(const SecondaryStructure&, bool = true);
	//  SecondaryStructure &operator =(const SecondaryStructure&);
  void get(SecondaryStructure&, bool = true) const;
  void swap(SecondaryStructure&);
  Protein* getProtein();
  const Protein* getProtein() const;
  Chain* getChain();
  const Chain* getChain() const;
  Residue* getResidue(int);
  const Residue* getResidue(int) const;
  Residue* getNTerminal();
  const Residue* getNTerminal() const;
  Residue* getCTerminal();
  const Residue* getCTerminal() const;
  PDBAtom* getPDBAtom(int);
  const PDBAtom* getPDBAtom(int) const;
  int countResidues() const;
  int countPDBAtoms() const;
  void prepend(Residue&);
  void append(Residue&);
  void insert(Residue&);
  void insertBefore(Residue&, Composite&);
  void insertAfter(Residue&, Composite&);
  bool remove(Residue&);
  void spliceBefore(SecondaryStructure&);
  void spliceAfter(SecondaryStructure&);
  void splice(SecondaryStructure&);
  virtual bool isValid() const;
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;
	operator const char* ();
%MemberCode
  SecondaryStructure* ptr;
  if ((ptr = (SecondaryStructure*)sipGetCppPtr(sipThis,sipClass_SecondaryStructure)) == NULL)
    return NULL;

  return PyString_FromString(String(String("SecondaryStructure ") + ptr->getName() 
				+ " { " + String(ptr->countResidues()) + " residues }").c_str());
%End

};
