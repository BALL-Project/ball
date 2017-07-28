class NucleicAcid
	: Molecule
{
%TypeHeaderCode
	#include <BALL/KERNEL/nucleicAcid.h>
%End
	public:
	NucleicAcid();
	NucleicAcid(const NucleicAcid&, bool deep = true);
	NucleicAcid(const String&, const String& id = "" );
	~NucleicAcid() throw();

	virtual void clear() throw();
	virtual void destroy() throw();

	void set(const NucleicAcid&, bool deep = true);
	void get(NucleicAcid&, bool deep = true) const;
	void swap(NucleicAcid&);

	Nucleotide* getNucleotide(int);
	Nucleotide* get3Prime();
	Nucleotide* get5Prime();
	void setID(const String&);
	const String& getID() const;
	int countNucleotides() const;
	virtual bool isValid() const throw();

	SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(String("NucleicAcid ") + sipCpp->getName()
		+ " { " + String(sipCpp->countNucleotides()) + " nucleotides,  "
		+ String(sipCpp->countAtoms()) + " atoms }").c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	sipRes = PyString_FromString(String(String("NucleicAcid ") + sipCpp->getName()
		+ " { " + String(sipCpp->countNucleotides()) + " nucleotides,  "
		+ String(sipCpp->countAtoms()) + " atoms }").c_str());
%End
};
