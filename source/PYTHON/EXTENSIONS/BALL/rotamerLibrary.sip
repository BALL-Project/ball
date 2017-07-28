class RotamerLibrary
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/rotamerLibrary.h>
%End
	public:
	static const String DEFAULT_LIBRARY;
	static const String DEFAULT_FRAGMENT_DB;

	RotamerLibrary() throw();
	RotamerLibrary(const String& filename, const FragmentDB& fragment_db) throw();
	RotamerLibrary(const FragmentDB& fragment_db) throw();
	RotamerLibrary(const RotamerLibrary& library) throw();
	virtual ~RotamerLibrary();
	ResidueRotamerSet* getRotamerSet(const String& name);
	ResidueRotamerSet* getRotamerSet(const String& name, float phi, float psi);
	ResidueRotamerSet* getRotamerSet(const Residue& residue);
	Size getNumberOfRotamers() const;
	Size getNumberOfRotamers(const String& name) const;
	Size getNumberOfRotamerSets() const;
	void addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions, Index phi, Index psi);
	void addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions);
	bool isBackboneDependent() const;
	void setBackboneDependent(bool dependent);
	bool hasRotamers(const String& name) const;
	bool validate();
	void sort();
	void clear();
};
