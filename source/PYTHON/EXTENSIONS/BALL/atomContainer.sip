class AtomContainer
	: Composite,
	  PropertyManager
{
%TypeHeaderCode
	#include <BALL/KERNEL/atomContainer.h>
%End

	public:

	enum Property
	{
		NUMBER_OF_PROPERTIES = 0
	};

	AtomContainer();
	AtomContainer(const AtomContainer&, bool = true);
	AtomContainer(const String&);
	~AtomContainer() throw();

	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const AtomContainer&, bool = true);
	void get(AtomContainer&, bool = true) const;
	void swap(AtomContainer&);
	void setName(const String&);
	const String& getName() const;

	AtomContainer* getSuperAtomContainer();
	AtomContainer* getAtomContainer(Position);
	Atom* getAtom(Position);
	Atom* getAtom(const String&);

	Size countAtomContainers() const;
	Size countAtoms() const;
	Size countBonds() const;
	Size countInterBonds() const;
	Size countIntraBonds() const;
	void prepend(Atom&);
	void append(Atom&);
	void insert(Atom&);
	void insertBefore(Atom&, Composite&);
	void insertAfter(Atom&, Composite&);
	bool remove(Atom&);
	void prepend(AtomContainer&);
	void append(AtomContainer&);
	void insert(AtomContainer&);
	void insertBefore(AtomContainer&, Composite&);
	void insertAfter(AtomContainer&, Composite&);
	void spliceBefore(AtomContainer&);
	void spliceAfter(AtomContainer&);
	void splice(AtomContainer&);
	bool remove(AtomContainer&);
	void destroyBonds();
	bool isSubAtomContainerOf(const AtomContainer&) const;
	bool isSuperAtomContainerOf(const AtomContainer&) const;
	virtual bool isValid() const throw();

	bool apply(AtomProcessor&);
	bool apply(ChainProcessor&);
	bool apply(CompositeProcessor&);
	bool apply(FragmentProcessor&);
	bool apply(ResidueProcessor&);

	AtomIterator iteratoms();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new AtomIterator(sipCpp->beginAtom()), AtomIterator, Py_None);
%End

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("AtomContainer ") + sipCpp->getName()
		+ " { " + String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
};
