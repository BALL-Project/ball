// $Id: chain.sip,v 1.3 2003/03/25 17:08:56 anhi Exp $

class Chain
 : AtomContainer
{
%HeaderCode
	#include <BALL/KERNEL/chain.h>
	using namespace BALL;
%End
  public:
  Chain();
  Chain(const Chain&, bool = true);
  Chain(const String&);
  ~Chain() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
  void set(const Chain&, bool = true);
	//  Chain& operator = (const Chain&);
  void get(Chain&, bool = true) const;
  void swap(Chain&);
  Protein* getProtein();
  const Protein* getProtein() const;
  SecondaryStructure* getSecondaryStructure(int);
  const SecondaryStructure* getSecondaryStructure(int) const;
  Residue* getResidue(int);
  const Residue* getResidue(int) const;
  Residue* getNTerminal();
  const Residue* getNTerminal() const;
  Residue* getCTerminal();
  const Residue* getCTerminal() const;
  PDBAtom* getPDBAtom(int);
  const PDBAtom* getPDBAtom(int) const;
  int countSecondaryStructures() const;
  int countResidues() const;
  int countPDBAtoms() const;
  void prepend(SecondaryStructure&);
  void append(SecondaryStructure&);
  void insert(SecondaryStructure&);
  void insertBefore(SecondaryStructure&, Composite&);
  void insertAfter(SecondaryStructure&, Composite&);
  bool remove(SecondaryStructure&);
  void prepend(Residue&);
  void append(Residue&);
  void insert(Residue&);
  void insertBefore(Residue&,Composite&);
  void insertAfter(Residue&, Composite&);
  bool remove(Residue&);
  void spliceBefore(Chain&);
  void spliceAfter(Chain&);
  void splice(Chain&);
  virtual bool isValid() const throw();
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;
	
	char *__str__() /NoLazy/;
%MemberCode
  Chain* ptr;

	if (sipParseArgs(&sipArgsParsed,sipArgs,"n",sipClass_Chain,&ptr))
	{
  return PyString_FromString(String(String("Chain ") + ptr->getName()
				+ " { " + String(ptr->countResidues()) + " residues }").c_str());
	}
	else
	{
		return NULL;
	}
%End

};
