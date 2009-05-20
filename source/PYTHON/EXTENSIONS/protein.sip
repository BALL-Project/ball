// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Protein
  : Molecule
{
%TypeHeaderCode
	#include <BALL/KERNEL/protein.h>
	using namespace BALL;
%End
  public:
  Protein();
  Protein(const Protein&, bool = true);
  Protein(const String&, const String& = "" );
	~Protein() throw();
  virtual void clear() throw();
  virtual void destroy() throw();
	//  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
	//  void persistentRead(PersistenceManager& pm);
  void set(const Protein&, bool = true);
	//  Protein& operator = (const Protein& protein);
  void get(Protein&, bool = true) const;
  void swap(Protein&);

  Chain* getChain(int);
  // const Chain* getChain(int) const;

  SecondaryStructure* getSecondaryStructure(int);
  //const SecondaryStructure* getSecondaryStructure(int) const;

  Residue* getResidue(int);
  // const Residue* getResidue(int) const;

  Residue* getNTerminal();
  // const Residue* getNTerminal() const;

  Residue* getCTerminal();
  // const Residue* getCTerminal() const;

  PDBAtom* getPDBAtom(int);
  // const PDBAtom* getPDBAtom(int) const;

  void setID(const String&);
  const String& getID() const;
  int countChains() const;
  int countSecondaryStructures() const;
  int countResidues() const;
  int countPDBAtoms() const;
  virtual bool isValid() const throw();
	//  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
	//  virtual void read(istream& s);
	//  virtual void write(ostream& s) const;


  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Protein ") + sipCpp->getName() 
						+ " { " + String(sipCpp->countResidues()) + " residues }").c_str());
%End

};
