class System
	: AtomContainer,
	  PropertyManager
{
%TypeHeaderCode
	#include <BALL/KERNEL/system.h>
%End
	public:
	System();
	System(const System&, bool deep = true);
	System(const String&);
	~System() throw();

	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const System&, bool = true);
	void get(System&, bool = true) const;
	void swap(System&);
	void setName(const String&);
	const String& getName() const;
	Molecule* getMolecule(Position position);
	Protein* getProtein(Position position);
	Size countMolecules() const;
	Size countFragments() const;
	Size countAtoms() const;
	Size countSecondaryStructures() const;
	Size countResidues() const;
	Size countChains() const;
	Size countProteins() const;

	void prepend(Molecule& /Transfer/);
	void append(Molecule& /Transfer/);
	void insert(Molecule& /Transfer/);
	void insertBefore(Molecule& /Transfer/, Composite&);
	void insertAfter(Molecule& /Transfer/, Composite&);
	bool remove(Molecule&);
	void spliceBefore(System&);
	void spliceAfter(System&);
	void splice(System&);
	void destroyBonds();
	virtual bool isValid() const throw();

	FragmentIterator iterfragments();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new FragmentIterator(sipCpp->beginFragment()), FragmentIterator, Py_None);
%End

	ChainIterator iterchains();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new ChainIterator(sipCpp->beginChain()), ChainIterator, Py_None);
%End

	ResidueIterator iterresidues();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new ResidueIterator(sipCpp->beginResidue()), ResidueIterator, Py_None);
%End

	MoleculeIterator itermolecules();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new MoleculeIterator(sipCpp->beginMolecule()), MoleculeIterator, Py_None);
%End

	ProteinIterator iterproteins();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new ProteinIterator(sipCpp->beginProtein()), ProteinIterator, Py_None);
%End

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("System ") + sipCpp->getName()
		+ " { " + String(sipCpp->countMolecules()) + " molecules,  "
		+ String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
};
