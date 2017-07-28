class SecondaryStructure
	: AtomContainer
{
%TypeHeaderCode
	#include <BALL/KERNEL/secondaryStructure.h>
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
	void set(const SecondaryStructure&, bool = true);
	void get(SecondaryStructure&, bool = true) const;
	void swap(SecondaryStructure&);
	Type getType() const throw();
	void setType(Type) throw();

	Protein* getProtein();
	Chain* getChain();
	Residue* getResidue(int);
	Residue* getNTerminal();
	Residue* getCTerminal();
	PDBAtom* getPDBAtom(int);
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
