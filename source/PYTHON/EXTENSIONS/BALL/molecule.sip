class Molecule
	: AtomContainer
{
%TypeHeaderCode
	#include <BALL/KERNEL/molecule.h>
%End
	public:
	Molecule();
	Molecule(const Molecule&, bool = true);
	Molecule(const String&);
	~Molecule() throw();
	virtual void clear() throw();
	virtual void destroy() throw();
	void set(const Molecule&, bool = true);
	void get(Molecule&, bool = true) const;
	void swap(Molecule&);

	System* getSystem();
	void prepend(Atom& /Transfer/);
	void append(Atom& /Transfer/);
	void insert(Atom& /Transfer/);
	void insertBefore(Atom& /Transfer/, Composite&);
	void insertAfter(Atom& /Transfer/, Composite&);
	bool remove(Atom&);
	void prepend(AtomContainer& /Transfer/);
	void append(AtomContainer& /Transfer/);
	void insert(AtomContainer& /Transfer/);
	void insertBefore(AtomContainer& /Transfer/, Composite&);
	void insertAfter(AtomContainer& /Transfer/, Composite&);
	void spliceBefore(AtomContainer&);
	void spliceAfter(AtomContainer&);
	void splice(AtomContainer&);
	bool remove(AtomContainer&);
	virtual bool isValid() const throw();

	FragmentIterator iterfragments();
%MethodCode
	return BALL_CONVERT_FROM_INSTANCE(new FragmentIterator(sipCpp->beginFragment()), FragmentIterator, Py_None);
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("Molecule ") + sipCpp->getName()
		+ " { " + String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
};
