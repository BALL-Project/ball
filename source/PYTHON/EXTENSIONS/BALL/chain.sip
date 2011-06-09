// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Chain
 : AtomContainer
{
%TypeHeaderCode
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
  // const Protein* getProtein() const;

  SecondaryStructure* getSecondaryStructure(int);
  // const SecondaryStructure* getSecondaryStructure(int) const;

  Residue* getResidue(int);
  // const Residue* getResidue(int) const;

  Residue* getNTerminal();
  // const Residue* getNTerminal() const;

  Residue* getCTerminal();
  // const Residue* getCTerminal() const;

  PDBAtom* getPDBAtom(int);
  // const PDBAtom* getPDBAtom(int) const;

  int countSecondaryStructures() const;
  int countResidues() const;
  int countPDBAtoms() const;
  void prepend(SecondaryStructure& /Transfer/);
  void append(SecondaryStructure& /Transfer/);
  void insert(SecondaryStructure& /Transfer/);
  void insertBefore(SecondaryStructure& /Transfer/, Composite&);
  void insertAfter(SecondaryStructure& /Transfer/, Composite&);
  bool remove(SecondaryStructure&);
  void prepend(Residue& /Transfer/);
  void append(Residue& /Transfer/);
  void insert(Residue& /Transfer/);
  void insertBefore(Residue& /Transfer/,Composite&);
  void insertAfter(Residue& /Transfer/, Composite&);
  bool remove(Residue&);
  void spliceBefore(Chain&);
  void spliceAfter(Chain&);
  void splice(Chain&);
  virtual bool isValid() const throw();
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;
	
  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Chain ") + sipCpp->getName()
						+ " { " + String(sipCpp->countResidues()) + " residues }").c_str());
%End

	PDBAtomIterator iterpdbatoms();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new PDBAtomIterator(sipCpp->beginPDBAtom()), PDBAtomIterator, Py_None);
%End

	SecondaryStructureIterator itersecondarystructure();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new SecondaryStructureIterator(sipCpp->beginSecondaryStructure()),
	                          SecondaryStructureIterator,
	                          Py_None);
%End

	ResidueIterator iterresidues();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new ResidueIterator(sipCpp->beginResidue()), ResidueIterator, Py_None);
%End

};
