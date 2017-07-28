class PDBAtom
	: Atom
{
%TypeHeaderCode
	#include <BALL/KERNEL/PDBAtom.h>
	using AtomType = Atom::Type;
%End
	public:
	PDBAtom();
	PDBAtom(const PDBAtom&, bool = true);
	PDBAtom(Element&,
		const String&,
		const String&,
		AtomType,
		const Vector3&,
		const Vector3&,
		const Vector3&,
		float,
		float,
		char,
		char,
		char,
		float,
		float);
	~PDBAtom() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const PDBAtom&, bool = true);
	void get(PDBAtom&, bool = true) const;
	void swap(PDBAtom&);

	Protein* getProtein();

	Chain* getChain();

	Residue* getResidue();

	void setBranchDesignator(char);
	char getBranchDesignator() const;
	void setRemotenessIndicator(char);
	char getRemotenessIndicator() const;
	void setAlternateLocationIndicator(char);
	char getAlternateLocationIndicator() const;
	void setOccupancy(float);
	float getOccupancy() const;
	void setTemperatureFactor(float);
	float getTemperatureFactor() const;
	virtual bool isValid() const throw();
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("PDBAtom ") + sipCpp->getName()
		+ " { " + sipCpp->getElement().getSymbol() + " @ ("
		+ String(sipCpp->getPosition().x) + " " + String(sipCpp->getPosition().y) + " "
		+ String(sipCpp->getPosition().z) + " }").c_str());
%End
};
