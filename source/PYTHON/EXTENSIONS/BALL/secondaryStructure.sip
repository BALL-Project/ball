// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class SecondaryStructure
  : AtomContainer
{
%TypeHeaderCode
	#include <BALL/KERNEL/secondaryStructure.h>
	using namespace BALL;
%End
  public:
	enum Type
	{
		HELIX,
		COIL,
		STRAND,
		TURN,
		UNKNOWN, 
		NUMBER_OF_TYPES
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

	Type getType() const throw();
	void setType(Type) throw();

  Protein* getProtein();
  // const Protein* getProtein() const;

  Chain* getChain();
  // const Chain* getChain() const;

  Residue* getResidue(int);
  // const Residue* getResidue(int) const;

  Residue* getNTerminal();
  // const Residue* getNTerminal() const;

  Residue* getCTerminal();
  // const Residue* getCTerminal() const;

  PDBAtom* getPDBAtom(int);
  // const PDBAtom* getPDBAtom(int) const;

  int countResidues() const;
  int countPDBAtoms() const;
  void prepend(Residue& /Transfer/);
  void append(Residue& /Transfer/);
  void insert(Residue& /Transfer/);
  void insertBefore(Residue& /Transfer/, Composite&);
  void insertAfter(Residue& /Transfer/, Composite&);
  bool remove(Residue&);
  void spliceBefore(SecondaryStructure&);
  void spliceAfter(SecondaryStructure&);
  void splice(SecondaryStructure&);
  virtual bool isValid() const throw();
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;


  SIP_PYOBJECT __str__();
%MethodCode
	return PyString_FromString(String(String("SecondaryStructure ") + sipCpp->getName() 
						+ " { " + String(sipCpp->countResidues()) + " residues }").c_str());
%End

  SIP_PYOBJECT __repr__();
%MethodCode
	return PyString_FromString(String(String("SecondaryStructure ") + sipCpp->getName() 
						+ " { " + String(sipCpp->countResidues()) + " residues }").c_str());
%End

};
