class Protein
	: Molecule
{
%TypeHeaderCode
	#include <BALL/KERNEL/protein.h>
%End
	public:
	Protein();
	Protein(const Protein&, bool = true);
	Protein(const String&, const String& = "" );
	~Protein() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const Protein&, bool = true);
	void get(Protein&, bool = true) const;
	void swap(Protein&);

	Chain* getChain(int);
	SecondaryStructure* getSecondaryStructure(int);
	Residue* getResidue(int);
	Residue* getNTerminal();
	Residue* getCTerminal();
	PDBAtom* getPDBAtom(int);
	void setID(const String&);
	const String& getID() const;
	int countChains() const;
	int countSecondaryStructures() const;
	int countResidues() const;
	int countPDBAtoms() const;
	virtual bool isValid() const throw();

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

	ChainIterator iterchains();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new ChainIterator(sipCpp->beginChain()), ChainIterator, Py_None);
%End

	ResidueIterator iterresidues();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new ResidueIterator(sipCpp->beginResidue()), ResidueIterator, Py_None);
%End


  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Protein ") + sipCpp->getName()
		+ " { " + String(sipCpp->countResidues()) + " residues }").c_str());
%End
};
