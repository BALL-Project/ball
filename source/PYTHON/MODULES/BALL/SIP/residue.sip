// $Id: residue.sip,v 1.1 2000/07/18 21:28:23 oliver Exp $

class Residue
  : Fragment
{
%HeaderCode
	#include <BALL/KERNEL/residue.h>
	using namespace BALL;
%End
  public:
  Residue();
  Residue(const Residue&, bool = true);
  Residue(const String&, const String&, char);
	~Residue();
  virtual void clear();
  virtual void destroy();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
  void set(const Residue&, bool = true);
	//  Residue& operator = (const Residue&);
  void get(Residue&, bool = true) const;
  void swap(Residue&);
  Protein* getProtein();
  const Protein* getProtein() const;
  Chain* getChain();
  const Chain* getChain() const;
  PDBAtom* getPDBAtom(int);
  const PDBAtom* getPDBAtom(int) const;
  void setID(const String&);
  const String& getID() const;
  void setInsertionCode(char);
  char getInsertionCode() const;
  int countPDBAtoms() const;
  void prepend(PDBAtom&);
  void append(PDBAtom&);
  void insert(PDBAtom&);
  void insertBefore(PDBAtom&, Composite&);
  void insertAfter(PDBAtom&, Composite&);
  bool remove(PDBAtom&);
  void spliceBefore(Residue&);
  void spliceAfter(Residue&);
  void splice(Residue&);
  bool isAminoAcid() const;
  bool isTerminal() const;
  bool isNTerminal() const;
  bool isCTerminal() const;
  virtual bool isValid() const;
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;
	operator const char* ();
%MemberCode
  Residue* ptr;
  if ((ptr = (Residue*)sipGetCppPtr(sipThis,sipClass_Residue)) == NULL)
    return NULL;

  return PyString_FromString(String(String("Residue ") + ptr->getName() + ptr->getID()
				+ " { " + String(ptr->countAtoms()) + " atoms }").c_str());
%End
};
