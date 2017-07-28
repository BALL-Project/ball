class Chain
	: AtomContainer
{
%TypeHeaderCode
	#include <BALL/KERNEL/chain.h>
%End
	public:
	Chain();
	Chain(const Chain&, bool = true);
	Chain(const String&);
	~Chain() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const Chain&, bool = true);
	void get(Chain&, bool = true) const;
	void swap(Chain&);

	Protein* getProtein();
	SecondaryStructure* getSecondaryStructure(int);
	Residue* getResidue(int);
	Residue* getNTerminal();
	Residue* getCTerminal();
	PDBAtom* getPDBAtom(int);

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
