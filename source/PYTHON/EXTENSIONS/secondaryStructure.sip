// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: secondaryStructure.sip,v 1.5 2003/08/26 09:18:23 oliver Exp $
//

class SecondaryStructure
  : AtomContainer
{
%HeaderCode
	#include <BALL/KERNEL/secondaryStructure.h>
	using namespace BALL;
%End
  public:
	enum Property
	{
		PROPERTY__HELIX,
		PROPERTY__RANDOM_COIL,
		PROPERTY__STRAND,
		PROPERTY__TURN,
		NUMBER_OF_PROPERTIES
	};
  SecondaryStructure();
  SecondaryStructure(const SecondaryStructure&,bool = true);
  SecondaryStructure(const String&);
  ~SecondaryStructure() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
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
  virtual bool isValid() const throw();
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;


  char* __str__() /NonLazy/;
%MemberCode
	  SecondaryStructure* ptr;

    if (sipParseArgs(&sipArgsParsed,sipArgs,"n",&sipSelfObj,sipClass_Residue,&ptr))
    {
		  return PyString_FromString(String(String("SecondaryStructure ") + ptr->getName() 
						+ " { " + String(ptr->countResidues()) + " residues }").c_str());
		}
%End

};
