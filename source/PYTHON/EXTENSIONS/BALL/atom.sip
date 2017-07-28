class AtomType
{
%TypeHeaderCode
	#include <BALL/KERNEL/atom.h>
	using AtomType = Atom::Type;
%End
	public:
%ConvertToTypeCode
	if (sipIsErr == NULL)
		return PyInt_Check(sipPy) || BALL_CAN_CONVERT_TO_INSTANCE(sipPy, Atom);

	if (PyInt_Check(sipPy))
	{
		*sipCppPtr = new AtomType(PyInt_AS_LONG(sipPy));

		return 1;
	}

	*sipCppPtr = BALL_CONVERT_TO_CPP(AtomType);

	return 0;
%End

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(*sipCpp).c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(*sipCpp).c_str());
%End
};

class Atom
	: Composite,
	  PropertyManager
{
%TypeHeaderCode
	#include <BALL/KERNEL/atom.h>
	using AtomType = Atom::Type;
%End

	public:

	enum
	{
		UNKNOWN_TYPE = -1,
		ANY_TYPE = 0,
		MAX_NUMBER_OF_BONDS = 12
	};

	enum Property
	{
		NUMBER_OF_PROPERTIES = 0
	};

	enum FullNameType
	{
		NO_VARIANT_EXTENSIONS,
		ADD_VARIANT_EXTENSIONS,
		ADD_RESIDUE_ID,
		ADD_VARIANT_EXTENSIONS_AND_ID
	};

	Atom() throw();
	Atom(const Atom&, bool deep = true) throw();
	Atom
		(Element&,
		const String&, const String& type_name = "UNK" ,
		int atom_type = UNKNOWN_TYPE ,
		const Vector3& position = Vector3(0,0,0 ),
		const Vector3& velocity = Vector3(0,0,0 ),
		const Vector3& force = Vector3(0,0,0 ),
		float charge = 0,
		float radius = 0,
		Index formal_charge = 0);
	~Atom() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const Atom&, bool deep = true) throw();
	void get(Atom&, bool deep = true) const throw();
	void swap(Atom&) throw();

	// Predicates
	bool operator == (const Atom&) const throw();
	bool operator != (const Atom&) const throw();

	// Accessors
	void setElement(Element&) throw();
	const Element& getElement() const throw();

	void setCharge(float) throw();
	float getCharge() const throw();

	void setFormalCharge(Index) throw();
	Index getFormalCharge() const throw();

	Fragment* getFragment() throw();
	Residue* getResidue() throw();
	Chain* getChain() throw();
	SecondaryStructure* getSecondaryStructure() throw();
	Molecule* getMolecule() throw();

	void setName(const String&) throw();
	const String& getName() const throw();
	String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const throw();

	void setPosition(const Vector3&) throw();
	const Vector3& getPosition() const throw();
	float getDistance(const Atom& a) const;

	void setRadius(float) throw();
	float getRadius() const throw();

	void setType(AtomType);
	AtomType getType() const;

	String getTypeName() const throw();
	void setTypeName(const String&) throw();

	void setVelocity(const Vector3&) throw();
	const Vector3& getVelocity() const throw();

	void setForce(const Vector3&) throw();
	const Vector3& getForce() const throw();

	Size countBonds() const;
	Bond* getBond(Position) throw(IndexOverflow);
	Bond* getBond(const Atom&) throw();

	Bond* createBond(Atom&) throw(TooManyBonds);
	Bond* createBond(Bond&, Atom&) throw(TooManyBonds);

	Bond* cloneBond(Bond&, Atom&) throw();
	bool destroyBond(const Atom&) throw();
	void destroyBonds() throw();

	bool hasBond(const Bond&) const throw();
	bool isBoundTo(const Atom&) const throw();
	bool isBound() const throw();
	bool isGeminal(const Atom&) const throw();
	bool isVicinal(const Atom&) const throw();
	virtual bool isValid() const throw();

	bool applyBonds(BondProcessor&) throw();

	// convert the atom to a string representation
	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("Atom ") + sipCpp->getName()
		+ " { " + sipCpp->getElement().getSymbol() + " @ ("
		+ String(sipCpp->getPosition().x) + " " + String(sipCpp->getPosition().y) + " "
		+ String(sipCpp->getPosition().z) + ") }").c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("Atom ") + sipCpp->getName()
		+ " { " + sipCpp->getElement().getSymbol() + " @ ("
		+ String(sipCpp->getPosition().x) + " " + String(sipCpp->getPosition().y) + " "
		+ String(sipCpp->getPosition().z) + " }").c_str());
%End
};
